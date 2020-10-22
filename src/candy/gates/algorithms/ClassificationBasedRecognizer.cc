#include "ClassificationBasedRecognizer.h"

ClassificationBasedRecognizer::ClassificationBasedRecognizer(BooleanCircuit& circuit)
        : classifier(GateClassifier(&circuit)) {
    this->formula = circuit.getRemainder();
    this->circuit = circuit;
    this->classifier.classify();
}

const GateVertex& ClassificationBasedRecognizer::findABaseGate() {
    for (const auto& gate : this->circuit.getGates()) {
        if (classifier.getResult(gate) == GateClassifier::cHALFAND) return gate;
    }
    return BooleanCircuit::nullVertex;
}

vector<int> ClassificationBasedRecognizer::findBaseClause() {
    vector<int> clause;
    for (unsigned int i = 0; i < formula.size(); i++) {
        clause = formula.clauseAt(i);
        if (Unificator::unifyClauseL2({ "-x_2", "-s_1_1"}, clause, this->eta)) {
            this->formula.mark(i);
            return clause;
        }
    }
    return { };
}

/**
 * Finds a single gate realized by a half-and and searches the remaining clauses to complete the n = 2, k = 1 case
 * @return A structural sequential counter encoding
 */
StructuralFormula ClassificationBasedRecognizer::recognizeBaseCase() {
    const GateVertex& baseGate = findABaseGate();
    if (GateVertex::isNull(baseGate)) return StructuralFormula{TupleNotation(), { } };
    eta.insert({"x_1", baseGate.getInVariables().at(0) + 1 });
    eta.insert({"s_1_1", (unsigned int) abs(baseGate.getOutLiteral())  + 1 });
    vector<int> clause = findBaseClause();
    if (!clause.empty()) {
        if (clause.at(0) != eta.find("s_1_1")->second) {
            eta.insert({ "x_2", abs(clause.at(0)) });
        } else {
            eta.insert({ "x_2", abs(clause.at(1)) });
        }
    }
    return StructuralFormula{ TupleNotation(clause), { baseGate } };
}

unsigned int determineConstraintVariable(const GateVertex& previousGate, const GateVertex& nextGate) {
    unsigned int previousOut = abs(previousGate.getOutLiteral() + 1);
    vector<int> clause1 = nextGate.getFormula().clauseAt(0);
    vector<int> clause2 = nextGate.getFormula().clauseAt(1);
    if (clause1.at(0) != previousOut && clause1.at(1) != previousOut) return abs(clause1.at(0));
    if (clause2.at(0) != previousOut && clause2.at(1) != previousOut) return abs(clause2.at(0));
    return 0;
}

StructuralFormula ClassificationBasedRecognizer::recognizek1(const StructuralFormula& baseCase) {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    if (baseCase.getCircuit().size() != 1) return emptyStructuralFormula;
    const GateVertex& baseGate = baseCase.getCircuit().at(0);
    vector<GateVertex> gamma;
    GateVertex currentGate = BooleanCircuit::nullVertex;
    for (const auto& gate : circuit.getOutEdges(baseGate)) {
        if (classifier.getResult(gate) == GateClassifier::cHALFAND) currentGate = gate;
    }
    if (GateVertex::isNull(currentGate)) return emptyStructuralFormula;
    unsigned int x_2 = determineConstraintVariable(baseGate, currentGate);
    const auto& mx_2 = eta.find("x_2");
    if (mx_2->second != x_2) {
        if (mx_2 == eta.end()) {
            eta.insert({ "x_2", x_2 });
        } else {
            return emptyStructuralFormula;
        }
    }
    eta.insert({ "s_2_1", abs(currentGate.getOutLiteral()) + 1 });
    gamma.push_back(currentGate);
    GateVertex traversalGate = currentGate;
    unsigned int i = 3;
    do {
        traversalGate = BooleanCircuit::nullVertex;
        for (const auto& gate : circuit.getOutEdges(currentGate)) {
            if (classifier.getResult(gate) == GateClassifier::cHALFAND) {
                traversalGate = gate;
                currentGate = traversalGate;
                gamma.push_back(currentGate);
                eta.insert({ "x_" + to_string(i), determineConstraintVariable(currentGate, traversalGate) });
                eta.insert({ "s_" + to_string(i++) + "_1", abs(traversalGate.getOutLiteral()) + 1 });
                break;
            }
        }
    } while (!GateVertex::isNull(traversalGate));

    return StructuralFormula{ TupleNotation(), gamma }; // todo: also return missing clauses
}

