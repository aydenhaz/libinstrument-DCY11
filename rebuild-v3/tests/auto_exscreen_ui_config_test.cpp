#include "../internal/NS_MultiScreenBase/AutoExscreenUiConfig.h"
#include "../internal/NS_MultiScreenBase/AutoExscreenMapStatusPlan.h"
#include <DiceString16.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <hsl/Bundle.h>
#include <hsl/Config.h>
#include <hsl/HSL.h>
#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace {
using Key = std::u16string;
Key key(const dice::String16& s) {
    return Key(reinterpret_cast<const char16_t*>(s.c_str()), s.size());
}
struct Node {
    std::map<Key, Node> bundles;
    std::map<Key, int32_t> ints;
};
std::map<const hsl::Bundle*, Node> bundleData;
std::map<Key, Node> rootData;
std::map<Key, int> rootInts;
hsl::Config config;
bool configAvailable = true;
std::vector<NS_MultiScreenBase::tagMultiViewModeInfo> modeValues;
bool modeValuesAvailable = true;
int modeReads = 0;
}
namespace hsl {
Bundle::Bundle() { bundleData[this] = Node{}; }
Bundle::~Bundle() { bundleData.erase(this); }
void Bundle::PutBundle(const dice::String16& k, const Bundle& value) {
    bundleData[this].bundles[key(k)] = bundleData.at(&value);
}
bool Bundle::GetBundle(const dice::String16& k, Bundle& value) {
    const auto found = bundleData.at(this).bundles.find(key(k));
    if (found == bundleData.at(this).bundles.end()) return false;
    bundleData[&value] = found->second;
    return true;
}
void Bundle::PutInt32(const dice::String16& k, int32_t value) {
    bundleData[this].ints[key(k)] = value;
}
bool Bundle::GetInt32(const dice::String16& k, int32_t& value) {
    const auto found = bundleData.at(this).ints.find(key(k));
    if (found == bundleData.at(this).ints.end()) return false;
    value = found->second;
    return true;
}
bool Config::GetBundle(const dice::String16& k, Bundle& result) {
    const auto found = rootData.find(key(k));
    if (found == rootData.end()) return false;
    bundleData[&result] = found->second;
    return true;
}
bool Config::SetBundle(const dice::String16& k, const Bundle& value, SetterType_ setter) {
    assert(static_cast<int>(setter) == 1);
    rootData[key(k)] = bundleData.at(&value);
    return true;
}
bool Config::GetInt32(const dice::String16& k, int& result, VALUE_STRATEGY strategy) {
    assert(strategy == VALUE_STRATEGY_DEFAULT);
    const auto found = rootInts.find(key(k));
    if (found == rootInts.end()) return false;
    result = found->second;
    return true;
}
Config* HSL::GetConfigHandle() { return configAvailable ? &config : nullptr; }
}
namespace NS_MultiScreenBase {
AutoConfigJsonData* AutoConfigJsonData::GetInstance() {
    static AutoConfigJsonData instance;
    return &instance;
}
bool AutoConfigJsonData::GetMultiViewModeInfo(
    ScreenType, std::vector<tagMultiViewModeInfo>& values) {
    ++modeReads;
    if (!modeValuesAvailable) return false;
    values = modeValues;
    return true;
}
AutoConfigScreenMode* AutoConfigScreenMode::GetInstance() {
    static AutoConfigScreenMode instance;
    return &instance;
}
int AutoConfigScreenMode::GetCurrentScreenMode(ScreenType) { return 7; }
}
int main() {
    const Key screen = u"ExScreenLarge";
    Node& before = rootData[screen];
    before.bundles[u"kExScreenModeKey7"].bundles[u"kExScreenGuideKey"]
          .ints[u"int32MapLevel"] = 12;
    before.bundles[u"kExScreenModeKey7"].bundles[u"kExScreenCuriseKey"]
          .ints[u"int32MapViewMode"] = 3;
    NS_MultiScreenBase::AutoExscreenUtil_SetMapParamsToUiConfig(
        NS_MultiScreenBase::SCREEN_TYPE_MAIN, 7, 1, 2, 1);
    const auto& after = rootData.at(screen);
    const auto& mode7 = after.bundles.at(u"kExScreenModeKey7");
    const auto& guide = mode7.bundles.at(u"kExScreenGuideKey");
    assert(guide.ints.at(u"int32CrossControl") == 1);
    assert(guide.ints.at(u"int32MapLevel") == 12);
    assert(mode7.bundles.at(u"kExScreenCuriseKey").ints.at(u"int32MapViewMode") == 3);
    assert(after.bundles.at(u"kExScreenModeKey19")
           .bundles.at(u"kExScreenPreviewKey").ints.at(u"int32MapPreviwe") == 99);
    const Node unchanged = after;
    NS_MultiScreenBase::AutoExscreenUtil_SetMapParamsToUiConfig(
        NS_MultiScreenBase::SCREEN_TYPE_MAIN, 20, 1, 2, 1);
    assert(rootData.at(screen).bundles.size() == unchanged.bundles.size());
    assert(NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(0, -1));
    assert(NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(-1, 99));
    assert(!NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(0, 99));
    assert(!NS_MultiScreenBase::AutoExscreenUtil_IsDynamicLevelRequest(-1, 1));

    using namespace NS_MultiScreenBase;
    int* currentScene = AutoExscreenUtil_GetCurrentSceneSlot(SCREEN_TYPE_MAIN);
    assert(*currentScene == 0);
    *currentScene = 2;
    AutoExscreenUtil_SetMapViewLevelToUiConfig(SCREEN_TYPE_MAIN, 15);
    AutoExscreenUtil_SetMapViewModeToUiConfig(SCREEN_TYPE_MAIN, 2);
    const auto& written = rootData.at(screen).bundles.at(u"kExScreenModeKey7");
    assert(written.bundles.at(u"kExScreenPreviewKey").ints.at(u"int32MapLevel") == 15);
    assert(written.bundles.at(u"kExScreenPreviewKey").ints.at(u"int32MapViewMode") == 99);
    assert(written.bundles.at(u"kExScreenGuideKey").ints.at(u"int32MapViewMode") == 2);
    assert(written.bundles.at(u"kExScreenGuideKey").ints.at(u"int32CrossControl") == 1);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 7, 2, 0) == 15);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 7, 1, 1) == 2);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 7, 2, 1) == 99);
    assert(AutoExscreenUtil_GetMapViewLevelFromUiConfig(SCREEN_TYPE_MAIN) == 15);
    assert(AutoExscreenUtil_GetMapViewModeFromUiConfig(SCREEN_TYPE_MAIN) == 99);
    assert(AutoExscreenUtil_GetMapDayNightModeFromUiConfig(SCREEN_TYPE_MAIN) == 99);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 0, 0, 1) == 99);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 20, 0, 1) == 99);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, -1, 0, 1) == 99);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 7, 3, 1) == 99);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 7, 0, 6) == 99);
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(static_cast<ScreenType>(3), 7, 0, 0) == 99);
    configAvailable = false;
    assert(AutoExscreenUtil_GetMapParamsFromUiConfig(SCREEN_TYPE_MAIN, 7, 2, 0) == 99);
    assert(AutoExscreenUtil_IsOnlyShowCrossImage(SCREEN_TYPE_MAIN, 1) == 1);
    configAvailable = true;
    rootInts[u"IsLargePannelOnlyShowCrossImage"] = 0;
    rootInts[u"IsSmallPannelOnlyShowCrossImage"] = 1;
    rootInts[u"IsWidgetOnlyShowCrossImage"] = 99;
    assert(AutoExscreenUtil_IsOnlyShowCrossImage(SCREEN_TYPE_MAIN, 1) == 0);
    assert(AutoExscreenUtil_IsOnlyShowCrossImage(static_cast<ScreenType>(1), 0) == 1);
    assert(AutoExscreenUtil_IsOnlyShowCrossImage(static_cast<ScreenType>(2), 0) == 0);
    assert(AutoExscreenUtil_IsOnlyShowCrossImage(static_cast<ScreenType>(3), 1) == 1);
    *currentScene = 0;
    AutoExscreenUtil_SetMapViewModeToUiConfig(SCREEN_TYPE_MAIN, 3);
    assert(rootData.at(screen).bundles.at(u"kExScreenModeKey7")
           .bundles.at(u"kExScreenCuriseKey").ints.at(u"int32MapViewMode") == 3);

    const MapStatusPlan noChanges = AutoExscreenUtil_PlanMapStatus(4, 99, 99);
    assert(!noChanges.writeLevel && !noChanges.writeMode);
    assert(noChanges.resetMapBindingList);
    const MapStatusPlan dynamic = AutoExscreenUtil_PlanMapStatus(-1, 99, 1);
    assert(!dynamic.writeLevel && dynamic.writeMode && dynamic.disableDynamicLevel);
    assert(dynamic.attemptTimerStart && dynamic.setMapViewMode);
    const MapStatusPlan requestDynamic = AutoExscreenUtil_PlanMapStatus(10, -1, 1);
    assert(requestDynamic.writeLevel && requestDynamic.setMapLevel);
    assert(requestDynamic.writeMode && !requestDynamic.disableDynamicLevel);
    assert(!requestDynamic.attemptTimerStart && !requestDynamic.setMapViewMode);
    const MapStatusPlan fixed = AutoExscreenUtil_PlanMapStatus(10, 12, 1);
    assert(fixed.writeLevel && fixed.writeMode && fixed.setMapViewMode);
    assert(!fixed.disableDynamicLevel && !fixed.attemptTimerStart);

    // V21 0x2b106..0x2b1b4: bit zero fans out the EagleEye guide field to
    // every available mode; otherwise only the current screen mode is written.
    rootInts[u"ControlAllMultiModeFlag"] = 1;
    modeValues = {{1}, {0}, {1}};
    AutoExscreenUtil_SetEagleEyeEnabledToUiConfig(SCREEN_TYPE_MAIN, true);
    assert(modeReads == 1);
    for (int i = 0; i < 3; ++i) {
        const Key modeKey = Key(u"kExScreenModeKey") + char16_t(u'0' + i);
        const auto& entry = rootData.at(screen).bundles.at(modeKey);
        assert(entry.bundles.at(u"kExScreenGuideKey").ints.at(u"int32EagleEyeEnable")
               == (i == 1 ? 0 : 1));
        assert(entry.bundles.at(u"kExScreenCuriseKey").ints.at(u"int32EagleEyeEnable") == 99);
    }
    AutoExscreenUtil_SetEagleEyeEnabledToUiConfig(SCREEN_TYPE_MAIN, false);
    assert(modeReads == 2);
    for (int i = 0; i < 3; ++i) {
        const Key modeKey = Key(u"kExScreenModeKey") + char16_t(u'0' + i);
        assert(rootData.at(screen).bundles.at(modeKey)
               .bundles.at(u"kExScreenGuideKey").ints.at(u"int32EagleEyeEnable") == 0);
    }
    rootInts[u"ControlAllMultiModeFlag"] = 2;
    AutoExscreenUtil_SetEagleEyeEnabledToUiConfig(SCREEN_TYPE_MAIN, true);
    assert(modeReads == 2);
    assert(rootData.at(screen).bundles.at(u"kExScreenModeKey7")
           .bundles.at(u"kExScreenGuideKey").ints.at(u"int32EagleEyeEnable") == 1);
    rootInts[u"ControlAllMultiModeFlag"] = 3;
    modeValuesAvailable = false;
    AutoExscreenUtil_SetEagleEyeEnabledToUiConfig(SCREEN_TYPE_MAIN, false);
    assert(modeReads == 3);
    assert(rootData.at(screen).bundles.at(u"kExScreenModeKey7")
           .bundles.at(u"kExScreenGuideKey").ints.at(u"int32EagleEyeEnable") == 1);
}
