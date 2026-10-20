#pragma once
namespace NS_MultiScreenBase {
class AutoConfigScreenMode {
public:
    static AutoConfigScreenMode *GetInstance();
    int GetCurrentScreenMode(int);
    void SetCurrentScreenMode(int, int);
};
}