GateVertex ClassificationBasedRecognizer::overHop(const GateVertex &previousGate, const GateVertex &currentGate) {
    for (const auto& toGate : circuit.getOutEdges(currentGate)) {
        for (const auto& fromGate : circuit.getInEdges(toGate)) {
            if (fromGate.getId() != previousGate.getId() && fromGate.getId() != currentGate.getId()
                    && classifier.getResult(fromGate) == GateClassifier::cCONSTRAINTGATE) {
                return fromGate;
            }
        }
    }
    return BooleanCircuit::nullVertex;
}

GateVertex ClassificationBasedRecognizer::underHop(const GateVertex &previousGate, const GateVertex &currentGate) {
    for (const auto& toGate : circuit.getInEdges(currentGate)) {
        for (const auto& fromGate : circuit.getOutEdges(toGate)) {
            if (fromGate.getId() != previousGate.getId() && fromGate.getId() != currentGate.getId()
                && classifier.getResult(fromGate) == GateClassifier::cCONSTRAINTGATE) {
                return fromGate;
            }
        }
    }
    return BooleanCircuit::nullVertex;
}

StructuralFormula ClassificationBasedRecognizer::recognizeRectangle(const StructuralFormula& baseCase,
                                                                    const StructuralFormula& nColumn) {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    GateVertex previousGate = BooleanCircuit::nullVertex;
    GateVertex currentGate = previousGate;
    GateVertex traversalGate = currentGate;
    vector<GateVertex> gamma;

    unsigned int j = 2;
    for (unsigned int i = 0; i < nColumn.getCircuit().size(); i++) {
        previousGate = nColumn.getCircuit().at(i);
        currentGate = previousGate;
        traversalGate = overHop(currentGate, currentGate);
        if (GateVertex::isNull(traversalGate)) break;
        gamma.push_back(traversalGate);
        eta.insert({ "s_" + to_string(i + 2) + "_" + to_string(j), abs(traversalGate.getOutLiteral()) + 1 });
        currentGate = traversalGate;
        j++;
        do {
            traversalGate = overHop(previousGate, currentGate);
            previousGate = currentGate;
            currentGate = traversalGate;
            if (!GateVertex::isNull(traversalGate)) {
                gamma.push_back(traversalGate);
                eta.insert({ "s_" + to_string(i + 2) + "_" + to_string(j),
                             abs(traversalGate.getOutLiteral()) + 1 });
                unsigned int s_i_jm = abs(traversalGate.getFormula().clauseAt(0).at(0));
                if (eta.find("x_" + to_string(i + 2))->second == s_i_jm) {
                    s_i_jm = abs(traversalGate.getFormula().clauseAt(0).at(1));
                }
                eta.insert({ "s_" + to_string(i + 1) + "_" + to_string(j - 1), s_i_jm });
                eta.insert({ "s_" + to_string(i + 1) + "_" + to_string(j),
                             abs(traversalGate.getFormula().clauseAt(1).at(0)) });
                j++;
            }
        } while (!GateVertex::isNull(traversalGate));
        j = 1;
    }
    previousGate = nColumn.getCircuit().back();
    currentGate = previousGate;
    traversalGate = underHop(currentGate, currentGate);
    if (GateVertex::isNull(traversalGate)) return emptyStructuralFormula;
    gamma.push_back(traversalGate);
    j = 2;
    eta.insert({ "s_" + to_string(nColumn.getCircuit().size() + 1) + "_" + to_string(j),
                 abs(traversalGate.getOutLiteral()) + 1 });
    currentGate = traversalGate;
    do {
        traversalGate = underHop(previousGate, currentGate);
        previousGate = currentGate;
        currentGate = traversalGate;
        if (!GateVertex::isNull(traversalGate)) {
            j++;
            gamma.push_back(traversalGate);
            eta.insert({ "s_" + to_string(nColumn.getCircuit().size() + 1) + "_" + to_string(j),
                         abs(traversalGate.getOutLiteral()) + 1 });
        }
    } while (!GateVertex::isNull(traversalGate));
    this->n = nColumn.getCircuit().size() + 4;
    this->k = j;
    return StructuralFormula(TupleNotation(), gamma);
}

