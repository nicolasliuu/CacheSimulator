#ifndef _CACHE_H
#define _CACHE_H

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "Set.h"
#include <cmath>
using namespace std;

class Cache {
    public:
        int cacheSets;
        int numBlocks;
        int blockSize;
        std::string writeAlloc;
        std::string writeThru_back;
        string lru_fifo;
        bool writeAllocate;
        bool noWriteAllocate;
        bool writeThru;
        bool writeBack;
        bool lru;
        bool fifo;
        uint64_t counter; //number of addresses read so far (regardless of l/s or fifo/lru), use this to set load_ts/access_ts
        int set_index_bits;
        int block_offset_bits;
        int tag_bits;        

        std::unordered_map<uint32_t, Set> sets;

        int totalLoads;
        int totalStores;
        int loadHits;
        int loadMisses;
        int storeHits;
        int storeMisses;
        int totalCycles;

        Cache(int num_sets, int blocks, int size, string alloc, string writeThru, string lruOrFifo) {
            cacheSets = num_sets;
            numBlocks = blocks;
            blockSize = size;
            set_index_bits = log(num_sets) / log(2);
            block_offset_bits = log(size) / log(2);
            tag_bits = 32 - (set_index_bits + block_offset_bits);
            writeAlloc = alloc;
            writeThru_back = writeThru;
            lru_fifo = lruOrFifo;

            // Create the cache sets
            for (int i = 0; i < cacheSets; ++i) {
                sets[i] = Set(numBlocks);
            }

            if(writeAlloc.compare("write-allocate") == 0) {
                writeAllocate = true;
                noWriteAllocate = false;
            } else {
                writeAllocate = false;
                noWriteAllocate = true;
            }

            if(writeThru_back.compare("write-through") == 0) {
                writeThru = true;
                writeBack = false;
            } else {
                writeThru = false;
                writeBack = true;
            }

            if(lru_fifo.compare("lru") == 0) {
                lru = true;
                fifo = false;
            } else {
                lru = false;
                fifo = true;
            }
        }

        void printParameters(){
            cout << "cacheSets: " << cacheSets << "\n";
            cout << "numBlocks: " << numBlocks << "\n";
            cout << "blockSize: " << blockSize << "\n";
            cout << "writeAllocate: " << writeAllocate << "\n";
            cout << "noWriteAllocate: " << noWriteAllocate << "\n";
            cout << "writeThru: " << writeThru << "\n";
            cout << "writeBack: " << writeBack << "\n";
            cout << "lru: " << lru << "\n";
            cout << "fifo: " << fifo << "\n";
            cout << "t" << tag_bits << "\n";
            cout << "b" << block_offset_bits << "\n";
            cout << "s" << set_index_bits << "\n";
        }
    
    uint32_t getTag(string address) {
        uint32_t decimalAddress = stoul(address, 0, 16);

        return decimalAddress >> (block_offset_bits + set_index_bits);
    }

   uint32_t getIndex(string address) {
        uint32_t decimalAddress = stoul(address, 0, 16);

        // Get rid of right most bits of decimalAddress
        decimalAddress = decimalAddress >> block_offset_bits;
        // Create a mask to isolate the set_index_bits
        uint32_t mask = (1 << set_index_bits) - 1;

        // Discard all bits other than index bits
        return decimalAddress & mask;
    }

    uint32_t getOffset(string address) {
        uint32_t decimalAddress = stoul(address, 0, 16);

        // Create mask for block offset
        uint32_t mask = (1 << block_offset_bits) - 1;

        return decimalAddress & mask;
    }

    void incrementCounter() {
        counter++;
    }

    void loadAddress(string address) {
        // Get the index of the address and check the particular Set at that index, call hasAddress
        uint32_t index = getIndex(address);
        uint32_t tag = getTag(address);

        if (sets.at(index).hasSlot(tag)) {
            // Cache hit
            Slot& slot = sets.at(index).getSlot(tag);

            if (slot.isValid() && slot.getTag() == tag) {
                // Increment load hits 
                loadHits++;

                // Update information for lru/fifo
                if (lru) {
                    // nothing for now, just testing counts
                }

                // Update cycle:
                totalCycles++;          // Only loading to cache
            } else {
                loadMisses++;           // Load miss
            }
        }
        else {
            // Set the tag for the slot
            sets.at(index).updateSlot(tag);
            Slot& slot = sets.at(index).getSlot(tag);
            
            // Cache miss
            loadMisses++;   

            // Change slot metadata
            slot.setValid(true);
            slot.setTag(tag);

            // Update information for lru/fifo
            if (lru) {
                // nothing for now, just testing counts
            }

            // Update cycle:
            // Loading from main memory + load to cache,
            totalCycles += 101;
        }
        // Increment Total Loads
        totalLoads++;
    }

    void storeAddress(string address) {
        //hit
            //if write thru is true
                //increase storeHits, totalStores
                //totalCycles += 101

            //if write back is true
                //set dirty = true
                //totalCycles++

        //miss
            //if write allocate is true
                //put address in cache
                //totalCycles++

            //if no write allocate is true
                //totalCycles += 100

        int index = getIndex(address); // key for map of sets
        int tag = getTag(address); // key for map of slots

        if(sets[index].hasSlot(tag)) { //hit
            Slot& slot = sets[index].getSlot(tag);
            storeHits++;
            

            if(writeThru) { //write to cache and memory
                totalCycles += 101;
            } 

            if(writeBack) { //write to cache only and write to memory when block is evicted
                slot.setDirty(true);
                totalCycles++;
            }
        } else { //miss
            if(writeAllocate) {
                if(sets[index].isFull()) { //need to evict
                    if(lru) {
                        //iterate thru set hashmap and see which block has lowest access_ts
                    }
                }

                sets[index].addSlot(tag);
                if(writeBack) {
                    sets[index].getSlot(tag).setDirty(true);
                }

                if(lru) {
                    sets[index].getSlot(tag).setAccess_ts(counter);
                }
                totalCycles++;
            }

            if(noWriteAllocate) {
                totalCycles += 100;
            }
        }
    }

    bool inCache(string address) {
        //go to spot in hashmap where the address should be
        //check valid
        //if not valid, not in cache

        int index = getIndex(address); // key for map of sets
        int tag = getTag(address); // key for map of slots
        Set addressSet = sets.at(index);
        if (addressSet.hasSlot(tag)) {
            Slot addressSlot = addressSet.getSlot(tag);
        } else {
            return false;
        }
        // return addressSlot.isValid();

    }

    void printStatistics() {
        cout << "Total loads: " << totalLoads << "\n";
        cout << "Total stores: " << totalStores << "\n";
        cout << "Load hits: " << loadHits << "\n";
        cout << "Load misses: " << loadMisses << "\n";
        cout << "Store hits: " << storeHits << "\n";
        cout << "Store misses: " << storeMisses << "\n";
        cout << "Total cycles: " << totalCycles << "\n";
    }
};
#endif