#include "BooleanCircuit.h"

const vector<int> BooleanCircuit::emptyClause = { };
const TupleNotation BooleanCircuit::emptyFormula = TupleNotation();
const Gate BooleanCircuit::nullGate = Gate({}, emptyFormula, 0);
const GateVertex BooleanCircuit::nullVertex = GateVertex(0, BooleanCircuit::nullGate);