/*
 * Batch 6 — Observer callback family
 *
 * NS_MultiScreenBase::AutoExscreenNotifyObserver (vtable, 6 slots):
 *   [0] sub_29bf0 (2288B) — OnExScreenNotify main handler
 *   [1] sub_2a4e0 (100B)  — small cb (log/assert only)
 *   [2] sub_2a544 (100B)  — small cb (log/assert only)
 *   [3] sub_2a5a8 (568B)  — deferred action A
 *   [4] sub_2a7e0 (584B)  — deferred action B
 *   [5] sub_2aa28 (688B)  — deferred action C (with ScreenMode read)
 *
 * Verified call traces:
 *   [0]: GetSurfaceConfig -> GAI_ExScreenRenderStatusNotify (x2)
 *        -> new x3 + GPI::Scheduler::get
 *        -> GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(tagSTExtScreen*)
 *        -> MultiScreenService::SetStartOpenScreenMode(mode, tag)
 *   [3]/[4]: GetInstance->GetMapEngineId -> new CallableWrapper x2
 *        -> UIObject::RunAsyncDelayByEngineId(closure, engineId, delay)
 *   [5]: + AutoConfigScreenMode::GetCurrentScreenMode(ScreenType)
 */
#include "Observers.h"
#include "AutoExscreenUiConfig.h"
#include "AutoExscreenMapStatusDispatch.h"
#include <hsl/MultiScreenService.h>
#include <hsl/UIObject.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
#include <GFrame/GblAlc.h>
#include <GAI.h>
#include <GFrame/GPIScheduler.h>
#include <GFrame/GPIClosure.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoMultiScreenConfig.h>
#include <hsl/HSL.h>
#include <new>

namespace NS_MultiScreenBase {

/* sub_29bf0: main ex-screen notify */
void AutoExscreenNotifyObserver::OnExScreenNotify(int32_t event, tagSTExtScreen *st) {
    AutoConfigJsonData *cfg = AutoConfigJsonData::GetInstance();
    SurfaceConfig sc;
    cfg->GetSurfaceConfig(screenType_, sc);

    std::vector<tagMultiViewModeInfo> modes;
    cfg->GetMultiViewModeInfo(screenType_, modes);

    GAI_ExScreenRenderStatusNotify(/*args from sc/modes*/);
    /* ... state transition ... */
    GAI_ExScreenRenderStatusNotify(/*args*/);

    GPI::Scheduler::get(/*engine id*/);

    hsl::GOTHERProtocolHmiToAl::ExScreenRenderActionHmiToAl(st);
    hsl::MultiScreenService::GetInstance()->SetStartOpenScreenMode(/*mode*/ 0, *st);
}

/* sub_2a5a8 / sub_2a7e0 / sub_2aa28: deferred UI actions */
void AutoExscreenNotifyObserver::DeferActionA(int screen, bool enabled) {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(screen);
    GPI::Closure::Callable<void()> task = GPI::Closure::bind(
        this, &AutoExscreenNotifyObserver::OnSetExternalScreenCrossControlUi,
        static_cast<int>(eng), enabled);
    reinterpret_cast<hsl::UIObject*>(asyncState_)->RunAsyncDelayByEngineId(
        task, eng, 0, 0);
}
void AutoExscreenNotifyObserver::DeferActionB(int screen, int level, int mode, int pos) {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(screen);
    GPI::Closure::Callable<void()> task = GPI::Closure::bind(
        this, &AutoExscreenNotifyObserver::OnSetExternalScreenMapStatusUi,
        static_cast<int>(eng), level, mode, pos);
    reinterpret_cast<hsl::UIObject*>(asyncState_)->RunAsyncDelayByEngineId(
        task, eng, 0, 0);
}
void AutoExscreenNotifyObserver::DeferActionC(const tagSTExtScreenControl& control) {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(
        control.words[kExtScreenSelector]);
    GPI::Closure::Callable<void()> task = GPI::Closure::bind(
        this, &AutoExscreenNotifyObserver::OnSetExScreenMapStatusUi, control);
    reinterpret_cast<hsl::UIObject*>(asyncState_)->RunAsyncDelayByEngineId(
        task, eng, 0, 0);
}

// sub_2bcdc: persist Guide scene's cross-control setting, then notify the
// collection guarded by the mutex embedded at observer +0x6c.
void AutoExscreenNotifyObserver::OnSetExternalScreenCrossControlUi(int mapEngineId,
                                                                     bool enabled) {
    (void)mapEngineId; // The original only uses this argument for logging.
    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screenType_);
    AutoExscreenUtil_SetMapParamsToUiConfig(screenType_, mode, 1, 2, enabled ? 1 : 0);

    ObserverCollectionAbi& collection = crossControlObservers_;
    void** mutexVtable = *reinterpret_cast<void***>(&collection);
    using MutexOp = void (*)(void*);
    reinterpret_cast<MutexOp>(mutexVtable[2])(&collection);
    for (uint32_t i = 0; i < collection.count; ++i) {
        AutoMapCrossControlObserver* observer =
            static_cast<AutoMapCrossControlObserver*>(collection.observers[i]);
        observer->OnSetExternalScreenCrossControlUi(screenType_, enabled);
    }
    reinterpret_cast<MutexOp>(mutexVtable[3])(&collection);
}

// V21 0x2c070: the fourth integer is used only by its logging path.
// The entry view gate precedes singleton initialization in the original.
void AutoExscreenNotifyObserver::OnSetExternalScreenMapStatusUi(
    int mapEngineId, int level, int mode, int position) {
    (void)position;
    hsl::MapView* view = hsl::HSL::GetMapViewHandle(
        static_cast<bl::MapEngineID>(mapEngineId));
    if (!view) return;
    AutoMultiScreenConfig* config = AutoMultiScreenConfig::GetInstance(screenType_);
    void* timerState = *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(this) + 0xcc);
    AutoExscreenUtil_ApplyMapStatusWithView(config, screenType_, timerState,
                                             view, level, mode);
}

} // namespace
