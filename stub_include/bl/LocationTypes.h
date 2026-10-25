#pragma once
#include <DiceString16.h>
#include <cstddef>
#include <cstdint>

namespace bl {
// V21 array allocator: two destructor slots, allocate, release; scalar new/delete.
class LocationAllocator {
public:
    uint8_t flags = 0x11;
    virtual ~LocationAllocator() {}
    virtual void* allocate(uint32_t bytes);
    virtual void release(void* pointer);
};
struct CarLocationArray {
    void* data = nullptr;
    uint32_t capacity = 0, count = 0;
    LocationAllocator allocator;
    CarLocationArray() = default;
    ~CarLocationArray();
    CarLocationArray(const CarLocationArray&) = delete;
    CarLocationArray& operator=(const CarLocationArray&) = delete;
};
struct CarLoc {
    uint32_t prefix[2] = {};
    CarLocationArray locations;
};
// Fields with unknown semantics stay opaque. The owning string must remain a
// real object, with deep copy and destruction, rather than raw byte storage.
struct alignas(8) LocInfo {
    unsigned char prefix[0x128];
    dice::String16 text;
    unsigned char suffix[0x560];
    LocInfo();
};
#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(LocationAllocator) == 8, "V21 allocator size");
static_assert(sizeof(CarLocationArray) == 20, "V21 array size");
static_assert(sizeof(CarLoc) == 28, "V21 CarLoc size");
static_assert(offsetof(CarLoc, locations) == 8, "V21 CarLoc array");
static_assert(offsetof(CarLocationArray, allocator) == 12, "V21 allocator offset");
static_assert(offsetof(LocInfo, text) == 0x128, "V21 LocInfo string");
static_assert(offsetof(LocInfo, suffix) == 0x12c, "V21 LocInfo suffix");
static_assert(sizeof(LocInfo) == 0x690, "V21 LocInfo aligned size");
#endif
}
