#include "candy/gbd/hash.h"

#include "candy/core/SolverTypes.h"
#include <candy/gbd/md5/md5.h>

typedef struct gzFile_s *gzFile;

namespace Candy {

Hash::Hash() {}

std::string Hash::gbd_hash_from_dimacs(const char* filename) {
    unsigned char sig[MD5_SIZE];
    char str[MD5_STRING_SIZE];
    md5::md5_t md5;
    gzFile input_stream = gzopen(filename, "rb");
    StreamBuffer in(input_stream);
    std::string clause("");
    while (!in.eof()) {
        in.skipWhitespace();
        if (in.eof()) {
            break;
        }
        if (*in == 'p' || *in == 'c') {
            in.skipLine();
        }
        else {
            for (int plit = in.readInteger(); plit != 0; plit = in.readInteger()) {
                clause.append(std::to_string(plit)); 
                clause.append(" ");
            }
            clause.append("0");
            md5.process(clause.c_str(), clause.length());
            clause.assign(" ");
        }
    }
    gzclose(input_stream);
    md5.finish(sig);
    md5::sig_to_string(sig, str, sizeof(str));
    return std::string(str);
}

std::string Hash::gbd_hash(Candy::CNFProblem& problem) {
    unsigned char sig[MD5_SIZE];
    char str[MD5_STRING_SIZE];

    md5::md5_t md5;
    std::string s("");

    for (Candy::Cl* clause : problem) {
        for (Candy::Lit lit : *clause) {
            s.append(std::to_string(lit));
            s.append(" ");
        }
        s.append("0");
        md5.process(s.c_str(), s.length());
        s.assign(" ");
    }

    md5.finish(sig);
    md5::sig_to_string(sig, str, sizeof(str));
    return std::string(str);
}

std::string Hash::degree_hash(Candy::CNFProblem& problem) {
    std::vector<std::vector<Candy::Cl*>> occurrences(problem.nVars()*2);
    std::vector<unsigned int> degrees(problem.nVars()*2);
    std::vector<Candy::Lit> facts;

    for (Candy::Cl* clause : problem) {
        if (clause->size() == 1) {
            facts.push_back(clause->front());
        }
        for (Candy::Lit lit : *clause) {
            occurrences[lit].push_back(clause);
        }
    }
    for (unsigned int lit = 0; lit < problem.nVars()*2; lit++) {
        degrees[lit] = occurrences[lit].size();
    }
    // normalize degree-list by unit-propagation
    for (Candy::Lit lit : facts) {
        for (Candy::Cl* clause : occurrences[lit]) {
            for (Candy::Lit lit : *clause) {
                degrees[lit]--;
            }
        }
        degrees[~lit] = 0;
    }

    unsigned char sig[MD5_SIZE];
    char str[MD5_STRING_SIZE];

    md5::md5_t md5;
    std::string s("");

    std::sort(degrees.begin(), degrees.end());

    for (unsigned int degree : degrees) {
        if (degree > 0) {
            s.append(std::to_string(degree));
            md5.process(s.c_str(), s.length());
            s.assign(" ");
        }
    }

    md5.finish(sig);
    md5::sig_to_string(sig, str, sizeof(str));
    return std::string(str);
}

}