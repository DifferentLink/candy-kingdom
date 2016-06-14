/***************************************************************************************[MultiSolvers.cc]
 Glucose -- Copyright (c) 2009-2014, Gilles Audemard, Laurent Simon
                                CRIL - Univ. Artois, France
                                LRI  - Univ. Paris Sud, France (2009-2013)
                                Labri - Univ. Bordeaux, France

 Syrup (Glucose Parallel) -- Copyright (c) 2013-2014, Gilles Audemard, Laurent Simon
                                CRIL - Univ. Artois, France
                                Labri - Univ. Bordeaux, France

Glucose sources are based on MiniSat (see below MiniSat copyrights). Permissions and copyrights of
Glucose (sources until 2013, Glucose 3.0, single core) are exactly the same as Minisat on which it 
is based on. (see below).

Glucose-Syrup sources are based on another copyright. Permissions and copyrights for the parallel
version of Glucose-Syrup (the "Software") are granted, free of charge, to deal with the Software
without restriction, including the rights to use, copy, modify, merge, publish, distribute,
sublicence, and/or sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

- The above and below copyrights notices and this permission notice shall be included in all
copies or substantial portions of the Software;
- The parallel version of Glucose (all files modified since Glucose 3.0 releases, 2013) cannot
be used in any competitive event (sat competitions/evaluations) without the express permission of 
the authors (Gilles Audemard / Laurent Simon). This is also the case for any competitive event
using Glucose Parallel as an embedded SAT engine (single core or not).


--------------- Original Minisat Copyrights

Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **************************************************************************************************/

#include <pthread.h>
#include "parallel/MultiSolvers.h"
#include "utils/System.h"
#include "simp/SimpSolver.h"
#include <errno.h>
#include <string.h>
#include "parallel/SolverConfiguration.h"

using namespace Glucose;

extern const char* _parallel ;
extern const char* _cunstable;
// Options at the parallel solver level
static IntOption opt_nbsolversmultithreads (_parallel, "nthreads", "Number of core threads for syrup (0 for automatic)", 0);
static IntOption opt_maxnbsolvers (_parallel, "maxnbthreads", "Maximum number of core threads to ask for (when nbthreads=0)", 4);
static IntOption opt_maxmemory    (_parallel, "maxmemory", "Maximum memory to use (in Mb, 0 for no software limit)", 3000);
static IntOption opt_statsInterval (_parallel, "statsinterval", "Seconds (real time) between two stats reports", 5);
//
// Shared with ClausesBuffer.cc
BoolOption opt_whenFullRemoveOlder (_parallel, "removeolder", "When the FIFO for exchanging clauses between threads is full, remove older clauses", false);
IntOption opt_fifoSizeByCore(_parallel, "fifosize", "Size of the FIFO structure for exchanging clauses between threads, by threads", 100000);
//
// Shared options with Solver.cc 
BoolOption    opt_dontExportDirectReusedClauses (_cunstable, "reusedClauses",    "Don't export directly reused clauses", false);
BoolOption    opt_plingeling (_cunstable, "plingeling",    "plingeling strategy for sharing clauses (exploratory feature)", false);

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

static inline double cpuTime(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
return (double)ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000; }


void MultiSolvers::informEnd(lbool res) {
  result = res;
  pthread_cond_broadcast(&cfinished);
}

MultiSolvers::MultiSolvers(ParallelSolver *s):
  ok (true)
  , maxnbthreads(4), nbthreads(opt_nbsolversmultithreads), nbsolvers(opt_nbsolversmultithreads)
  , nbcompanions(4), nbcompbysolver(2)
  , allClonesAreBuilt(0)
  , showModel(false)
  , winner(-1)
  , var_decay(1 / 0.95), clause_decay(1 / 0.999),cla_inc(1), var_inc(1)
  , random_var_freq(0.02)
  , restart_first(100), restart_inc(1.5)
  , learntsize_factor((double)1/(double)3), learntsize_inc(1.1)
  , expensive_ccmin(true)
  , polarity_mode    (polarity_false)
  , maxmemory(opt_maxmemory), maxnbsolvers(opt_maxnbsolvers)
  , verb(0) , verbEveryConflicts(10000)
  , numvar(0), numclauses(0)

