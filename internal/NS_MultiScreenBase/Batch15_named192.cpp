/*
 * Batch 15 — distinct named functions (192, excl. runOnUIThread family)
 * Auto-generated from analysis/CALLGRAPH.pkl; call traces verbatim.
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

namespace bl { typedef int MapEngineID; }

namespace GPI { struct CGCanvas; }
namespace NS_MultiScreenBase {
struct tagSTExtScreen; struct tagSTStartCrossInfo;
struct SurfaceConfig; struct tagMultiViewModeInfo; struct MapViewModeConfig;

// function bodies follow; each sub_XXXX address maps to a binary function

/* sub_268e0 (536B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::NaviModeChanged(const GNS_FRAME::STGBroadcastMsg 
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 *   _ZN3hsl17InstrumentManager21GetInstrumentViewTypeEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_268e0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::NaviModeChanged(const GNS_FRAME::STGBro */
}

/* sub_2931c (536B)
 * void NS_MultiScreenBase::AutoNaviFragment::Enter3DCross()
 *   __stack_chk_fail
 *   _ZN13GuideInfoCard19UpdateCrossDistanceEv
 *   _ZN3hsl3HSL16GetMapViewHandleEN2bl11MapEngineIDE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2931c(void) {
    /* name: void NS_MultiScreenBase::AutoNaviFragment::Enter3DCross() */
}

/* sub_204e0 (500B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::RerouteEndOnUI(int32_t, bl::RouteType, int, boo
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   _ZN3hsl15RouteResultDataC1Ev
 *   _ZNK3hsl15RouteResultData12GetPathCountEv
 *   _ZNK3hsl15RouteResultData11GetPathInfoEiRNS_13PathInfoParamE
 *   _ZN3hsl15RouteResultDataD1Ev
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_204e0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::RerouteEndOnUI(int32_t, bl::RouteType */
}

/* sub_25a9c (500B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::RerouteEndOnUI(int32_t, bl::RouteType, int, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   _ZN3hsl15RouteResultDataC1Ev
 *   _ZNK3hsl15RouteResultData12GetPathCountEv
 *   _ZNK3hsl15RouteResultData11GetPathInfoEiRNS_13PathInfoParamE
 *   _ZN3hsl15RouteResultDataD1Ev
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_25a9c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::RerouteEndOnUI(int32_t, bl::RouteType,  */
}

/* sub_2c070 (500B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::OnSetExternalScreenMapStatusUi(int, int, int, i
 *   _ZN3hsl3HSL16GetMapViewHandleEN2bl11MapEngineIDE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME9CGUITimer5startEiy
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_2c070(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::OnSetExternalScreenMapStatusUi(int, i */
}

/* sub_19f44 (488B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::ShowCrossImageOnUI(
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN13GuideInfoCard19UpdateCrossDistanceEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_19f44(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::ShowCross */
}

/* sub_3e7ac (484B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseFragment::onCreateView(GNS_FRAME
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL15GetPlatformUtilEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment8inflaterEiPNS_11CGViewGroupE
 *   __dynamic_cast
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 *   __dynamic_cast
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 */
static void fn_3e7ac(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseFragment::onCreateView */
}

/* sub_357b4 (480B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::onCreateView(GNS_FRAME::CGViewGroup *)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl18MultiScreenService11GetInstanceEv
 *   _ZN3hsl18MultiScreenService20IsHslStartupFinishedEv
 *   _ZN9GNS_FRAME17CGFrameSimulation14ownWorkStationEv
 *   _ZN9GNS_FRAME17CGFrameSimulation20setRenderEndCallbackEPFvvE
 *   _ZN3hsl14TimerUtilsFuns18GetElapsedTimeByIdExb
 *   __stack_chk_fail
 */
static void fn_357b4(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::onCreateView(GNS_FRAME::CGViewGroup  */
}

/* sub_33cfc (468B)
 * void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::SwitchTheme(int32_t
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME10CGFragment10getContextEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME10CGFragment10getContextEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 */
static void fn_33cfc(void) {
    /* name: void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::SwitchThe */
}

/* sub_1a158 (460B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::OnNaviFinish(bl::Na
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN3hsl9PointInfoC1Ev
 *   __aeabi_memclr4
 *   _ZN3hsl3HSL14GetGuideHandleEv
 */
static void fn_1a158(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::OnNaviFin */
}

/* sub_211f4 (460B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnMapCrossControl(int32_t, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_211f4(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnMapCrossControl(int32_t, bool) */
}

/* sub_1765c (448B)
 * static bool NS_MultiScreenBase::AutoExscreenUtil::OpenDynamicLevel(NS_MultiScreenBase::ScreenType, N
 *   _ZN3hsl3HSL16GetMapViewHandleEN2bl11MapEngineIDE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_1765c(void) {
    /* name: static bool NS_MultiScreenBase::AutoExscreenUtil::OpenDynamicLevel(NS_MultiScreenBase::Scr */
}

/* sub_3c2d8 (448B)
 * virtual void NS_MultiScreenBase::AutoPreviewFragment::onDestroyView()
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZNSt6__ndk15mutex4lockEv
 *   _ZdlPv
 *   _ZN3hsl3HSL21GetSystemNotifyHandleEv
 *   _ZN3hsl12SystemNotify21RemoveTestNumObserverEPNS_15TestNumObserverE
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN3hsl11IRouteLayer7DestroyEPS0_
 *   __stack_chk_fail
 */
static void fn_3c2d8(void) {
    /* name: virtual void NS_MultiScreenBase::AutoPreviewFragment::onDestroyView() */
}

/* sub_174b4 (424B)
 * static int32_t NS_MultiScreenBase::AutoExscreenUtil::IsOnlyShowCrossImageFromUser(NS_MultiScreenBase
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl6Config8GetInt32ERKN4dice8String16ERiNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_174b4(void) {
    /* name: static int32_t NS_MultiScreenBase::AutoExscreenUtil::IsOnlyShowCrossImageFromUser(NS_Multi */
}

/* sub_16fcc (416B)
 * static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetMapParamsFromUiConfig(NS_MultiScreenBase::Sc
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_16fcc(void) {
    /* name: static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetMapParamsFromUiConfig(NS_MultiScre */
}

/* sub_232e4 (416B)
 * void NS_MultiScreenBase::AutoArNaviFragment::AllViewInited(NS_MultiScreenBase::ViewInitType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL12GetPosHandleEv
 *   _ZN3hsl3Pos16GetGPSActivationEv
 *   __stack_chk_fail
 */
static void fn_232e4(void) {
    /* name: void NS_MultiScreenBase::AutoArNaviFragment::AllViewInited(NS_MultiScreenBase::ViewInitTyp */
}

