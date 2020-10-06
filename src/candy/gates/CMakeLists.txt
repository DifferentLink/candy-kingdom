add_subdirectory(algorithms)
add_subdirectory(datastructures)
add_subdirectory(utilities)

add_library(gates OBJECT
    GateAnalyzer.cc
    GateAnalyzer.h
    GateDFSTraversal.h
    GateProblem.h
    AIGProblem.h
    MiterDetector.h
    MiterDetector.cc)