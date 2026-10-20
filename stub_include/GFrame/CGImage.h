#pragma once
#include <cstdint>
class CGString;
// V21 CGSize RTTI: STGSize_ base at +4 following the virtual pointer.
struct STGSize_ { int32_t w,h; };
namespace GPI {
struct CGSize : STGSize_ {
    CGSize(int32_t width=0,int32_t height=0):STGSize_{width,height} {}
    virtual ~CGSize() = default;
};
}
// The original CoverUtil create call supplies 0x101 and a four-byte pixel stride.
enum EGColorType_ { EGColorType_RGBA=0x101 };
using EGColorType = EGColorType_;
namespace GNS_FRAME {
class CGImage {
    // Opaque host-owned layout: original CoverUtil allocates 0x1e0 before C1.
    // This does not reconstruct inheritance, RTTI or internal field ownership.
    alignas(4) unsigned char storage_[0x1e0];
public:
    CGImage();
    ~CGImage();
    bool create(const GPI::CGSize&, EGColorType, unsigned char*);
    bool loadImageFromFile(const CGString&,int,bool,bool,CGString*);
    uint32_t getDataLen() const;
    unsigned char* getData() const;
};
static_assert(sizeof(CGImage)==0x1e0,"V21 CGImage allocation extent");
}
#if UINTPTR_MAX == 0xffffffffu
static_assert(sizeof(GPI::CGSize)==12,"V21 polymorphic CGSize extent");
#endif
