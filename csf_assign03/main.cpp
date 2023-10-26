#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Cache.h"
#include "cache.cpp"
#include "set.cpp"
#include "Set.h"
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

    Cache c(cacheSets, numBlocks, blockSize, writeAlloc, writeThru_back, lru_fifo);

    string line; //holds current line in trace file
    string loadStore; //l or s
    string address; //address

    while (getline(cin, line)) {
        if (line.empty()) {
            continue;  // skip processing for this line
        }
        istringstream iss(line);
        if (!(iss >> loadStore >> address)) {
            cerr << "Failed to read line: " << line << endl;
            continue;  // skip processing for this line
        }

        if (loadStore == "l") {
            c.loadAddress(address);
        } else if (loadStore == "s") {
            c.storeAddress(address);
        } else {
            cerr << "Error: loadStore (" << loadStore << ") is not l or s on line: " << line << endl;
            return 1;
        }
    }


    // Testing purposes
    // c.printParameters();
    // cout << "tag: " << c.getTag("23DC13AF") << "\n";
    // cout << "index: " << c.getIndex("23DC13AF") << "\n";

    c.printStatistics();

    return 0;
}