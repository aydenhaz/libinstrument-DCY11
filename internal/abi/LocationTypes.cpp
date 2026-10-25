#include <bl/LocationTypes.h>
#include <cstring>
#include <initializer_list>
#include <new>

namespace bl {
void* LocationAllocator::allocate(uint32_t bytes) { return ::operator new(bytes); }
void LocationAllocator::release(void* pointer) { ::operator delete(pointer); }
CarLocationArray::~CarLocationArray() {
    if (allocator.flags & 0x10) allocator.release(data);
    data = nullptr;
    capacity = count = 0;
}

// Preserve the original untouched padding at +0x2c..0x2f and +0x49..0x4b.
void CarLocationEntry::assignLocInfoRecord(const unsigned char* record) {
    std::memset(bytes + 0x30, 0, 0x19);
    std::memset(bytes + 0x4c, 0xff, 12);
    std::memset(bytes + 0x58, 0, 16);
    std::memcpy(bytes, record + 0x88, 12);
    std::memcpy(bytes + 0x0c, record + 0x98, 8);
    std::memcpy(bytes + 0x14, record + 0x98, 4);
    std::memcpy(bytes + 0x18, record, 16);
    std::memcpy(bytes + 0x28, record + 0x18, 4);
    int32_t available;
    std::memcpy(&available, record + 0x58, 4);
    if (available >= 1) {
        std::memcpy(bytes + 0x30, record + 0x20, 24);
        bytes[0x48] = 1;
    }
}

// Original 0x39660. Snapshot before growing: entry may alias the old array.
void CarLocationArray::append(const CarLocationEntry& entry) {
    const uint32_t max_count = UINT32_MAX / sizeof(CarLocationEntry);
    if (count >= max_count) throw std::bad_alloc();
    if (count + 1 > capacity) {
        CarLocationEntry saved;
        std::memcpy(&saved, &entry, sizeof saved);
        uint32_t next = count + 1;
        if ((allocator.flags & 0xf) == 1) {
            const uint32_t extra = capacity < 5 ? 5 :
                (capacity >= 500 ? count / 4 : count);
            if (extra > max_count - next) throw std::bad_alloc();
            next += extra;
        }
        auto* replacement = static_cast<unsigned char*>(allocator.allocate(next * sizeof saved));
        if (count) std::memcpy(replacement, data, count * sizeof saved);
        allocator.release(data);
        data = replacement;
        capacity = next;
        std::memcpy(replacement + count * sizeof saved, &saved, sizeof saved);
    } else {
        std::memcpy(static_cast<unsigned char*>(data) + count * sizeof entry, &entry, sizeof entry);
    }
    ++count;
}

// V21 instrument 0x394ec: preserve untouched padding and nonzero sentinels.
LocInfo::LocInfo() {
    auto at = [this](size_t offset) -> unsigned char* {
        return offset < 0x128 ? prefix + offset : suffix + offset - 0x12c;
    };
    auto zero = [&](size_t offset, size_t length) { std::memset(at(offset), 0, length); };
    auto word = [&](size_t offset, uint32_t value) { std::memcpy(at(offset), &value, 4); };
    word(0x6c, 0);
    zero(0, 0x55); zero(0x58, 0x11); zero(0xf0, 0x30);
    word(0x120, 1); word(0x124, 0);
    word(0x12c, 0); word(0x130, 0); word(0x134, 0); word(0x138, 1);
    zero(0x13c, 0x40);
    for (size_t base = 0x180; base < 0x4b0; base += 0x110) {
        zero(base, 0x5c); zero(base + 0x60, 0x18);
        for (size_t n = 0x78; n <= 0x84; n += 4) word(base + n, 0xffffffff);
        zero(base + 0x88, 0x24); zero(base + 0xb0, 12); zero(base + 0xc0, 16);
        word(base + 0xd0, 0xbf800000); word(base + 0xd8, 0);
        for (size_t n : {size_t(0xdc), size_t(0xe4), size_t(0xec)}) word(base + n, 0xbff00000);
        word(base + 0xe0, 0); word(base + 0xe8, 0);
        for (size_t n = 0xf0; n <= 0x100; n += 4) word(base + n, 0xbf800000);
        zero(base + 0x104, 1); zero(base + 0x108, 8);
    }
    word(0x4b0, 0); word(0x4b4, 0xbf800000);
    for (size_t n = 0; n < 0x1c0; n += 0x70) {
        zero(n + 0x4b8, 0x15); word(n + 0x4d0, 0);
        zero(n + 0x4d8, 0x15); word(n + 0x4f0, 0);
        zero(n + 0x4f8, 0x1c);
        word(n + 0x514, 0xbf800000); word(n + 0x518, 0xbf800000);
        word(n + 0x51c, 0xffffffff); word(n + 0x520, 0xffffffff); word(n + 0x524, 0);
    }
    zero(0x678, 12); zero(0x684, 2); *at(0x686) = 0xff; word(0x688, 0);
    zero(0x14c, 0x1c); zero(0x70, 0x80);
}
}
