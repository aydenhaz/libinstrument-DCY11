#include "CoverUtil.h"
#include "../abi/CoverRenderState.h"
#include "../abi/CoverCaptureResult.h"
namespace NS_MultiScreenBase {
CoverUtil::CoverUtil() = default;
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
    IPL_ABI::applyCoverCaptureResult(event,bytes_,length_,width_,height_,stride_,requested,state);
    // External calls may mutate this object; never write stale local flags back.
    return true;
}
bool CoverUtil::isUIFirstRenderFinish() {
    return IPL_ABI::coverUiFirstRenderFinished(image_!=nullptr,enabled_!=0,state_!=0,counter_,threshold_);
}
}