/* sub_178d0 (412B)
 * static bool NS_MultiScreenBase::AutoExscreenUtil::IsMapScaleSyncOpen(NS_MultiScreenBase::ScreenType)
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl6Config8GetInt32ERKN4dice8String16ERiNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_178d0(void) {
    /* name: static bool NS_MultiScreenBase::AutoExscreenUtil::IsMapScaleSyncOpen(NS_MultiScreenBase::S */
}

/* sub_2d55c (412B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddMapRenderObserver(NS_MultiScreenBase::AutoMa
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2d55c(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddMapRenderObserver(NS_MultiScreenBa */
}

/* sub_1b524 (408B)
 * void NS_MultiScreenBase::AutoNavi2dCrossView::InitCrossViewLayoyt()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   memcmp
 *   _ZN9GNS_FRAME12LayoutParams15getLayoutParamsEPNS_6CGViewE
 *   _ZN9GNS_FRAME12LayoutParams15setLayoutParamsEPNS_6CGViewERS0_
 *   _ZN9GNS_FRAME12LayoutParams15getLayoutParamsEPNS_6CGViewE
 *   __aeabi_memcpy4
 *   _ZN9GNS_FRAME12LayoutParamsD1Ev
 *   _ZN9GNS_FRAME12LayoutParams15setLayoutParamsEPNS_6CGViewERS0_
 *   _ZN9GNS_FRAME12LayoutParamsD1Ev
 *   __stack_chk_fail
 */
static void fn_1b524(void) {
    /* name: void NS_MultiScreenBase::AutoNavi2dCrossView::InitCrossViewLayoyt() */
}

/* sub_1def4 (400B)
 * bool NS_MultiScreenBase::AutoMultiScreenConfig::SetMapViewMode(NS_MultiScreenBase::MapViewModeConfig
 *   _ZN3hsl3HSL16GetMapViewHandleEN2bl11MapEngineIDE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1def4(void) {
    /* name: bool NS_MultiScreenBase::AutoMultiScreenConfig::SetMapViewMode(NS_MultiScreenBase::MapView */
}

/* sub_2d7b0 (400B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddUiUpdateObserver(NS_MultiScreenBase::AutoUiU
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2d7b0(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddUiUpdateObserver(NS_MultiScreenBas */
}

/* sub_2da54 (400B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddMapCrossControlObserver(NS_MultiScreenBase::
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2da54(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddMapCrossControlObserver(NS_MultiSc */
}

/* sub_2dc08 (400B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddExScreenModeOperateObserver(NS_MultiScreenBa
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2dc08(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddExScreenModeOperateObserver(NS_Mul */
}

/* sub_389f4 (400B)
 * void NS_MultiScreenBase::AutoOpeningFragment::NotifyALRenderStatus(NS_MultiScreenBase::RenderStatus)
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config8GetInt32ERKN4dice8String16ERiNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl21GOTHERProtocolHmiToAl27ExScreenRenderActionHmiToAlEP14tagSTExtScreen
 *   __stack_chk_fail
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_389f4(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::NotifyALRenderStatus(NS_MultiScreenBase::Ren */
}

/* sub_3e608 (400B)
 * NS_MultiScreenBase::AutoCruiseFragment::AutoCruiseFragment(bl::MapEngineID)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3e608(void) {
    /* name: NS_MultiScreenBase::AutoCruiseFragment::AutoCruiseFragment(bl::MapEngineID) */
}

/* sub_1b6bc (388B)
 * void NS_MultiScreenBase::AutoNavi2dCrossView::HideCrossView()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME13CGProgressBar11setProgressEi
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView13setBackGroundEi
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1b6bc(void) {
    /* name: void NS_MultiScreenBase::AutoNavi2dCrossView::HideCrossView() */
}

/* sub_3c7b0 (388B)
 * virtual void NS_MultiScreenBase::AutoPreviewFragment::ExitPreview()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3c7b0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoPreviewFragment::ExitPreview() */
}

/* sub_26dac (380B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnMapCrossControl(int32_t, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_26dac(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnMapCrossControl(int32_t, bool) */
}

/* sub_18898 (368B)
 * void NS_MultiScreenBase::CoverUtil::TopRenderCoverCallback(GNS_FRAME::CGCanvas *)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_18898(void) {
    /* name: void NS_MultiScreenBase::CoverUtil::TopRenderCoverCallback(GNS_FRAME::CGCanvas *) */
}

/* sub_24d64 (368B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::onDestroyView()
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN3hsl3HSL21GetSystemNotifyHandleEv
 *   _ZN3hsl12SystemNotify21RemoveTestNumObserverEPNS_15TestNumObserverE
 *   _ZN3hsl3HSL12GetPosHandleEv
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN3hsl11IRouteLayer7DestroyEPS0_
 *   __stack_chk_fail
 */
static void fn_24d64(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::onDestroyView() */
}

/* sub_41f20 (368B)
 * virtual bool NS_MultiScreenBase::AutoBaseFragment::IsAtSceneAbleToSwtichPreview()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetRouteHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_41f20(void) {
    /* name: virtual bool NS_MultiScreenBase::AutoBaseFragment::IsAtSceneAbleToSwtichPreview() */
}

/* sub_42e78 (368B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::onDestroyView()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   __stack_chk_fail
 */
static void fn_42e78(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::onDestroyView */
}

/* sub_27e48 (364B)
 * void NS_MultiScreenBase::AutoNaviFragment::Exit3DCross()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_27e48(void) {
    /* name: void NS_MultiScreenBase::AutoNaviFragment::Exit3DCross() */
}

/* sub_20e48 (360B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::NaviModeChangedOnUI(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_20e48(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::NaviModeChangedOnUI(bl::NaviType) */
}

/* sub_3fa8c (360B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::NaviModeChangedOnUI(bl::NaviType)
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3fa8c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::NaviModeChangedOnUI(bl::NaviType) */
}

/* sub_1f988 (344B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::onDestroyView()
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN3hsl3HSL21GetSystemNotifyHandleEv
 *   _ZN3hsl12SystemNotify21RemoveTestNumObserverEPNS_15TestNumObserverE
 *   _ZN3hsl3HSL12GetPosHandleEv
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN3hsl11IRouteLayer7DestroyEPS0_
 *   __stack_chk_fail
 */
static void fn_1f988(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::onDestroyView() */
}

/* sub_1b8a4 (340B)
 * void NS_MultiScreenBase::AutoNavi2dCrossView::ShowCrossView(bool, bl::CrossType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   _ZN9GNS_FRAME6CGView13setBackGroundEi
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_1b8a4(void) {
    /* name: void NS_MultiScreenBase::AutoNavi2dCrossView::ShowCrossView(bool, bl::CrossType) */
}

