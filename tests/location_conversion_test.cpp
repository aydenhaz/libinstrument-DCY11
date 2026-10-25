#include <bl/LocationTypes.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <new>
int main() {
    unsigned char record[0x110];
    for (unsigned i = 0; i < sizeof record; ++i) record[i] = (i * 37 + 11) & 255;
    for (int32_t valid : {-1, 0, 1, 2}) {
        std::memcpy(record + 0x58, &valid, 4);
        bl::CarLocationEntry entry;
        std::memset(&entry, 0xa5, sizeof entry);
        entry.assignLocInfoRecord(record);
        std::fwrite(&entry, 1, sizeof entry, stdout);
    }
    for (uint8_t mode : {uint8_t(0x11), uint8_t(0x10)}) {
        bl::CarLocationArray array;
        array.allocator.flags = mode;
        for (uint32_t i = 0; i < 1100; ++i) {
            bl::CarLocationEntry entry;
            std::memset(&entry, i & 255, sizeof entry);
            array.append(entry);
            assert(array.count == i + 1 && array.capacity >= array.count);
            if (i == 0 || i == 5 || i == 6 || i == 500 || i == 1099)
                std::fwrite(&array.capacity, 4, 1, stdout);
        }
        auto* entries = static_cast<bl::CarLocationEntry*>(array.data);
        for (uint32_t i = 0; i < array.count; ++i)
            for (auto byte : entries[i].bytes) assert(byte == (i & 255));
        // Force growth with an input that aliases the allocation to be freed.
        while (array.count < array.capacity) array.append(entries[0]);
        entries = static_cast<bl::CarLocationEntry*>(array.data);
        array.append(entries[1099]);
        entries = static_cast<bl::CarLocationEntry*>(array.data);
        for (auto byte : entries[array.count - 1].bytes) assert(byte == (1099 & 255));
    }
    bl::CarLocationArray overflow;
    overflow.count = UINT32_MAX / sizeof(bl::CarLocationEntry);
    bl::CarLocationEntry entry = {};
    bool rejected = false;
    try { overflow.append(entry); } catch (const std::bad_alloc&) { rejected = true; }
    assert(rejected && !overflow.data);
    overflow.count = 0;
}
