#include "CoverUtil.h"
#include <GFrame/CGWorkStation.h>
#include <functional>
namespace NS_MultiScreenBase {
namespace {
// Entirely private rebuilt storage. No std::function/GPI closure object crosses
// the host ABI; CGWorkStation receives only the plain canvas function pointer.
std::function<void(GNS_FRAME::CGCanvas*)> topRender;
void dispatchTopRender(GNS_FRAME::CGCanvas* canvas) {
    const auto callback=topRender;
    if(callback) callback(canvas);
}
}
void CoverUtil::installTopRenderCallback() {
    topRender=[this](GNS_FRAME::CGCanvas* canvas){TopRenderCoverCallback(canvas);};
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(&dispatchTopRender);
}
void CoverUtil::uninstallTopRenderCallback() {
    GNS_FRAME::CGWorkStation::ownWorkStation()->setTopRenderCallback(nullptr);
    topRender=nullptr;
}

}
