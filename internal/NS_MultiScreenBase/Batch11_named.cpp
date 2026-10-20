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
namespace bl { typedef int IMapDevice; }
namespace NS_MultiScreenBase {
struct SurfaceConfig; struct tagMultiViewModeInfo;
// ScreenType is shared in AbiTypes.h.
enum ENaviMode_ { }; enum ECruiseMode_ { };

/* sub_38ce8 (1780B) — AutoOpeningFragment::EnableMapRender(bool, RenderReason) */
void AutoOpeningFragment_EnableMapRender(bool enable, int reason) {
    if (!CGFragment::isTopFragment()) return;
    /* "isSurfaceVisable==false" early-out */
    CGFrameSimulation *sim = CGFrameSimulation::ownWorkStation();
    bool isRender = sim->isRender(enable);
    /* "step2 enableMapRender:%d, isRenderMapStatusChanged:%d" log */
    if (!isRender) {
        /* pFrameSimulation NULL assert */
        sim->sendMsg(0, 0, 0, 0, nullptr);
    }
    void *guide = hsl::HSL::GetGuideHandle();
    (void)guide;
    /* ... render enable chain ... */
}

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

/* sub_18264 (1588B) — CoverUtil::setConfig(CGContext*, IMapDevice*, bl) */
void CoverUtil_setConfig(void *ctx, bl::IMapDevice *dev) {
    CGString cover;
    hsl::Config *c = hsl::HSL::GetConfigHandle();
    dice::String16 s;
    c->GetString(dice::String16("sm_enable_widgetCoverForWndId"), s, hsl::Config::VALUE_STRATEGY_DEFAULT);
    int maxFrame = 0;
    c->GetString(dice::String16("sm_support_widgetCoverMaxFrame"), s, hsl::Config::VALUE_STRATEGY_DEFAULT);
    maxFrame = asl::String16Utils::atoi(s.c_str());
    /* "sm_ WidgetCoverForWndId=%s, WidgetCoverMaxFrame=%d" */
    GNS_FRAME::CGImage *img = new GNS_FRAME::CGImage();
    img->create(GPI::CGSize(), EGColorType_RGBA, nullptr);
    /* ... cover widget setup ... */
    delete img;
}

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

/* sub_3eb3c (1348B) — AutoCruiseFragment::onCreateView(CGViewGroup*) */
void AutoCruiseFragment_onCreateView(void *parent, void *self) {
    hsl::SystemNotify::GetSystemNotifyHandle()->AddTestNumObserver(self);
    void *mv = hsl::HSL::GetMapViewHandle((bl::MapEngineID)0);
    void *guide = hsl::HSL::GetGuideHandle();
    (void)mv; (void)guide;
    bl::CarLoc loc; bl::LocInfo li;
    hsl::HSL::GetCarPosition(loc, (bl::MapEngineID)0);
    hsl::Pos *pos = static_cast<hsl::Pos*>(hsl::HSL::GetPosHandle());
    if (!pos) { /* "null posHandle" */ }
    else pos->GetLocInfo(li);
    /* "AutoCruiseFragment::GoToCarLocation (%f,%f)" / "invalid car loc" */
}

/* sub_31538 (1168B) — GuideGuangQiA106Fragment::onCreateView
   sub_42a04 (1080B) — GuideFragment::onCreateView (same body family) */
void GuideFragment_onCreateView(void *parent, bool a106, void *self) {
    GNS_FRAME::CGViewGroup *root = CGFragment::inflater(0, static_cast<GNS_FRAME::CGViewGroup*>(parent));
    /* findViewById + dynamic_cast chains; CGView::show();
       CGTextView::setEllipsizeType; "maneuverIconOnLine has data already!" */
}

/* sub_32e30 (1120B) — GuideS311UnnormalFragment::InitTheme() */
void GuideS311UnnormalFragment_InitTheme() {
    CGViewGroup *root = CGFragment::inflater(0, nullptr);
    hsl::Config *c = hsl::HSL::GetConfigHandle();
    int customType = 0;
    c->GetInt32(dice::String16("largeInstrumentCustomType"), customType, hsl::Config::VALUE_STRATEGY_DEFAULT);
    /* container null asserts: _guideViewContainer / _classicViewContainer / _motionViewContainer / _techViewContainer */
    GNS_FRAME::CGView *v = (GNS_FRAME::CGView*)CGFragment::findViewById(0);
    v->setBackGround(0);
}

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
    CGFrameSimulation::ownWorkStation();
    CGFrameSimulation::getId();
    /* GAI_SurfaceSizeGet(); */ (void)0;
    return true;
}

/* sub_2732c (984B) — AutoNaviFragment (navi view binder) */
void AutoNaviFragment_bind(void *parent) {
    /* findViewById + dynamic_cast to navi widgets; pos handle refresh */
}

} // namespace
