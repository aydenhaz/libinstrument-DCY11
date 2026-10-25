#include <AbiTypes.h>
#pragma once
#include <bl/LocationTypes.h>
namespace bl { typedef int MapDeviceID; struct ObjectRuntimeInfo { explicit ObjectRuntimeInfo(void*){} }; }

namespace hsl {
class SystemNotify; class Config; class Guide; class MapView; class ThirdManager;
class HSL {
public:
    static SystemNotify *GetSystemNotifyHandle();
    static Config *GetConfigHandle();
    static ThirdManager *Get3rdHandle();
    static HSL *GetHandle();
    static void *GetMapServiceHandle();
    static MapView *GetMapViewHandle(bl::MapEngineID);
    static Guide *GetGuideHandle();
    static void *GetPosHandle();
    static void *GetRouteHandle();
    static void GetCarPosition(bl::CarLoc&, bl::MapEngineID);
    void InitModule(ModuleType, bl::MapEngineID);
};
}
