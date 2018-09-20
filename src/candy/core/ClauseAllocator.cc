/*
 * ClauseAllocator.cc
 *
 *  Created on: Feb 20, 2017
 *      Author: markus
 */

#include "candy/core/Clause.h"
#include <candy/core/ClauseAllocator.h>

#include <vector>
#include <iostream>
#include <algorithm>

namespace Candy {

constexpr unsigned int ClauseAllocator::INITIAL_PAGE_SIZE;

ClauseAllocator::ClauseAllocator() : pages(), cursor(0), page_size(INITIAL_PAGE_SIZE) {
    memory = (unsigned char*)std::malloc(INITIAL_PAGE_SIZE);
    pages.push_back(memory);
}

ClauseAllocator::~ClauseAllocator() {
    for (unsigned char* page : pages) {
        std::free((void*)page);
    }
}

std::vector<Clause*> ClauseAllocator::defrag(std::vector<Clause*> keep) {
    std::vector<Clause*> keep2 {};
    keep2.reserve(keep.size());
    page_size *= 2*pages.size();
    std::vector<unsigned char*> oldpages;
    oldpages.swap(pages);
    memory = (unsigned char*)std::malloc(page_size);
    pages.push_back(memory);
    cursor = 0;
    for (Clause* clause : keep) {
        Clause* clause2 = new (allocate(clause->size())) Clause((const Clause&)*clause);
        keep2.push_back(clause2);
    }
    for (unsigned char* page : oldpages) {
        free((void*)page);
    }
    assert(pages.size() == 1);
    return keep2;
}

} /* namespace Candy */
