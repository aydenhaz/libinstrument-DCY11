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
#include <hsl/MultiScreenService.h>
#include <hsl/UIObject.h>
#include <hsl/GOTHERProtocolHmiToAl.h>
#include <GFrame/GblAlc.h>
#include <GAI.h>
#include <GFrame/GPIScheduler.h>
#include <GFrame/GPIClosure.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
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
void AutoExscreenNotifyObserver::DeferActionA() {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(0);
    hsl::UIObject::RunAsyncDelayByEngineId(GPI::Closure::bind(this, &AutoExscreenNotifyObserver::ActionA),
                                           eng, /*delay*/ 0);
}
void AutoExscreenNotifyObserver::DeferActionB() {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(0);
    hsl::UIObject::RunAsyncDelayByEngineId(GPI::Closure::bind(this, &AutoExscreenNotifyObserver::ActionB),
                                           eng, /*delay*/ 0);
}
void AutoExscreenNotifyObserver::DeferActionC() {
    bl::MapEngineID eng = hsl::MultiScreenService::GetInstance()->GetMapEngineId(0);
    int cur = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screenType_);
    hsl::UIObject::RunAsyncDelayByEngineId(GPI::Closure::bind(this, &AutoExscreenNotifyObserver::ActionC),
                                           eng, /*delay*/ 0);
}

} // namespace
