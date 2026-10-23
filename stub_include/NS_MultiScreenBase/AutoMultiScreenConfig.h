#pragma once
#include <AbiTypes.h>
namespace NS_MultiScreenBase {
class AutoMultiScreenConfig {
public:
    static AutoMultiScreenConfig* GetInstance(ScreenType screen);
    void ResetMapBindingList(int scene);
    void UpdateTrafficEventState(bool);
    void SetMapLevel(int fallbackLevel, int requestedLevel);
    void SetMapViewMode(int fallbackMode, int requestedMode);
private:
    explicit AutoMultiScreenConfig(ScreenType screen);
    ~AutoMultiScreenConfig();
    alignas(4) unsigned char raw_[0x340];
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(AutoMultiScreenConfig) == 0x340,
              "V21 AutoMultiScreenConfig extent");
#endif
}
