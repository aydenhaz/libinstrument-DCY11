#include "CoverUtil.h"
#include <GFrame/CGWorkStation.h>
#include <atomic>
namespace NS_MultiScreenBase {
namespace {
// Private callback target. CGWorkStation receives the plain canvas function
// pointer. This avoids introducing a libc++ std::function dependency absent
// from the original V21 library's undefined-symbol set.
std::atomic<CoverUtil*> topRenderOwner{nullptr};
void dispatchTopRender(GNS_FRAME::CGCanvas* canvas) {
    CoverUtil* owner = topRenderOwner.load(std::memory_order_acquire);
    if (owner) owner->TopRenderCoverCallback(canvas);
}
}
void CoverUtil::installTopRenderCallback() {
    topRenderOwner.store(this, std::memory_order_release);
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(&dispatchTopRender);
}
void CoverUtil::uninstallTopRenderCallback() {
    topRenderOwner.store(nullptr, std::memory_order_release);
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(nullptr);
}

void DispatchTopRenderCallback(GNS_FRAME::CGCanvas* canvas) {
    dispatchTopRender(canvas);
}

}
