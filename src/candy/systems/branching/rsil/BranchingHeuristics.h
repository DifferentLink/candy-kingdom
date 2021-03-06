/* Copyright (c) 2017 Felix Kutzner (github.com/fkutzner)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 Except as contained in this notice, the name(s) of the above copyright holders
 shall not be used in advertising or otherwise to promote the sale, use or
 other dealings in this Software without prior written authorization.
 
 */

#ifndef X_66055FCA_E0CE_46B3_9E4C_7F093C37330B_BRANCHINGHEURISTICS_H
#define X_66055FCA_E0CE_46B3_9E4C_7F093C37330B_BRANCHINGHEURISTICS_H

#include <candy/core/SolverTypes.h>
#include <candy/core/clauses/ClauseDatabase.h>
#include <candy/systems/branching/VSIDS.h>
#include <candy/systems/branching/rsil/ImplicitLearningAdvice.h>
#include <candy/randomsimulation/Conjectures.h>
#include <candy/utils/FastRand.h>
#include <candy/utils/Attributes.h>
#include <candy/rsar/Heuristics.h>
#include <candy/gates/GateAnalyzer.h>
#include <candy/gates/MiterDetector.h>
#include <candy/frontend/RandomSimulationFrontend.h>

#include "RSARHeuristicsFilter.h"

#include <type_traits>

namespace Candy {    
    /**
     * \class RSILBranchingHeuristic
     *
     * \ingroup RS_ImplicitLearning
     *
     * \brief A PickBranchLit type for branching decisions using implicit learning.
     *
     * A TBranchingHeuristic type (see candy/core/Solver.h) for plain random-simulation-based
     * implicit learning solvers. A pickBranchLit implementation is provided for the
     * RSILBranchingHeuristic TBranchingHeuristic type.
     *
     * \tparam AdviceType    The advice type used by the branching heuristic. May be any
     *                       type having the same interface as AdviceType defined in the
     *                       ImplicitLearningAdvice.h file. Advices are used as compact
     *                       storages of conjectures about literal equivalencies and may
     *                       carry further information about these conjectures, e.g. usage
     *                       budgets.
     */
    template<class AdviceType>
    class RSILBranchingHeuristic : public BranchingDiversificationInterface {

        static_assert(std::is_class<typename AdviceType::BasicType>::value, "AdviceType must have an inner type BasicType");
        
    public:
        ClauseDatabase& clause_db;
        Trail& trail;

        /// The conjectures to be used for implicit learning, e.g. obtained via random simulation.
        std::unique_ptr<Conjectures> conjectures;
        bool initializationCompleted;
        bool blockInitialization;

        /**
         * A few definitions for the TBranchingHeuristic concept
         */
        VSIDS defaultBranchingHeuristic;

        Lit pickBranchLit();

        void setPolarity(Var v, bool sign) override {
            defaultBranchingHeuristic.setPolarity(v, sign);
        }

        Lit getLastDecision() override {
            return trail[(*trail.trail_lim.rbegin())];
        }

        void clear() {
            defaultBranchingHeuristic.clear();
        }

    	virtual void init(const CNFProblem& problem) {
    		defaultBranchingHeuristic.init(problem);
            if (!blockInitialization) {
                blockInitialization = true;
                generateConjectures(problem);
                if (initializationCompleted) {
                    m_backbonesEnabled = !conjectures->getBackbones().empty();
                    m_advice.init(*conjectures.get(), problem.nVars());
                }
            }
    	}

    	virtual void init(std::unique_ptr<Conjectures> _con, bool backbonesEnabled = true) {
            this->conjectures = std::move(_con); 
            initializationCompleted = true;
            m_backbonesEnabled = backbonesEnabled;
            m_advice.init(*conjectures.get(), conjectures->getMaxVar()+1);
    	}

        void process_conflict() {
            defaultBranchingHeuristic.process_conflict();
    	}

    	void reset() {
    		defaultBranchingHeuristic.reset();
        }

        void process_reduce() {
        }

