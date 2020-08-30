#ifndef GBD_HASH_H
#define GBD_HASH_H

#include <string>

#include "candy/core/CNFProblem.h"
#include "candy/utils/StreamBuffer.h"

namespace Candy {

class Hash {
public:
    Hash();

    std::string gbd_hash_from_dimacs(const char* filename);
    std::string gbd_hash(Candy::CNFProblem& problem);
    std::string degree_hash(Candy::CNFProblem& problem);

};

}

#endif