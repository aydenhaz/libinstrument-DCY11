#include "OpeningCoverBinding.h"
#include "CoverUtil.h"
#include <GFrame/CGFragment.h>
#include <hsl/HSL.h>
#include <cstring>
namespace NS_MultiScreenBase {
namespace {
template<class T> T field(void* object,unsigned offset) {
    T value;std::memcpy(&value,static_cast<unsigned char*>(object)+offset,sizeof(value));return value;
}
template<class F> F slot(void* object,unsigned index) {
    return reinterpret_cast<F>(field<void**>(object,0)[index]);
}
}
void bindOpeningCover(void* fragment,CoverUtil* cover) {
    auto* bytes=static_cast<unsigned char*>(fragment);
    std::memcpy(bytes+0x40c,&cover,sizeof(cover));
    if(!cover) return;
    void* service=hsl::HSL::GetMapServiceHandle();
    if(!service) return;
    void* facility=field<void*>(fragment,0x388);
    if(!facility) return;
    const uint32_t deviceId=slot<uint32_t(*)(void*)>(facility,42)(facility);
    auto* device=slot<bl::IMapDevice*(*)(void*,uint32_t)>(service,8)(service,deviceId);
    // The original snapshots cover before getContext, then reloads facility.
    CoverUtil* target=field<CoverUtil*>(fragment,0x40c);
    auto* context=reinterpret_cast<GNS_FRAME::CGFragment*>(fragment)->getContext();
    facility=field<void*>(fragment,0x388);
    auto* view=slot<bl::IMapView*(*)(void*)>(facility,9)(facility);
    // One nontrivial by-value SurfaceConfig copy, including its CGString.
    target->setConfig(context,device,view,*reinterpret_cast<const SurfaceConfig*>(bytes+0x3a0));
}
}