        void generateConjectures(const CNFProblem& problem) {
            GateAnalyzer analyzer { problem };
            analyzer.analyze();

            if (analyzer.hasTimeout()) {
                std::cout << "c gate analysis exceeded the preprocessing time limit." << std::endl;
            }
            else if (analyzer.getResult().getGateCount() <= RSILOptions::opt_rsil_minGateFraction * problem.nVars()) {
                std::cout << "c insufficient gate count." << std::endl;
            }
            else if (RSILOptions::opt_rsil_onlyMiters && !hasPossiblyMiterStructure(analyzer)) {
                std::cout << "c problem heuristically determined not to be a miter problem." << std::endl;
            }
            else {
                try {
                    this->conjectures = performRandomSimulation(analyzer, RandomSimulationOptions::opt_rs_ppTimeLimit);
                    if (conjectures->getEquivalences().empty() && conjectures->getBackbones().empty()) {
                        std::cout << "c no conjectures found." << std::endl;
                    } else {
                        initializationCompleted = true;
                    }
                }
                catch(OutOfTimeException& exception) {
                    std::cout << "c random simulation exceeded the preprocessing time limit." << std::endl;
                }
            }
        }

        /// A type used for recognition of RSILBranchingHeuristic types in template metaprogramming.
        /// This type is independent of the template argument AdviceType.
        using BasicType = RSILBranchingHeuristic<AdviceEntry<3>>;

        /**
         * \brief Constructs a new RSILBranchingHeuristic instance with a default configuration
         *        and no literal-equivalency/backbone conjectures.
         *
         * An instance constructed this way does not provide implicit learning advice, i.e.
         * getAdvice(...) always returns lit_Undef and getSignAdvice(L) returns L.
         *
         */
        RSILBranchingHeuristic(ClauseDatabase& clause_db_, Trail& trail_) :
            clause_db(clause_db_), trail(trail_), initializationCompleted(false), blockInitialization(false),
            defaultBranchingHeuristic(clause_db_, trail_), m_advice(),
            m_rng(0xFFFF)
        {
            // if (rsar_filter_ != nullptr) {
            //     std::vector<RefinementHeuristic*> heuristics;
            //     heuristics.push_back(rsar_filter_);
            //     filterWithRSARHeuristics(heuristics, m_advice, RSILOptions::opt_rsil_filterOnlyBackbone);
            // }
        }

        RSILBranchingHeuristic(RSILBranchingHeuristic&& other) = default;
        RSILBranchingHeuristic& operator=(RSILBranchingHeuristic&& other) = default;
        
        /**
         * \brief Gets a branching literal advice using implicit learning.
         *
         * If possible, a branching decision literal is proposed with implicit learning using
         * the literals occurring on the trail's most recent decision level. If no such literal
         * can be picked, lit_Undef is returned. Note that getAdvice(...) does not return literals
         * conjectured to belong to the problem's backbone.
         *
         * \param trail         The sequence of current variable assignments, ordered by the
         *                      time of assignment.
         * \param trailSize     The size of the initial segment of \p trail containing the current
         *                      sequence of variable assignments.
         * \param trailLimits   A sequence of indices of \p trail partitioning \p trail into assignments
         *                      occurring on different decision levels.
         * \param assigns       The underlying solver's variable assignments. For a variable \p v,
         *                      \p assigns[v] must be the lbool representing its current assignment.
         * \param decision      A data structure marking variables eligible to be used for branching
         *                      decisions. A variable \p v is considered eligible iff \p decision[v]
         *                      is not 0.
         *
         * \returns a literal chosen via implicit learning or lit_Undef.
         */
        Lit getAdvice() noexcept;
        
        /**
         * \brief Gets a branching literal sign advice using implicit learning.
         *
         * For a branching literal L having already been picked for a branching decision, this method
         * returns a literal with the same variable as L, but a sign chosen via implicit learning. Note
         * that with this method, implicit learning is only used for backbone variable conjectures.
         *
         * \returns a literal L having the same variable as \p literal.
         */
        Lit getSignAdvice(Lit literal) noexcept;
        
        /**
         * \brief Gets the pseudorandom number generator used by the heuristic.
         *
         * This method exposes the heuristic's internal PRNG to allow more efficient implementations
         * of this heuristic's variants. Clients may use and modify the PRNG.
         *
         * \returns a PRNG.
         */
        FastRandomNumberGenerator& getRandomNumberGenerator() noexcept;
        
    protected:
        /// The conjectures about literal equivalencies and backbones.
        ImplicitLearningAdvice<AdviceType> m_advice;
        
    private:
        /// The heuristic's pseudorandom number generator.
        FastRandomNumberGenerator m_rng;
        
