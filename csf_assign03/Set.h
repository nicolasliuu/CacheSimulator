#ifndef _SET_H
#define _SET_H

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "Slot.h"

class Set {
    private: 
        std::unordered_map<uint32_t, Slot> slots;
        int maxBlocks;

    public:

        Set() : maxBlocks(0) { }

        Set(int numBlocks) : maxBlocks(numBlocks) { }

        Slot& getSlot(uint32_t tag) {
            return slots[tag];
        }

        void addSlot(uint32_t tag) {
            slots[tag] = Slot();
        }

        bool hasSlot(uint32_t tag) {
            return slots.find(tag) != slots.end();
        }

        bool isFull() {
            return slots.size() == maxBlocks;
        }

        
};
#endif  