StructuralFormula ClassificationBasedRecognizer::recognizeClosing(const StructuralFormula& rectangle) {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    unsigned int numGates = rectangle.getCircuit().size();
    if (numGates < 3) return emptyStructuralFormula;
    const GateVertex& minus1 = rectangle.getCircuit().at(numGates - 1);
    const GateVertex& minus2 = rectangle.getCircuit().at(numGates - 2);

    GateVertex first = circuit.getOutEdges(minus1).at(0);
    eta.insert({ "s_" + to_string(n - 2) + "_" + to_string(k), abs(first.getOutLiteral()) + 1 });
    GateVertex second = circuit.getOutEdges(minus2).at(0);
    eta.insert({ "s_" + to_string(n - 2) + "_" + to_string(k - 1), abs(second.getOutLiteral()) + 1 });
    if (second.getId() == first.getId()) second = circuit.getOutEdges(minus2).at(1);
    if (first.getFormula().clauseAt(0).at(0) == second.getFormula().clauseAt(0).at(0)
    || first.getFormula().clauseAt(0).at(1) == second.getFormula().clauseAt(0).at(0)) {
        eta.insert({ "x_" + to_string(n - 2), abs(second.getFormula().clauseAt(0).at(0)) });
    } else if (first.getFormula().clauseAt(0).at(0) == second.getFormula().clauseAt(0).at(1)
               || first.getFormula().clauseAt(0).at(1) == second.getFormula().clauseAt(0).at(1)) {
        eta.insert({ "x_" + to_string(n - 2), abs(second.getFormula().clauseAt(0).at(1)) });
    } else {
        return emptyStructuralFormula;
    }
    GateVertex third = circuit.getOutEdges(first).at(0);
    if (abs(third.getFormula().clauseAt(0).at(0)) == abs(second.getOutLiteral()) + 1) {
        eta.insert({ "x_" + to_string(n - 1), abs(third.getFormula().clauseAt(0).at(1)) });
    } else if (abs(third.getFormula().clauseAt(0).at(1)) == abs(second.getOutLiteral()) + 1) {
        eta.insert({ "x_" + to_string(n - 1), abs(third.getFormula().clauseAt(0).at(0)) });
    }
    eta.insert({ "s_" + to_string(n - 1) + "_" + to_string(k), abs(third.getOutLiteral()) + 1 });

    return StructuralFormula(TupleNotation(), { first, second, third });
}

StructuralFormula ClassificationBasedRecognizer::recognizeFloating() {
    vector<GateVertex> gamma;
    for (const auto& gate : circuit.getGates()) {
        if (circuit.getOutEdges(gate).empty()
         && circuit.getInEdges(gate).empty()
         && classifier.getResult(gate) == GateClassifier::cCONSTRAINTGATE) {
            gamma.push_back(gate);
        }
    }
    return StructuralFormula(TupleNotation(), gamma);
}

StructuralFormula ClassificationBasedRecognizer::recognizeSpecialCasesZeroColumn(StructuralFormula& baseCase) {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    vector<GateVertex> gamma = { baseCase.getCircuit().front() };
    GateVertex &baseGate = gamma.at(0);
    unsigned long size = circuit.getOutEdges(baseGate).size();
    if (size == 1) {
        if (classifier.getResult(circuit.getOutEdges(baseGate).front()) == GateClassifier::cCONSTRAINTGATE) {
            GateVertex totoGate = circuit.getOutEdges(baseGate).at(0);
            gamma.push_back(totoGate);
            if (circuit.getOutEdges(totoGate).empty()) { // n = 3, k = 2 case
                return StructuralFormula({ TupleNotation(), gamma });
            } else if (circuit.getOutEdges(totoGate).size() == 1 // n = 4, k = 3 case
                && classifier.getResult(circuit.getOutEdges(totoGate).front()) == GateClassifier::cCONSTRAINTGATE) {
                gamma.push_back(totoGate);
                GateVertex closing = underHop(baseGate, baseGate);
                if (classifier.getResult(closing) == GateClassifier::cCONSTRAINTGATE) {
                    gamma.push_back(closing);
                    StructuralFormula floating = recognizeFloating();
                    if (floating.getCircuit().empty()) return emptyStructuralFormula;
                    gamma.push_back(floating.getCircuit().front());
                    return StructuralFormula(TupleNotation(), gamma);
                }
            }
        }
    } else if (size == 2) {
        GateVertex andOut = baseCase.getCircuit().at(0);
        GateVertex andOrOut = baseCase.getCircuit().at(1);
        if (classifier.getResult(andOut) != GateClassifier::cAND) swap(andOut, andOrOut);
        if (circuit.getOutEdges(andOut).empty()) {
            return recognizeClosing(emptyStructuralFormula);
        } else if (circuit.getOutEdges(andOut).size() == 1 ) {
            if (classifier.getResult(circuit.getOutEdges(andOut).at(0)) == GateClassifier::cCONSTRAINTGATE) {

            }
        }
    }
    return emptyStructuralFormula;
}

