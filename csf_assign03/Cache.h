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

        Cache(int sets, int blocks, int size, string alloc, string writeThru, string lruOrFifo) {
            cacheSets = sets;
            numBlocks = blocks;
            blockSize = size;
            set_index_bits = log(sets) / log(2);
            block_offset_bits = log(size) / log(2);
            tag_bits = 32 - (set_index_bits + block_offset_bits);
            writeAlloc = alloc;
            writeThru_back = writeThru;
            lru_fifo = lruOrFifo;

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
    
    //put functions below

    int getTag(string address) {
        //direct: 12 bits tag, 12 bits index, 8 bits offset
        //associative: 24 bits tag, 8 bits offset
        //set associative: 14 bits tag, 10 bits index, 8 bits offset

        //direct
            //get tag: bitwise shift right 20 bits
            //get index: shift left 12 bits then shift right 20 bits
            //get offset(?): shift left 24 bits, then shift right 24 bits

        //associative
            //get tag: bitwise shift right 8 bits
            //get offset(?): shift left 24 bits, then shift right 24 bits

        //set associative
            //get tag: bitwise shift right 18 bits
            //get index: shift left 14 bits then shift right 22 bits
            //get offset(?): shift left 24 bits, then shift right 24 bits

        //address comes in as a hex string representing a 32 bit int
        //convert hex to decimal (did that in previous assignment)
        //if statements for direct, associative, set associative

        int decimalAddress = stoi(address, 0, 16);

        return decimalAddress >> (block_offset_bits + set_index_bits);
        
        if(cacheSets == 1) { //associative
            return decimalAddress >> 8;
        }

        if(numBlocks == 1) { //direct
            return decimalAddress >> 20;
        }

        //set associative
        return decimalAddress >> 18;
    }

    int getIndex(string address) {
        //direct: 12 bits tag, 12 bits index, 8 bits offset
        //associative: 24 bits tag, 8 bits offset
        //set associative: 14 bits tag, 10 bits index, 8 bits offset

        //direct
            //get index: shift left 12 bits then shift right 20 bits
            //get offset(?): shift left 24 bits, then shift right 24 bits

        //associative
            //get tag: bitwise shift right 8 bits
            //get offset(?): shift left 24 bits, then shift right 24 bits

        //set associative
            //get index: shift left 14 bits then shift right 22 bits
            //get offset(?): shift left 24 bits, then shift right 24 bits

        //address comes in as a hex string representing a 32 bit int
        //convert hex to decimal (did that in previous assignment)
        //if statements for direct, associative, set associative

        int decimalAddress = stoi(address, 0, 16);

        // Get rid of right most bits of decimalAddress
        decimalAddress = decimalAddress >> block_offset_bits;
        // Create a mask to isolate the set_index_bits
        int mask = (1 << set_index_bits) - 1;

        // Discard all bits other than index bits
        return decimalAddress & mask;


        if(numBlocks == 1) { //direct
            decimalAddress = decimalAddress << 12;
            return decimalAddress >> 20;
        }

        //set associative
        decimalAddress = decimalAddress << 14;
        return decimalAddress >> 22;
    }

    int getOffset(string address) {
        int decimalAddress = stoi(address, 0, 16);

        // Create mask for block offset
        int mask = (1 << block_offset_bits) - 1;

        return decimalAddress & mask;

        decimalAddress = decimalAddress << 24;
        return decimalAddress >> 24;
    }

    void incrementCounter() {
        counter++;
    }

    void loadAddress(string address) {
        //dont actually load shit into the cpu, thats what the counter is for!!

        //check if address in cache
        

        //address not in cache:
            //put address in cache, load to cpu
        //address in cache:
            //load to cpu
    }

    void storeAddress() {

    }

    bool inCache(string address) {
        //go to spot in hashmap where the address should be
        //check valid
        //if not valid, not in cache

        int index = getIndex(address); // key for map of sets
        int tag = getTag(address); // key for map of slots
        Set addressSet = sets.at(index);
        Slot addressSlot = addressSet.getSlot(tag);

        return addressSlot.isValid();

    }
        
};
#endif