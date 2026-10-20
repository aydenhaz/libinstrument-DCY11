#include <AbiTypes.h>
#pragma once
#include <cstdint>
namespace bl {  }
namespace GPI { class Closure; }
namespace NS_MultiScreenBase {

struct tagSTExtScreen;
struct SurfaceConfig;
struct tagMultiViewModeInfo;

class AutoExscreenNotifyObserver {
public:
    virtual ~AutoExscreenNotifyObserver() {}
    void OnExScreenNotify(int32_t event, tagSTExtScreen *st);   // sub_29bf0
    void DeferActionA();                                        // sub_2a5a8
    void DeferActionB();                                        // sub_2a7e0
    void DeferActionC();                                        // sub_2aa28
private:
    int screenType_;
    void ActionA();
    void ActionB();
    void ActionC();
    char opaque_[32];
};

}
