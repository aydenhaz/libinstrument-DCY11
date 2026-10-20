#include <AbiTypes.h>
#pragma once
namespace NS_MultiScreenBase {
class AutoConfigScreenMode {
public:
    static AutoConfigScreenMode *GetInstance();
    int GetCurrentScreenMode(ScreenType);
    int GetCurrentScreenMode(int screen) { return GetCurrentScreenMode(static_cast<ScreenType>(screen)); }
    void SetCurrentScreenMode(ScreenType, int);
    void SetCurrentScreenMode(int screen, int mode) { SetCurrentScreenMode(static_cast<ScreenType>(screen), mode); }
    void SetIaccTheme(ScreenType, bool);
};
}