{
    result = l_Undef;
    SharedCompanion *sc = new SharedCompanion();
    this->sharedcomp = sc;

    // Generate only solver 0.
    // It loads the formula
    // All others solvers are clone of this one
    solvers.push(s);
    s->verbosity = 0; // No reportf in solvers... All is done in MultiSolver
    s->setThreadNumber(0);
    //s->belongsto = this;
    s->sharedcomp = sc;
    sc->addSolver(s);
    assert(solvers[0]->threadNumber() == 0);

    pthread_mutex_init(&m,NULL);  //PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&mfinished,NULL); //PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_init(&cfinished,NULL);

    if (nbsolvers > 0 ) 
	fprintf(stdout,"c %d solvers engines and 1 companion as a blackboard created.\n", nbsolvers);
}

MultiSolvers::MultiSolvers() : MultiSolvers(new ParallelSolver(-1)) {

}

MultiSolvers::~MultiSolvers()
{}

/**
 * Generate All solvers
 */

void MultiSolvers::generateAllSolvers() {
    assert(solvers[0] != NULL);
    assert(allClonesAreBuilt==0);

    for(int i=1;i<nbsolvers;i++) {
	ParallelSolver *s  = (ParallelSolver*)solvers[0]->clone();
	solvers.push(s);
	s->verbosity = 0; // No reportf in solvers... All is done in MultiSolver
	s->setThreadNumber(i);
	s->sharedcomp =   this->sharedcomp;
	this->sharedcomp->addSolver(s);
	assert(solvers[i]->threadNumber() == i);
    }

    adjustParameters(); 

    allClonesAreBuilt = 1;
}

/**
 * Choose solver for threads i (if no given in command line see above)
 */


ParallelSolver* MultiSolvers::retrieveSolver(int i) {
    return new ParallelSolver(i);
}

Var MultiSolvers::newVar(bool sign, bool dvar)
{
  assert(solvers[0] != NULL);
  numvar++;
  int v;
  sharedcomp->newVar(sign);
  if(!allClonesAreBuilt) { // At the beginning we want to generate only solvers 0
    v = solvers[0]->newVar(sign,dvar);
    assert(numvar == v+1); // Just a useless check
  } else {
      for(int i=0;i<nbsolvers;i++) {
	  v = solvers[i]->newVar(sign,dvar);
      }
  }
  return numvar;
}

bool MultiSolvers::addClause_(vector<Lit>&ps) {
  assert(solvers[0] != NULL); // There is at least one solver.
  // Check if clause is satisfied and remove false/duplicate literals:
  if (!okay())  return false;

  std::sort(ps.begin(), ps.end());

  Lit p;
  unsigned int i, j;

  for (i = j = 0, p = lit_Undef; i < ps.size(); i++)
    if (solvers[0]->value(ps[i]) == l_True || ps[i] == ~p)
      return true;
    else if (solvers[0]->value(ps[i]) != l_False && ps[i] != p)
      ps[j++] = p = ps[i];
  ps.resize(j);
  
  
  if (ps.size() == 0) {
    return ok = false;
  }
  else if (ps.size() == 1){
    assert(solvers[0]->value(ps[0]) == l_Undef); // TODO : Passes values to all threads
    solvers[0]->uncheckedEnqueue(ps[0]);
    if(!allClonesAreBuilt) {
	return ok = ( (solvers[0]->propagate()) == CRef_Undef); // checks only main solver here for propagation constradiction
    } 

    // Here, all clones are built.
    // Gives the unit clause to everybody
    for(int i=0;i<nbsolvers;i++)
      solvers[i]->uncheckedEnqueue(ps[0]);
    return ok = ( (solvers[0]->propagate()) == CRef_Undef); // checks only main solver here for propagation constradiction
  }else{
    //		printf("Adding clause %0xd for solver %d.\n",(void*)c, thn);
    // At the beginning only solver 0 load the formula
    solvers[0]->addClause(ps);
    
    if(!allClonesAreBuilt) {
	numclauses++;
	return true;
    }
    // Clones are built, need to pass the clause to all the threads 
    for(int i=1;i<nbsolvers;i++) {
      solvers[i]->addClause(ps);
    }
    numclauses++;
  }
  return true;
}


bool MultiSolvers::simplify() {
  assert(solvers[0] != NULL); // There is at least one solver.

  if (!okay()) return false;
  return ok = solvers[0]->simplify(); 
}


bool MultiSolvers::eliminate() {
    
    // TODO allow variable elimination when all threads are built!
    assert(allClonesAreBuilt==false);
    
    SimpSolver * s = (SimpSolver*)getPrimarySolver();
    return s->eliminate(true);
}


// TODO: Use a template here
void *localLaunch(void*arg) {
  ParallelSolver* s = (ParallelSolver*)arg;
  
  (void)s->solve();
  
  pthread_exit(NULL);
}


