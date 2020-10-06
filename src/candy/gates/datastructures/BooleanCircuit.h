#ifndef CANDY_BOOLEANCIRCUIT_H
#define CANDY_BOOLEANCIRCUIT_H

#include <vector>
#include "Gate.h"
#include "GateVertex.h"
#include "../../../../../candy-kingdom-thesisfork/src/candy/gates/datastructures/TupleNotation.h"

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

    vector<GateVertex> gates;
    vector<vector<GateVertex>> outEdges;
    vector<vector<GateVertex>> inEdges;

public: // todo: move implementation to cc
    static const vector<int> emptyClause;
    static TupleNotation const emptyFormula;
    static Gate const nullGate;
    static GateVertex const nullVertex;

    const vector<GateVertex> &getGates() const {
        return gates;
    }

    const vector<vector<GateVertex>> &getOutEdges() const {
        return outEdges;
    }

    const vector<vector<GateVertex>> &getInEdges() const {
        return inEdges;
    }

    static GateVertex getNullVertex() {
        return nullVertex;
    }

    /**
     * Adds a gate to the Boolean circuit that is known to not already be contained in the instance.
     * @param gate the gate to be added to the Boolean circuit
     * @return the ID of the gate added
     */
    unsigned long addUniqueGate(const Gate& gate) {
        GateVertex vertex {++this->lastID, gate};
        gates.push_back(vertex);
        return vertex.getId();
    }

    GateVertex& getGateVertex(const unsigned long id) {
        return gates.at(id - 1);
    }

    /**
     * Add an edge to the Boolean circuit that is known to not already be contained in the instance.
     * @param fromID the gate from which the edge goes
     * @param toID the gate to which the edge goes
     */
    void addUniqueEdge(const unsigned long fromID, const unsigned long toID) {
        outEdges.at(fromID - 1).push_back(getGateVertex(toID));
        inEdges.at(toID - 1).push_back(getGateVertex(fromID));
    }

    unsigned long getNumGates() const {
        return gates.size();
    }

    unsigned long getNumEdges() const {
        return outEdges.size();
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
    vector<GateVertex>& getOutEdges(const GateVertex& gate) {
        return outEdges.at(gate.getId() - 1);
    }

    /**
     * Get the ingoing edges from the gate with the given ID.
     * @param id the ID of the gate
     * @return the ingoing edges of the gate
     */
    vector<GateVertex>& getInEdges(const unsigned long id) {
        return inEdges.at(id - 1);
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

    const GateVertex& hop(const unsigned long id, const TupleNotation formula) { // todo add unification
        bool unifies = true;
        for (GateVertex& hopVertex : getOutEdges(id)) {
            for (GateVertex& targetVertex : getInEdges(hopVertex.getId())) {
                if (unifies) return targetVertex;
            }
        }
        return nullVertex;
    }

    //const vector<GateVertex*> hopTwo(const unsigned long id, const TupleNotation formula) { // todo implement this function properly
    //    bool unifies = true;
    //    // vector<GateVertex*> leftRight { &GateVertex::getNullVertex(), &GateVertex::getNullVertex() }; // todo: maybe make null objects static
    //    return leftRight;
    //}

    /*const GateVertex& traverse(const unsigned long id) {
        if (outDegree(id) == 1) {
            return getOutEdges(id).at(0);
        } else {
            return unsuccessfull;
        }
    }*/
};


#endif //CANDY_BOOLEANCIRCUIT_H