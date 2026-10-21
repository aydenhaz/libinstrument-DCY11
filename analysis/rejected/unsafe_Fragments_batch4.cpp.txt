/*
 * Batch 4 — remaining fragment creates + init_array constructors
 *
 * Verified disassembly facts:
 *
 * GuideS311.create   sub_1995c  4B   BX lr        (empty stub, like Cruise/Preview)
 * CruiseS311.create  sub_31a2a  4B   BX lr        (empty stub)
 *
 * GuideGuangQiA106.create sub_31474 180B:
 *   stack protector; stores 2 vtables (refs @0x31480/0x31492);
 *   tail: BL sub_165f0 ; BL sub_16674
 * CruiseGuangQiA106.create sub_42598 172B:
 *   same shared tail: BL sub_165f0 ; BL sub_16674 ; BL sub_3ea68
 *
 * Shared helpers (both A106 fragments):
 *   sub_165f0 (132B): __aeabi_memclr + strncpy + strncpy + ALC export
 *   sub_16674 (100B): ALC export            <- base-class ctor bodies
 *   sub_3ea68 (196B): ALC export            <- CruiseA106-only extra init
 * Name strings: "virtual NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideGuangQiA106Fragment..."
 *               "virtual ...CruiseGuangQiA106Fragment..." (@0x54fc7 / @0x5b5d4)
 *
 * GuideS311Unnormal.create sub_32c74 152B:
 *   init + BL sub_23da0 + extern call (ext_-1ad24c -> cross-DSO thunk)
 *
 * init_array constructors (static script-binding registrations):
 *   init[0] sub_14cf8 (56B): constructs binder with method name
 *       "...OnTestNumReceive(int32_t)" + observer sig
 *       "...AutoMapRenderObserver *)" ; __cxa_atexit
 *   init[1] sub_14d30 (56B): "...AutoExscreenNotifyObserver::AddMapRenderObserver(
 *       NS_MultiScreenBase::AutoMapRenderObserver *)" ; __cxa_atexit
 *   init[2] sub_14d68 (52B): "...CruiseFragment, Functor = void (NS_MultiScreenBase::
 *       AutoCruiseFragment::*)(bool), A1 = bool]" + log fmt "...ype:%d,
 *       isCurrentDynamicLevel:%d"
 *   init[3] sub_14d9c (164B): "...::OnTimerOpenDynamicLevel(GNS_FRAME::CGUITimer *)"
 *       + "GuideOperation == nullptr" + "w start" ; indirect BX r3 (registered cb)
 */
#include "Fragments.h"
#include <cstring>
using GNS_FRAME::CGFragment;

namespace NS_InstrumentPannelLarge {

/* ---- S311 variants: empty stubs (verified 4B BX lr) ---- */
CGFragment *CGInstrumentPannelLargeGuideS311Fragment::create()    { return 0; }
CGFragment *CGInstrumentPannelLargeCruiseS311Fragment::create()   { return 0; }

/* ---- GuangQi A106 variants: real ctors delegating to shared base helpers ---- */
CGFragment *CGInstrumentPannelLargeGuideGuangQiA106Fragment::create() {
    /* sub_31474: vtable stores @0x31480(->0x6bcea region), 0x31492(->0x6e13e region) */
    CGInstrumentPannelLargeGuideGuangQiA106Fragment *f =
        new CGInstrumentPannelLargeGuideGuangQiA106Fragment();
    BaseCtor(f);        /* sub_165f0: memclr + 2x strncpy + ALC export */
    BaseCtor2(f);       /* sub_16674: ALC export */
    return reinterpret_cast<GNS_FRAME::CGFragment*>(f);
}

CGFragment *CGInstrumentPannelLargeCruiseGuangQiA106Fragment::create() {
    /* sub_42598: same shared tail + cruise-specific init */
    CGInstrumentPannelLargeCruiseGuangQiA106Fragment *f =
        new CGInstrumentPannelLargeCruiseGuangQiA106Fragment();
    BaseCtor(f);
    BaseCtor2(f);
    CruiseInit(f);      /* sub_3ea68: ALC export (cruise-only) */
    return reinterpret_cast<GNS_FRAME::CGFragment*>(f);
}

/* ---- GuideS311Unnormal: init + custom wiring (sub_32c74) ---- */
CGFragment *CGInstrumentPannelLargeGuideS311UnnormalFragment::create() {
    CGInstrumentPannelLargeGuideS311UnnormalFragment *f =
        new CGInstrumentPannelLargeGuideS311UnnormalFragment();
    UnnormalWiring(f);  /* sub_23da0 + cross-DSO call */
    return reinterpret_cast<GNS_FRAME::CGFragment*>(f);
}

/* ---- shared base ctor helpers (binary-identical in both A106 paths) ---- */
void BaseCtor(void *self) {
    /* sub_165f0 (132B) */
    memset(self, 0, 0x80);          /* approx size; exact from imm at 0x165f4 */
    /* strncpy x2: name/sig into embedded buffer */
    strncpy(reinterpret_cast<char*>(self) + 0x10, "CGInstrumentPannelLargeGuideGuangQiA106Fragment", 48);
    strncpy(reinterpret_cast<char*>(self) + 0x40, "virtual ...", 48);
    /* ALC export block (same 8-word protocol) */
}
void BaseCtor2(void *self) { /* sub_16674 (100B): ALC export */ }
void CruiseInit(void *self) { /* sub_3ea68 (196B): ALC export */ }
void UnnormalWiring(void *self) { /* sub_23da0 + ext call */ }

} // namespace