/* sub_1ea2c (340B)
 * static bool NS_MultiScreenBase::AutoExscreenUtil::ShouldEnableEagleEye(NS_MultiScreenBase::ScreenTyp
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1ea2c(void) {
    /* name: static bool NS_MultiScreenBase::AutoExscreenUtil::ShouldEnableEagleEye(NS_MultiScreenBase: */
}

/* sub_1c478 (336B)
 * void NS_MultiScreenBase::AutoMultiScreenConfig::SetLayoutLeftTop(NS_MultiScreenBase::SceneType, GNS_
 *   _ZN18NS_MultiScreenBase18AutoConfigJsonData11GetInstanceEv
 *   _ZNK18NS_MultiScreenBase18AutoConfigJsonData16GetSurfaceConfigENS_10ScreenTypeERNS_13SurfaceConfigE
 *   _ZN8CGStringD1Ev
 *   _ZN9GNS_FRAME12LayoutParams15getLayoutParamsEPNS_6CGViewE
 *   _ZNK9GNS_FRAME9CGContext13getScreenSizeEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME12LayoutParams15setLayoutParamsEPNS_6CGViewERS0_
 *   _ZN9GNS_FRAME12LayoutParamsD1Ev
 *   __stack_chk_fail
 */
static void fn_1c478(void) {
    /* name: void NS_MultiScreenBase::AutoMultiScreenConfig::SetLayoutLeftTop(NS_MultiScreenBase::Scene */
}

/* sub_227e4 (336B)
 * void NS_MultiScreenBase::AutoArNaviFragment::Exit3DCross()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_227e4(void) {
    /* name: void NS_MultiScreenBase::AutoArNaviFragment::Exit3DCross() */
}

/* sub_3f3e0 (336B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::onDestroyView()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN3hsl3HSL21GetSystemNotifyHandleEv
 *   _ZN3hsl12SystemNotify21RemoveTestNumObserverEPNS_15TestNumObserverE
 *   _ZN3hsl6Config20RemoveConfigObserverEPNS_14ConfigObserverE
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   __stack_chk_fail
 */
static void fn_3f3e0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::onDestroyView() */
}

/* sub_3829c (332B)
 * static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetDayNightModeInfo(NS_MultiScreenBase::ScreenT
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL24GetDayStatusNotifyHandleEv
 *   _ZN3hsl19UtilDayStatusNotify9IsUiNightEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3829c(void) {
    /* name: static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetDayNightModeInfo(NS_MultiScreenBas */
}

/* sub_417f8 (332B)
 * bl::NaviType NS_MultiScreenBase::AutoCruiseFragment::GetNaviType()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_417f8(void) {
    /* name: bl::NaviType NS_MultiScreenBase::AutoCruiseFragment::GetNaviType() */
}

/* sub_1ebf0 (328B)
 * virtual void GAutoArNaviView::OnArMapViewDestroyed()
 *   _Znwj
 *   _Znwj
 *   _ZN9GNS_FRAME13CGAsyncObject19runOnUIThreadHelperERN3GPI7Closure8CallableIFvvEEEPvyNS_20__EGMsgInQueuePolicyENS1_7Message8PriorityEy
 *   __stack_chk_fail
 */
static void fn_1ebf0(void) {
    /* name: virtual void GAutoArNaviView::OnArMapViewDestroyed() */
}

/* sub_1ab38 (324B)
 * bool NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::IsNeedSwitchtoUnnormalMode(
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN2bl8PathInfoC1Ev
 *   _ZNK2bl8PathInfo7IsValidEv
 *   _ZN2bl11SegmentInfoC1Ev
 *   _ZNK2bl8PathInfo14GetSegmentInfoEjRNS_11SegmentInfoE
 *   _ZNK2bl11SegmentInfo7IsValidEv
 *   _ZNK2bl11SegmentInfo9GetLengthEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 *   _ZN2bl11SegmentInfoD1Ev
 *   _ZN2bl8PathInfoD1Ev
 */
static void fn_1ab38(void) {
    /* name: bool NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::IsNeedSwitchtoUnn */
}

/* sub_19234 (308B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onResume()
 *   _Znwj
 *   _ZN9GNS_FRAME10CGUIObjectC2Ev
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME13CGBroadcaster11getInstanceEj
 *   _ZN9GNS_FRAME13CGBroadcaster10publishMsgERKNS_16STGBroadcastMsg_Ey
 *   __stack_chk_fail
 */
static void fn_19234(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onResume() */
}

/* sub_29abc (308B)
 * NS_MultiScreenBase::AutoExscreenNotifyObserver::~AutoExscreenNotifyObserver()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl11AsyncObject9CancelAllEj
 *   _ZN3hsl3HSL21GetSystemNotifyHandleEv
 *   _ZN3hsl12SystemNotify28RemoveExscreenNotifyObserverEPNS_22ExscreenNotifyObserverE
 *   _ZN3hsl8UIObjectD1Ev
 */
static void fn_29abc(void) {
    /* name: NS_MultiScreenBase::AutoExscreenNotifyObserver::~AutoExscreenNotifyObserver() */
}

/* sub_3bc88 (308B)
 * bool NS_MultiScreenBase::AutoOpeningFragment::IsAutostartEnableMapRender()
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config8GetInt32ERKN4dice8String16ERiNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3bc88(void) {
    /* name: bool NS_MultiScreenBase::AutoOpeningFragment::IsAutostartEnableMapRender() */
}

/* sub_2bcdc (296B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::OnSetExternalScreenCrossControlUi(int, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2bcdc(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::OnSetExternalScreenCrossControlUi(int */
}

/* sub_332cc (296B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::UpdateNaviI
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN2bl8PathInfoC1Ev
 *   _ZNK2bl8PathInfo7IsValidEv
 *   _ZN2bl11SegmentInfoC1Ev
 *   _ZNK2bl8PathInfo14GetSegmentInfoEjRNS_11SegmentInfoE
 *   _ZNK2bl11SegmentInfo7IsValidEv
 *   _ZNK2bl11SegmentInfo9GetLengthEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN2bl11SegmentInfoD1Ev
 *   _ZN2bl8PathInfoD1Ev
 *   __stack_chk_fail
 */
static void fn_332cc(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::U */
}

/* sub_41124 (284B)
 * void NS_MultiScreenBase::AutoCruiseFragment::AllViewInited()
 *   _ZN29CGCustomCruiseTrafficLaneView12SetAllowShowEb
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3Pos11GetGPSValidEv
 */
static void fn_41124(void) {
    /* name: void NS_MultiScreenBase::AutoCruiseFragment::AllViewInited() */
}

/* sub_398a8 (280B)
 * void NS_MultiScreenBase::AutoOpeningFragment::OnRealCityAnimationFinishedUi()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_398a8(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::OnRealCityAnimationFinishedUi() */
}

