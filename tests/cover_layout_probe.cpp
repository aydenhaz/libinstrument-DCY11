#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#if UINTPTR_MAX == 0xffffffffu
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess {
#define CHECK(field, offset) static_assert(__builtin_offsetof(CoverUtil,field)==offset,"V21 " #field)
 CHECK(text_,0x18); CHECK(threshold_,0x1c); CHECK(surface_,0x20);
 CHECK(enabled_,0x70); CHECK(screenshot_,0x74);
 static_assert(__builtin_offsetof(CoverUtil,screenshot_)+offsetof(bl::ScreenShotInfo,stride)==0x7c,"stride");
 static_assert(__builtin_offsetof(CoverUtil,screenshot_)+offsetof(bl::ScreenShotInfo,width)==0x80,"width");
 static_assert(__builtin_offsetof(CoverUtil,screenshot_)+offsetof(bl::ScreenShotInfo,height)==0x84,"height");
 static_assert(__builtin_offsetof(CoverUtil,screenshot_)+offsetof(bl::ScreenShotInfo,bytes)==0x88,"bytes");
 static_assert(__builtin_offsetof(CoverUtil,screenshot_)+offsetof(bl::ScreenShotInfo,length)==0x8c,"length");
 CHECK(requested_,0x98); CHECK(image_,0x9c); CHECK(state_,0xa0); CHECK(counter_,0xa4);
#undef CHECK
};
}
#endif
