#ifndef _SLOT_H
#define _SLOT_H

#include <cstdlib>
#include <cstdint>

class Slot {
    private:
        uint32_t tag;
        bool valid;
        uint32_t load_ts;//fifo
        uint32_t access_ts;//lru
        bool dirty;

    //put functions below (implement if the functions are small enough??)

    public:
        Slot() : tag(0), valid(false), load_ts(0), access_ts(0), dirty(false) {} // Default constructor

        uint32_t getTag() {
            return tag;
        }

        uint32_t getLoad_ts() {
            return load_ts;
        }

        uint32_t getAccess_ts() {
            return access_ts;
        }

        void increaseAccess_ts() {
            access_ts++;
        }

        bool isValid() {
            return valid;
        }

        bool isDirty() {
            return dirty;
        }

        void setTag(uint32_t newTag) {
            tag = newTag;
            valid = true;
        }

        void used() { // Call this when a slot is used
            access_ts++;
        }

        void setLoad_ts(uint32_t new_ts) {
            load_ts = new_ts;
        }

        void setAccess_ts(uint32_t new_ts) {
            access_ts = new_ts;
        }

        void setValid(bool trueOrFalse) {
            valid = trueOrFalse;
        }

        void setDirty(bool trueOrFalse) {
            dirty = trueOrFalse;
        }
};
#endif