#ifndef _SET_H
#define _SET_H

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "Slot.h"

class Set {
    // int cacheSets;
    // int numBlocks;
    // int blockSize;
    // string writeAlloc;
    // string writeThru_back;
    // string lru_fifo;
    // bool writeAllocate;
    // bool noWriteAllocate;
    // bool writeThru;
    // bool writeBack;
    // bool lru;
    // bool fifo;
    std::unordered_map<uint32_t, Slot> slots;

    //put functions below (implement if the functions are small enough??)
    public:
        Slot getSlot(uint32_t key) {
            return slots.at(key);
        }

};
#endif  