#pragma once
#include <hsl/MapSurfaceViewObserver.h>
#include <bl/IBLMapBusinessDataObserver.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <DiceString16.h>
#include <cstdint>
#include <cstddef>
#include "../abi/CoverGraphics.h"
namespace GNS_FRAME { class CGContext; }
namespace GPI { struct CGSize; }
namespace bl { class IMapDevice; class IMapView; }
namespace NS_MultiScreenBase {
enum class CoverInitializationResult { InvalidInputs, Disabled, ImageCreateFailed, ImageUnavailable, MapObserverRegistered, TopRenderRegistered };
enum class CoverResourceResult { MissingFile, Loaded, LoadFailed, NoImage };
enum class CoverConfigurationStage { InvalidInputs, Disabled, NeedsImageSetup };
struct CoverUtilTestAccess;
class CoverUtil : public hsl::MapSurfaceViewObserver, public bl::IBLMapBusinessDataObserver {
public:
    CoverUtil();
    ~CoverUtil() override;
    void MapSurfaceViewStatusChange(int,MAPSURFACE,void*,int,int,int) override;
    bool onBusinessDataObserver(int,const void*,void*) override;
    bool isUIFirstRenderFinish();
    void TopRenderCoverCallback(GNS_FRAME::CGCanvas*);
    void installTopRenderCallback();
    // Explicit stages; resource-file loading must precede the buffer binding.
    bool recreateImageStorage();
    CoverResourceResult loadConfiguredResourceImage();
    bool bindScreenshotSource(const GPI::CGSize& finalSize);
    void setConfig(GNS_FRAME::CGContext*,bl::IMapDevice*,bl::IMapView*,SurfaceConfig);
    CoverInitializationResult initializeConfiguredCover(GNS_FRAME::CGContext*,bl::IMapDevice*,bl::IMapView*,SurfaceConfig);
    // Explicit prefix operation, not the unrecovered complete setConfig method.
    CoverConfigurationStage prepareConfiguration(GNS_FRAME::CGContext*,bl::IMapDevice*,bl::IMapView*,const SurfaceConfig&);
    // Logging and Packet integration remain unrecovered. This class is not enabled in the Packet factory.
private:
    CoverInitializationResult initializeConfiguredCoverImpl(GNS_FRAME::CGContext*,bl::IMapDevice*,bl::IMapView*,const SurfaceConfig&);
    bool recreateImageStorage(const GPI::CGSize&);
    friend struct CoverUtilTestAccess;
    GNS_FRAME::CGContext* context_=nullptr;
    void* unknown0c_=nullptr;
    void* unknown10_=nullptr;
    uint8_t unknown14_=0, inhibitCapture_=0;
    uint16_t padding16_;
    dice::String16 text_;
    int32_t threshold_=2;
    SurfaceConfig surface_;
    uint8_t enabled_=0;
    uint8_t padding71_[3];
    bl::ScreenShotInfo screenshot_;
    int32_t unknown94_=1;
    uint8_t requested_=0;
    uint8_t padding99_[3];
    void* image_=nullptr;
    uint8_t state_=0;
    uint8_t paddinga1_[3];
    int32_t counter_=0;
};
#if UINTPTR_MAX == 0xffffffffu
static_assert(sizeof(CoverUtil)==0xa8,"V21 CoverUtil ARM32 extent");
#endif
}