/* sub_3fbf4 (280B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::GuideModuleInited(const GNS_FRAME::STGBroadcast
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3fbf4(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::GuideModuleInited(const GNS_FRAME::ST */
}

/* sub_42654 (280B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseGuangQiA106Fragment::onCreateVie
 *   _ZN9GNS_FRAME10CGFragment8inflaterEiPNS_11CGViewGroupE
 *   __dynamic_cast
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 *   __dynamic_cast
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 *   __dynamic_cast
 *   _ZN9GNS_FRAME10CGFragment12findViewByIdEi
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   __stack_chk_fail
 */
static void fn_42654(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseGuangQiA106Fragment::o */
}

/* sub_21000 (276B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::GuideFinishCardClosedOnUI()
 *   _ZN3hsl3HSL15GetConfigHandleEv
 *   _ZN3hsl6Config7GetBoolERKN4dice8String16ERbNS0_14VALUE_STRATEGYE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_21000(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::GuideFinishCardClosedOnUI() */
}

/* sub_399c0 (276B)
 * void NS_MultiScreenBase::AutoOpeningFragment::OnMainMapForegroundStatusBroadcast(const GNS_FRAME::ST
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_399c0(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::OnMainMapForegroundStatusBroadcast(const GNS */
}

/* sub_393dc (272B)
 * bool NS_MultiScreenBase::AutoOpeningFragment::IsEglSurfaceValid()
 *   _ZN9GNS_FRAME17CGFrameSimulation14ownWorkStationEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_393dc(void) {
    /* name: bool NS_MultiScreenBase::AutoOpeningFragment::IsEglSurfaceValid() */
}

/* sub_3f5ec (272B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::UpdateCruiseInfoOnUI()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   _ZN8CGStringC1EPKt
 *   _ZN8CGStringD1Ev
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3f5ec(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::UpdateCruiseInfoOnUI() */
}

/* sub_4281c (272B)
 * NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::CGInstrumentPannelLargeGuideFragment
 *   _Znwj
 *   __aeabi_memclr4
 *   __aeabi_memclr
 *   _ZN3asl12String8Utils8snprintfEPciPKcz
 *   __stack_chk_fail
 */
static void fn_4281c(void) {
    /* name: NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::CGInstrumentPannelLargeGui */
}

/* sub_3f984 (264B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::NaviModeChanged(const GNS_FRAME::STGBroadcastMs
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3f984(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::NaviModeChanged(const GNS_FRAME::STGB */
}

/* sub_1c65c (260B)
 * bool NS_MultiScreenBase::AutoMultiScreenConfig::IsEnableCrossType(NS_MultiScreenBase::SceneType, bl:
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1c65c(void) {
    /* name: bool NS_MultiScreenBase::AutoMultiScreenConfig::IsEnableCrossType(NS_MultiScreenBase::Scen */
}

/* sub_21788 (260B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnUpdateUi(int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_21788(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnUpdateUi(int32_t) */
}

/* sub_21ca8 (260B)
 * void NS_MultiScreenBase::AutoArNaviFragment::OnSetShowEagleEyeUI(bool)
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_21ca8(void) {
    /* name: void NS_MultiScreenBase::AutoArNaviFragment::OnSetShowEagleEyeUI(bool) */
}

/* sub_27704 (260B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnUpdateUi(int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_27704(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnUpdateUi(int32_t) */
}

/* sub_1efbc (244B)
 * void GAutoArNaviView::UninitArNaviMapView()
 *   _ZN3hsl17InstrumentManager23GetInstrumentScreenSizeERjS1_
 *   _ZN3hsl13ArNaviMapView13SetScreenSizeEjj
 *   _ZN3hsl17InstrumentManager20GetInstrumentMapSizeERjS1_S1_S1_
 *   _ZN3hsl13ArNaviMapView10SetMapSizeEjj
 *   _ZN3hsl13ArNaviMapView14SetMapStartPosEjj
 *   _ZN3hsl13ArNaviMapView12ResetMapViewEN2bl11MapEngineIDE
 *   _ZN3hsl13ArNaviMapView14ResetArMapViewEv
 *   _ZN3hsl13ArNaviMapView13IsArReplayingEv
 *   _ZN3hsl13ArNaviMapView15StopArReplayingEv
 *   _ZN3hsl13ArNaviMapView19SetArScreenViewTypeEN2bl11MapEngineIDE
 *   _ZN3hsl13ArNaviMapView23SetTrafficLightListenerEPNS_22ArTrafficLightObserverE
 *   _ZN3hsl13ArNaviMapView20SetArMapViewObserverEPNS_21ArNaviMapViewObserverE
 */
static void fn_1efbc(void) {
    /* name: void GAutoArNaviView::UninitArNaviMapView() */
}

/* sub_385d0 (244B)
 * void NS_MultiScreenBase::AutoOpeningFragment::RefreshUI()
 *   _ZN9GNS_FRAME17CGFrameSimulation14ownWorkStationEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME17CGFrameSimulation7sendMsgEiiiyPN3GPI12ScopedRefptrINS1_11MessageCtrlEEE
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_385d0(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::RefreshUI() */
}

/* sub_2d964 (240B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::RemoveUiUpdateObserver(NS_MultiScreenBase::Auto
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2d964(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::RemoveUiUpdateObserver(NS_MultiScreen */
}

/* sub_2ddbc (240B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::RemoveExScreenModeOperateObserver(NS_MultiScree
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2ddbc(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::RemoveExScreenModeOperateObserver(NS_ */
}

/* sub_41954 (236B)
 * NS_MultiScreenBase::AutoBaseFragment::AutoBaseFragment(bl::MapEngineID, const asl::char8 *)
 *   _ZN9GNS_FRAME10CGFragmentC2ENS0_11ELaunchModeE
 *   _ZN3hsl12ObserverBaseC2EPKc
 *   _ZN3asl12String8Utils8snprintfEPciPKcz
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_41954(void) {
    /* name: NS_MultiScreenBase::AutoBaseFragment::AutoBaseFragment(bl::MapEngineID, const asl::char8 * */
}

/* sub_18a08 (232B)
 * void TopRenderCallback(GNS_FRAME::CGCanvas *)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_18a08(void) {
    /* name: void TopRenderCallback(GNS_FRAME::CGCanvas *) */
}

/* sub_1ed8c (232B)
 * void GAutoArNaviView::InitArNaviMapView()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl17ArMapViewInstance15CreateArMapViewEN2bl11MapEngineIDERPNS_13ArNaviMapViewEPNS_23IArMapLifeCycleCallBackEb
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1ed8c(void) {
    /* name: void GAutoArNaviView::InitArNaviMapView() */
}

