#include <AbiTypes.h>
#pragma once
namespace NS_MultiScreenBase { struct tagSTExtScreen; }
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
    int GetStartOpenScreenMode(int);
    void SetStartOpenScreenMode(int, const NS_MultiScreenBase::tagSTExtScreen&);
    bl::MapEngineID GetMapEngineId(int);
    int SetNaviType(ENaviMode_, ECruiseMode_);
    int SetNaviType(int navigation,int cruise) {
        return SetNaviType(static_cast<ENaviMode_>(navigation),static_cast<ECruiseMode_>(cruise));
    }
};
}