        /// Iff true, backbones are taken into consideration for implicit learning.
        bool m_backbonesEnabled;
    };
    
    
    /**
     * \ingroup RS_ImplicitLearning
     *
     * A specialization of RSILBranchingHeuristic<n> with n=3.
     */
    using RSILBranchingHeuristic3 = RSILBranchingHeuristic<AdviceEntry<3>>;
    
    /**
     * \ingroup RS_ImplicitLearning
     *
     * A specialization of RSILBranchingHeuristic<n> with n=2.
     */
    using RSILBranchingHeuristic2 = RSILBranchingHeuristic<AdviceEntry<2>>;
    
    
    /**
     * \class RSILBudgetBranchingHeuristic
     *
     * \ingroup RS_ImplicitLearning
     *
     * \brief A PickBranchLit type for branching decisions using implicit learning, limited by budgets.
     *
     * This is a RSILBranchingHeuristic tailored for implicit learning with implication
     * budgets. Each implication is assigned an initial budget, and each time it is used
     * for implicit learning, the budget is decreased by one. If an implication's budget
     * is 0, it is not further used for implicit learning.
     *
     * \tParam tAdviceSize      The maximum of size equivalence conjectures taken into account
     * \tParam SolverTypes      See the SolverTypes template parameter of RSILBranchingHeuristic
     */
    template<unsigned int tAdviceSize>
    class RSILBudgetBranchingHeuristic : public RSILBranchingHeuristic<BudgetAdviceEntry<tAdviceSize>> {
        static_assert(tAdviceSize > 1, "advice size must be >= 2");
    public:

        Lit pickBranchLit();

        void init(const CNFProblem& problem) override {
            RSILBranchingHeuristic<BudgetAdviceEntry<tAdviceSize>>::init(problem);
            for (Var i = 0; this->m_advice.hasPotentialAdvice(i); ++i) {
                auto& advice = this->m_advice.getAdvice(i);
                for (size_t j = 0; j < advice.getSize(); ++j) {
                    advice.setBudget(j, RSILOptions::opt_rsil_impBudgets);
                }
            }
        }

    	void init(std::unique_ptr<Conjectures> _con, bool backbonesEnabled = true, unsigned int budget = 1 << 20) {
            RSILBranchingHeuristic<BudgetAdviceEntry<tAdviceSize>>::init(std::move(_con), backbonesEnabled);
            for (Var i = 0; this->m_advice.hasPotentialAdvice(i); ++i) {
                auto& advice = this->m_advice.getAdvice(i);
                for (size_t j = 0; j < advice.getSize(); ++j) {
                    advice.setBudget(j, budget);
                }
            }
    	}

        /// A type used for recognition of RSILBudgetBranchingHeuristic types in template metaprogramming.
        /// This type is independent of the template argument AdviceType.
        using BasicType = RSILBudgetBranchingHeuristic<3>;
        
        RSILBudgetBranchingHeuristic(ClauseDatabase& clause_db_, Trail& trail_)
         : RSILBranchingHeuristic<BudgetAdviceEntry<tAdviceSize>>(clause_db_, trail_) 
        { }

        RSILBudgetBranchingHeuristic(RSILBudgetBranchingHeuristic&& other) = default;
        RSILBudgetBranchingHeuristic& operator=(RSILBudgetBranchingHeuristic&& other) = default;
    };
    
    /**
     * \ingroup RS_ImplicitLearning
     *
     * A specialization of RSILBudgetBranchingHeuristic<n> with n=3.
     */
    using RSILBudgetBranchingHeuristic3 = RSILBudgetBranchingHeuristic<3>;
    
    /**
     * \ingroup RS_ImplicitLearning
     *
     * A specialization of RSILBudgetBranchingHeuristic<n> with n=2.
     */
    using RSILBudgetBranchingHeuristic2 = RSILBudgetBranchingHeuristic<2>;
    