StructuralFormula ClassificationBasedRecognizer::recognizeSpecialCasesOneColumn(StructuralFormula& baseCase) {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    vector<GateVertex> gamma = { baseCase.getCircuit().front() };
    GateVertex &baseGate = gamma.at(0);
    unsigned long size = circuit.getOutEdges(baseGate).size();
    if (size == 1) { // n = 3, k = 1
        GateVertex toGate = circuit.getOutEdges(baseGate).front();
        if (circuit.getOutEdges(toGate).empty()) {
            return StructuralFormula(TupleNotation(), { baseGate, toGate });
        }
    } else if (size == 2) {
        GateVertex andOut = circuit.getOutEdges(baseGate).at(0);
        unsigned int pos = 1;
        if (classifier.getResult(andOut) != GateClassifier::cHALFAND) {
            andOut = circuit.getOutEdges(baseGate).at(1);
            pos = 0;
        }
        GateVertex andOrOut = circuit.getOutEdges(baseGate).at(pos);
        if (circuit.getOutEdges(andOut).empty()) { // n = 5, k = 4 case
            GateVertex firstRowSecond = overHop(andOrOut, andOrOut);
            GateVertex firstRowThird = overHop(andOrOut, firstRowSecond);
            if (!GateVertex::isNull(firstRowSecond)
             && !GateVertex::isNull(firstRowThird)
             && classifier.getResult(firstRowSecond) == GateClassifier::cCONSTRAINTGATE
             && classifier.getResult(firstRowThird) == GateClassifier::cCONSTRAINTGATE
             && circuit.getOutEdges(andOrOut).size() == 1
             && circuit.getOutEdges(firstRowSecond).size() == 2
             && circuit.getOutEdges(firstRowThird).size() == 1) {
                GateVertex secondRowFirst = circuit.getOutEdges(andOrOut).front();
                GateVertex secondRowSecond = circuit.getOutEdges(firstRowThird).front();
                if (classifier.getResult(secondRowFirst) == GateClassifier::cCONSTRAINTGATE
                 && classifier.getResult(secondRowSecond) == GateClassifier::cCONSTRAINTGATE
                 && circuit.getOutEdges(secondRowSecond).size() == 1
                 && circuit.getOutEdges(secondRowSecond).size() == 1) {
                    GateVertex thirdRowFirst = circuit.getOutEdges(secondRowFirst).front();
                    if (classifier.getResult(firstRowSecond) == GateClassifier::cCONSTRAINTGATE) {
                        StructuralFormula floating = recognizeFloating();
                        if (floating.getCircuit().size() == 2) {
                            gamma = { baseGate, andOut, andOrOut, firstRowSecond, firstRowThird, secondRowFirst,
                                      secondRowSecond, thirdRowFirst,
                                      floating.getCircuit().at(0), floating.getCircuit().at(1) };
                            return StructuralFormula(TupleNotation(), gamma);
                        }
                    }
                }
            }

            return recognizeClosing(emptyStructuralFormula);
        } else if (circuit.getOutEdges(andOut).size() == 1 && circuit.getOutEdges(andOrOut).size() == 1 ) {
            GateVertex lastGate = circuit.getOutEdges(andOut).at(0);
            if (classifier.getResult(lastGate) == GateClassifier::cCONSTRAINTGATE) {
                if (circuit.getOutEdges(lastGate).empty()) { // n = 4, k = 2
                    gamma.push_back(andOut);
                    gamma.push_back(andOrOut);
                    gamma.push_back(lastGate);
                    return StructuralFormula(TupleNotation(), gamma);
                }
            }
        } else if (circuit.getOutEdges(andOut).size() == 1 && circuit.getOutEdges(andOrOut).size() == 2 ) {
            GateVertex lastGate = circuit.getOutEdges(andOut).at(0);
            if (circuit.getOutEdges(lastGate).size() == 1) {
                GateVertex firstRowLast = overHop(andOut, andOrOut);
                if (circuit.getOutEdges(firstRowLast).size() == 1) {
                    GateVertex secondRowLast = circuit.getOutEdges(firstRowLast).front();
                    if (circuit.getOutEdges(secondRowLast).size() == 1) {
                        GateVertex thirdRowLast = circuit.getOutEdges(secondRowLast).front();
                        if (circuit.getOutEdges(thirdRowLast).empty()) {
                            gamma.push_back(andOut);
                            gamma.push_back(andOrOut);
                            gamma.push_back(lastGate);
                            gamma.push_back(firstRowLast);
                            gamma.push_back(secondRowLast);
                            gamma.push_back(thirdRowLast);
                            StructuralFormula floating = recognizeFloating();
                            if (floating.getCircuit().empty()) return emptyStructuralFormula;
                            gamma.push_back(floating.getCircuit().front());
                            return StructuralFormula(TupleNotation(), gamma);
                        }
                    }
                }
            }
        }
    }
    return emptyStructuralFormula;
}

