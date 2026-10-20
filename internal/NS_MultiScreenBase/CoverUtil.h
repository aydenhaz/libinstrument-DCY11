#pragma once
#include <hsl/MapSurfaceViewObserver.h>
#include <bl/IBLMapBusinessDataObserver.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <DiceString16.h>
#include <cstdint>
#include <cstddef>
namespace NS_MultiScreenBase {
struct CoverUtilTestAccess;
class CoverUtil : public hsl::MapSurfaceViewObserver, public bl::IBLMapBusinessDataObserver {
public:
    CoverUtil();
    ~CoverUtil() override;
    void MapSurfaceViewStatusChange(int,MAPSURFACE,void*,int,int,int) override;
    bool onBusinessDataObserver(int,const void*,void*) override;
    bool isUIFirstRenderFinish();
    // Configuration, draw/capture services and full module integration are still
    // unrecovered. This class is not enabled in the Packet factory.
private:
    friend struct CoverUtilTestAccess;
    uint32_t unknown08_=0;
    void* unknown0c_=nullptr;
    void* unknown10_=nullptr;
    uint8_t unknown14_=0, inhibitCapture_=0;
    uint16_t padding16_;
    dice::String16 text_;
    int32_t threshold_=2;
    SurfaceConfig surface_;
    uint8_t enabled_=0;
    uint8_t padding71_[3];
    uint32_t unknown74_=0, unknown78_=0;
    int8_t stride_=0;
    uint8_t padding7d_[3];
    int32_t width_=0,height_=0;
    uint8_t* bytes_=nullptr;
    uint32_t length_=0;
    uint16_t unknown90_=0,padding92_;
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
