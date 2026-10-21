/*
 * VTable-aligned fragment skeletons (CGInstrumentPannelLarge*Fragment).
 *
 * Layout rule (from analysis/VTABLE_LAYOUT.md class family 1):
 *   vtable slot order = [ ~X() complete dtor, ~X() deleting dtor,
 *                         virtual_null() returning 0, adjust_thunk() ]
 * Declare virtuals in EXACTLY this order; do not add virtuals before them.
 * Object layout: vptr@0, then members in field order (see batch analyses).
 */
#pragma once
#include <cstdint>

namespace GNS_FRAME { class CGFragment; class CGContext; class CGView; class CGViewGroup; }

namespace NS_InstrumentPannelLarge {

class CGInstrumentPannelLargeFragmentBase {
public:
    /* slot 0: complete object destructor (teardown: remove observers etc.) */
    virtual ~CGInstrumentPannelLargeFragmentBase();
    /* slot 1: deleting destructor */
    virtual void destroy();                       // operator delete path (16B pattern)
    /* slot 2: null slot — virtual returning 0 (shared sub_156e0 body) */
    virtual void *virtual_null() { return 0; }
    /* slot 3: vbase adjust thunk (shared sub_156e4 body: this-8 then jmp) */
    virtual void adjust_thunk();                  // { /* this -= 8; */ }
protected:
    GNS_FRAME::CGContext *ctx_ = 0;
    GNS_FRAME::CGView *v1_ = 0, *v2_ = 0, *v3_ = 0;
    int32_t theme_type_ = 0;
};

#define IPL_FRAGMENT_ALIGNED(name) \
class name : public CGInstrumentPannelLargeFragmentBase { \
public: \
    virtual ~name();          /* slot 0: own teardown */ \
    virtual void destroy();   /* slot 1 */ \
};

IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeOpeningFragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeGuideFragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeCruiseFragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeARGuideFragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargePreviewFragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeGuideS311Fragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeCruiseS311Fragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeGuideGuangQiA106Fragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeCruiseGuangQiA106Fragment)
IPL_FRAGMENT_ALIGNED(CGInstrumentPannelLargeGuideS311UnnormalFragment)

} // namespace