    /**
     * \class RSILVanishingBranchingHeuristic
     *
     * \ingroup RS_ImplicitLearning
     *
     * \brief A PickBranchLit type for branching decisions using implicit learning, limited by the amount of performed decisions.
     *
     * A TBranchingHeuristic type (see candy/core/Solver.h) for plain random-simulation-based
     * implicit learning solvers. This heuristic produces results with decreased probability,
     * configured via the parameter \p probHalfLife : every \p probHalfLife decisions, the
     * probability of possibly overriding the solver's internal decision heuristic is halved.
     *
     * A pickBranchLit implementation is provided for the RSILVanishingBranchingHeuristic
     * TBranchingHeuristic type.
     *
     * \tParam AdviceType       See the AdviceType template parameter of RSILBranchingHeuristic
     * \tParam SolverTypes      See the SolverTypes template parameter of RSILBranchingHeuristic
     */
    template<class AdviceType>
    class RSILVanishingBranchingHeuristic : public RSILBranchingHeuristic<AdviceType> {
    public:

        Lit pickBranchLit();

        void init(const CNFProblem& problem) override {
            RSILBranchingHeuristic<AdviceType>::init(problem);
        }

    	void init(std::unique_ptr<Conjectures> _con, bool backbonesEnabled = true, unsigned int halflife = 1 << 24) {
            RSILBranchingHeuristic<AdviceType>::init(std::move(_con), backbonesEnabled);
            m_probHalfLife = halflife; 
    	}
        
        /// A type used for recognition of RSILVanishingBranchingHeuristic types in template metaprogramming.
        /// This type is independent of the template argument AdviceType.
        using BasicType = RSILVanishingBranchingHeuristic<AdviceEntry<3>>;

        RSILVanishingBranchingHeuristic(ClauseDatabase& clause_db_, Trail& trail_)
         : RSILBranchingHeuristic<AdviceType>(clause_db_, trail_),
            m_callCounter(RSILOptions::opt_rsil_vanHalfLife),
            m_probHalfLife(RSILOptions::opt_rsil_vanHalfLife), m_mask(0ull)
        { }

        RSILVanishingBranchingHeuristic(RSILVanishingBranchingHeuristic&& other) = default;
        RSILVanishingBranchingHeuristic& operator=(RSILVanishingBranchingHeuristic&& other) = default;
                
    private:
        /**
         * \brief Decides if implicit learning can used for this decision.
         *
         * Increases m_callCounter by one. If the counter exceeds the probability half-life,
         * the probability of using implicit learning is halved and the counter is reset to 0.
         *
         * \returns true iff implicit learning can used for this decision.
         */
        bool makeRSILDecision() noexcept;
                
        /// The counter used to keep track of the number of branching decisions modulo the
        /// intervention probability half-life.
        uint64_t m_callCounter;
        
        /// The intervention probability half-life (in terms of overall performed decisions)
        uint64_t m_probHalfLife;
        
        /// The mask used on a pseudorandom number for deciding whether to use implicit learning:
        /// If the current probability of using IL is 2^{-P}, the P lowest bits of m_mask are set
        /// to 1. To decide whether to use IL, a pseudorandom number X is generated; if X & m_mask
        /// is 0, implicit learning is used, otherwise not.
        fastnextrand_state_t m_mask;
        
    };
    
    /**
     * \ingroup RS_ImplicitLearning
     *
     * A specialization of RSILVanishingBranchingHeuristic<n> with n=3.
     */
    using RSILVanishingBranchingHeuristic3 = RSILVanishingBranchingHeuristic<AdviceEntry<3>>;
    
    /**
     * \ingroup RS_ImplicitLearning
     *
     * A specialization of RSILVanishingBranchingHeuristic<n> with n=2.
     */
    using RSILVanishingBranchingHeuristic2 = RSILVanishingBranchingHeuristic<AdviceEntry<2>>;

    
    //******* Implementation ************************************************************
     
    namespace BranchingHeuristicsImpl {
        // The following provides an implementation of usedAdvice() and canUseAdvice() for each supported advice entry
        // type, via SFINAE. It is required that ...AdviceEntry<n>::BasicType == ...AdviceEntry<m>::BasicType for all
        // admissible values of n and m. Therefore, it suffices to compare types with ...AdviceEntry<2>::BasicType.
        
        template<typename AdviceType>
        static inline
        typename std::enable_if<std::is_same<typename AdviceType::BasicType, AdviceEntry<2>::BasicType>::value, void>::type
        usedAdvice(AdviceType& advice, size_t index) noexcept {
            (void)advice;
            (void)index;
        }
        
        template<typename AdviceType>
        static inline
        typename std::enable_if<std::is_same<typename AdviceType::BasicType, AdviceEntry<2>::BasicType>::value, bool>::type
        canUseAdvice(AdviceType& advice, size_t index) noexcept {
            (void)advice;
            (void)index;
            return true;
        }
        