StructuralFormula ClassificationBasedRecognizer::recognizeSpecialCasesTwoColumn(StructuralFormula& baseCase, 
                                                                                StructuralFormula& k1Result) {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    GateVertex baseGate = baseCase.getCircuit().front();
    vector<GateVertex> gamma;
    GateVertex andOut = circuit.getOutEdges(baseGate).at(0);
    unsigned int pos = 1;
    if (classifier.getResult(andOut) != GateClassifier::cHALFAND) {
        andOut = circuit.getOutEdges(baseGate).at(1);
        pos = 0;
    }
    GateVertex andOrOut = circuit.getOutEdges(baseGate).at(pos);
    gamma.push_back(baseCase.getCircuit().front());
    gamma.insert(gamma.end(), k1Result.getCircuit().begin(), k1Result.getCircuit().end());
    gamma.push_back(andOrOut);
    if (classifier.getResult(andOrOut) == GateClassifier::cCONSTRAINTGATE) {
        if (circuit.getOutEdges(andOrOut).size() == 1) {
            GateVertex nextGate = circuit.getOutEdges(andOrOut).front();
            gamma.push_back(nextGate);
            if (classifier.getResult(nextGate) == GateClassifier::cCONSTRAINTGATE
             && circuit.getOutEdges(nextGate).size() == 1
             && circuit.getOutEdges(k1Result.getCircuit().back()).size() == 1) {
                GateVertex lastGate = circuit.getOutEdges(k1Result.getCircuit().back()).front();
                gamma.push_back(lastGate);
                if (circuit.getOutEdges(lastGate).empty()) return StructuralFormula(TupleNotation(), gamma);
            }
        } else if (circuit.getOutEdges(andOrOut).size() == 2) {
            GateVertex firstRowThird = overHop(andOut, andOrOut);
            if (!GateVertex::isNull(firstRowThird)
             && classifier.getResult(firstRowThird) == GateClassifier::cCONSTRAINTGATE
             && circuit.getOutEdges(firstRowThird).size() == 1) {
                GateVertex secondRowThird = circuit.getOutEdges(firstRowThird).front();
                GateVertex secondRowSecond = overHop(secondRowThird, secondRowThird);
                if (!GateVertex::isNull(secondRowThird) && !GateVertex::isNull(secondRowSecond)
                 && circuit.getOutEdges(secondRowThird).size() == 1) {
                    GateVertex thirdRowSecond = circuit.getOutEdges(secondRowThird).front();
                    GateVertex thirdRowFirst = overHop(thirdRowSecond, thirdRowSecond);
                    if (!GateVertex::isNull(thirdRowSecond) && !GateVertex::isNull(thirdRowFirst)
                     && circuit.getOutEdges(thirdRowSecond).size() == 1
                     && circuit.getOutEdges(thirdRowFirst).size() == 1) {
                        GateVertex final = circuit.getOutEdges(thirdRowFirst).front();
                        if (classifier.getResult(final) == GateClassifier::cCONSTRAINTGATE) {
                            gamma.push_back(firstRowThird);
                            gamma.push_back(secondRowSecond);
                            gamma.push_back(secondRowThird);
                            gamma.push_back(thirdRowFirst);
                            gamma.push_back(thirdRowSecond);
                            gamma.push_back(final);
                            StructuralFormula floating = recognizeFloating();
                            if (floating.getCircuit().size() == 1) {
                                gamma.push_back(floating.getCircuit().front());
                                return StructuralFormula(TupleNotation(), gamma);
                            }
                        }
                    }
                }
            }
        }
    }

    return emptyStructuralFormula;
}

