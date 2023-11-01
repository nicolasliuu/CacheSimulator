#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "Slot.h"
#include "Set.h"
using namespace std;

Set::Set() : maxBlocks(0), currBlocks(0) {} // Default constructor

Set::Set(int numBlocks) { 
    maxBlocks = numBlocks;
    currBlocks = 0;
}

// Slot* Set::getSlot(uint32_t tag) {
//     // return slots[tag];
//     for(int i = 0; i < slots.size(); i++) {
//         if (slots[i].getTag() == tag && slots[i].isValid()) {
//             // We have a hit, update according to lru/fifo (LRU for MS2)
//             // Check if timestamp is highest
//             int slot_ts = slots[i].getAccess_ts();

//             for (int i = 0; i< slots.size(); i++) {
//                 if (slots[i].getAccess_ts() < slot_ts) {
//                     slots[i].increaseAccess_ts();
//                 }
//             }
            
//             // Set the access_ts of the hit to 0
//             slots[i].setAccess_ts(0);
//             return &slots[i];
            
//         }
//     }
//     return nullptr;
// }

Slot* Set::getSlot(uint32_t tag, uint64_t globalCounter) {
    for(int i = 0; i < slots.size(); i++) {
        if (slots[i].getTag() == tag && slots[i].isValid()) {
            slots[i].setAccess_ts(globalCounter);  // Update last accessed timestamp.
            return &slots[i];
        }
    }
    return nullptr; // Not found
}


bool Set::addSlot(uint32_t tag, bool lru, bool fifo, uint64_t globalCounter) {//returns true if block evicted
    // Check if the set is full, if yes we need to evict, otherwise just add the new slot
    if (currBlocks == maxBlocks) {
        // LRU
        if(lru) {
            int lowestCounterIndex = 0;
            for (int i = 1; i < slots.size(); ++i) {
                if (slots[i].getAccess_ts() < slots[lowestCounterIndex].getAccess_ts()) {
                    lowestCounterIndex = i;
                }
            }

            slots[lowestCounterIndex].setTag(tag);
            slots[lowestCounterIndex].setValid(true);
            slots[lowestCounterIndex].setAccess_ts(globalCounter);  // Resetting counter

            // Increment the counters for all other slots to signify they've aged by one cycle.
            // for (int i = 0; i < slots.size(); ++i) {
            //     if (i != lowestCounterIndex) {  // All slots except the one we've just accessed.
            //         slots[i].increaseAccess_ts(); 
            //     }
            // }            
        }else if(fifo) {
            //increment load_ts in every block
            //find highest load_ts
            //set highest load_ts to 0, set valid to true, set tag to new tag
            for (int i = 0; i < slots.size(); i++) {
                slots[i].increaseLoad_ts();
            }

            int lowestCounterIndex = 0;
            for (int i = 1; i < slots.size(); ++i) {
                if (slots[i].getLoad_ts() > slots[lowestCounterIndex].getLoad_ts()) {
                    lowestCounterIndex = i;
                }
            }

            slots[lowestCounterIndex].setLoad_ts(0);
            slots[lowestCounterIndex].setValid(true);
            slots[lowestCounterIndex].setTag(tag);
        }
        return true;
    } else { // Set not full, make new slot and adjust counters as necessary
        if(lru) {
            Slot newSlot;
            newSlot.setTag(tag);
            newSlot.setValid(true);
            newSlot.setAccess_ts(globalCounter);  // This is the most recently used entry.

            slots.push_back(newSlot);
            currBlocks++;

            // As before, we increment the counters for all other entries.
            // for (int i = 0; i < slots.size() - 1; ++i) {  // 'slots.size() - 1' because we don't want to increment the counter for the new entry.
            //     slots[i].increaseAccess_ts();
            // }
        } else if(fifo) {
            //increment load_ts in every block
            //insert new block with load_ts = 0

            for (int i = 0; i < slots.size(); i++) {
                slots[i].increaseLoad_ts();
            }

            Slot newSlot;
            newSlot.setTag(tag);
            newSlot.setValid(true);
            newSlot.setAccess_ts(0);  // This is the last inserted entry.

            slots.push_back(newSlot);
            currBlocks++;
        }
        return false;
    }
}

bool Set::hasSlot(uint32_t tag) {
    // return slots.find(tag) != slots.end();
    for(int i = 0; i < slots.size(); i++) {
        if (slots[i].getTag() == tag) {
            return true;
        }
    }
    return false;
}

bool Set::isFull() {
    return currBlocks == maxBlocks;
}