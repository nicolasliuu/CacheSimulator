#ifndef _SLOT_H
#define _SLOT_H

#include <cstdlib>
#include <cstdint>

class Slot {
    uint32_t tag;
    bool valid;
    uint32_t load_ts;//fifo
    uint32_t access_ts;//lru
    bool dirty;

    //put functions below (implement if the functions are small enough??)

    public:
        uint32_t getTag() {
            return tag;
        }

        uint32_t getLoad_ts() {
            return load_ts;
        }

        uint32_t getAccess_ts() {
            return access_ts;
        }

        bool isValid() {
            return valid;
        }

        bool isDirty() {
            return dirty;
        }

        void setTag(uint32_t newTag) {
            tag = newTag;
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