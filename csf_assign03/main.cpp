#include <iostream>
#include <fstream>
#include <string>
#include "Cache.h"
using namespace std;

int main(int argc, char *argv[]) {
    int cacheSets = stoi(argv[1]);
    int numBlocks = stoi(argv[2]);
    int blockSize = stoi(argv[3]);
    string writeAlloc = argv[4];
    string writeThru_back = argv[5];
    string lru_fifo = argv[6];

    // Check number of sets is a power of 2
    if ((cacheSets & (cacheSets - 1)) != 0) {
        fprintf(stderr, "Number of sets must be a power of 2\n");
        return 1;
    }

    // Check block size is a power of 2
    if ((blockSize & (blockSize - 1)) != 0) {
        fprintf(stderr, "Block size must be a power of 2\n");
        return 1;
    }

    // Check block size is greater than 4
    if (blockSize < 4) {
        fprintf(stderr, "Block size cannot be less than 4\n");
        return 1;
    }

    // Check that write-back and no-write-allocate were NOT both specified
    if (writeAlloc == "no-write-allocate" && writeThru_back == "write-back") {
        fprintf(stderr, "write-back and no-write-allocate were both specified\n");
        return 1;
    }

    // cout << "number of sets in the cache: " << cacheSets << "\n";
    // cout << "number of blocks in each set: " << numBlocks << "\n";
    // cout << "number of bytes in each block: " << blockSize << "\n";
    // cout << "write-allocate or no-write-allocate: " << writeAlloc << "\n";
    // cout << "write-through or write-back: " << writeThru_back << "\n";
    // cout << "lru (least-recently-used) or fifo evictions: " << lru_fifo << "\n";

    string line; //holds current line in trace file
    string loadStore; //l or s
    string address; //address
    // while (getline(std::cin, line)) {
    //     cin >> loadStore >> address;
    //     cout << "load or store: " << loadStore << "\n";
    //     cout << "address: " << address << "\n";
        
    // }

    Cache c(cacheSets, numBlocks, blockSize, writeAlloc, writeThru_back, lru_fifo);
    c.printParameters();
    cout << "tag: " << c.getTag("23DC13AF") << "\n";
    cout << "index: " << c.getIndex("23DC13AF") << "\n";

    return 0;
}