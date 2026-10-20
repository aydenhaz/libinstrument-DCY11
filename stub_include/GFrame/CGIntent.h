#pragma once
#include <cstdint>
namespace GNS_FRAME {
class CGIntent {
    alignas(4) unsigned char abi_storage_[20]; // V21 ctor 0x2837f4, stack delta 0x14.
public:
    CGIntent();
    ~CGIntent();
    void putStringExtra(const unsigned short *key, const char *value);
    void putInt32Extra(const unsigned short *key, int32_t value);
    void putBoolExtra(const unsigned short *key, bool value);
};
}

static_assert(sizeof(GNS_FRAME::CGIntent) == 20, "V21 ARM32 CGIntent ABI size");
