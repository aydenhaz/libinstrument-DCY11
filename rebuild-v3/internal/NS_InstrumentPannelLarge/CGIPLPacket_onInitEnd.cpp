/*
 * Batch 5 — CGIPLPacket::onInitEnd() (sub_2eb88, 1892B) + observer registration hub
 *
 * Signature string recovered @0x54cb0:
 *   "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onInitEnd()"
 *
 * Original path landmarks (the later Fragment stage is not implemented):
 *   1. ALC export block (module "HMI_PANNELLARGE", name "onInitEnd")
 *   2. LargeScreenImpl ctor (operator new 0x10 = 16B)
 *      ExModuleManager::GetInstance()->ModuleReg((ModuleInterfaceId)0x1f40, impl)
 *      -- ModuleReg string @0x54420: "NS_InstrumentPannelLarge::LargeScreenImpl::LargeScreenImpl()"
 *   3. UI thread wiring:
 *      HSL::Get3rdHandle(); new UITaskThreadProxy(renderThreadExecutorId)
 *      ThirdManager::SetUITaskThread(UI_TASK_DEVICE_ID, engineId, 0, false, proxy)
 *      CommonUtils::GetEngineIdByDeviceId(id); CGFrameSimulation::getId()
 *      UIObject::SetUITaskThread(frameId, engineId)
 *   4. Packet package-base getContext(), then test the host activity.
 *   5. Allocate a separate 0x440-byte Opening Fragment and call its
 *      constructor at 0x41954 with the HMI_OPENING tag. The fragment owns
 *      ObjectRuntimeInfo at +0x384 (with a +0x380 BLObject argument) and
 *      observer interfaces at +0x36c etc.
 *   6. Set context/type on the constructed fragment, and pass that same
 *      fragment to CGActivity::setFragment. Bind Packet CoverUtil (+0xc0)
 *      to the fragment's +0x40c field in the tail call to 0x3bdbc.
 *      This whole fragment stage is still unrecovered and is not called.
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
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGPackage.h>
#include <hsl/HSL.h>
#include <hsl/ThirdManager.h>
#include <hsl/CommonUtils.h>
#include <hsl/UIObject.h>
#include <hsl/UITaskThreadProxy.h>
#include <new>

namespace NS_InstrumentPannelLarge {
namespace {
// Original BSS +0x6fd4c: module singleton separate from the Packet's
// constructed host base. The Packet singleton occupies +0x6fd48 instead.
LargeScreenImpl* activeLargeScreenImpl;
}

void CGIPLPacket::onInitEnd() {
    /* V21 0x2ebba logs line 0x12e using seven stack fields. */
    AlcExport("onInitEnd",
              "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onInitEnd()",
              0x12e);

    /* 2. Module lifetime is separate from the Packet's virtual base at +0. */
    if (!activeLargeScreenImpl) activeLargeScreenImpl = new LargeScreenImpl();

    /* 3. UI task thread wiring */
    hsl::ThirdManager* third = hsl::HSL::Get3rdHandle();
    if (third) { // V21 0x2ec58..0x2ecc2.
        if (!uiThreadProxy_) {
            const int executor = GNS_FRAME::CGFrameSimulation::ownWorkStation()->getRenderThreadExecutorId();
            uiThreadProxy_ = new hmi::UITaskThreadProxy(executor);
        }
        const int executor = GNS_FRAME::CGFrameSimulation::ownWorkStation()->getRenderThreadExecutorId();
        const uint32_t render_thread = GNS_FRAME::CGFrameSimulation::ownWorkStation()->getRenderThreadId();
        third->SetUITaskThread(
            hsl::UI_TASK_DEVICE_INSTRUMENT, executor,
            static_cast<long long>(render_thread), true, uiThreadProxy_->taskInterface());
        const int engine = hsl::CommonUtils::GetEngineIdByDeviceId(hsl::UI_TASK_DEVICE_INSTRUMENT);
        hsl::UIObject::SetUITaskThread(engine, GNS_FRAME::CGFrameSimulation::ownWorkStation()->getId());
    }

    /* 4. V21 retrieves these from real host objects before creating the
       Opening Fragment. The current Packet has no constructed host base. */
    GNS_FRAME::CGFragmentActivity *activity =
        GNS_FRAME::CGFrameSimulation::ownWorkStation()->getFragmentActivity();
    GNS_FRAME::CGContext* context =
        reinterpret_cast<GNS_FRAME::CGPackage*>(this)->getContext();
    (void)activity;
    (void)context;
    // Stop before fragment allocation. Its 0x440-byte base and observer
    // subobjects do not exist in the rebuild. The old legacy_ fields read
    // unconstructed Packet bytes and fabricated a heap ObjectRuntimeInfo,
    // timer ID 0 and screen mode; none can be used for this stage.
}

} // namespace
