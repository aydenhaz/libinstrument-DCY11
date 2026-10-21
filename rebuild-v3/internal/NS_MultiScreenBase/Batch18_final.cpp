/*
 * Batch 18 — final named business functions (6, compiled C++)
 */
#include <cstdint>
#include <mutex>
#include <GFrame/CGString.h>
#include <GFrame/CGFragment.h>
#include <GFrame/CGView.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGBroadcaster.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include <hsl/SystemNotify.h>
#include <hsl/PointInfo.h>
#include <hsl/InstrumentManager.h>
#include <hsl/TimerUtilsFuns.h>
#include <hsl/RouteLayer.h>
#include <NS_MultiScreenBase/AutoExscreenNotifyObserver.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
#include <hsl/MultiScreenService.h>
#include "../AlcExport.h"

using GNS_FRAME::CGFragment;
using GNS_FRAME::CGView;
using GNS_FRAME::CGViewGroup;

namespace NS_InstrumentPannelLarge {
class GuideS311UnnormalFragment {
public:
    void OnUpdateUiWidgets(int32_t);
private:
    GNS_FRAME::CGView *v1_ = 0, *v2_ = 0, *v3_ = 0;
    GNS_FRAME::CGContext *getContext() { return 0; }
    void setContext(GNS_FRAME::CGContext*) {}
};
class GuideGuangQiA106Fragment : public GNS_FRAME::CGFragment {
public:
    GNS_FRAME::CGViewGroup *onCreateView(GNS_FRAME::CGViewGroup*);
};

/* sub_33cfc (468B) CGInstrumentPannelLargeGuideS311UnnormalFragment::
   OnUpdateUiWidgets — theme container switch (classic/motion/tech/unnormal) */
void NS_InstrumentPannelLarge::GuideS311UnnormalFragment::OnUpdateUiWidgets(int32_t type) {
    AlcExport("OnUpdateUiWidgets", "virtual void ...OnUpdateUiWidgets(int32_t)", 0, this);
    switch (type) {
    case 1:   /* classic theme: hide 3, show guide container */
        v1_->hide(false); v2_->hide(false); v3_->hide(false);
        setContext(getContext());
        break;
    case 2:   /* motion theme */
        v1_->hide(false); v2_->hide(false); v3_->hide(false);
        setContext(getContext());
        break;
    case 3:   /* tech theme */
        v1_->hide(false); v2_->hide(false); v3_->hide(false);
        setContext(getContext());
        break;
    default:  /* unnormal: getContext + show */
        setContext(getContext());
        v1_->show();
        break;
    }
}

/* sub_1a158 (460B) CGInstrumentPannelLargeGuideGuangQiA106Fragment::
   onCreateView — widget wiring + PointInfo + pushFragment log */
GNS_FRAME::CGViewGroup *NS_InstrumentPannelLarge::GuideGuangQiA106Fragment::onCreateView(GNS_FRAME::CGViewGroup *parent) {
    GNS_FRAME::CGView *w1 = findViewById(0);  w1->hide(false);
    GNS_FRAME::CGView *w2 = findViewById(1);  w2->show();
    w1->hide(false); w2->hide(false);
    /* PointInfo init; GetGuideHandle; findViewById + dynamic_cast x2;
       CGMagicalString(CrossGuideGuangQiA106Fragment label) x2; pushFragment log */
    hsl::PointInfo pi;
    (void)hsl::HSL::GetGuideHandle();
    return CGFragment::inflater(0, parent);
}

} // namespace NS_InstrumentPannelLarge

namespace NS_MultiScreenBase {
class AutoNaviFragment : public GNS_FRAME::CGFragment {
public:
    void NaviModeChange();
private:
    int targetType_ = 0;
};
class AutoOpeningFragment : public GNS_FRAME::CGFragment {
public:
    void NotifyALRenderStatus();
    GNS_FRAME::CGViewGroup *onCreateView(GNS_FRAME::CGViewGroup*);
private:
    uint64_t timerId_ = 0;
    static void RenderEndCb() {}
};
class AutoPreviewFragment : public GNS_FRAME::CGFragment {
public:
    void onDestroyView();
private:
    void *impl_ = 0;
    void *routeLayer_ = 0;
};

/* sub_268e0 (536B) AutoNaviFragment::NaviModeChange */
void AutoNaviFragment::NaviModeChange() {
    if (!isTopFragment()) return;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    bool flag = false;
    cfg->GetBool(dice::String16("naviModeFlag"), flag, hsl::Config::VALUE_STRATEGY_DEFAULT);
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide;
    int vt = hsl::InstrumentManager::GetInstrumentViewType();
    if (vt != targetType_) {
        hsl::InstrumentManager::SetInstrumentViewType(targetType_);
    }
}

/* sub_389f4 (400B) AutoOpeningFragment::NotifyALRenderStatus() */
void AutoOpeningFragment::NotifyALRenderStatus() {
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    int v = 0;
    cfg->GetInt32(dice::String16("alRenderStatus"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    int cur = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(0);
    (void)cur;
    tagSTExtScreen st{};
    hsl::GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(&st);
    /* multi-branch ALC dispatch (0x7167c/0x71e78/0x71eea/0x71e74/0x71e0a) */
}

/* sub_357b4 (480B) AutoOpeningFragment::onCreateView(CGViewGroup*) */
GNS_FRAME::CGViewGroup *AutoOpeningFragment::onCreateView(GNS_FRAME::CGViewGroup *parent) {
    AlcExport("onCreateView", "virtual void ...onCreateView(CGViewGroup*)", 0, this);
    if (hsl::MultiScreenService::GetInstance()->IsHslStartupFinished()) {
        GNS_FRAME::CGFrameSimulation::ownWorkStation()->setRenderEndCallback(&RenderEndCb);
    }
    uint64_t t = hsl::TimerUtilsFuns::GetElapsedTimeById(timerId_, false);
    (void)t;
    return inflater(0, parent);
}

/* sub_3c2d8 (448B) AutoPreviewFragment::onDestroyView() */
void AutoPreviewFragment::onDestroyView() {
    GNS_FRAME::CGBroadcaster::getInstance(0);
    GNS_FRAME::CGBroadcaster::getInstance(0);
    static std::mutex m;
    { std::lock_guard<std::mutex> lk(m);
      operator delete(impl_); impl_ = 0; }
    hsl::SystemNotify::GetSystemNotifyHandle()->RemoveTestNumObserver(this);
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide;
    hsl::IRouteLayer::Destroy(static_cast<hsl::IRouteLayer*>(routeLayer_));
}

} // namespace NS_MultiScreenBase