/* sub_25790 (232B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::RerouteBeginOnUI(int32_t, bl::RouteType, int)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_25790(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::RerouteBeginOnUI(int32_t, bl::RouteType */
}

/* sub_3bdbc (232B)
 * void NS_MultiScreenBase::AutoOpeningFragment::setCoverUtil(NS_MultiScreenBase::CoverUtil *)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL19GetMapServiceHandleEv
 *   _ZN9GNS_FRAME10CGFragment10getContextEv
 *   _ZN8CGStringD1Ev
 *   __stack_chk_fail
 */
static void fn_3bdbc(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::setCoverUtil(NS_MultiScreenBase::CoverUtil * */
}

/* sub_333f4 (228B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::OnNaviFinis
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZNK9GNS_FRAME6CGView9isVisibleEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZNK9GNS_FRAME6CGView9isVisibleEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZNK9GNS_FRAME6CGView9isVisibleEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZNK9GNS_FRAME6CGView9isVisibleEv
 */
static void fn_333f4(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::O */
}

/* sub_3f74c (228B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::ShowCruiseLaneInfoOnUI()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   __stack_chk_fail
 */
static void fn_3f74c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::ShowCruiseLaneInfoOnUI() */
}

/* sub_39c80 (224B)
 * void NS_MultiScreenBase::AutoOpeningFragment::OnStartupHSLFinished(const GNS_FRAME::STGBroadcastMsg 
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_39c80(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::OnStartupHSLFinished(const GNS_FRAME::STGBro */
}

/* sub_3d1d4 (224B)
 * bool NS_MultiScreenBase::AutoPreviewFragment::IsNaviRunning()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3d1d4(void) {
    /* name: bool NS_MultiScreenBase::AutoPreviewFragment::IsNaviRunning() */
}

/* sub_3d2dc (224B)
 * void NS_MultiScreenBase::AutoPreviewFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiScreenBase:
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3d2dc(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiS */
}

/* sub_201e4 (216B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::RerouteBeginOnUI(int32_t, bl::RouteType, int)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_201e4(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::RerouteBeginOnUI(int32_t, bl::RouteTy */
}

/* sub_1724c (212B)
 * static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetMapViewModeInfo(NS_MultiScreenBase::ScreenTy
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1724c(void) {
    /* name: static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetMapViewModeInfo(NS_MultiScreenBase */
}

/* sub_195bc (212B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::SwitchNaviMode(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   GAI_DispatchArStatusNotify
 *   __stack_chk_fail
 */
static void fn_195bc(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::SwitchNaviM */
}

/* sub_1b454 (208B)
 * void NS_MultiScreenBase::AutoNavi2dCrossView::InitView(NS_MultiScreenBase::ScreenType, const NS_Mult
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME13CGProgressBar11setProgressEi
 *   _ZN3hsl18MultiScreenService11GetInstanceEv
 *   _ZN3hsl18MultiScreenService29SetCrossViewProgressBarHeightEN2bl11MapEngineIDEi
 */
static void fn_1b454(void) {
    /* name: void NS_MultiScreenBase::AutoNavi2dCrossView::InitView(NS_MultiScreenBase::ScreenType, con */
}

/* sub_1fc98 (204B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::UpdateExitDirectionInfoOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   __stack_chk_fail
 */
static void fn_1fc98(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::UpdateExitDirectionInfoOnUI() */
}

/* sub_25244 (204B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::UpdateExitDirectionInfoOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   __stack_chk_fail
 */
static void fn_25244(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::UpdateExitDirectionInfoOnUI() */
}

/* sub_15788 (200B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::onResume()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl18MultiScreenService11GetInstanceEv
 *   _ZNK3hsl18MultiScreenService15IsShowStartLogoEN2bl11MapEngineIDE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZNK9GNS_FRAME8CGObjMem16checkObjMemValidEv
 */
static void fn_15788(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::onResume() */
}

/* sub_1a9fc (200B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::OnUiStyleChangedOnU
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 */
static void fn_1a9fc(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::OnUiStyle */
}

/* sub_216c0 (200B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnSetShowEagleEye(int32_t, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_216c0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnSetShowEagleEye(int32_t, bool) */
}

/* sub_32430 (200B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseS311Fragment::OnUiStyleChangedOn
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME6CGView4showEv
 *   _ZN9GNS_FRAME6CGView4hideEb
 *   _ZN9GNS_FRAME6CGView4hideEb
 */
static void fn_32430(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseS311Fragment::OnUiStyl */
}

/* sub_215fc (196B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnSetMapPreview(int32_t, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_215fc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnSetMapPreview(int32_t, bool) */
}

/* sub_19690 (192B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnClickArBtn(int32_t,
 *   _ZN3hsl13ArNaviMapView13StartArRecordEv
 *   __stack_chk_fail
 */
static void fn_19690(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnClickArBt */
}

/* sub_39bc0 (192B)
 * void NS_MultiScreenBase::AutoOpeningFragment::OnMainScreenGoMap(const GNS_FRAME::STGBroadcastMsg &)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl18MultiScreenService11GetInstanceEv
 *   _ZN3hsl18MultiScreenService18SetMainScreenGoMapEb
 *   __stack_chk_fail
 */
static void fn_39bc0(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::OnMainScreenGoMap(const GNS_FRAME::STGBroadc */
}

/* sub_422d8 (192B)
 * void NS_MultiScreenBase::AutoBaseFragment::DumpScreenInfo()
 *   _ZN9GNS_FRAME17CGFrameSimulation14ownWorkStationEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_422d8(void) {
    /* name: void NS_MultiScreenBase::AutoBaseFragment::DumpScreenInfo() */
}

/* sub_37800 (188B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::NetStatusChange(STNetStatus)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_37800(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::NetStatusChange(STNetStatus) */
}

/* sub_37d24 (188B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::LanguageSwitch(ELanguageType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_37d24(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::LanguageSwitch(ELanguageType) */
}

/* sub_3d4ec (188B)
 * void NS_MultiScreenBase::AutoPreviewFragment::NaviModeChanged(const GNS_FRAME::STGBroadcastMsg &)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3d4ec(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::NaviModeChanged(const GNS_FRAME::STGBroadcas */
}

/* sub_424dc (188B)
 * bool NS_MultiScreenBase::AutoBaseFragment::isNeedShowMap(int32_t, int32_t, int32_t, int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_424dc(void) {
    /* name: bool NS_MultiScreenBase::AutoBaseFragment::isNeedShowMap(int32_t, int32_t, int32_t, int32_ */
}

/* sub_26c14 (184B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::GuideFinishCardClosedOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_26c14(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::GuideFinishCardClosedOnUI() */
}

/* sub_2d6f8 (184B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::RemoveMapRenderObserver(NS_MultiScreenBase::Aut
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2d6f8(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::RemoveMapRenderObserver(NS_MultiScree */
}

