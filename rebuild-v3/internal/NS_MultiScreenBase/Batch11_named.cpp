/*
 * Batch 11 — named business-logic functions (top 12 by size, string-identified)
 */
#include <cstdint>
#include <vector>
#include <GFrame/CGString.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/Config.h>
#include <hsl/Bundle.h>
#include <hsl/StringUtils.h>
#include <hsl/Encoding.h>
#include <hsl/InstrumentManager.h>
#include <hsl/MultiScreenService.h>
#include <hsl/DataUtil.h>
#include <hsl/SystemUtils.h>
#include <hsl/Pos.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGFragment.h>
#include <GFrame/CGView.h>
#include <GFrame/CGTextView.h>
#include <GFrame/CGAsyncObject.h>
#include <GFrame/CGGFrameCrashLogger.h>
#include <GFrame/CGImage.h>
#include <asl/String16Utils.h>
#include <GAI.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>

using GNS_FRAME::CGFragment;
using GNS_FRAME::CGFrameSimulation;
using GNS_FRAME::CGView;
using GNS_FRAME::CGViewGroup;
using GNS_FRAME::CGImage;
using hsl::Bundle;
using dice::String16;
namespace NS_MultiScreenBase {
struct SurfaceConfig; struct tagMultiViewModeInfo;
// ScreenType is shared in AbiTypes.h.
enum ENaviMode_ { }; enum ECruiseMode_ { };

/* sub_168e8 (1764B) — AutoExscreenUtil::SetMapParamsToUiConfig(SurfaceConfig&) */
void AutoExscreenUtil_SetMapParamsToUiConfig(const SurfaceConfig &cfg) {
    hsl::Config *c = hsl::HSL::GetConfigHandle();
    if (!c) return;   /* "config == nullptr" */
    /* "screenType=%d, mode=%d, scene=%d, t=%d, val=%d" log */
    { hsl::Bundle b; b.PutBundle(dice::String16("ExScreenLarge"), b); }
    { hsl::Bundle b; b.PutBundle(dice::String16("ExScreenWidget"), b); }
    { hsl::Bundle b; b.PutBundle(dice::String16("ExScreenSmall"), b); }
    /* int32EagleEyeEnable / int32CrossControl / int32MapLevel ... via PutBundle */
}

// Original 0x18264 is implemented by CoverUtil::setConfig and its staged helpers.

/* sub_1ba40 (1452B) — config bundle reader (unnamed, keys recovered) */
bool ReadMapInt32Bundle(hsl::Config *cfg_, void *self) {
    hsl::Config *cfg = cfg_;
    /* int32CrossControl | int32EagleEyeEnable | int32MapLevel | int32MapPreviwe |
       int32MapViewMode | int32mapDayNightMode */
    hsl::Bundle b;
    if (!cfg->GetBundle(dice::String16("mapInt32Bundle"), b)) { /* "_cfg->GetBundle() return false." */ return false; }
    /* per-key GetBundle(dice::String16(key), sub) + Int32ToString16 round-trip */
    return true;
}

/* Receiver-less Batch11 wrappers for these Fragment methods were synthetic:
   they omitted the real `this` pointer and had no callers. Keep their recovered
   code-address notes in analysis/FUNCTIONS.txt; concrete receiver methods are
   provided only where a matching derived Fragment is defined. */

/* sub_36c50 (1176B) — AutoOpeningFragment::OnDeviceRender(int32_t, EGLDevice) */
void AutoOpeningFragment_OnDeviceRender(int32_t dev, int egl, void *self) {
    bool firstCam = hsl::InstrumentManager::GetFirstCameraImageFlag();
    bool isAndroid = hsl::SystemUtils::IsAndroidSystem();
    int offline = hsl::DataUtil::CheckOfflineDataStatus();
    (void)isAndroid; (void)offline;
    /* "isGetFirstCameraImage:%d" / "need send first draw,isWaitARImage:%d,isMapReady:%d" */
    if (!firstCam) {
        /* runOnUIThread closure: "runOnUIThread pTarget must is this" assert */
        /* runOnUIThread closure dispatch ("runOnUIThread pTarget must is this" assert site) */
        CGString s; s.Format(reinterpret_cast<const unsigned short*>(u"OnDeviceRender dev=%d"), dev);
        GNS_FRAME::CGGFrameCrashLogger::exportToAndroidLogCat(0, "OnDeviceRender", 0, s);
    }
}

/* sub_37928 (1020B) — AutoOpeningFragment::MapSurfaceViewStatusChange(int32_t,...) */
void AutoOpeningFragment_MapSurfaceViewStatusChange(int32_t surfaceId, int type, void *handle, int format, int w, int h) {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(0);
    /* "surfaceId:%d, type:%d, handle:%p, format:%d, wh:(%d,%d)" */
    void *ms = hsl::HSL::GetMapServiceHandle();
    if (!ms) { /* "[mapViewServiceImpl == nullptr]" */ return; }
    /* "[DetachSurfaceFromDevice][mapDevice = %p]" / "[Not Match Screen ID" / "[mapViewBase == nullptr]" */
}

/* sub_2ca8c (1016B) — AutoExscreenNotifyObserver::OnOpenExScreenUi(tagSTExtScreen&) */
bool AutoExscreenNotifyObserver_OnOpenExScreenUi(const tagSTExtScreen &st) {
    AutoConfigScreenMode *sm = AutoConfigScreenMode::GetInstance();
    int cur = sm->GetCurrentScreenMode(SCREEN_TYPE_MAIN);
    sm->SetCurrentScreenMode(SCREEN_TYPE_MAIN, /*new mode*/ cur);
    /* InstrumentManager::SetInstrumentScreenSize / SetInstrumentMapSize */
    hsl::InstrumentManager::SetInstrumentScreenSize(0, 0);
    hsl::InstrumentManager::SetInstrumentMapSize(0, 0, 0, 0);
    hsl::MultiScreenService::GetInstance()->SetNaviType(0, 0);
    SurfaceConfig sc;
    AutoConfigJsonData::GetInstance()->GetSurfaceConfig(SCREEN_TYPE_MAIN, sc);
    CGFrameSimulation::ownWorkStation()->getId();
    /* GAI_SurfaceSizeGet(); */ (void)0;
    return true;
}

/* sub_2732c (984B) — AutoNaviFragment (navi view binder) */
void AutoNaviFragment_bind(void *parent) {
    /* findViewById + dynamic_cast to navi widgets; pos handle refresh */
}

} // namespace
