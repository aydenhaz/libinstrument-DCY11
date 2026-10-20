#pragma once
#include <cstdint>
extern "C" void IPL_RefIncStrong(const void*, const void*) asm("_ZNK3asl7RefBase9incStrongEPKv");
extern "C" void IPL_RefDecStrong(const void*, const void*) asm("_ZNK3asl7RefBase9decStrongEPKv");
namespace GNS_FRAME {
class CGColorDrawable;
// V21 0x295b0..0x295cc: one pointer, RefBase subobject at drawable +4.
class CGDrawablePtr {
    CGColorDrawable* pointer_;
public:
    explicit CGDrawablePtr(CGColorDrawable* p) : pointer_(p) {
        if (p) IPL_RefIncStrong(reinterpret_cast<const unsigned char*>(p) + 4, this);
    }
    ~CGDrawablePtr() {
        if (pointer_) IPL_RefDecStrong(reinterpret_cast<const unsigned char*>(pointer_) + 4, this);
    }
    CGDrawablePtr(const CGDrawablePtr&) = delete;
    CGDrawablePtr& operator=(const CGDrawablePtr&) = delete;
};
static_assert(sizeof(CGDrawablePtr) == sizeof(void*), "DrawablePtr pointer extent");
}
