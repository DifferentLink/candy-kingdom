add_library(gates OBJECT
    GateAnalyzer.cc
    GateAnalyzer.h
    GateDFSTraversal.h
    GateProblem.h
    AIGProblem.h
    MiterDetector.h
    MiterDetector.cc
    utilities/IOTools.h
    utilities/FormulaTools.cc
    utilities/FormulaTools.h
    algorithms/GateClassifier.cc
    algorithms/GateClassifier.h
    algorithms/BooleanCircuitVisualizer.cc
    algorithms/BooleanCircuitVisualizer.h
    algorithms/Unificator.cc
    algorithms/Unificator.h
    algorithms/GraphBasedSequentialCounterRecognizer.cc
    algorithms/GraphBasedSequentialCounterRecognizer.h
    algorithms/SyntacticalSequentialCounterRecognizer.cc
    algorithms/SyntacticalSequentialCounterRecognizer.h
    datastructures/TupleNotation.h
    datastructures/BooleanCircuit.h
    datastructures/GateVertex.h
    datastructures/Signature.h
    datastructures/StructuralFormula.h
    datastructures/PermutationList.h
    datastructures/BooleanCircuit.cc datastructures/TupleGate.h)