StructuralFormula ClassificationBasedRecognizer::removeDuplicates(const StructuralFormula &formula) {
    set<unsigned int> gateIDs;
    for (const auto& gate : formula.getCircuit()) gateIDs.insert(gate.getId());
    vector<GateVertex> gates;
    gates.reserve(gateIDs.size());
    for (const auto& id : gateIDs) gates.push_back(circuit.getGateVertex(id));
    return StructuralFormula(formula.getFormula(), gates);
}

StructuralFormula ClassificationBasedRecognizer::recognize() {
    StructuralFormula emptyStructuralFormula = StructuralFormula{ TupleNotation(), { } };
    vector<GateVertex> gamma;
    StructuralFormula baseCase  = recognizeBaseCase();
    if (baseCase.getCircuit().empty()) {
        return emptyStructuralFormula;
    } else if (baseCase.getCircuit().size() == 1 && circuit.getOutEdges(baseCase.getCircuit().front()).empty()) {
        return baseCase;
    }
    StructuralFormula k1Result  = recognizek1(baseCase);
    if (k1Result.getCircuit().empty()) return recognizeSpecialCasesZeroColumn(baseCase);
    gamma.push_back(baseCase.getCircuit().front());
    gamma.insert(gamma.end(), k1Result.getCircuit().begin(), k1Result.getCircuit().end());
    if (k1Result.getCircuit().size() == 1) return recognizeSpecialCasesOneColumn(baseCase);
    StructuralFormula rectangle = recognizeRectangle(baseCase, k1Result);
    rectangle = removeDuplicates(rectangle);
    if (rectangle.getCircuit().size() < 6
     && k1Result.getCircuit().size() == 2
     && circuit.getOutEdges(baseCase.getCircuit().front()).size() == 2) {
        return recognizeSpecialCasesTwoColumn(baseCase, k1Result);
    }
    if (rectangle.getCircuit().empty()) return StructuralFormula(TupleNotation(), gamma);
    if (circuit.getOutEdges(k1Result.getCircuit().back()).empty()) {
        StructuralFormula closing   = recognizeClosing(rectangle);
        StructuralFormula floating  = recognizeFloating();
        if (!closing.getCircuit().empty() && !floating.getCircuit().empty()) {
            gamma.insert(gamma.end(), rectangle.getCircuit().begin(), rectangle.getCircuit().end());
            gamma.insert(gamma.end(), closing.getCircuit().begin(), closing.getCircuit().end());
            gamma.insert(gamma.end(), floating.getCircuit().begin(), floating.getCircuit().end());
        }
    } else { // k = 2 cases
        if (circuit.getOutEdges(rectangle.getCircuit().back()).size() == 1) {
            GateVertex last = circuit.getOutEdges(rectangle.getCircuit().back()).front();
            for (const auto& outGate : circuit.getOutEdges(k1Result.getCircuit().back())) {
                if (outGate.getId() == last.getId()) {
                    gamma.insert(gamma.end(), rectangle.getCircuit().begin(), rectangle.getCircuit().end());
                    gamma.push_back(last);
                    return StructuralFormula(TupleNotation(), gamma);
                }
            }
        }
    }
    return StructuralFormula(TupleNotation(), gamma);
}

void ClassificationBasedRecognizer::deleteGates(vector<GateVertex> &gates) {
    this->circuit = circuit.deleteGates(gates);
    this->classifier = GateClassifier(&this->circuit);
    this->classifier.classify();
}

BooleanCircuit &ClassificationBasedRecognizer::getCircuit() {
    return this->circuit;
}
