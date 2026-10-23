#include <AbiTypes.h>
#pragma once
#include <ExternalScreenTypes.h>
namespace bl {  }
namespace hsl {
class MultiScreenService {
public:
    static MultiScreenService *GetInstance();
    bool IsCreatedLargeScreen();
    void SetCreatedLargeScreen(bool);
    void InitLock();
    bool IsHslStartupFinished();
    void InitLockFree();
    tagSTExtScreen GetStartOpenScreenMode(int);
    void SetStartOpenScreenMode(int, tagSTExtScreen);
    bl::MapEngineID GetMapEngineId(int);
    int GetWindowId(bl::MapEngineID);
    int SetNaviType(ENaviMode_, ECruiseMode_);
    int SetNaviType(int navigation,int cruise) {
        return SetNaviType(static_cast<ENaviMode_>(navigation),static_cast<ECruiseMode_>(cruise));
    }
};
}
