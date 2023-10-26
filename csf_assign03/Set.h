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
                int highestCounterIndex = 0;
                for (int i = 1; i < slots.size(); ++i) {
                    if (slots[i].getAccess_ts() > slots[highestCounterIndex].getAccess_ts()) {
                        highestCounterIndex = i;
                    }
                }

                slots[highestCounterIndex].setTag(tag);
                slots[highestCounterIndex].setValid(true);
                slots[highestCounterIndex].setAccess_ts(0);  // Resetting counter

                // Increment the counters for all other slots to signify they've aged by one cycle.
                for (int i = 0; i < slots.size(); ++i) {
                    if (i != highestCounterIndex) {  // All slots except the one we've just accessed.
                        slots[i].increaseAccess_ts(); 
                    }
                }

                // // Overwrite whatever is in the vector for now
                // slots[0].setTag(tag);
                // slots[0].setValid(true);

                // FIFO
            } else {
                // Set not full, make new slot and adjust counters as necessary
                Slot newSlot;
                newSlot.setTag(tag);
                newSlot.setValid(true);
                newSlot.setAccess_ts(0);  // This is the most recently used entry.

                slots.push_back(newSlot);
                currBlocks++;

                // As before, we increment the counters for all other entries.
                for (int i = 0; i < slots.size() - 1; ++i) {  // 'slots.size() - 1' because we don't want to increment the counter for the new entry.
                    slots[i].increaseAccess_ts();
                }
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

        int getCurrBlocks() {
            return currBlocks;
        }

        
};
#endif  