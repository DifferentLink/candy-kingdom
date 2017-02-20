/*
 * ClauseAllocator.h
 *
 *  Created on: Feb 20, 2017
 *      Author: markus
 */

#ifndef SRC_CANDY_CORE_CLAUSEALLOCATOR_H_
#define SRC_CANDY_CORE_CLAUSEALLOCATOR_H_

#include <vector>
#include <cstdint>

namespace Candy {

class Clause;

class ClauseAllocator {

public:
    ClauseAllocator(uint32_t _number_of_pools, uint32_t _elements_per_pool);
    virtual ~ClauseAllocator();

    void* allocate(uint32_t length);
    void deallocate(Clause*);

    uint32_t clauseBytes(uint32_t length);

private:
    std::vector<std::vector<void*>> pools;
    std::vector<char*> pages;

    uint32_t number_of_pools;
    uint32_t initial_elements_per_pool;

    uint32_t stats_active_total = 0;
    uint32_t stats_active_long = 0;
    std::vector<uint32_t> stats_active_counts;

    std::vector<void*>& getPool(int32_t length);
    void refillPool(int32_t nElem, int32_t clause_size);

};

} /* namespace Candy */

#endif /* SRC_CANDY_CORE_CLAUSEALLOCATOR_H_ */
