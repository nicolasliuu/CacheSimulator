
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "Set.h"
#include "Cache.h"
#include <cmath>
#include <iostream>
using namespace std;

Cache::Cache(int num_sets, int blocks, int size, std::string alloc, std::string writeThru, std::string lruOrFifo) {      
    cacheSets = num_sets;
    numBlocks = blocks;
    blockSize = size;
    set_index_bits = log(num_sets) / log(2);
    block_offset_bits = log(size) / log(2);
    tag_bits = 32 - (set_index_bits + block_offset_bits);
    writeAlloc = alloc;
    writeThru_back = writeThru;
    lru_fifo = lruOrFifo;
    globalCounter = 0;

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

void Cache::printParameters(){
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
    
uint32_t Cache::getTag(string address) {
    uint32_t decimalAddress = stoul(address, 0, 16);

    return decimalAddress >> (block_offset_bits + set_index_bits);
}

uint32_t Cache::getIndex(string address) {
    uint32_t decimalAddress = stoul(address, 0, 16);

    // Get rid of right most bits of decimalAddress
    decimalAddress = decimalAddress >> block_offset_bits;
    // Create a mask to isolate the set_index_bits
    uint32_t mask = (1 << set_index_bits) - 1;

    // Discard all bits other than index bits
    return decimalAddress & mask;
}

uint32_t Cache::getOffset(string address) {
    uint32_t decimalAddress = stoul(address, 0, 16);

    // Create mask for block offset
    uint32_t mask = (1 << block_offset_bits) - 1;

    return decimalAddress & mask;
}

void Cache::loadAddress(string address) { 
    // Get the index of the address and check the particular Set at that index, call hasSlot
    uint32_t index = getIndex(address);
    uint32_t tag = getTag(address);

    if (sets.at(index).hasSlot(tag)) { //hit
        loadHits++;
        // Cache hit, update according to lru/fifo
        Slot* slot = sets.at(index).getSlot(tag, globalCounter); // Update access time to globalCounter
        // Only loading to cache so increase totalCycles by 1
        totalCycles++;         
    }
    else { //miss
        loadMisses++;
        // Set the tag for the slot
        bool evicted = sets.at(index).addSlot(tag, lru, fifo, globalCounter);
        Slot* slot = sets.at(index).getSlot(tag, globalCounter);

        // Check if we just evicted a dirty bit with the writeback policy
        if (writeBack && evicted && slot->isDirty()) {
            totalCycles += (100 * (blockSize / 4));
            slot->setDirty(false);
        }

        // Update cycle:
        // Loading from main memory + load to cache,
        totalCycles += (100 * (blockSize / 4));
        totalCycles++;
    }
    // Increment Total Loads
    totalLoads++;
    // Increment globalCounter
    globalCounter++;
}

void Cache::storeAddress(string address) {
    int index = getIndex(address); // key for map of sets
    int tag = getTag(address); // key for map of slots

    if(sets[index].hasSlot(tag)) { //hit
        Slot* slot = sets[index].getSlot(tag, globalCounter);
        storeHits++;
        
        if (writeBack) {
            slot->setDirty(true);
            totalCycles++;
        } else { // writeThru
            totalCycles += 100; // We are only writing a 4-byte value   // (100 * (blockSize / 4));
            totalCycles++;
        }
    } else { //miss
        storeMisses++;

        if(writeAllocate) {

            bool evicted = sets[index].addSlot(tag, lru, fifo, globalCounter);
            Slot* slot = sets[index].getSlot(tag, globalCounter);

            if(evicted && slot->isDirty()) {
                totalCycles += (100 * (blockSize / 4));
                slot->setDirty(false);
            } else {
                totalCycles++;
            }

            totalCycles += 100 * (blockSize / 4);
        }

        if(noWriteAllocate) {
            totalCycles += 100;
        }
        
    }
    totalStores++;
    // Increment globalCounter
    globalCounter++;
}

void Cache::incrementGlobalCounter() {
    globalCounter++;
}

uint64_t Cache::getGlobalCounter() {
    return globalCounter;
}

void Cache::printStatistics() {
    cout << "Total loads: " << totalLoads << "\n";
    cout << "Total stores: " << totalStores << "\n";
    cout << "Load hits: " << loadHits << "\n";
    cout << "Load misses: " << loadMisses << "\n";
    cout << "Store hits: " << storeHits << "\n";
    cout << "Store misses: " << storeMisses << "\n";
    cout << "Total cycles: " << totalCycles << "\n";
}