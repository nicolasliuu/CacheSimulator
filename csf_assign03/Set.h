#ifndef _SET_H
#define _SET_H

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "Slot.h"

class Set {
    private: 
        std::vector<Slot> slots;//indices aren't associated with tags
        int maxBlocks;
        int currBlocks;

    public:

        Set() : maxBlocks(0), currBlocks(0) {} // Default constructor

        Set(int numBlocks) { 
            maxBlocks = numBlocks;
            currBlocks = 0;
            // Initialize the slots
            // for (int i = 0; i < numBlocks; ++i) {
            //     slots[i] = Slot();
            // }
        }

        Slot* getSlot(uint32_t tag) {
            // return slots[tag];
            for(int i = 0; i < slots.size(); i++) {
                if (slots[i].getTag() == tag && slots[i].isValid()) {
                    return &slots[i];
                }
            }
            return nullptr;
        }

        void addSlot(uint32_t tag) {
            // Check if the set is full, if yes we need to evict, otherwise just add the new slot
            if (currBlocks == maxBlocks) {
                // LRU


                // FIFO
            } else {
                // Find a slot that's not valid
                slots.emplace_back(); // Create a new slot at the end of the vector.
                Slot& newSlot = slots.back(); // Reference to the newly created slot.
                newSlot.setTag(tag);
                newSlot.setValid(true);
                currBlocks++;
            }
            return;
        }

        bool hasSlot(uint32_t tag) {
            // return slots.find(tag) != slots.end();
            for(int i = 0; i < slots.size(); i++) {
                if (slots[i].getTag() == tag) {
                    return true;
                }
            }
            return false;
        }

        bool isFull() {
            return currBlocks == maxBlocks;
        }

        
};
#endif  