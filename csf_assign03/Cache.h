// Cache.h
#ifndef _CACHE_H
#define _CACHE_H

#include <string>
#include <unordered_map>
#include "Set.h"

class Cache {

private:
    int cacheSets;
    int numBlocks;
    int blockSize;
    std::string writeAlloc;
    std::string writeThru_back;
    std::string lru_fifo;
    bool writeAllocate;
    bool noWriteAllocate;
    bool writeThru;
    bool writeBack;
    bool lru;
    bool fifo;
    uint64_t counter;
    int set_index_bits;
    int block_offset_bits;
    int tag_bits;
    std::unordered_map<uint32_t, Set> sets;
    int totalLoads = 0;
    int totalStores = 0;
    int loadHits = 0;
    int loadMisses = 0;
    int storeHits = 0;
    int storeMisses = 0;
    int totalCycles = 0;


public:
    Cache(int num_sets, int blocks, int size, std::string alloc, std::string writeThru, std::string lruOrFifo);
    void printParameters();
    uint32_t getTag(std::string address);
    uint32_t getIndex(std::string address);
    uint32_t getOffset(std::string address);
    void incrementCounter();
    void loadAddress(std::string address);
    void storeAddress(std::string address);
    bool inCache(std::string address);
    void printStatistics();
};

#endif  // _CACHE_H