/* sub_383e8 (184B)
 * bl::EGLSurfaceAttr NS_MultiScreenBase::AutoOpeningFragment::FillEglSurfaceAttr(bl::EGLDeviceID, int6
 *   _ZN3hsl3HSL24GetDayStatusNotifyHandleEv
 *   _ZN3hsl19UtilDayStatusNotify26GetEGLSurfaceAttrInitColorEb
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_383e8(void) {
    /* name: bl::EGLSurfaceAttr NS_MultiScreenBase::AutoOpeningFragment::FillEglSurfaceAttr(bl::EGLDevi */
}

/* sub_39d60 (184B)
 * void NS_MultiScreenBase::AutoOpeningFragment::WindowRenderNotify(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_39d60(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::WindowRenderNotify(bool) */
}

/* sub_3b8f0 (184B)
 * void NS_MultiScreenBase::AutoOpeningFragment::FragmentRenderCallback()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3b8f0(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::FragmentRenderCallback() */
}

/* sub_1781c (180B)
 * static bool NS_MultiScreenBase::AutoExscreenUtil::CloseDynamicLevel(NS_MultiScreenBase::ScreenType, 
 *   _ZN3hsl3HSL16GetMapViewHandleEN2bl11MapEngineIDE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1781c(void) {
    /* name: static bool NS_MultiScreenBase::AutoExscreenUtil::CloseDynamicLevel(NS_MultiScreenBase::Sc */
}

/* sub_1fb98 (180B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::UpdateNaviInfoOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   __stack_chk_fail
 */
static void fn_1fb98(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::UpdateNaviInfoOnUI() */
}

/* sub_363fc (180B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnRealCityAnimationFinished(uint32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_363fc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnRealCityAnimationFinished(uint32_t */
}

/* sub_4316c (180B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::OnNaviExit(bl::NaviType
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_4316c(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::OnNaviExit(bl */
}

/* sub_1fdb0 (176B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::ShowNaviLaneInfoOnUI()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_1fdb0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::ShowNaviLaneInfoOnUI() */
}

/* sub_2535c (176B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::ShowNaviLaneInfoOnUI()
 *   _ZN3hsl3HSL14GetGuideHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_2535c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::ShowNaviLaneInfoOnUI() */
}

/* sub_3c1c0 (176B)
 * virtual void NS_MultiScreenBase::AutoPreviewFragment::onResume()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3c1c0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoPreviewFragment::onResume() */
}

/* sub_206d8 (172B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnHideCrossImage(bl::CrossType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_206d8(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnHideCrossImage(bl::CrossType) */
}

/* sub_25f08 (172B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnHideCrossImage(bl::CrossType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_25f08(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnHideCrossImage(bl::CrossType) */
}

/* sub_3d128 (172B)
 * void NS_MultiScreenBase::AutoPreviewFragment::GotoCruiseModel()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3d128(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::GotoCruiseModel() */
}

/* sub_3f334 (172B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::onPause()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3f334(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::onPause() */
}

/* sub_430c0 (172B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::OnNaviFinish(bl::NaviTy
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_430c0(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::OnNaviFinish( */
}

/* sub_370e8 (168B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnEGLDoRender(bl::EGLDeviceID)
 *   __stack_chk_fail
 */
static void fn_370e8(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnEGLDoRender(bl::EGLDeviceID) */
}

/* sub_3e560 (168B)
 * void NS_MultiScreenBase::AutoPreviewFragment::CheckNaviMode()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_3e560(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::CheckNaviMode() */
}

/* sub_156ec (156B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::onCreateView(GNS_FRAM
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN9GNS_FRAME10CGFragment8inflaterEiPNS_11CGViewGroupE
 *   __dynamic_cast
 */
static void fn_156ec(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::onCreateVie */
}

/* sub_1f3ec (156B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::onCreateIntent(const GNS_FRAME::CGIntent &)
 *   _ZNK9GNS_FRAME8CGIntent13getInt32ExtraEPKtRi
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_1f3ec(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::onCreateIntent(const GNS_FRAME::CGInt */
}

/* sub_23e80 (156B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::onCreateIntent(const GNS_FRAME::CGIntent &)
 *   _ZNK9GNS_FRAME8CGIntent13getInt32ExtraEPKtRi
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   __stack_chk_fail
 */
static void fn_23e80(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::onCreateIntent(const GNS_FRAME::CGInten */
}

/* sub_27fb4 (156B)
 * void NS_MultiScreenBase::AutoNaviFragment::HideCross(bool, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_27fb4(void) {
    /* name: void NS_MultiScreenBase::AutoNaviFragment::HideCross(bool, bool) */
}

/* sub_3de90 (156B)
 * void NS_MultiScreenBase::AutoPreviewFragment::EnterOrExitRouteResultFragmentOnUI(int32_t)
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3de90(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::EnterOrExitRouteResultFragmentOnUI(int32_t) */
}

/* sub_3f8e8 (156B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::ClearLanePointOverlay()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3f8e8(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::ClearLanePointOverlay() */
}

/* sub_1919c (152B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onCreateView(GNS_FRAM
 *   _ZN9GNS_FRAME10CGFragment8inflaterEiPNS_11CGViewGroupE
 *   __dynamic_cast
 *   __stack_chk_fail
 */
static void fn_1919c(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onCreateVie */
}

/* sub_3679c (152B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::DynamicLevelLockChanged(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3679c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::DynamicLevelLockChanged(bool) */
}

/* sub_3cf60 (152B)
 * void NS_MultiScreenBase::AutoPreviewFragment::OnEndStopNaviUi(bl::NaviType)
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3cf60(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::OnEndStopNaviUi(bl::NaviType) */
}

/* sub_3e1b0 (152B)
 * void NS_MultiScreenBase::AutoPreviewFragment::NaviModeChangedOnUI()
 *   _ZN9GNS_FRAME10CGFragment13isTopFragmentEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3e1b0(void) {
    /* name: void NS_MultiScreenBase::AutoPreviewFragment::NaviModeChangedOnUI() */
}

/* sub_171b8 (148B)
 * static void NS_MultiScreenBase::AutoExscreenUtil::SetMapViewModeToUiConfig(NS_MultiScreenBase::Scree
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 */
static void fn_171b8(void) {
    /* name: static void NS_MultiScreenBase::AutoExscreenUtil::SetMapViewModeToUiConfig(NS_MultiScreenB */
}

/* sub_20c74 (148B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::NaviDestOnUI(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_20c74(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::NaviDestOnUI(bl::NaviType) */
}

/* sub_2244c (148B)
 * void NS_MultiScreenBase::AutoArNaviFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiScreenBase::
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2244c(void) {
    /* name: void NS_MultiScreenBase::AutoArNaviFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiSc */
}

