#pragma once
#include <cstring>
#include <hsl/HSL.h>
#include <hsl/MapViewServiceImpl.h>
#include "V21PacketTrace.h"
namespace IPL_ABI {
// V21-specific host slots, verified at instrument 0x2f39e..0x2f3fa.
// Map service primary slot 15 returns the engine's display facility;
// facility slot 42 returns its device identifier (not the engine argument).
inline void destroyInstrumentMapFacility() {
    void* service=hsl::HSL::GetMapServiceHandle();
    if(!service)return;
    void** serviceTable;
    std::memcpy(&serviceTable,service,sizeof(serviceTable));
    typedef hsl::MapViewBase* (*Lookup)(void*,int);
    hsl::MapViewBase* facility=reinterpret_cast<Lookup>(serviceTable[15])(service,5);
    if(!facility)return;
    void** facilityTable;
    std::memcpy(&facilityTable,facility,sizeof(facilityTable));
    typedef int (*DeviceId)(hsl::MapViewBase*);
    const int device=reinterpret_cast<DeviceId>(facilityTable[42])(facility);
    packetTraceInt("deviceId:%d", "virtual void NS_InstrumentPannelLarge::CGIPLPacket::onDestroy()",0xff,true,device);
    reinterpret_cast<hsl::MapViewServiceImpl*>(service)->DestroyDisplayFacility(facility);
}
}
