
#ifndef CANDY_GATECLASSIFIER_H
#define CANDY_GATECLASSIFIER_H

#include <vector>
#include <candy/gates/GateProblem.h>
#include <candy/gates/datastructures/BooleanCircuit.h>

using namespace std;

class GateClassifier {
public:
    enum characteristicFunction {
        cUNDEF,
        cEQUIV,
        cOR,
        cHALFOR,
        cAND,
        cHALFAND,
        cXOR,
        cCONSTRAINTGATE
    };
private:
    const BooleanCircuit* circuit;
    vector<characteristicFunction> classification;
public:
    explicit GateClassifier(const BooleanCircuit* circuit) {
        this->circuit = circuit;
    }

    static characteristicFunction patternCheck(const TupleGate &gate);
    static characteristicFunction classify(const TupleGate &gate);
    void classify();
    vector<characteristicFunction> getResult();
    characteristicFunction getResult(const GateVertex& gate);
};


#endif //CANDY_GATECLASSIFIER_H
