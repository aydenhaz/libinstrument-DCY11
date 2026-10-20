/*
 * Batch 12 — named business functions, ranks 13-32 by size (18 functions)
 */
#include <cstdint>
#include <GFrame/CGString.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/CGPackage.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/Config.h>
#include <hsl/Bundle.h>
#include <hsl/InstrumentManager.h>
#include <hsl/MultiScreenService.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <asl/String16Utils.h>
#include <GFrame/CGView.h>

using GNS_FRAME::CGFragment;
using GNS_FRAME::CGView;
using GNS_FRAME::CGViewGroup;


namespace NS_MultiScreenBase {
class AutoOpeningFragment {
public:
    void MapSurfaceViewStatusChange(int32_t,int32_t,void*,int32_t,int32_t,int32_t);
    void OnMapLevelChanged(uint32_t, bool);
    void OnMapPreviewExitOnUI(uint32_t);
    void OnMapPreviewExit(uint32_t);
    void OnUpdateUi(int32_t);
    void OnEnableMapRender(bool, int32_t);
    void FragmentRenderCallbackUI();
    void onDestroyView();
    void EnableMapRender(bool, int);
};
class AutoExscreenNotifyObserver {
public:
    bool OnOpenExScreenUi(const tagSTExtScreen&);
};
class AutoNaviFragment { public: void OnExScreenModeChange(int32_t,int32_t); };
class AutoMultiScreenConfig { public: static void SetMapLevel(int,int); };
class AutoPreviewFragment { public: void ShowPreview(); };
class AutoArNaviFragment { public: void onResume(); };
class AutoCruiseFragment {
public:
    void SetCarlocationToLocAtHMI();
    void OnUpdateUi(int32_t);
    void EnableMapRender(bool, int);
};
class GuideS311UnnormalFragment { public: void OnUpdateUiWidgets(int32_t); };
class CGIPLOpeningFragment { public: static void GoToCross(); };
void ExScreenRenderSync();
void CGAsyncObject_runOnUIThread();
void AutoExscreenNotifyObserver_Ctor();
using NS_MultiScreenBase::AutoOpeningFragment;
using NS_MultiScreenBase::AutoExscreenNotifyObserver;
using NS_MultiScreenBase::AutoNaviFragment;
using NS_MultiScreenBase::AutoMultiScreenConfig;
using NS_MultiScreenBase::AutoPreviewFragment;
using NS_MultiScreenBase::AutoArNaviFragment;
using NS_MultiScreenBase::AutoCruiseFragment;
using NS_MultiScreenBase::GuideS311UnnormalFragment;

struct tagSTStartCrossInfo;

// --- implementations ---
void AutoOpeningFragment::MapSurfaceViewStatusChange(int32_t surfaceId, int32_t type,
                                                     void *handle, int32_t format, int32_t w, int32_t h) {
    /* "surfaceId:%d, type:%d, handle:%p, format:%d, wh:(%d,%d)" */
    void *ms = hsl::HSL::GetMapServiceHandle();
    if (!ms) return;   /* "[mapViewServiceImpl == nullptr]" */
    /* "[DetachSurfaceFromDevice][mapDevice = %p]" */
    /* "[Not Match Screen ID]" / "[mapViewBase == nullptr]" guards */
}

/* sub_2ca8c (1016B) AutoExscreenNotifyObserver::OnOpenExScreenUi */
bool AutoExscreenNotifyObserver::OnOpenExScreenUi(const tagSTExtScreen &st) {
    AutoConfigScreenMode *sm = NS_MultiScreenBase::AutoConfigScreenMode::GetInstance();
    int cur = sm->GetCurrentScreenMode(0);
    sm->SetCurrentScreenMode(0, cur);
    hsl::InstrumentManager::SetInstrumentScreenSize(0, 0);
    hsl::InstrumentManager::SetInstrumentMapSize(0, 0, 0, 0);
    hsl::MultiScreenService::GetInstance()->SetNaviType(0, 0);
    NS_MultiScreenBase::SurfaceConfig sc;
    NS_MultiScreenBase::AutoConfigJsonData::GetInstance()->GetSurfaceConfig(0, sc);
    GNS_FRAME::CGFrameSimulation::ownWorkStation();
    GNS_FRAME::CGFrameSimulation::getId();
    /* GAI_SurfaceSizeGet(); */
    return true;
}

/* sub_2732c (984B) AutoNaviFragment::OnExScreenModeChange */
void AutoNaviFragment::OnExScreenModeChange(int32_t a, int32_t b) {
    /* CrossRouteView setVisible + cross info struct fill; ALC export tail */
}

/* sub_1e084 (904B) AutoMultiScreenConfig::SetMapLevel(int, int) */
void AutoMultiScreenConfig::SetMapLevel(int mapEngineId, int level) {
    hsl::Config *c = hsl::HSL::GetConfigHandle();
    int32_t v = 0;
    c->GetInt32(dice::String16("int32MapLevel"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    /* level clamp + InstrumentManager::SetInstrumentMapSize callout */
}

/* sub_35e2c (840B) AutoOpeningFragment::OnMapLevelChanged(uint32_t, bool) */
void AutoOpeningFragment::OnMapLevelChanged(uint32_t level, bool animated) {
    /* "mapLevel:%d" log; GetMapViewHandle->setMapLevel(level, animated) */
}

/* sub_347c0 (824B) — unnamed; calls: GetSurfaceConfig/GetMultiViewModeInfo/GAI_ExScreenRenderStatusNotify */
void ExScreenRenderSync() {
    NS_MultiScreenBase::SurfaceConfig sc; NS_MultiScreenBase::AutoConfigJsonData::GetInstance()->GetSurfaceConfig(0, sc);
    /* GAI_ExScreenRenderStatusNotify chain */
}

/* sub_41bf4 (812B) CGAsyncObject::runOnUIThread(Functor, Target*, int, Message::Priority, int)
   — GPI::Scheduler::Cancelable wrapper (template instantiation) */
void CGAsyncObject_runOnUIThread() {
    /* Scheduler::postMessageWithCancelable(...) + AsyncObject store */
}

/* sub_29798 (804B) — AutoExscreenNotifyObserver ctor body */
void AutoExscreenNotifyObserver_Ctor() {
    /* member init; GetMapEngineId; RunAsyncDelay chains */
}

/* sub_37470 (800B) AutoOpeningFragment::OnMapPreviewExitOnUI(uint32_t) */
void AutoOpeningFragment::OnMapPreviewExitOnUI(uint32_t id) {
    /* ownWorkStation()->getExhibition(); findFragmentById; hide/show chain on UI thread */
}

/* sub_3c498 (792B) AutoPreviewFragment::ShowPreview() */
void AutoPreviewFragment::ShowPreview() {
    /* preview fragment show: setVisible(true) + size refresh */
}

/* sub_1f614 (780B) AutoArNaviFragment::onResume() */
void AutoArNaviFragment::onResume() {
    /* ArNaviMapView resume: SetScreenSize + observer re-arm */
}

/* sub_41484 (760B) AutoCruiseFragment::SetCarlocationToLocAtHMI() */
void AutoCruiseFragment::SetCarlocationToLocAtHMI() {
    /* GetCarPosition + SetCarlocationToLoc + map center set */
}

/* sub_15878 (748B) CGInstrumentPannelLargeOpeningFragment::GoToCross(...) */
void CGIPLOpeningFragment_GoToCross() {
    /* cross guidance entry: CGIntent putStringExtra + fragment switch */
}

/* sub_3b9a8 (736B) AutoOpeningFragment::FragmentRenderCallbackUI() */
void AutoOpeningFragment::FragmentRenderCallbackUI() {
    /* "FragmentRenderCallback" log + render invalidation */
}

/* sub_35a64 (732B) AutoOpeningFragment::onDestroyView() */
void AutoOpeningFragment::onDestroyView() {
    /* CGFragment::onDestroyView(); base-class teardown */
    /* observer removal + view teardown */
}

/* sub_37194 (732B) AutoOpeningFragment::OnMapPreviewExit(uint32_t) */
void AutoOpeningFragment::OnMapPreviewExit(uint32_t id) {
    /* RunAsyncDelayByEngineId(Closure::bind(OnMapPreviewExitOnUI)) */
}

/* sub_336fc (724B) CGInstrumentPannelLargeGuideS311UnnormalFragment::OnUpdateUiWidgets */
void GuideS311UnnormalFragment::OnUpdateUiWidgets(int32_t type) {
    /* per-theme container switch (classic/motion/tech) + widget refresh */
}

/* sub_37de0 (716B) AutoOpeningFragment::OnUpdateUi(int32_t) */
void AutoOpeningFragment::OnUpdateUi(int32_t type) {
    /* "OnUpdateUi" + dynamic_cast widget refresh */
}

/* sub_40384 (696B) AutoCruiseFragment::OnUpdateUi(int32_t) */
void AutoCruiseFragment::OnUpdateUi(int32_t type) {
    /* cruise widget refresh + cross icon update */
}

/* sub_3689c (684B) AutoOpeningFragment::OnEnableMapRender(bool, int32_t) */
void AutoOpeningFragment::OnEnableMapRender(bool enable, int32_t reason) {
    EnableMapRender(enable, reason);   /* delegates to sub_38ce8 */
}

} // namespace
