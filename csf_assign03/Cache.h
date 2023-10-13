#ifndef _CACHE_H
#define _CACHE_H

#include <cstdlib>
#include <string>
#include <unordered_map>

class Cache {
    int cacheSets;
    int numBlocks;
    int blockSize;
    string writeAlloc;
    string writeThru_back;
    string lru_fifo;
    bool writeAllocate;
    bool noWriteAllocate;
    bool writeThru;
    bool writeBack;
    bool lru;
    bool fifo;
    uint64_t counter; //number of addresses read so far (regardless of l/s or fifo/lru), use this to set load_ts/access_ts
    
    std::unordered_map<uint32_t, Set> sets;
    
    //put functions below


}