#define MAXIMUM_SLEEP_DURATION 5
void MultiSolvers::printStats() {
	static int nbprinted = 1;
	double cpu_time = cpuTime();
    printf("c\n");

    printf("c |-------------------------------------------------------------------------------------------------------|\n");
        printf("c | id | starts | decisions  |  confls    |  Init T  |  learnts | exported | imported | promoted |    %%   | \n");
        printf("c |-------------------------------------------------------------------------------------------------------|\n");

	//printf("%.0fs | ",cpu_time);
	for(int i=0;i<solvers.size();i++) {
	    solvers[i]->reportProgress();
    //printf(" %2d: %12ld confl. |", i,  (long int) solvers[i]->conflicts);
    }
	long long int totalconf = 0;
	long long int totalprop = 0;
	for(int i=0;i<solvers.size();i++) {
		totalconf+=  (long int) solvers[i]->conflicts;
		totalprop+= solvers[i]->propagations;
    }
    printf("c \n");
   
    printf("c synthesis %11lld conflicts %11lld propagations %8.0f conflicts/sec %8.0f propagations/sec\n",
            totalconf, totalprop, (double)totalconf / cpu_time, (double) totalprop / cpu_time);


	nbprinted ++;
}

// Still a ugly function... To be rewritten with some statistics class some day
void MultiSolvers::printFinalStats() {
    sharedcomp->printStats();
    printf("c\nc\n");
    printf("c\n");
    printf("c |---------------------------------------- FINAL STATS --------------------------------------------------|\n");
    printf("c\n");
    
    printf("c |---------------");
    for(int i = 0;i<solvers.size();i++) 
        printf("|------------");
    printf("|-----------------|\n");    

    printf("c | Threads       ");
    for(int i = 0;i < solvers.size();i++) {
        printf("| %10d ",i);
    }
    printf("|      Total      |\n");

    printf("c |---------------");
    for(int i = 0;i<solvers.size();i++) 
        printf("|------------");
    printf("|-----------------|\n");    

    
    printf("c | Conflicts     ");

    long long int totalconf = 0;
    for(int i=0;i<solvers.size();i++)  {
	printf("| %10" PRIu64" ", solvers[i]->conflicts);
	totalconf +=  solvers[i]->conflicts;
    }
    printf("| %15lld |\n",totalconf);
   
    printf("c | Exported      ");
    uint64_t exported = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbexported);
        exported += solvers[i]->nbexported;
    }
    printf("| %15" PRIu64" |\n", exported);

    printf("c | Imported      ");
    uint64_t imported = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbimported);
        imported += solvers[i]->nbimported;
    }
    printf("| %15" PRIu64" |\n", imported);

    printf("c | Good          ");
    uint64_t importedGood = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbImportedGoodClauses);
        importedGood += solvers[i]->nbImportedGoodClauses;
    }
    printf("| %15" PRIu64" |\n", importedGood);

    printf("c | Purge         ");
    uint64_t importedPurg = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbimportedInPurgatory);
        importedPurg += solvers[i]->nbimportedInPurgatory;
    }
    printf("| %15" PRIu64" |\n", importedPurg);

    printf("c | Promoted      ");
    uint64_t promoted = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbPromoted);
        promoted += solvers[i]->nbPromoted;
    }
    printf("| %15" PRIu64" |\n", promoted);

    printf("c | Remove imp    ");
    uint64_t removedimported = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbRemovedUnaryWatchedClauses);
        removedimported += solvers[i]->nbRemovedUnaryWatchedClauses;
    }
    printf("| %15" PRIu64" |\n", removedimported);

    printf("c | Blocked Reuse ");
    uint64_t blockedreused = 0;
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbNotExportedBecauseDirectlyReused);
        blockedreused += solvers[i]->nbNotExportedBecauseDirectlyReused;
    }
    printf("| %15" PRIu64" |\n",blockedreused);


    printf("c | Orig seen     ");
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->originalClausesSeen);
    }
    printf("|                 |\n"); 

    printf("c | Unaries       ");
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbUn);
    }
    printf("|                 |\n"); 

    printf("c | Binaries      ");
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbBin);
    }
    printf("|                 |\n"); 

    
     printf("c | Glues         ");
    for(int i=0;i<solvers.size();i++) {
	printf("| %10" PRIu64" ", solvers[i]->nbDL2);
    }
    printf("|                 |\n"); 


    int winner = -1;
   for(int i=0;i<solvers.size();i++) {
     if(sharedcomp->winner()==solvers[i])
       winner = i;
     }
   
   if(winner!=-1) {
int sum = 0;
   printf("c | Hamming       ");
   for(int i = 0;i<solvers.size();i++) {
     if(i==winner) {
       printf("|      X     ");
       continue;
     }
     int nb = 0;
     for(int j = 0;j<nVars();j++) {
       if(solvers[i]->valuePhase(j)!= solvers[winner]->valuePhase(j)) nb++;
     }
     printf("| %10d ",nb);
     sum+=nb;

   }
   printf("| %15d |\n",sum/(solvers.size()>1?solvers.size()-1:1));
   }
 
   printf("c |---------------");
   for(int i = 0;i<solvers.size();i++) 
     printf("|------------");
   printf("|-----------------|\n");    

  

}

