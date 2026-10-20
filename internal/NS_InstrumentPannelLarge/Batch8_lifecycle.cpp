/*
 * Batch 8 — CGIPLPacket lifecycle family (verified by string xref)
 *
 * onInitEnd     sub_2eb88 (1892B)  — batch 5 (observer registration hub)
 * onPreLoad     sub_2f464 (400B)   — render callbacks + 2 allocations (0x16, 0x36)
 * onPostLoadRes sub_2f5fc (1588B)  — resource load completion
 * onPostAlreadyLoadRes sub_2fc30 (112B) — post-load hook
 * refreshGFramePaint   sub_30d34 (128B) — paint refresh
 * onResume      sub_3f080 (692B)   — resume with MapService handle refresh
 * onDestroy     sub_2f2f0 (372B)   — teardown (below)
 */
#include "CGIPLPacket.h"
#include "../AlcExport.h"
#include <GFrame/CGString.h>
#include <GFrame/GblAlc.h>
#include <GFrame/CGWorkStation.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/CGPackage.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/MultiScreenService.h>
#include <new>

namespace NS_InstrumentPannelLarge {

/* ---- onPreLoad sub_2f464 (400B) ---- */
void CGIPLPacket::onPreLoad() {
    AlcExport(0, 0, 0x7c87, this);           /* f44f7c87 */
    /* 1. set render callbacks */
    GNS_FRAME::CGWorkStation::ownWorkStation()
        ->setTopRenderCallback(&CGIPLPacket::TopRenderCallback);
    /* 2. two allocations: 0x16(22B) object + 0x36(54B) object
          (stored this+0x34c/0x34e/0x34f per f8d4 STRB offsets) */
    void *a = operator new(0x16);
    void *b = operator new(0x36);
    /* 3. wiring (e9c0/e9cd strd chains store vtable pairs into new objects) */
    /* 4. ownWorkStation()->setMapRenderCallback(...) per sub_2f4ba region */
}

/* ---- onPostLoadRes sub_2f5fc (1588B) ---- */
void CGIPLPacket::onPostLoadRes() {
    AlcExport(0, 0, 0x7c8d, this);           /* f44f7c8d */
    /* 1. CGString(u"...") pool @0x2f670 -> resolve widget name */
    CGString name(reinterpret_cast<const unsigned short*>(u""));
    void *ctx = GNS_FRAME::CGPackage::getContext();
    /* 2. allocate 0x56(86B) object; store via vtable ptrs (0x2f698 pool) */
    void *obj = operator new(0x56);
    /* 3. init member chain this+0x34c etc. */
    /* 4. AsyncObject::cancelScheduleTaskHelper(this) at tail */
    GNS_FRAME::CGAsyncObject::cancelScheduleTaskHelper(this);
}

/* ---- onPostAlreadyLoadRes sub_2fc30 (112B) ---- */
void CGIPLPacket::onPostAlreadyLoadRes() {
    /* small: ALC export + member update; see analysis */
}

/* ---- refreshGFramePaint sub_30d34 (128B) ---- */
void CGIPLPacket::refreshGFramePaint() {
    /* small paint invalidation: member write + return */
}

/* ---- onResume sub_3f080 (692B) ---- */
void CGIPLPacket::onResume() {
    AlcExport(0, 0, /*tag*/ 0x700, this);    /* f04f0c70 -> block tag=0x70 */
    /* resume timer/member set at f89423cc (this+0x3cc) */
    void *ms = hsl::HSL::GetMapServiceHandle();
    if (ms) {
        /* vtable call ms+0x1a8; store r0; re-read +0x1a8 on this+0x34c chain;
           conditional re-query when member != 0 (0x3f114 CMP/BNE) */
    }
    /* string @0x5a9e7: "onResume, _mapViewBase:%p, _bFirstResume:%d" -> log */
}

/* ---- onDestroy sub_2f2f0 (372B) ---- */
void CGIPLPacket::onDestroy() {
    AlcExport(0, 0, /*tag*/ 0xea, this);     /* f04f0cea */
    /* 1. clear render callbacks */
    GNS_FRAME::CGWorkStation::ownWorkStation()->setMapRenderCallback(0);
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(0);
    /* 2. CGString(); showFpsInfo(false, name, 0x24=36) */
    CGString empty;
    GNS_FRAME::CGWorkStation::ownWorkStation()->showFpsInfo(false, empty, 0x24);
    /* 3. AsyncObject::cancelScheduleTaskHelper(x2) + closeAllFragments */
    GNS_FRAME::CGAsyncObject::cancelScheduleTaskHelper(0);
    GNS_FRAME::CGAsyncObject::cancelScheduleTaskHelper(0);
    GNS_FRAME::CGPackage::closeAllFragments();
    /* 4. observers removal + MultiScreenService cleanup (tail region) */
    hsl::SystemNotify *sn = hsl::HSL::GetSystemNotifyHandle();
    sn->RemoveInstrumentPannelObserver(this);
    /* MultiScreenService::GetInstance()->Reset... */
}

} // namespace