        template<typename AdviceType>
        static inline
        typename std::enable_if<std::is_same<typename AdviceType::BasicType, BudgetAdviceEntry<2>::BasicType>::value, void>::type
        usedAdvice(AdviceType& advice, size_t index) noexcept {
            assert (index < advice.getSize());
            assert (advice.getBudget(index) > 0);
            advice.setBudget(index, advice.getBudget(index)-1);
        }
        
        template<typename AdviceType>
        static inline
        typename std::enable_if<std::is_same<typename AdviceType::BasicType, BudgetAdviceEntry<2>::BasicType>::value, bool>::type
        canUseAdvice(AdviceType& advice, size_t index) noexcept {
            assert (index < advice.getSize());
            return advice.getBudget(index) > 0;
        }
    }
    
    template<class AdviceType>
    inline FastRandomNumberGenerator& RSILBranchingHeuristic<AdviceType>::getRandomNumberGenerator() noexcept {
        return m_rng;
    }
    
    template<class AdviceType>
    ATTR_ALWAYSINLINE
    inline Lit RSILBranchingHeuristic<AdviceType>::getAdvice() noexcept {
        if (trail.trail_lim.size() == 0) {
            return lit_Undef;
        }
        
        auto trailStart = trail.trail_lim.back();
        auto scanLen = trail.size() - trailStart;
        auto randomNumber = trail.trail_lim.size(); // decision level "randomness" suffices here
        
        for (decltype(scanLen) j = 0; j < scanLen; ++j) {
            auto i = trailStart + j;
            
            Lit cursor = trail[i];
            Var variable = cursor.var();
            
            if (!m_advice.hasPotentialAdvice(variable)) {
                continue;
            }
            
            auto& advice = m_advice.getAdvice(variable);
            
            if (advice.isBackbone()) {
                continue;
            }
            
            size_t adviceSize = advice.getSize();
            for (decltype(adviceSize) a = 0; a < adviceSize; ++a) {
                auto idx = (randomNumber + a) % adviceSize;
                
                auto advisedLit = advice.getLiteral(idx);
                if (trail.value(advisedLit.var()) == l_Undef
                     && this->trail.isDecisionVar(advisedLit.var())
                     && BranchingHeuristicsImpl::canUseAdvice(advice, idx)
                    ) 
                {
                    BranchingHeuristicsImpl::usedAdvice(advice, idx);
                    auto result = cursor.sign() ? ~advisedLit : advisedLit;
                    return result;
                }
            }
        }
        
        return lit_Undef;
    }
    
    template<class AdviceType> ATTR_ALWAYSINLINE
    inline Lit RSILBranchingHeuristic<AdviceType>::getSignAdvice(Lit literal) noexcept {
        if (m_backbonesEnabled) {
            Var decisionVariable = literal.var();
            if (m_advice.hasPotentialAdvice(decisionVariable)) {
                auto& advice = m_advice.getAdvice(decisionVariable);
                
                if (advice.isBackbone() && BranchingHeuristicsImpl::canUseAdvice(advice, 0) && advice.getSize() == 1) {
                    assert(advice.getLiteral(0) != lit_Undef);
                    BranchingHeuristicsImpl::usedAdvice(advice, 0);
                    return ~(advice.getLiteral(0));
                }
            }
        }
        return literal;
    }
    
    // Note: The RSILBudgetBranchingHeuristic implementation itself is not concerned with
    // actually updating the AdviceEntry objects, and does not directly alter the behaviour
    // of getAdvice(). Any changes to the behaviour of getAdvice() (selecting implications,
    // updating advice entries) are realized by specializations of usedAdvice() and
    // canUseAdvice() for BudgetAdviceEntry - see the BranchingHeuristicsImpl namespace
    // for further details.
    
    template<class AdviceType>
    inline bool RSILVanishingBranchingHeuristic<AdviceType>::makeRSILDecision() noexcept {
        auto randomNumber = this->getRandomNumberGenerator()();
        bool result = (randomNumber & m_mask) == 0ull;
        // update call counter
        assert(m_callCounter > 0);
        --m_callCounter;
        if (m_callCounter == 0) {
            m_mask = (m_mask << 1) | 1ull;
            m_callCounter = m_probHalfLife;
        }
        return result;
    }

}

#endif
