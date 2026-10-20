#include "CoverUtil.h"
#include <GFrame/CGImage.h>
#include <hsl/HSL.h>
#include <hsl/SystemNotify.h>
namespace NS_MultiScreenBase {
void CoverUtil::setConfig(GNS_FRAME::CGContext* context,bl::IMapDevice* device,
        bl::IMapView* view,SurfaceConfig config) {
    (void)initializeConfiguredCoverImpl(context,device,view,config);
}
CoverInitializationResult CoverUtil::initializeConfiguredCover(GNS_FRAME::CGContext* context,
        bl::IMapDevice* device,bl::IMapView* view,SurfaceConfig config) {
    return initializeConfiguredCoverImpl(context,device,view,config);
}
CoverInitializationResult CoverUtil::initializeConfiguredCoverImpl(GNS_FRAME::CGContext* context,
        bl::IMapDevice* device,bl::IMapView* view,const SurfaceConfig& config) {
    const auto stage=prepareConfiguration(context,device,view,config);
    if(stage==CoverConfigurationStage::InvalidInputs) return CoverInitializationResult::InvalidInputs;
    if(stage==CoverConfigurationStage::Disabled) return CoverInitializationResult::Disabled;
    // Original captures these dimensions before invoking external image/file services.
    const GPI::CGSize finalSize(static_cast<int32_t>(surface_.scalar[0]),static_cast<int32_t>(surface_.scalar[1]));
    if(!recreateImageStorage(finalSize)) return CoverInitializationResult::ImageCreateFailed;
    (void)loadConfiguredResourceImage(); // Missing/failed optional file still proceeds.
    if(!bindScreenshotSource(finalSize)) return CoverInitializationResult::ImageUnavailable;
    hsl::HSL::GetSystemNotifyHandle()->AddMapSurfaceViewObserver(
        static_cast<hsl::MapSurfaceViewObserver*>(this));
    // Checked after registration: that external call can mutate object state.
    if(unknown14_) {
        installTopRenderCallback();
        return CoverInitializationResult::TopRenderRegistered;
    }
    return CoverInitializationResult::MapObserverRegistered;
}
}
