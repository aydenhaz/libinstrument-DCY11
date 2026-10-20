#include "CoverUtil.h"
#include "../abi/CoverConfigPolicy.h"
namespace NS_MultiScreenBase {
CoverConfigurationStage CoverUtil::prepareConfiguration(GNS_FRAME::CGContext* context,
        bl::IMapDevice* device,bl::IMapView* view,SurfaceConfig config) {
    if(!context || !device || !view) return CoverConfigurationStage::InvalidInputs;
    context_=context;unknown0c_=device;unknown10_=view;
    surface_=config;
    IPL_ABI::readCoverConfigPolicy(static_cast<int32_t>(surface_.scalar[7]),text_,threshold_,enabled_);
    return enabled_?CoverConfigurationStage::NeedsImageSetup:CoverConfigurationStage::Disabled;
}
}
