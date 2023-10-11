#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
    int cacheSets = stoi(argv[1]);
    int numBlocks = stoi(argv[2]);
    int blockBytes = stoi(argv[3]);
    string writeAlloc = argv[4];
    string writeThru_back = argv[5];
    string lru_fifo = argv[6];

    cout << "number of sets in the cache: " << cacheSets << "\n";
    cout << "number of blocks in each set: " << numBlocks << "\n";
    cout << "number of bytes in each block: " << blockBytes << "\n";
    cout << "write-allocate or no-write-allocate: " << writeAlloc << "\n";
    cout << "write-through or write-back: " << writeThru_back << "\n";
    cout << "lru (least-recently-used) or fifo evictions: " << lru_fifo << "\n";
}