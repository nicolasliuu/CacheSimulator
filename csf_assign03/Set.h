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
        // std::unordered_map<uint32_t, Slot> slots;//easier if this were a vector T-T
        std::vector<Slot> slots;//indices aren't associated with tags
        int maxBlocks;
        int currBlocks;

    public:

        Set() : maxBlocks(0), currBlocks(0) {} // Default constructor

        Set(int numBlocks) { 
            maxBlocks = numBlocks;
            currBlocks = 0;
            // Initialize the slots
            for (int i = 0; i < numBlocks; ++i) {
                slots[i] = Slot();
            }
        }

        Slot& getSlot(uint32_t tag) {
            // return slots[tag];
            for(int i = 0; i < slots.size(); i++) {
                if (slots[i].getTag() == tag) {
                    return slots[i];
                }
            }
        }

        void updateSlot(uint32_t tag) {
            // Check if the set is full
            if (currBlocks == maxBlocks) {
                // LRU


                // FIFO
            } else {
                // // Find slot with tag 0, then add the tag
                //     for (auto& slotPair : slots) {
                //         Slot& slot = slotPair.second;

                //         if (!slot.isValid()) {
                //             // We found an invalid slot, so use this one.
                //             slot.setTag(tag);
                //             slot.setValid(true);

                //             // Update slot timestamps

                //             return;
                //         }
                //     }
            }
            slots[tag] = Slot();
            slots[tag].setTag(tag);
            slots[tag].setValid(true);
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