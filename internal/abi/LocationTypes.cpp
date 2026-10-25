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
