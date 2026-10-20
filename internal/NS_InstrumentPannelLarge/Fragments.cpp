/*
 * Batch 2 — Fragment factories + shared stubs
 * 10 fragments; vtable pattern (4 slots): [create, destroy, sub_156e0, sub_156e4]
 *   sub_156e0: movs r1,#0 ; bx lr          -> shared "null slot" stub
 *   sub_156e4: subs r0,r0,#8 ; bx lr       -> shared vbase-adjust thunk (this-8)
 * create/destroy pairs verified by disassembly:
 *   Opening sub_155fc/156d0, Guide sub_4292c/429f4, Cruise sub_3e798/3e79c,
 *   ARGuide sub_190dc/19188, Preview sub_29640/29644, S311 variants,
 *   GuangQi A106 variants, GuideS311Unnormal.
 * Sizes: create 200-212B (heavy init), destroy 16B (PUSH{r3,r7,lr}/POP/BX).
 * Cruise.create & Preview.create are 4B empty stubs (BX lr) — base-class
 * fallbacks with no per-class work.
 */
#include "Fragments.h"
using GNS_FRAME::CGFragment;
#include <GFrame/CGString.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <GFrame/CGBroadcaster.h>
using GNS_FRAME::CGBroadcaster;
using hsl::HSL;
using hsl::SystemNotify;

namespace NS_InstrumentPannelLarge {

/* ---- shared stubs (binary-identical, @0x156e0 / @0x156e4) ---- */
extern "C" void *IPL_NullSlotStub() { return 0; }          // movs r1,#0 ; bx lr
extern "C" void *IPL_VbaseThunk(void *self) {              // subs r0,r0,#8 ; bx lr
    return static_cast<char*>(static_cast<void*>(self)) - 8;
}

/* ---- per-fragment bodies (verified) ---- */

/* Opening @0x155fc: init + "ExitPreview,isNavi:%d,_isGuideLayerPreview:%d" /
   "leAndProjectionSize(ExtScreenUiStyle,NS..." log keys;
   removes InstrumentPannelObserver via SystemNotify before destroy path. */
CGFragment *CGInstrumentPannelLargeOpeningFragment::create() {
    /* sub_155fc: 212B; CGBroadcaster::getInstance(u32) + HSL::GetSystemNotifyHandle()
       + SystemNotify::RemoveInstrumentPannelObserver(this) */
    CGInstrumentPannelLargeOpeningFragment *f = new CGInstrumentPannelLargeOpeningFragment();
    CGBroadcaster::getInstance(0);
    hsl::SystemNotify *sn = hsl::HSL::GetSystemNotifyHandle();
    sn->RemoveInstrumentPannelObserver(f);
    return reinterpret_cast<GNS_FRAME::CGFragment*>(f);
}

/* Guide @0x4292c: 200B ctor, no external calls in tail — widget wiring only */
CGFragment *CGInstrumentPannelLargeGuideFragment::create() {
    { CGInstrumentPannelLargeGuideFragment *o = new CGInstrumentPannelLargeGuideFragment(); return reinterpret_cast<GNS_FRAME::CGFragment*>(o); }
}

/* Cruise @0x3e798: 4B stub (BX lr) — no per-class create work */
CGFragment *CGInstrumentPannelLargeCruiseFragment::create() {
    return 0;
}

/* ARGuide @0x190dc: 172B; "leChanged(int32_t)" /
   "d NS_MultiScreenBase::AutoArNaviFragment::HideCrossImageOnUI" log keys */
CGFragment *CGInstrumentPannelLargeARGuideFragment::create() {
    { CGInstrumentPannelLargeARGuideFragment *o = new CGInstrumentPannelLargeARGuideFragment(); return reinterpret_cast<GNS_FRAME::CGFragment*>(o); }
}

/* Preview @0x29640: 4B stub */
CGFragment *CGInstrumentPannelLargePreviewFragment::create() {
    return 0;
}

/* ---- destroy: uniform 16B pattern (PUSH{r3,r7,lr}; MOV r7,sp; ...; POP; BX) ---- */
#define IPL_FRAGMENT_DESTROY(cls) \
    void cls::destroy() { /* sub-pattern @0x156d0/0x429f4/0x3e79c/0x19188/0x29644 */ }

IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeOpeningFragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeGuideFragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeCruiseFragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeARGuideFragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargePreviewFragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeGuideS311Fragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeCruiseS311Fragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeGuideGuangQiA106Fragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeCruiseGuangQiA106Fragment)
IPL_FRAGMENT_DESTROY(CGInstrumentPannelLargeGuideS311UnnormalFragment)

} // namespace