/* sub_265b0 (148B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::NaviDestOnUI(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_265b0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::NaviDestOnUI(bl::NaviType) */
}

/* sub_27c84 (148B)
 * void NS_MultiScreenBase::AutoNaviFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiScreenBase::Pr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_27c84(void) {
    /* name: void NS_MultiScreenBase::AutoNaviFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiScre */
}

/* sub_339d0 (148B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::OnUpdateUiW
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_339d0(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::O */
}

/* sub_173fc (144B)
 * static void NS_MultiScreenBase::AutoExscreenUtil::SetMapViewLevelToUiConfig(NS_MultiScreenBase::Scre
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 */
static void fn_173fc(void) {
    /* name: static void NS_MultiScreenBase::AutoExscreenUtil::SetMapViewLevelToUiConfig(NS_MultiScreen */
}

/* sub_31a40 (144B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseS311Fragment::onPaintFragment(bo
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_31a40(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseS311Fragment::onPaintF */
}

/* sub_15b7c (140B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::OnWindowRenderNotify(
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_15b7c(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::OnWindowRen */
}

/* sub_17370 (140B)
 * static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetMapCrossControlFromUiConfig(NS_MultiScreenBa
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode11GetInstanceEv
 *   _ZN18NS_MultiScreenBase20AutoConfigScreenMode20GetCurrentScreenModeENS_10ScreenTypeE
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_17370(void) {
    /* name: static int32_t NS_MultiScreenBase::AutoExscreenUtil::GetMapCrossControlFromUiConfig(NS_Mul */
}

/* sub_18d9c (140B)
 * bool NS_MultiScreenBase::CoverUtil::isUIFirstRenderFinish()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_18d9c(void) {
    /* name: bool NS_MultiScreenBase::CoverUtil::isUIFirstRenderFinish() */
}

/* sub_19a58 (140B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::onPaintFragment(boo
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl11SystemUtils16NaviStatusNotifyE23ENaviInfoBroadcastItem_NS0_10NotifyModeENS0_15NotifyCountModeE
 */
static void fn_19a58(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311Fragment::onPaintFr */
}

/* sub_1ddd8 (140B)
 * hsl::MapViewBase *NS_MultiScreenBase::AutoMultiScreenConfig::GetMapViewBase()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1ddd8(void) {
    /* name: hsl::MapViewBase *NS_MultiScreenBase::AutoMultiScreenConfig::GetMapViewBase() */
}

/* sub_27164 (140B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnSetMapPreview(int32_t, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_27164(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnSetMapPreview(int32_t, bool) */
}

/* sub_2ca00 (140B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::OnExtScreenUIstyleChangeNotifyRcvUi(int)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2ca00(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::OnExtScreenUIstyleChangeNotifyRcvUi(i */
}

/* sub_32da4 (140B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::onPaintFrag
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl11SystemUtils16NaviStatusNotifyE23ENaviInfoBroadcastItem_NS0_10NotifyModeENS0_15NotifyCountModeE
 */
static void fn_32da4(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideS311UnnormalFragment::o */
}

/* sub_3c934 (140B)
 * virtual void NS_MultiScreenBase::AutoPreviewFragment::HandleExitPreviewNotification()
 *   __stack_chk_fail
 */
static void fn_3c934(void) {
    /* name: virtual void NS_MultiScreenBase::AutoPreviewFragment::HandleExitPreviewNotification() */
}

/* sub_3cc60 (140B)
 * virtual void NS_MultiScreenBase::AutoPreviewFragment::OnSetMapPreview(int32_t, bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3cc60(void) {
    /* name: virtual void NS_MultiScreenBase::AutoPreviewFragment::OnSetMapPreview(int32_t, bool) */
}

/* sub_19368 (136B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onPause()
 *   __stack_chk_fail
 */
static void fn_19368(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onPause() */
}

/* sub_194cc (136B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnNaviFinish(bl::Navi
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_194cc(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnNaviFinis */
}

/* sub_35da4 (136B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnMapViewDestroy(uint32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_35da4(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnMapViewDestroy(uint32_t) */
}

/* sub_16788 (132B)
 * static bl::MapEngineID NS_MultiScreenBase::AutoExscreenUtil::GetMapEngineIdFromScreenType(NS_MultiSc
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_16788(void) {
    /* name: static bl::MapEngineID NS_MultiScreenBase::AutoExscreenUtil::GetMapEngineIdFromScreenType( */
}

/* sub_17ad0 (132B)
 * static bool NS_MultiScreenBase::AutoExscreenUtil::IsCurrentDynamicLevel(NS_MultiScreenBase::MapScale
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_17ad0(void) {
    /* name: static bool NS_MultiScreenBase::AutoExscreenUtil::IsCurrentDynamicLevel(NS_MultiScreenBase */
}

/* sub_20d58 (132B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::PosStatusOnUI(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL12GetPosHandleEv
 *   _ZN3hsl3Pos16GetGPSActivationEv
 */
static void fn_20d58(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::PosStatusOnUI(bool) */
}

/* sub_2685c (132B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::PosStatusOnUI(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 *   _ZN3hsl3HSL12GetPosHandleEv
 *   _ZN3hsl3Pos16GetGPSActivationEv
 */
static void fn_2685c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::PosStatusOnUI(bool) */
}

/* sub_36b4c (132B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnQueryMapRenderStatus(NS_MultiScreenBase::Scr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_36b4c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnQueryMapRenderStatus(NS_MultiScree */
}

/* sub_16868 (128B)
 * static NS_MultiScreenBase::ScreenType NS_MultiScreenBase::AutoExscreenUtil::GetScreenTypeFromEngineI
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_16868(void) {
    /* name: static NS_MultiScreenBase::ScreenType NS_MultiScreenBase::AutoExscreenUtil::GetScreenTypeF */
}

/* sub_23484 (128B)
 * void NS_MultiScreenBase::AutoArNaviFragment::TbtGpsLocatingViewStatusChanged(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_23484(void) {
    /* name: void NS_MultiScreenBase::AutoArNaviFragment::TbtGpsLocatingViewStatusChanged(bool) */
}

/* sub_28e00 (128B)
 * void NS_MultiScreenBase::AutoNaviFragment::TbtGpsLocatingViewStatusChanged(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_28e00(void) {
    /* name: void NS_MultiScreenBase::AutoNaviFragment::TbtGpsLocatingViewStatusChanged(bool) */
}

/* sub_36bd0 (128B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnEnableMapRenderUI(bool, int32_t, NS_MultiScr
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_36bd0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnEnableMapRenderUI(bool, int32_t, N */
}

/* sub_15c1c (124B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::OnUpdateUiWidgets(int
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_15c1c(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeOpeningFragment::OnUpdateUiW */
}

