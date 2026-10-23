#include <ExternalScreenTypes.h>
#include <AbiTypes.h>
/*
 * Batch 13 — named business functions, ranks 33-52 (20 functions)
 */
#include <cstdint>
#include <GFrame/CGString.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include <hsl/Bundle.h>
#include <hsl/MultiScreenService.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include "AutoMultiScreenConfigBinding.h"
#include <asl/String16Utils.h>

namespace GPI { struct CGCanvas; }
namespace asl { class RefBase; }
namespace bl {  }
namespace NS_MultiScreenBase {

 struct tagSTStartCrossInfo;
struct SurfaceConfig; struct tagMultiViewModeInfo;
struct MapViewModeConfig {};

class AutoOpeningFragment {
public:
    void OnMapModeChanged(uint32_t, ...);
    void OpenDynamicLevelChanged(uint32_t, ...);
};
class AutoPreviewFragment { public: void OnEnableMapRender(bool, int); };
class AutoNaviFragment {
public:
    void ShowCrossImageOnUIEx(asl::RefBase*);
    void HideCrossImageOnUIEx();
    void UpdateNaviInfoOnUI();
};
class AutoCruiseFragment { public: void OnEnableMapRender(bool, int); };
class AutoArNaviFragment {
public:
    AutoArNaviFragment(bl::MapEngineID, bool);
    void HideCrossImageOnUI();
};
class AutoMultiScreenConfig {
public:
    void ResetMapBindingList(int scene);
    MapViewModeConfig GetMapViewModeConfig();
    void UpdateTrafficEventState(bool);
    void SetMapLevel(int, int);
};
class CGIPLGuideS311Fragment { public: bool GoToCross(...); };
class CGIPLCruiseS311Fragment { public: bool GoToCross(...); };

/* sub_30864 (1232B) NS_InstrumentPannelLarge::TopRenderFpsTimeCallback(CGCanvas*) */
namespace NS_InstrumentPannelLarge {
void TopRenderFpsTimeCallback(GPI::CGCanvas *canvas) {
    /* FPS timing hook: GetElapsedTime + showFpsInfo throttle */
    (void)canvas;
}
}

/* sub_449c4 (1092B) unnamed — GetSurfaceConfig + MultiViewModeInfo + GAI chain */
void ExScreenModeDispatch() {
    AutoConfigJsonData *cfg = AutoConfigJsonData::GetInstance();
    SurfaceConfig sc; cfg->GetSurfaceConfig(0, sc);
    /* GAI_ExScreenRenderStatusNotify dispatch */
}

/* sub_19ae4 (1020B) unnamed — runOnUIThread functor (GPI Closure instantiation) */
void AsyncDispatch() {
    /* CGAsyncObject::runOnUIThread(Closure) — postMessageWithCancelable path */
}

/* sub_3c9c0 (672B) AutoPreviewFragment::OnEnableMapRender(bool, int) */
void AutoPreviewFragment::OnEnableMapRender(bool enable, int reason) {
    (void)enable; (void)reason;
    /* preview map render enable/disable + observer notify */
}

/* sub_1e790: calculate the scene's 0x20-byte binding record and register
   through the map-service and map-view vtable slot +0x3c. */
void AutoMultiScreenConfig::ResetMapBindingList(int scene) {
    AutoMultiScreenConfig_ResetMapBindingList(this, scene);
}

/* sub_26038 (664B) AutoNaviFragment::ShowCrossImageOnUIEx(RefBase*) */
void AutoNaviFragment::ShowCrossImageOnUIEx(asl::RefBase *obj) {
    /* cross image show + refcount guard */
}

/* sub_1a54c (656B) CGInstrumentPannelLargeGuideS311Fragment::GoToCross(...) */
bool CGIPLGuideS311Fragment::GoToCross(...) {
    /* S311 cross guidance entry */
    return true;
}

/* sub_31f84 (656B) CGInstrumentPannelLargeCruiseS311Fragment::GoToCross(...) */
bool CGIPLCruiseS311Fragment::GoToCross(...) {
    return true;
}

/* sub_400f4 (652B) AutoCruiseFragment::OnEnableMapRender(bool, int) */
void AutoCruiseFragment::OnEnableMapRender(bool enable, int reason) {
    (void)enable; (void)reason;
}

/* sub_36174 (648B) AutoOpeningFragment::OnMapModeChanged(uint32_t, ...) */
void AutoOpeningFragment::OnMapModeChanged(uint32_t mode, ...) {
    (void)mode;
}

/* sub_36514 (648B) AutoOpeningFragment::OpenDynamicLevelChanged(uint32_t, ...) */
void AutoOpeningFragment::OpenDynamicLevelChanged(uint32_t level, ...) {
    (void)level;
}

/* sub_18e58 (644B) AutoArNaviFragment ctor */
void ArNaviCtor(bl::MapEngineID eng, bool flag) {
    (void)eng; (void)flag;
}

/* sub_262d0 (644B) AutoNaviFragment::HideCrossImageOnUIEx() */
void AutoNaviFragment::HideCrossImageOnUIEx() {
}

/* sub_25c90 (632B) CGAsyncObject::runOnUIThread (instantiation A) */
void RunOnUIThread_A() {
}

/* sub_24f8c (620B) AutoNaviFragment::UpdateNaviInfoOnUI() */
// Definition retained in Batch17_compiled.cpp.


/* sub_2be04 (620B) AutoMultiScreenConfig::GetMapViewModeConfig() */
MapViewModeConfig AutoMultiScreenConfig::GetMapViewModeConfig() {
    MapViewModeConfig c;
    return c;
}

/* sub_43a04 (616B) unnamed — config GetInt32 cluster */
void ConfigInt32Cluster() {
    hsl::Config *c = hsl::HSL::GetConfigHandle();
    int v = 0;
    c->GetInt32(dice::String16("int32MapViewMode"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
}

/* sub_1e40c (604B) AutoMultiScreenConfig::UpdateTrafficEventState(bool) */
void AutoMultiScreenConfig::UpdateTrafficEventState(bool on) {
    (void)on;
}

/* sub_209bc (604B) AutoArNaviFragment::HideCrossImageOnUI() */
void AutoArNaviFragment::HideCrossImageOnUI() {
}

/* sub_213c0 (572B) CGAsyncObject::runOnUIThread (instantiation B) */
void RunOnUIThread_B() {
}

} // namespace
