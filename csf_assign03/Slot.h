#ifndef _SLOT_H
#define _SLOT_H

#include <cstdlib>

class Slot {
    uint32_t tag;
    bool valid;
    uint32_t load_ts;//fifo
    uint32_t access_ts;//lru
    bool dirty;

    //put functions below (implement if the functions are small enough??)


}