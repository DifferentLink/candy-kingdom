add_subdirectory(algorithms)
add_subdirectory(datastructures)

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
    utilities/FormulaTools.h)