/*
 * Batch 5 — CGIPLPacket::onInitEnd() (sub_2eb88, 1892B) + observer registration hub
 *
 * Signature string recovered @0x54cb0:
 *   "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onInitEnd()"
 *
 * Complete call trace (70 calls, verified):
 *   1. ALC export block (module "HMI_PANNELLARGE", name "onInitEnd")
 *   2. LargeScreenImpl ctor (operator new 0x2c = 44B)
 *      ExModuleManager::GetInstance()->ModuleReg((ModuleInterfaceId)0x9f, impl)
 *      -- ModuleReg string @0x54420: "NS_InstrumentPannelLarge::LargeScreenImpl::LargeScreenImpl()"
 *   3. UI thread wiring:
 *      HSL::Get3rdHandle(); new UITaskThreadProxy(renderThreadExecutorId)
 *      ThirdManager::SetUITaskThread(UI_TASK_DEVICE_ID, engineId, 0, false, proxy)
 *      CommonUtils::GetEngineIdByDeviceId(id); CGFrameSimulation::getId()
 *      UIObject::SetUITaskThread(frameId, engineId)
 *   4. Fragment context:
 *      ownWorkStation(); getFragmentActivity(); CGPackage::getContext()
 *      new bl::ObjectRuntimeInfo(BLObject*)
 *   5. Timer: TimerUtilsFuns::InsertTimerById((long long)... )
 *   6. AutoConfigJsonData::GetInstance()->Init()
 *      GetSurfaceConfig(ScreenType, SurfaceConfig&)   <- "AutoConfigJsonData::GetInstance() is NULL" assert
 *   7. Observer registrations (SystemNotify):
 *      AddMapSurfaceViewObserver(this)          name "AddMapSurfaceViewObserver"
 *      AddLanguageSwitchObserver(this, bool)    name "_systemNotify->AddLanguageSwitchObserver"
 *      MultiScreenService::GetInstance()->GetStartOpenScreenMode(i)
 *      AddNetObserver(this)
 *      CGBroadcaster::getInstance(u32); GetSystemNotifyHandle()
 *      AddInstrumentPannelObserver(this)        name "AddInstrumentPannelObserver"
 *   8. Fragment host wiring:
 *      CGFragment::setContext(ctx); setType(void*)
 *      CGIntent intent; CGActivity::setFragment(int, fragment, intent)
 *   9. Screen mode publish:
 *      AutoConfigScreenMode::GetInstance()->SetCurrentScreenMode(ScreenType, mode)
 *      MultiScreenService::SetStartOpenScreenMode(mode, tagSTExtScreen)
 *  10. Logging: CGString::Format + CGGFrameCrashLogger::exportToAndroidLogCat
 *      CGFrameAlcLog::getInstance() + I_GetAlcDevPtr() flush pairs
 *  11. abort() on fatal path
 *
 * Additional strings @0x55ec9: "NS_MultiScreenBase::AutoOpeningFragment::AutoOpeningFragment
 * (bl::MapEngineID, hsl::UI_TASK_DEVICE_ID)", "_engineId:%d", "windowId:%d",
 * "window:%d, mode:%d, operation:%d"
 * Build path leak @0x54cfa: "//home/jenkins/.abtor/libraries/GFrame_shared/dev.6.2.0.54.1146/
 * android/ndk21/armeabi-v7a/include/..."  (jenkins CI, GFrame dev.6.2.0.54.1146, NDK21)
 */
#include "CGIPLPacket.h"
#include "LargeScreenImpl.h"
#include "../AlcExport.h"
#include <GFrame/CGString.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGPackage.h>
#include <GFrame/CGIntent.h>
#include <GFrame/CGBroadcaster.h>
#include <GFrame/CGGFrameCrashLogger.h>
#include <GFrame/CGFrameAlcLog.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
#include <hsl/MultiScreenService.h>
#include <hsl/ThirdManager.h>
#include <hsl/CommonUtils.h>
#include <hsl/UIObject.h>
#include <hsl/TimerUtilsFuns.h>
#include <hsl/UITaskThreadProxy.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include "Observers.h"
#include <NS_MultiScreenBase/AutoExscreenNotifyObserver.h>
#include <ExModuleManager.h>
#include <new>

