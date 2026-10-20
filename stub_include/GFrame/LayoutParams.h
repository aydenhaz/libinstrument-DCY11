#pragma once
#include <cstring>
#include <cstdint>
namespace GNS_FRAME {
class CGView;
// getLayoutParams 0x2b1c80 copies 0x0c + 0x4c bytes into sret storage.
class LayoutParams {
    alignas(4) unsigned char abi_storage_[0x58];
public:
    ~LayoutParams();
    static LayoutParams getLayoutParams(CGView*);
    static void setLayoutParams(CGView*, LayoutParams&);
    void matchParent() {
        const int32_t match = -1;
        std::memcpy(abi_storage_ + 0x10, &match, 4);
        std::memcpy(abi_storage_ + 0x14, &match, 4);
    }
};
static_assert(sizeof(LayoutParams) == 0x58, "V21 LayoutParams sret extent");
}
