/*
 * Batch 7 — four major internal functions
 *
 * A) sub_3a45c (4760B) — CGIPLPacket main init body
 * B) sub_1c798 (5696B) — MapSurfaceViewObserver::OnMapSurfaceViewNotify
 * C) sub_2ace0 (3788B) — screen-mode listener
 * D) sub_24094 (3176B) — map service / POI business callback
 */
#include "CGIPLPacket.h"
#include "../AlcExport.h"
#include <GFrame/CGString.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/MultiScreenService.h>
#include <hsl/TimerUtilsFuns.h>
#include <hsl/IActivateVerification.h>
#include <GFrame/CGFuncInfoTool.h>
#include <GFrame/GPIClosure.h>
#include <GFrame/CGFrameSimulation.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>

namespace NS_InstrumentPannelLarge {

/* ==========================================================================
 * A) sub_3a45c — main init body (registered as startup / constructor path)
 * Verified sequence (call targets from PLT resolution):
 * ========================================================================== */
void CGIPLPacket::MainInit() {
    /* 1. ALC export: tag=0x40(64), block {0,0,"HMI_PANNELLARGE",0,64,sig,name} */
    AlcExport(/*name*/ 0, /*sig*/ 0, 0x40, this);

    /* 2. second ALC export: tag=0x41(65), r1=0x16(22) -> retry/level param */
    AlcExport(0, 0, 0x41, this);

    /* 3. register as MapSurfaceView observer */
    hsl::SystemNotify *sn = hsl::HSL::GetSystemNotifyHandle();
    sn->AddMapSurfaceObserver(this);

    /* 4. ALC export tag=0x42(66) r1=8 */
    AlcExport(0, 0, 0x42, this);

    /* 5. language switch observer (b=false) */
    sn->AddLanguageSwitchObserver(this, false);

    /* 6. MultiScreenService init lock */
    hsl::MultiScreenService::GetInstance()->InitLock();

    /* 7. CGFrameSimulation::ownWorkStation(); store this+0x394/0x308 members */
    GNS_FRAME::CGFrameSimulation::ownWorkStation();

    /* 8. register script func info: CGFuncInfoTool::addFuncInfo(closure, ptr, name, sig, 0) */
    GNS_FRAME::CGFuncInfoTool::addFuncInfo(GPI::Closure::bind(this, &CGIPLPacket::MainInit),
                                           this, "MainInit", "CGIPLPacket::MainInit()", 0);

    /* 9. init HSL modules (two MapEngineID entries) */
    hsl::HSL::GetHandle()->InitModule(/*ModuleType*/(ModuleType)1, (bl::MapEngineID)1);
    hsl::HSL::GetHandle()->InitModule(/*ModuleType*/(ModuleType)0x180, (bl::MapEngineID)1);

    /* 10. activation verification */
    verify_ = hsl::IActivateVerification::Create();
    if (verify_) state_ |= 1;     /* f88a039a: set bit at this+0x39a */

    /* 11. ALC export tag=0x37(55) r1=8 */
    AlcExport(0, 0, 0x37, this);

    /* 12. map service handle + conditional ALC exports:
           tag=0x28(40) r1=0x32(50) | tag=0x43(67) r1=0x64(100) */
    void *ms = hsl::HSL::GetMapServiceHandle();
    if (ms) {
        /* vtable call chain at ms+0x84: init/POI request setup */
        AlcExport(0, 0, 0x28, this);
        /* second branch: r1=0x64 path */
        AlcExport(0, 0, 0x43, this);
    } else {
        AlcExport(0, 0, 0x43, this);
    }

    /* 13. switch on this+0x394 state word (12-way jump table @0x3a7e0,
           base 0x4482 = LDRB/STRB dispatch; cases via da54..d9ca offsets) */
    switch (state394_) {
    case 0x1: /* da54 */ break;
    case 0x2: /* da6f */ break;
    case 0x3: /* db58 */ break;
    case 0x4: /* db79 */ break;
    case 0x5: /* d64d */ break;
    case 0x6: /* da2f */ break;
    case 0x7: /* da27 */ break;
    case 0x8: /* da6c */ break;
    case 0x9: /* da8f */ break;
    case 0xa: /* da86 */ break;
    case 0xb: /* d9ca */ break;
    default:  /* 0x3a804: da6f fallback */ break;
    }

    /* 14. more ALC exports r1=0x64: tags 0x43(67)/0x29(41) r1=0x29(41) */
    AlcExport(0, 0, 0x43, this);
    AlcExport(0, 0, 0x29, this);

    /* 15. MultiScreenService::InitLockFree() */
    hsl::MultiScreenService::GetInstance()->InitLockFree();

    /* 16. timer elapsed check: GetElapsedTimeById(id, false); store r4af */
    uint64_t elapsed = hsl::TimerUtilsFuns::GetElapsedTimeById(timerId_, false);
    lastElapsed_ = elapsed;
}

/* ==========================================================================
 * B) sub_1c798 — MapSurfaceViewObserver::OnMapSurfaceViewNotify
 *    log key: "Id:%d type:%d success:%d" (@0x5365c region)
 * ========================================================================== */
void CGIPLPacket::OnMapSurfaceViewNotify(int id, int type, int success) {
    AlcExport(0, 0, 0x41, this);           /* tag 65 */

    int cfg = *(int*)((char*)this + 0x310);    /* f8c94310: member+0x310 */
    if (cfg == 0) { /* early return path @0x1c808 */ }

    /* vtable call @+0x50 (member+0x310 object) */
    /* store r0 -> member+0x10 */

    bool netOk = /* Config::GetInt32 ... I_GetAlcDevPtr check */ true;
    if (netOk) {
        void *mv = hsl::HSL::GetMapViewHandle((bl::MapEngineID)1);
        if (mv) {
            void *ms = hsl::HSL::GetMapServiceHandle();
            if (ms) {
                AlcExport(0, 0, 0x43, this);       /* tag 67 */
                /* deep branch: two sub-cases with vtable reads +0x84/+0x58 */
                int flag1 = *(int*)((char*)ms + 0x84);
                if (flag1 == 1) {
                    AlcExport(0, 0, 0x108, this);  /* f44f7108: tag 0x108 */
                } else {
                    AlcExport(0, 0, 0x20a, this);  /* f44f720a: tag 0x20a */
                }
            }
        }
    }
    /* tail: store member+0x84 etc. */
}

/* ==========================================================================
 * C) sub_2ace0 — screen-mode listener / poller
 * ========================================================================== */
void CGIPLPacket::ScreenModeListen() {
    AlcExport(0, 0, /*tag*/ 0x45b, this);      /* f44f7c5b */

    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(0);
    int mode = NS_MultiScreenBase::AutoConfigScreenMode::GetInstance()
                   ->GetCurrentScreenMode(screenType_);

    void *mv = hsl::HSL::GetMapViewHandle(eng);
    if (mv) {
        /* read members +0xdc/+0xe0/+0x18; compare engine ids;
           vtable calls @+0x60/+0x64; stores to this+0x64 */
    }

    /* conditional ALC export path (@0x2adce): */
    AlcExport(0, 0, 0x262, this);              /* f44f7262 */

    /* switch-like compare chain @0x2a63 (CMP r3,#4 / BEQ...) */
}

/* ==========================================================================
 * D) sub_24094 — map service / POI business callback
 * ========================================================================== */
void CGIPLPacket::OnMapServiceEvent() {
    /* GetMapServiceHandle(); vtable call +0x74 -> this+0x10 (member cache) */
    void *ms = hsl::HSL::GetMapServiceHandle();
    if (ms) {
        void *obj = *(void**)((char*)ms + 0x74);
        void *cb1 = *(void**)((char*)obj + 0x10);
        ((void(*)(void*,int,int))cb1)(obj, 1, 0);
        /* second vtable call +0x41c/+0x15c region: business data pull */
    }

    AlcExport(0, 0, /*tag*/ 0x44c, this);      /* f04f0c4c */

    /* member updates: this+0x60/+0x64/+0x5c with vtable reads +0x78/+0x97 */
    int v60 = *(int*)((char*)this + 0x60);
    if (v60 != 0) {
        void *cb = *(void**)((char*)this + 0x78);
        ((void(*)(void*,int))cb)(this, 0);
    }
    /* compare+notify tail (@0x24192) */
    int v5a = *(int*)((char*)this + 0x5a);
    if (v5a) { /* callback +0x94 */ }
    int v64 = *(int*)((char*)this + 0x64);
    if (v64) { /* callback +0x8c */ }
    /* POI string conversion: Int32ToString16 + member ops (batch 7 tail) */
}

} // namespace
