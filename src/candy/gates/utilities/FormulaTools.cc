
#include <candy/core/CNFProblem.h>
#include <candy/gates/GateAnalyzer.h>
#include <fstream>
#include "FormulaTools.h"

unsigned int countVariables(const string &formula) {
    unsigned int max = 0;
    istringstream stream(formula);
    for (string s; stream >> s; ) {
        unsigned int var = abs(stoi(s));
        if (var > max) {
            max = var;
        }
    }
    return max;
}

unsigned int countClauses(const string &formula) {
    unsigned int count = 0;
    for (unsigned long lastFound = 0; lastFound != string::npos; lastFound = formula.find(" 0", ++lastFound)) {
        ++count;
    }
    return count;
}

string generateDIMACSHeader(const string &formula) {
    return "p cnf "
           + to_string(countVariables(formula)) + " "
           + to_string(countClauses(formula));
}

unsigned int getMaxVar(const vector<int> &literals) {
    unsigned int max = 0;
    for (const auto literal : literals) if (abs(literal) > max) max = abs(literal);
    return max;
}

void buildOutEdges(BooleanCircuit& circuit, const GateVertex& gate) {
    for (const auto& toGate : circuit.getGates()) {
        const auto& inVariables = toGate.getGate().getInVariables();
        if (find(inVariables.begin(), inVariables.end(), abs(gate.getGate().getOutLiteral())) != inVariables.end()) {
            circuit.addUniqueEdge(gate.getId(), toGate.getId());
        }
    }
}

void buildExplicitConnections(BooleanCircuit& circuit) {
    for (const auto& gate : circuit.getGates()) {
        buildOutEdges(circuit, gate);
    }
}

void writeToDIMACS(const TupleNotation &formula, const string &filename) {
    string outFilename = filename + ".cnf";
    ofstream oFile(outFilename);
    string formulaAsDIMACS = formula.toDIMACS();
    oFile << generateDIMACSHeader(formulaAsDIMACS) << endl << formulaAsDIMACS << endl;
    oFile.close();
}

Recognition::Gate toRecognitionGate(const Candy::Gate& gate) {
    For charFunction = gate.fwd;
    charFunction.insert(charFunction.end(), gate.bwd.begin(), gate.bwd.end());
    Recognition::Gate newGate(TupleNotation(gate.fwd), gate.out);
    return newGate;
}

BooleanCircuit toCircuit(const GateProblem& gateProblem) {
    BooleanCircuit circuit;
    for (const Candy::Gate& gate : gateProblem.gates) {
        //if (gate.isDefined()) circuit.addUniqueGate(gate); todo: convert gate
    }
    buildExplicitConnections(circuit);
    return circuit;
}

BooleanCircuit toCircuit(For& formula) {
    CNFProblem problem(formula);
    GateAnalyzer gateAnalyzer{ problem };
    gateAnalyzer.analyze();
    GateProblem gateProblem = gateAnalyzer.getResult();
    return toCircuit(gateProblem);
}

GateProblem gateProblemFromDIMACS(const string& filename) {
    CNFProblem problem;
    problem.readDimacsFromFile(filename.c_str());
    GateAnalyzer gateAnalyzer { problem };
    gateAnalyzer.analyze();
    return gateAnalyzer.getResult();
}

BooleanCircuit toCircuit(const TupleNotation &formula) {
    string filename = "tmpDIMACS";
    writeToDIMACS(formula, filename);
    return toCircuit(gateProblemFromDIMACS(filename));
}