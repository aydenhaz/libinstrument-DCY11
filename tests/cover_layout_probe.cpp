#include "../internal/NS_MultiScreenBase/CoverUtil.h"
#if UINTPTR_MAX == 0xffffffffu
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess {
#define CHECK(field, offset) static_assert(__builtin_offsetof(CoverUtil,field)==offset,"V21 " #field)
 CHECK(text_,0x18); CHECK(threshold_,0x1c); CHECK(surface_,0x20);
 CHECK(enabled_,0x70); CHECK(stride_,0x7c); CHECK(width_,0x80);
 CHECK(height_,0x84); CHECK(bytes_,0x88); CHECK(length_,0x8c);
 CHECK(requested_,0x98); CHECK(image_,0x9c); CHECK(state_,0xa0); CHECK(counter_,0xa4);
#undef CHECK
};
}
#endif