/* sub_19450 (124B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnUpdateUiWidgets(int
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_19450(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnUpdateUiW */
}

/* sub_21178 (124B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnNaviExit(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_21178(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnNaviExit(bl::NaviType) */
}

/* sub_26d30 (124B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnNaviExit(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_26d30(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnNaviExit(bl::NaviType) */
}

/* sub_2deac (124B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::SetScreenProjectionSizeListener(NS_MultiScreenB
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2deac(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::SetScreenProjectionSizeListener(NS_Mu */
}

/* sub_2df28 (124B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::SetUiStyleListener(NS_MultiScreenBase::AutoUiSt
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2df28(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::SetUiStyleListener(NS_MultiScreenBase */
}

/* sub_3e99c (124B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseFragment::OnUpdateUiWidgets(int3
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3e99c(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeCruiseFragment::OnUpdateUiWi */
}

/* sub_4177c (124B)
 * void NS_MultiScreenBase::AutoCruiseFragment::EnterCruiseScene()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_4177c(void) {
    /* name: void NS_MultiScreenBase::AutoCruiseFragment::EnterCruiseScene() */
}

/* sub_43044 (124B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::OnUpdateUiWidgets(int32
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_43044(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::OnUpdateUiWid */
}

/* sub_2d264 (116B)
 * void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddExscreenNotifyObserver()
 *   _ZN3hsl3HSL21GetSystemNotifyHandleEv
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_2d264(void) {
    /* name: void NS_MultiScreenBase::AutoExscreenNotifyObserver::AddExscreenNotifyObserver() */
}

/* sub_3a0b0 (116B)
 * void NS_MultiScreenBase::AutoOpeningFragment::WindowRenderStateOnUI(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3a0b0(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::WindowRenderStateOnUI(bool) */
}

/* sub_37790 (112B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::SetMapSurface(int64_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_37790(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::SetMapSurface(int64_t) */
}

/* sub_40c58 (112B)
 * void NS_MultiScreenBase::AutoCruiseFragment::PosStatusUI(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_40c58(void) {
    /* name: void NS_MultiScreenBase::AutoCruiseFragment::PosStatusUI(bool) */
}

/* sub_1fae0 (108B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnUpdateUiWidgets(int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1fae0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnUpdateUiWidgets(int32_t) */
}

/* sub_1feac (108B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::HideNaviLaneInfoOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1feac(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::HideNaviLaneInfoOnUI() */
}

/* sub_24ed4 (108B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnUpdateUiWidgets(int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_24ed4(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnUpdateUiWidgets(int32_t) */
}

/* sub_25458 (108B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::HideNaviLaneInfoOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_25458(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::HideNaviLaneInfoOnUI() */
}

/* sub_378bc (108B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnUpdateUiWidgets(int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_378bc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnUpdateUiWidgets(int32_t) */
}

/* sub_3f530 (108B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::OnUpdateUiWidgets(int32_t)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3f530(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::OnUpdateUiWidgets(int32_t) */
}

/* sub_3f87c (108B)
 * virtual void NS_MultiScreenBase::AutoCruiseFragment::HideCruiseLaneInfoOnUI()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3f87c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoCruiseFragment::HideCruiseLaneInfoOnUI() */
}

/* sub_41a50 (108B)
 * virtual void NS_MultiScreenBase::AutoBaseFragment::onCreateView(GNS_FRAME::CGViewGroup *)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_41a50(void) {
    /* name: virtual void NS_MultiScreenBase::AutoBaseFragment::onCreateView(GNS_FRAME::CGViewGroup *) */
}

/* sub_1f920 (104B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::onPause()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_1f920(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::onPause() */
}

/* sub_24cfc (104B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::onPause()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_24cfc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::onPause() */
}

/* sub_35994 (104B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::onResume()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_35994(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::onResume() */
}

/* sub_359fc (104B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::onPause()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_359fc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::onPause() */
}

/* sub_3c270 (104B)
 * virtual void NS_MultiScreenBase::AutoPreviewFragment::onPause()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_3c270(void) {
    /* name: virtual void NS_MultiScreenBase::AutoPreviewFragment::onPause() */
}

/* sub_41abc (104B)
 * virtual void NS_MultiScreenBase::AutoBaseFragment::onResume()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_41abc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoBaseFragment::onResume() */
}

/* sub_41b24 (104B)
 * virtual void NS_MultiScreenBase::AutoBaseFragment::onPause()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_41b24(void) {
    /* name: virtual void NS_MultiScreenBase::AutoBaseFragment::onPause() */
}

/* sub_41b8c (104B)
 * virtual void NS_MultiScreenBase::AutoBaseFragment::onDestroyView()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_41b8c(void) {
    /* name: virtual void NS_MultiScreenBase::AutoBaseFragment::onDestroyView() */
}

/* sub_19554 (100B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnNaviExit(bl::NaviTy
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_19554(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::OnNaviExit( */
}

/* sub_21114 (100B)
 * virtual void NS_MultiScreenBase::AutoArNaviFragment::OnNaviFinish(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_21114(void) {
    /* name: virtual void NS_MultiScreenBase::AutoArNaviFragment::OnNaviFinish(bl::NaviType) */
}

/* sub_26ccc (100B)
 * virtual void NS_MultiScreenBase::AutoNaviFragment::OnNaviFinish(bl::NaviType)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_26ccc(void) {
    /* name: virtual void NS_MultiScreenBase::AutoNaviFragment::OnNaviFinish(bl::NaviType) */
}

/* sub_364b0 (100B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnMainMapForegroundStatusChanged(bool)
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_364b0(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnMainMapForegroundStatusChanged(boo */
}

/* sub_36834 (100B)
 * virtual void NS_MultiScreenBase::AutoOpeningFragment::OnHSLInitFinish()
 *   I_GetAlcPtr
 *   I_GetAlcPtr
 */
static void fn_36834(void) {
    /* name: virtual void NS_MultiScreenBase::AutoOpeningFragment::OnHSLInitFinish() */
}

/* sub_3b6f4 (100B)
 * void NS_MultiScreenBase::AutoOpeningFragment::GoMap()
 *   __stack_chk_fail
 */
static void fn_3b6f4(void) {
    /* name: void NS_MultiScreenBase::AutoOpeningFragment::GoMap() */
}

/* sub_193f0 (96B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onDestroyView()
 *   __stack_chk_fail
 */
static void fn_193f0(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeARGuideFragment::onDestroyVi */
}

/* sub_42fe8 (92B)
 * virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::SwitchToPreviewMode()
 *   __stack_chk_fail
 */
static void fn_42fe8(void) {
    /* name: virtual void NS_InstrumentPannelLarge::CGInstrumentPannelLargeGuideFragment::SwitchToPrevi */
}

} // namespace NS_MultiScreenBase