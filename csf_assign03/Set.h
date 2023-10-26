// Set.h

#ifndef SET_H
#define SET_H

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <algorithm>
#include "Slot.h"
#include <vector>
#include "Slot.h"

class Set {
private: 
    std::vector<Slot> slots; // Indices aren't associated with tags
    int maxBlocks;
    int currBlocks;

public:
    Set(); // Default constructor
    Set(int numBlocks); // Constructor with size parameter

    Slot* getSlot(uint32_t tag); // Retrieve a pointer to a slot based on the tag
    bool addSlot(uint32_t tag); // Add a slot with the provided tag
    bool hasSlot(uint32_t tag); // Check if a slot with the specific tag exists
    bool isFull(); // Check if the set is full

    // Additional methods (if any) can be declared below
};

#endif // SET_H
