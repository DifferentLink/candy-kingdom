/*************************************************************************************************
Candy -- Copyright (c) 2015-2020, Markus Iser, KIT - Karlsruhe Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **************************************************************************************************/

#ifndef SRC_CANDY_EVENTS_H_
#define SRC_CANDY_EVENTS_H_

#include <deque>
#include <vector>
#include <algorithm>
#include <assert.h>

namespace Candy {

template<class T> class Events {
private:
    std::deque<T> events;
    std::vector<unsigned int> pos;

public:
    Events() : events(), pos() {}
    ~Events() {}

    // register new consumer, return consumer id
    unsigned int register() {
        pos.push_back(0);
        return pos.size() - 1;
    }

    // push new event to queue
    void push(T event) {
        events.push_back(T);
    }

    // return true iff consumer has new events
    bool has_events(unsigned int id) {
        assert(id < pos.size()); // consumer is registered
        return pos[id] < events.size();
    }

    // return event at consumer position, increment consumer position
    T consume(unsigned int id) {
        assert(pos[id] < events.size()); // new events exist
        unsigned int cpos = pos[id];
        pos[id]++;
        return events[cpos];
    }

    // remove alread consumed events from queue
    void cleanup() {
        unsigned int min = std::min(pos.begin(), pos.end());
        for_each(pos.begin(), pos.end(), [min](unsigned int& p) { p -= min; });
        events.erase(events.begin(), events.begin()+min);
    }

};

}

#endif