// Well, all those parameteres are just naive guesses... No experimental evidences for this.
void MultiSolvers::adjustParameters() {
    SolverConfiguration::configure(this,nbsolvers);
}

void MultiSolvers::adjustNumberOfCores() {
 float mem = memUsed();
  if (nbthreads==0) { // Automatic configuration
      if(verb>=1) 
          printf("c |  Automatic Adjustement of the number of solvers. MaxMemory=%5d, MaxCores=%3d.                       |\n", maxmemory, maxnbsolvers);
      unsigned int tmpnbsolvers = maxmemory * 4 /  10 / mem;
      if (tmpnbsolvers > maxnbsolvers) tmpnbsolvers = maxnbsolvers;
      if (tmpnbsolvers < 1) tmpnbsolvers = 1;
      if(verb>=1) 
        printf("c |  One Solver is taking %.2fMb... Let's take %d solvers for this run (max 40%% of the maxmemory).       |\n", mem, tmpnbsolvers);
      nbsolvers = tmpnbsolvers;
      nbthreads = nbsolvers;
  } else {
      assert(nbthreads == nbsolvers);
  }
}

lbool MultiSolvers::solve() {
  pthread_attr_t thAttr; 
  int i; 

  adjustNumberOfCores();
  sharedcomp->setNbThreads(nbsolvers); 
  if(verb>=1) 
    printf("c |  Generating clones                                                                                    |\n"); 
  generateAllSolvers();
  if(verb>=1) {
    printf("c |  all clones generated. Memory = %6.2fMb.                                                             |\n", memUsed());
    printf("c ========================================================================================================|\n");
  }
  
  
  model.clear();

  /* Initialize and set thread detached attribute */
  pthread_attr_init(&thAttr);
  pthread_attr_setdetachstate(&thAttr, PTHREAD_CREATE_JOINABLE);
  
  
  
  // Launching all solvers
  for (i = 0; i < nbsolvers; i++) {
    pthread_t * pt = (pthread_t*)malloc(sizeof(pthread_t));
    threads.push(pt);
    solvers[i]->pmfinished = &mfinished;
    solvers[i]->pcfinished = &cfinished;
    pthread_create(threads[i], &thAttr, &localLaunch, (void*)solvers[i]); 
  }
  
  bool done = false;
  
  (void)pthread_mutex_lock(&m);
  while (!done) { 
    struct timespec timeout;
    time(&timeout.tv_sec);
    timeout.tv_sec += MAXIMUM_SLEEP_DURATION;
    timeout.tv_nsec = 0;
    if (pthread_cond_timedwait(&cfinished, &mfinished, &timeout) != ETIMEDOUT) 
	    done = true;
    else 
      printStats();

    float mem = memUsed();
    if(verb>=1) printf("c Total Memory so far : %.2fMb\n",  mem);
    if ( (maxmemory > 0) && (mem > maxmemory) && !sharedcomp->panicMode) 
       printf("c ** reduceDB switching to Panic Mode due to memory limitations !\n"), sharedcomp->panicMode = true;
    
  }
  (void)pthread_mutex_unlock(&m);
  
  for (i = 0; i < nbsolvers; i++) { // Wait for all threads to finish
      pthread_join(*threads[i], NULL);
  }
  
  assert(sharedcomp != NULL);
  result = sharedcomp->jobStatus;
  if (result == l_True) {
      int n = sharedcomp->jobFinishedBy->nVars();
	model.growTo(n);
	for(int i = 0; i < n; i++)
	    model[i] = sharedcomp->jobFinishedBy->model[i];
  }

	
  return result;
  /*
  for(int i=0;i<NBTHREADS;i++)
    pthread_join(*threads[i],&status);
  */

}

