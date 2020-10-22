#ifndef CANDY_BOOLEANCIRCUIT_H
#define CANDY_BOOLEANCIRCUIT_H

#include <vector>
#include <fstream>
#include <candy/gates/GateAnalyzer.h>
#include <candy/gates/utilities/SequentialCounterEncoder.h>
#include "GateVertex.h"
#include "TupleNotation.h"

using namespace std;

/**
 * This class models a Boolean circuit as a graph implemented on an adjacency list.
 * Additionally, the data structure saves reversed edges to improve the time complexity
 * of some of the operations provided.
 */
class BooleanCircuit {
private:
    /**
     * IDs start counting at 1, while 0 indicates an unsuccessful operation.
     * All procedures guarantee that the the vertex with the corresponding ID
     * is saved in the gates array at position ID - 1.
     */
    unsigned long lastID = 0;

    vector<GateVertex> gates { };
    vector<vector<GateVertex>> outEdges { };
    vector<vector<GateVertex>> inEdges { };

    TupleNotation remainder;

    static bool containsClause(const vector<GateVertex>& gates, const vector<int> &clause) {
        for (const auto& gate : gates) {
            for (const auto& gateClause : gate.getFormula().getFormula()) {
                if (gateClause == clause) return true;
            }
        }
            return false;
    }

    static TupleNotation initRemainer(const BooleanCircuit& circuit, const TupleNotation& formula) {
        vector<vector<int>> remainder;
        for (const auto& clause : formula.getFormula()) {
            if (!BooleanCircuit::containsClause(circuit.gates, clause)) remainder.push_back(clause);
        }
        return TupleNotation(remainder);
    }

public: // todo: move implementation to cc
    static vector<int> const emptyClause;
    static TupleNotation const emptyFormula;
    static GateVertex const nullVertex;

    BooleanCircuit() = default;

    explicit BooleanCircuit(const string& formula, const bool hasHeader = false) {
        CNFProblem problem;
        string filename = "tmp";
        ofstream oFile(filename);
        string out;
        if (!hasHeader) out = SequentialCounterEncoder::generateDIMACSHeader(formula);
        out = out + formula;
        oFile << out << endl;
        oFile.close();
        problem.readDimacsFromFile(filename.c_str());
        GateAnalyzer gateAnalyzer { problem };
        gateAnalyzer.analyze();
        GateProblem gateProblem = gateAnalyzer.getResult();
        for (const Gate& gate : gateProblem.gates) {
            if (gate.isDefined()) {
                addUniqueGate(gate);
            }
        }
        For problemFor (problem.begin(), problem.end());
        remainder = initRemainer(*this, TupleNotation(problemFor));
    }

    explicit BooleanCircuit(vector<GateVertex>& gates, const TupleNotation& remainder) {
        for (GateVertex& gate : gates) {
            addUniqueGate(gate);
        }
        this->remainder = remainder;
    }

    explicit BooleanCircuit(CNFProblem& problem) {
        GateAnalyzer gateAnalyzer { problem };
        gateAnalyzer.analyze();
        GateProblem gateProblem = gateAnalyzer.getResult();
        for (const Gate& gate : gateProblem.gates) {
            if (gate.isDefined()) {
                addUniqueGate(gate);
            }
        }
        For problemFor (problem.begin(), problem.end());
        remainder = initRemainer(*this, TupleNotation(problemFor));
    }

    const vector<GateVertex> &getGates() const {
        return gates;
    }

    const vector<vector<GateVertex>> &getOutEdges() const {
        return outEdges;
    }

    const vector<vector<GateVertex>> &getInEdges() const {
        return inEdges;
    }

    TupleNotation& getRemainder() {
        return remainder;
    }

    static GateVertex getNullVertex() {
        return nullVertex;
    }

    const GateVertex& findGateByOutput(const Lit outLiteral) const {
        for (const auto& gate : gates) if (gate.getGate().out == outLiteral) return gate;
        return nullVertex;
    }

    bool hasInput(const GateVertex& gate, const unsigned int variable) {
        for (const unsigned int inVariable : gate.getInVariables()) {
            if (inVariable == variable) return true;
        }
        return false;
    }


    bool hasInput(const GateVertex& gate, const int literal) {
        for (const unsigned int variable : gate.getInVariables()) {
            if (variable == (unsigned int) abs(literal)) return true;
        }
        return false;
    }

    /**
     * Adds a gate to the Boolean circuit that is known to not already be contained in the instance.
     * @param gate the gate to be added to the Boolean circuit
     * @return the ID of the gate added
     */
    unsigned long addUniqueGate(const Gate& gate) {
        GateVertex vertex { ++this->lastID, gate };
        gates.push_back(vertex);
        outEdges.emplace_back();
        inEdges.emplace_back();
        for (const auto& existingGate : gates) {
            if (hasInput(existingGate, vertex.getOutLiteral())) addUniqueEdge(vertex, existingGate);
            if (hasInput(vertex, existingGate.getOutLiteral())) addUniqueEdge(existingGate, vertex);
        }
        return vertex.getId();
    }

