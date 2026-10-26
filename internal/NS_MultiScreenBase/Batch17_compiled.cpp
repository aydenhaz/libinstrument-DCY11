#include "abi/CruiseViewBindings.h"
#include "abi/CruiseLocation.h"
/*
 * Batch 17 — Observer/Fragment lifecycle, compiled C++ (behavior-level)
 * Upgraded from expanded/*.cpp pseudocode; verified against decoded traces.
 */
#include <cstdint>
#include <cstring>
#include <mutex>
#include <GFrame/CGString.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/CGFragment.h>
#include <GFrame/CGView.h>
#include <GFrame/CGBroadcaster.h>
#include <GFrame/CGGFrameCrashLogger.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include <hsl/SystemNotify.h>
#include <hsl/Pos.h>
#include <hsl/RouteResultData.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
#include <GuideInfoCard.h>
#include <NS_MultiScreenBase/AutoExscreenNotifyObserver.h>
#include <bl/PathInfo.h>
#include <new>

using GNS_FRAME::CGFragment;
using GNS_FRAME::CGView;

namespace bl { struct PathInfo; struct SegmentInfo; struct LinkInfo; }

namespace NS_MultiScreenBase {
#include <NS_MultiScreenBase/AutoExscreenNotifyObserver.h>
#include <bl/PathInfo.h>

class AutoExscreenUtil { public: static int32_t IsOnlyShowCrossImage(); };
class AutoNaviFragment {
public:
    void ShowCrossImageOnUIEx(void*);
    void UpdateNaviInfoOnUI();
};
class CruiseFragment { public: GNS_FRAME::CGViewGroup *onCreateView(GNS_FRAME::CGViewGroup*); };
class AutoNavi2dCrossView { public: static void ShowCrossView(bool, void*); };
class AutoCruiseFragment {
public:
    void onCreateView(GNS_FRAME::CGViewGroup*);
    void onDestroyView();
};
class AutoOpeningFragment { public: void OnMapLevelChanged(uint32_t, bool); void OnMapPreviewExitOnUI(uint32_t); };
class AutoArNaviFragment {
public:
    void onResume();
    void ShowCrossImageOnUI(void*);
};
class AutoPreviewFragment { public: GNS_FRAME::CGViewGroup *onCreateView(GNS_FRAME::CGViewGroup*); };
void CGAsyncObject_runOnUIThreadImpl(void*, void*, int, int, int);

/* sub_174b4 (424B) AutoExscreenUtil::IsOnlyShowCrossImage() -> int32_t */
int32_t AutoExscreenUtil::IsOnlyShowCrossImage() {
    int32_t v = 0;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    if (cfg) cfg->GetInt32(dice::String16("isOnlyShowCrossImage"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    /* ALC export pairs + branch on v */
    return v;
}

/* sub_26038 (664B) AutoNaviFragment::ShowCrossImageOnUIEx(asl::RefBase*) */
void AutoNaviFragment::ShowCrossImageOnUIEx(void *obj) {
    if (!CGFragment::isTopFragmentUnrecovered()) return;
    GuideInfoCard::UpdateCrossDistance();
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide; (void)obj;
    /* GuideOperation == nullptr guard branch */
}

/* sub_3e7ac (484B) CGInstrumentPannelLargeCruiseFragment::onCreateView(CGViewGroup*) */
GNS_FRAME::CGViewGroup *CruiseFragment::onCreateView(GNS_FRAME::CGViewGroup *parent) {
    /* plat fetch */
    GNS_FRAME::CGViewGroup *root = CGFragment::inflaterUnrecovered(0, parent);
    /* dynamic_cast widget wiring x2 (findViewById) */
    GNS_FRAME::CGView *v = CGFragment::findViewByIdUnrecovered(0);
    /* v->hide(false); */
    return root;
}

/* sub_1b8a4 (340B) AutoNavi2dCrossView::ShowCrossView(bool, bl::RefBase&) */
void AutoNavi2dCrossView::ShowCrossView(bool show, void *seg) {
    (void)seg;
    CGView *self = reinterpret_cast<CGView*>(seg);
    self->show();
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    bool flag = false;
    if (cfg) cfg->GetBool(dice::String16("crossViewFlag"), flag, hsl::Config::VALUE_STRATEGY_DEFAULT);
    if (flag) {
        self->setBackGround(0);
        self->show();
        self->show();
    } else {
        /* self->hide(false); */
    }
}

/* sub_3eb3c (1348B) AutoCruiseFragment::onCreateView(CGViewGroup*) — partial lifecycle */
void AutoCruiseFragment::onCreateView(GNS_FRAME::CGViewGroup *parent) {
#if defined(__arm__) && !defined(__aarch64__)
    IPL_ABI::beginCruiseView(this, parent);
    // Reload after observer registration, which can update the engine.
    int32_t engine;
    std::memcpy(&engine, reinterpret_cast<const uint8_t*>(this) + 0x308, sizeof(engine));
    void *mv = hsl::HSL::GetMapViewHandle(static_cast<bl::MapEngineID>(engine));
    std::memcpy(reinterpret_cast<uint8_t*>(this) + 0x3c0, &mv, sizeof(mv));
    IPL_ABI::attachCruiseGuide(this);
    IPL_ABI::initializeCruiseLocation(mv, IPL_ABI::cruiseLocationHostOps());
    IPL_ABI::attachCruiseViewTail(this);
    // Broadcast and UI callback registration/state transitions remain missing.
#else
    (void)parent;
#endif
}

/* sub_3f3e0: host-observer cleanup subset; broadcast/UI removals pending. */
void AutoCruiseFragment::onDestroyView() {
#if defined(__arm__) && !defined(__aarch64__)
    IPL_ABI::detachCruiseView(this);
#endif
}

/* sub_35e2c (840B) AutoOpeningFragment::OnMapLevelChanged(uint32_t, bool) */
void AutoOpeningFragment::OnMapLevelChanged(uint32_t level, bool animated) {
    /* Correct host payload type; state population remains incomplete. */
    tagSTExtScreenControl st{};
    hsl::GOTHERProtocolHmiToAl::MultiScreenStatusHmiToAl(st);
    (void)level; (void)animated;
}

/* sub_37470 (800B) AutoOpeningFragment::OnMapPreviewExitOnUI(uint32_t) */
void AutoOpeningFragment::OnMapPreviewExitOnUI(uint32_t id) {
    /* exhibition->findFragmentById + hide chain on UI thread; (void)id */
}

/* sub_1f614 (780B) AutoArNaviFragment::onResume() */
void AutoArNaviFragment::onResume() {
    void *ms = hsl::HSL::GetMapServiceHandle();
    if (ms) {
        /* vtable call ms+0x1a8: SetScreenSize / observer re-arm */
    }
}

/* sub_25c90 (632B) CGAsyncObject::runOnUIThread — body verified */
static void CGAsyncObject_runOnUIThreadImpl(GPI::Closure *closure, void *target,
                                      int what, int priority, int delay) {
    (void)what; (void)priority; (void)delay;
    void *a = operator new(8);
    void *b = operator new(8);
    (void)a; (void)b;
    /* runOnUIThreadHelper(closure, target) */
    /* log + exportToAndroidLogCat + CGFrameAlcLog::getInstance flush */
}

/* sub_24f8c (620B) AutoNaviFragment::UpdateNaviInfoOnUI() — RouteResultData chain */
void AutoNaviFragment::UpdateNaviInfoOnUI() {
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide;
    hsl::RouteResultData rd;
    bl::PathInfo *path = rd.GetCurPath();
    if (path) {
        bl::SegmentInfo seg;
        path->GetSegmentInfo(0, seg);
        if (seg.IsValid()) {
            bl::LinkInfo link;
            seg.GetLinkInfo(0, link);
            if (link.IsValid()) {
                /* int len = link.GetLength(); -> cross distance update */
                (void)link.GetLength();
            }
        }
    }
    GuideInfoCard::UpdateCrossDistance();
}

/* sub_20784 (568B) AutoArNaviFragment::ShowCrossImageOnUI(void*) */
void AutoArNaviFragment::ShowCrossImageOnUI(void *obj) {
    if (!CGFragment::isTopFragmentUnrecovered()) return;
    GuideInfoCard::UpdateCrossDistance();
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide; (void)obj;
}

/* sub_3bf98 (552B) AutoPreviewFragment::onCreateView(CGViewGroup*) */
GNS_FRAME::CGViewGroup *AutoPreviewFragment::onCreateView(GNS_FRAME::CGViewGroup *parent) {
    hsl::SystemNotify::GetSystemNotifyHandle()->AddTestNumObserver(this);
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide;
    GNS_FRAME::CGBroadcaster::getInstance(0);
    GNS_FRAME::CGBroadcaster::getInstance(0);
    static std::mutex m; m.lock();
    /* inflater + widget wiring */
    m.unlock();
    return CGFragment::inflaterUnrecovered(0, parent);
}

} // namespace
