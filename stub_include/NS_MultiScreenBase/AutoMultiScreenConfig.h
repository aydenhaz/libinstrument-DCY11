#pragma once
namespace NS_MultiScreenBase {
class AutoMultiScreenConfig {
public:
    static AutoMultiScreenConfig* GetInstance();
    void ResetMapBindingList(int scene);
    void UpdateTrafficEventState(bool);
    void SetMapLevel(int fallbackLevel, int requestedLevel);
    void SetMapViewMode(int fallbackMode, int requestedMode);
};
}