namespace NS_InstrumentPannelLarge {

void CGIPLPacket::onInitEnd() {
    /* 1. ALC export: {0,0,"HMI_PANNELLARGE",0,tag,"virtual void NS_InstrumentPannelLarge::
       CGIPLPacket::onInitEnd()","onInitEnd"} */
    AlcExport("onInitEnd",
              "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onInitEnd()",
              /*tag*/ 0, this);

    /* 2. LargeScreenImpl + module registration (interface 0x9f = 159) */
    impl_ = new LargeScreenImpl();
    ExModuleManager::GetInstance()->ModuleReg((ModuleInterfaceId)0x9f, impl_);

    /* 3. UI task thread wiring */
    third_ = hsl::HSL::Get3rdHandle();
    int execId = GNS_FRAME::CGFrameSimulation::getRenderThreadExecutorId();
    uiThreadProxy_ = new hsl::UITaskThreadProxy(execId);
    int engineId = hsl::CommonUtils::GetEngineIdByDeviceId(0);
    static_cast<hsl::ThirdManager*>(third_)->SetUITaskThread(0, engineId, 0, false, uiThreadProxy_);
    GNS_FRAME::CGFrameSimulation::getRenderThreadId();
    hsl::UIObject::SetUITaskThread(GNS_FRAME::CGFrameSimulation::getId(), engineId);

    /* 4. Fragment context + runtime info */
    GNS_FRAME::CGFrameSimulation::ownWorkStation();
    GNS_FRAME::CGPackage *pkg = GNS_FRAME::CGFrameSimulation::getFragmentActivity();
    ctx_ = GNS_FRAME::CGPackage::getContext();
    bl::ObjectRuntimeInfo *rti = new bl::ObjectRuntimeInfo(blObject_);

    /* 5. Init timer by id */
    hsl::TimerUtilsFuns::InsertTimerById(/*timer id*/ 0);

    /* 6. Surface config */
    NS_MultiScreenBase::AutoConfigJsonData *cfg =
        NS_MultiScreenBase::AutoConfigJsonData::GetInstance();
    if (cfg == 0) { /* "AutoConfigJsonData::GetInstance() is NULL" */ }
    cfg->Init();
    NS_MultiScreenBase::SurfaceConfig sc;
    cfg->GetSurfaceConfig(0, sc);

    /* 7. Observers */
    hsl::SystemNotify *sn = hsl::HSL::GetSystemNotifyHandle();
    sn->AddMapSurfaceViewObserver(this);
    sn->AddLanguageSwitchObserver(this, /*b*/ true);
    hsl::MultiScreenService::GetInstance()->GetStartOpenScreenMode(/*i*/ 0);
    sn->AddNetObserver(this);
    GNS_FRAME::CGBroadcaster::getInstance(/*u32*/ 0);
    sn->AddInstrumentPannelObserver(this);

    /* 8. Host fragment wiring */
    hostFragment_->setContext(ctx_);
    hostFragment_->setType(this);
    GNS_FRAME::CGIntent intent;
    static_cast<GNS_FRAME::CGActivity*>(activity_)->setFragment(0, hostFragment_, intent);

    /* 9. Publish screen mode */
    NS_MultiScreenBase::AutoConfigScreenMode::GetInstance()
        ->SetCurrentScreenMode(0, /*mode*/ mode_);
    static NS_MultiScreenBase::tagSTExtScreen st{};
    hsl::MultiScreenService::GetInstance()->SetStartOpenScreenMode(mode_, st);

    /* 10. Crash-log breadcrumb */
    CGString s; s.Format(reinterpret_cast<const unsigned short*>(u"onInitEnd"));
    GNS_FRAME::CGGFrameCrashLogger::exportToAndroidLogCat(0, "onInitEnd", 0, s);
}

} // namespace
