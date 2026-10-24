/*
 * Batch 8 — CGIPLPacket lifecycle family (verified by string xref)
 *
 * onInitEnd     sub_2eb88 (1892B)  — batch 5 (observer registration hub)
 * onPreLoad     sub_2f464 (400B)   — render callback + locked refresh-task scheduling
 * onPostLoadRes sub_2f5fc (1588B)  — resource load completion
 * onPostAlreadyLoadRes sub_2fc30 (112B) — post-load hook
 * refreshGFramePaint   sub_30d34 (128B) — paint refresh
 * onResume      sub_3f080 (692B)   — resume with MapService handle refresh
 * onDestroy     sub_2f2f0 (372B)   — teardown (below)
 */
#include "CGIPLPacket.h"
#include "../AlcExport.h"
#include "../abi/V21PacketTrace.h"
#include "../abi/V21MapTeardown.h"
#include "../abi/V21PacketTaskLock.h"
#include <cstring>
#include <cstdint>
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
#include <hsl/ThirdManager.h>
#include <hsl/UITaskThreadProxy.h>
#include <new>

namespace NS_InstrumentPannelLarge {

/* ---- onPreLoad sub_2f464 (400B) ---- */
void CGIPLPacket::onPreLoad() {
    IPL_ABI::packetTrace("onPreLoad", "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onPreLoad()", 0x10e, true);
    GNS_FRAME::CGWorkStation::ownWorkStation()
        ->setTopRenderCallback(&CGIPLPacket::TopRenderCallback);
    IPL_ABI::PacketTaskLock guard(this);
    auto task = GPI::Closure::bind(this, &CGIPLPacket::refreshGFramePaint);
    void (CGIPLPacket::*method)() = &CGIPLPacket::refreshGFramePaint;
    uintptr_t identity = 0;
    std::memcpy(&identity, &method, sizeof(identity));
    static const char source[] = "/home/jenkins/build/AutoCpp/app/components/MultiScreen/InstrumentPannelLarge/func/GIPLPacket.cpp";
    const uint64_t token = (uint64_t(0x110) << 32) |
        uint32_t(reinterpret_cast<uintptr_t>(source));
    reinterpret_cast<GNS_FRAME::CGAsyncObject*>(this)->scheduleTaskHelper(
        task, 1000, 0, -1, reinterpret_cast<void*>(identity), token);
}

/* ---- onPostLoadRes sub_2f5fc (1588B) ---- */
void CGIPLPacket::onPostLoadRes() {
    AlcExport(0, 0, 0x7c8d, this);           /* f44f7c8d */
    /* 1. CGString(u"...") pool @0x2f670 -> resolve widget name */
    CGString name(reinterpret_cast<const unsigned short*>(u""));
    void *ctx = GNS_FRAME::CGPackage::getContextUnrecovered();
    /* 2. allocate 0x56(86B) object; store via vtable ptrs (0x2f698 pool) */
    void *obj = operator new(0x56);
    /* 3. init member chain this+0x34c etc. */
    /* CORRECTION: original tail dispatches Packet vtable slot 6 after
       registering fragment builders. It does not cancel a task here.
       Builder registration and the dispatch remain to be reconstructed. */
}

/* ---- onPostAlreadyLoadRes sub_2fc30 (112B) ---- */
void CGIPLPacket::onPostAlreadyLoadRes() {
    IPL_ABI::packetTrace("onPostAlreadyLoadRes", "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onPostAlreadyLoadRes()", 0x115, false);
    void** table;
    std::memcpy(&table,this,sizeof(table));
    reinterpret_cast<void (*)(CGIPLPacket*)>(table[14])(this);
}

/* ---- refreshGFramePaint sub_30d34 (128B) ---- */
void CGIPLPacket::refreshGFramePaint() {
    IPL_ABI::packetTrace("refreshGFramePaint", "void NS_InstrumentPannelLarge::CGIPLPacket::refreshGFramePaint()", 0xdf, true);
    static const char source[]="/home/jenkins/build/AutoCpp/app/components/MultiScreen/InstrumentPannelLarge/func/GIPLPacket.cpp";
    const uint64_t token=(uint64_t(0xe0)<<32)|uint32_t(reinterpret_cast<uintptr_t>(source));
    GNS_FRAME::CGFrameSimulation::ownWorkStation()->sendMsg(0x30,0,0,token,nullptr);
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
    IPL_ABI::packetTrace("CGIPLPacket destructor", "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onDestroy()", 0xea, true);
    /* 1. clear render callbacks */
    GNS_FRAME::CGWorkStation::ownWorkStation()->setMapRenderCallback(0);
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(0);
    /* 2. V21 0x2f374: size argument is 0x18 (24). */
    CGString empty;
    GNS_FRAME::CGFrameSimulation::ownWorkStation()->showFpsInfo(false, empty, 0x18);
    /* 3. Original primary host base is at offset zero. Cancellation is
       protected by the constructed host lock at Packet+8. */
    void (CGIPLPacket::*refresh)() = &CGIPLPacket::refreshGFramePaint;
    uintptr_t entry = 0;
    std::memcpy(&entry, &refresh, sizeof(entry));
    {
        IPL_ABI::PacketTaskLock guard(this);
        reinterpret_cast<GNS_FRAME::CGAsyncObject*>(this)->cancelScheduleTaskHelper(
            reinterpret_cast<void*>(entry));
    }
    reinterpret_cast<GNS_FRAME::CGPackage*>(this)->closeAllFragments();
    /* V21 destroys engine-5 map facility before unregistering its UI thread. */
    IPL_ABI::destroyInstrumentMapFacility();
    /* Proxy field is at verified +0x168; the preceding host prefix still
       requires genuine base/script construction before Android use. */
    reinterpret_cast<hsl::ThirdManager*>(hsl::HSL::Get3rdHandle())
        ->RemoveUITaskThread(hsl::UI_TASK_DEVICE_INSTRUMENT);
    delete uiThreadProxy_;
    uiThreadProxy_=nullptr;
    active_=nullptr;
}

} // namespace
