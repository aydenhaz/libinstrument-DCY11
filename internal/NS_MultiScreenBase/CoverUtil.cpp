#include "CoverUtil.h"
#include "../abi/CoverRenderState.h"
#include "../abi/CoverCaptureResult.h"
namespace NS_MultiScreenBase {
CoverUtil::CoverUtil() = default;
// V21 0x17dfc destroys SurfaceConfig::name (+0x44), then text_ (+0x18)
// after optional logging. Image/observer release belongs to the lifecycle.
CoverUtil::~CoverUtil() = default;
void CoverUtil::MapSurfaceViewStatusChange(int surface,MAPSURFACE event,
        void*,int,int,int) {
    IPL_ABI::coverSurfaceStatusChanged(static_cast<int32_t>(surface_.scalar[7]),surface,
        static_cast<int32_t>(event),image_!=nullptr,enabled_!=0,counter_);
}
bool CoverUtil::onBusinessDataObserver(int event,const void*,void*) {
    if(event!=0x17) return true;
    bool requested=requested_!=0,state=state_!=0;
    // Publish original flag transitions before invoking external config services.
    IPL_ABI::coverCaptureResultState(event,requested,state);
    requested_=requested;state_=state;
    IPL_ABI::applyCoverCaptureResult(event,screenshot_.bytes,screenshot_.length,screenshot_.width,screenshot_.height,screenshot_.stride,requested,state);
    // External calls may mutate this object; never write stale local flags back.
    return true;
}
bool CoverUtil::isUIFirstRenderFinish() {
    return IPL_ABI::coverUiFirstRenderFinished(image_!=nullptr,enabled_!=0,state_!=0,counter_,threshold_);
}
void CoverUtil::TopRenderCoverCallback(GNS_FRAME::CGCanvas* canvas) {
    if(!image_ || !enabled_) return;
    if(counter_<=threshold_ && state_) {
        const GPI::CGRect rect(static_cast<int32_t>(surface_.scalar[0]),static_cast<int32_t>(surface_.scalar[1]));
        IPL_ABI::drawCoverImage(canvas,static_cast<GNS_FRAME::CGImage*>(image_),rect,&screenshot_.tail);
    }
    // Original reloads state after the external draw method returns.
    if(counter_==10 && !inhibitCapture_ && !requested_) {
        const dice::Rect<int32_t> rect={{0,static_cast<int32_t>(surface_.scalar[0]),0,static_cast<int32_t>(surface_.scalar[1])}};
        requested_=1;
        IPL_ABI::requestCoverScreenshot(unknown10_,rect,screenshot_,this);
    }
    counter_=IPL_ABI::coverPaintNextCounter(counter_);
}
}
