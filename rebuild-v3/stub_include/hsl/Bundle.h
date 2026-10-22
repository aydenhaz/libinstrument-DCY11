#pragma once
#include <DiceString16.h>
namespace hsl {
// hsl ctor 0x1eaaa8 stores vptr at +0 and calls turbo::Bundle ctor
// at this+4; turbo ctor 0xbe6c initializes one pointer. ARM32 size = 8.
class Bundle {
    alignas(4) unsigned char abi_storage_[8];
public:
    Bundle();
    ~Bundle();
    Bundle(const Bundle&) = delete;
    Bundle& operator=(const Bundle&) = delete;
    void PutBundle(const dice::String16&, const Bundle&);
    bool GetBundle(const dice::String16&, Bundle&);
    void PutInt32(const dice::String16&, int32_t);
    bool GetInt32(const dice::String16&, int32_t&);
};
static_assert(sizeof(Bundle) == 8, "V21 ARM32 hsl::Bundle layout");
}
