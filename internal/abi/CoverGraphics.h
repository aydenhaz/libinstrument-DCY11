#pragma once
#include <cstdint>
#include <cstring>
#include <bl/IBLMapBusinessDataObserver.h>
// CGRect RTTI in V21: nonvirtual STGRect_ at +4 after its vptr.
struct STGRect_ { int32_t coordinates[4]; };
namespace GPI {
class CGRect : public STGRect_ {
public:
    CGRect(int32_t width,int32_t height):STGRect_{{0,width,0,height}} {}
    virtual ~CGRect() = default;
};
}
namespace dice { template<class T> struct Rect { T coordinates[4]; }; }
namespace GNS_FRAME { class CGCanvas; class CGImage; enum EGDrawImageType_ : int; }
namespace bl {
// ScreenShotInfo fields consumed by original CoverUtil. Opaque fields retain
// their observed widths; this is not a new screenshot allocation/owner.
struct ScreenShotInfo {
    uint32_t unknown0=0,unknown4=0;
    int8_t stride=0;
    uint8_t padding9[3];
    int32_t width=0,height=0;
    uint8_t* bytes=nullptr;
    uint32_t length=0;
    uint16_t tail=0,padding1e;
};
}
namespace IPL_ABI {
inline void** coverVirtualTable(const void* object){void** table;std::memcpy(&table,object,sizeof(table));return table;}
inline void drawCoverImage(GNS_FRAME::CGCanvas* canvas,GNS_FRAME::CGImage* image,
        const GPI::CGRect& rect,void* style) {
    // CGCanvas vtable +0xcc: drawImgEx(CGImage*, const CGRect&, enum, void*).
    using Call=void(*)(GNS_FRAME::CGCanvas*,GNS_FRAME::CGImage*,const GPI::CGRect&,GNS_FRAME::EGDrawImageType_,void*);
    reinterpret_cast<Call>(coverVirtualTable(canvas)[51])(canvas,image,rect,static_cast<GNS_FRAME::EGDrawImageType_>(0),style);
}
inline void requestCoverScreenshot(void* device,const dice::Rect<int32_t>& rect,
        bl::ScreenShotInfo& info,bl::IBLMapBusinessDataObserver* observer) {
    // IMapView vtable +0x128. Return value is ignored by original callback.
    using Call=bool(*)(void*,const dice::Rect<int32_t>&,bl::ScreenShotInfo&,bl::IBLMapBusinessDataObserver*);
    (void)reinterpret_cast<Call>(coverVirtualTable(device)[74])(device,rect,info,observer);
}
#if UINTPTR_MAX == 0xffffffffu
static_assert(sizeof(GPI::CGRect)==20,"V21 CGRect extent");
static_assert(sizeof(dice::Rect<int32_t>)==16,"V21 screenshot rectangle");
static_assert(sizeof(bl::ScreenShotInfo)==32,"V21 screenshot state extent");
#endif
}