    unsigned long addUniqueGate(GateVertex& gate) {
        gate.setId(++this->lastID);
        gates.push_back(gate);
        outEdges.emplace_back();
        inEdges.emplace_back();
        for (const auto& existingGate : gates) {
            if (hasInput(existingGate, gate.getOutLiteral())) addUniqueEdge(gate, existingGate);
            if (hasInput(gate, existingGate.getOutLiteral())) addUniqueEdge(existingGate, gate);
        }
        return gate.getId();
    }

    GateVertex& getGateVertex(const unsigned long id) {
        return gates.at(id - 1);
    }

private:
    /**
     * Add an edge to the Boolean circuit that is known to not already be contained in the instance.
     * @param fromID the gate from which the edge goes
     * @param toID the gate to which the edge goes
     */
    void addUniqueEdge(const GateVertex& from, const GateVertex& to) {
        outEdges.at(from.getId() - 1).push_back(getGateVertex(to.getId()));
        inEdges.at(to.getId() - 1).push_back(getGateVertex(from.getId()));
    }

public:
    unsigned long numGates() const {
        return gates.size();
    }

    unsigned long numEdges() const {
        unsigned int numEdges = 0;
        for (const auto& vertex : outEdges) if (!vertex.empty()) numEdges++;
        return numEdges;
    }

    /**
     * Get the outgoing edges from the gate with the given ID.
     * @param id the ID of the gate
     * @return the outgoing edges of the gate
     */
    vector<GateVertex>& getOutEdges(const unsigned long id) {
        return outEdges.at(id - 1);
    }

    /**
     * Get the outgoing edges from the given gate.
     * @param the gate
     * @return the outgoing edges of the gate
     */
    const vector<GateVertex>& getOutEdges(const GateVertex& gate) const {
        return outEdges.at(gate.getId() - 1);
    }

    /**
     * Get the ingoing edges from the gate with the given ID.
     * @param id the ID of the gate
     * @return the ingoing edges of the gate
     */
    const vector<GateVertex>& getInEdges(const unsigned long id) const {
        return inEdges.at(id - 1);
    }

    /**
     * Get the ingoing edges from the gate with the given ID.
     * @param id the ID of the gate
     * @return the ingoing edges of the gate
     */
    const vector<GateVertex>& getInEdges(const GateVertex& gate) const {
        return inEdges.at(gate.getId() - 1);
    }

    /**
     * Get the number of edges going out of the gate with the given ID.
     * @param id the ID of the gate
     * @return the number of outgoing edges from the gate
     */
    unsigned long outDegree(const unsigned long id) const {
        return outEdges.at(id - 1).size();
    }

    /**
     * Get the number of edges going to the gate with the given ID.
     * @param id the ID of the gate
     * @return the number of ingoing edges from the gate
     */
    unsigned long inDegree(const unsigned long id) const {
        return inEdges.at(id - 1).size();
    }

    static bool in(const GateVertex& x, const vector<GateVertex>& xs) {
        for (const auto& y : xs) if (x.getId() == y.getId()) return true;
        return false;
    }

    static inline void removeAll(const GateVertex& gate, vector<GateVertex>& gates) {
        gates.erase(remove(gates.begin(), gates.end(), gate), gates.end());
    }

    BooleanCircuit deleteGates(vector<GateVertex>& gatesToRemove) {
        vector<GateVertex> remainingGates;
        remainingGates.reserve(gates.size() - gatesToRemove.size());
        for (auto gate : this->gates) {
            if (!in(gate, gatesToRemove)) {
                remainingGates.push_back(gate);
            }
        }

        return BooleanCircuit(remainingGates, this->remainder);
    }

    void depr_deleteGates(const vector<GateVertex>& gates) {
        vector<GateVertex> ids;
        ids.reserve(gates.size());
        for (const auto& gate : gates) ids.push_back(gate);
        vector<GateVertex> newGates;
        vector<vector<GateVertex>> newOutEdges;
        vector<vector<GateVertex>> newInEdges;
        unsigned int long i = 0;
        for (auto& gate : this->gates) {
            if (!in(gate, gates)) {
                newOutEdges.push_back(this->outEdges.at(gate.getId() - 1));
                newInEdges.push_back(this->inEdges.at(gate.getId() - 1));
                gate.setId(i + 1);
                newGates.push_back(gate);
                i++;
            }
        }
        for (vector<GateVertex>& edges : newOutEdges) {
            for (const auto& gate : ids) removeAll(gate, edges);
        }
        for (vector<GateVertex>& edges : newInEdges) {
            for (const auto& gate : ids) removeAll(gate, edges);
        }
        this->gates = newGates;
        this->outEdges = newOutEdges;
        this->inEdges = newInEdges;
        this->lastID = i;
    }

    string toDIMACS() {
        string dimacsCircuit;
        for (const auto& gate : this->gates) {
            dimacsCircuit.append(gate.getFormula().toDIMACS());
        }
        dimacsCircuit.append(this->remainder.toDIMACS());
        return dimacsCircuit;
    }
};


#endif //CANDY_BOOLEANCIRCUIT_H