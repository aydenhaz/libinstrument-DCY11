#include "AutoExscreenUiConfig.h"
#include <DiceString16.h>
#include <hsl/Bundle.h>
#include <hsl/Config.h>
#include <hsl/HSL.h>
#include <NS_MultiScreenBase/AutoConfigScreenMode.h>
#include <NS_MultiScreenBase/AutoConfigJsonData.h>
#include <vector>

namespace NS_MultiScreenBase {
namespace {
const char* const screenKeys[] = {
    "ExScreenLarge", "ExScreenSmall", "ExScreenWidget"
};
const char* const sceneKeys[] = {
    "kExScreenCuriseKey", "kExScreenGuideKey", "kExScreenPreviewKey"
};
const char* const fieldKeys[] = {
    "int32MapLevel", "int32MapViewMode", "int32CrossControl",
    "int32EagleEyeEnable", "int32MapPreviwe", "int32mapDayNightMode"
};
}

bool AutoExscreenUtil_IsDynamicLevelRequest(int configuredLevel, int requestedLevel) {
    return requestedLevel == -1 || (configuredLevel == -1 && requestedLevel == 99);
}

int* AutoExscreenUtil_GetCurrentSceneSlot(ScreenType screen) {
    static int scenes[3] = {0, 0, 0};
    const int index = static_cast<int>(screen);
    return &scenes[index == 1 ? 1 : index == 2 ? 2 : 0];
}

int AutoExscreenUtil_GetMapParamsFromUiConfig(ScreenType screen, int mode,
                                               int scene, int field) {
    const int screenIndex = static_cast<int>(screen);
    if (screenIndex < 0 || screenIndex >= 3 || mode < 0 || mode >= 20 ||
        scene < 0 || scene >= 3 || field < 0 || field >= 6) return 99;
    hsl::Config* config = hsl::HSL::GetConfigHandle();
    if (!config) return 99;

    const dice::String16 screenKey(screenKeys[screenIndex]);
    hsl::Bundle screens;
    if (!config->GetBundle(screenKey, screens)) return 99;

    char modeName[] = "kExScreenModeKey00";
    modeName[16] = static_cast<char>('0' + mode / 10);
    modeName[17] = static_cast<char>('0' + mode % 10);
    if (mode < 10) {
        modeName[16] = modeName[17];
        modeName[17] = 0;
    }
    const dice::String16 modeKey(modeName);
    hsl::Bundle modes;
    if (!screens.GetBundle(modeKey, modes)) return 99;
    const dice::String16 sceneKey(sceneKeys[scene]);
    hsl::Bundle scenes;
    if (!modes.GetBundle(sceneKey, scenes)) return 99;
    const dice::String16 fieldKey(fieldKeys[field]);
    int32_t result = 99;
    scenes.GetInt32(fieldKey, result);
    return result;
}

int AutoExscreenUtil_GetMapViewLevelFromUiConfig(ScreenType screen) {
    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
    return AutoExscreenUtil_GetMapParamsFromUiConfig(
        screen, mode, *AutoExscreenUtil_GetCurrentSceneSlot(screen), 0);
}

int AutoExscreenUtil_GetMapViewModeFromUiConfig(ScreenType screen) {
    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
    return AutoExscreenUtil_GetMapParamsFromUiConfig(
        screen, mode, *AutoExscreenUtil_GetCurrentSceneSlot(screen), 1);
}

int AutoExscreenUtil_GetMapDayNightModeFromUiConfig(ScreenType screen) {
    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
    return AutoExscreenUtil_GetMapParamsFromUiConfig(
        screen, mode, *AutoExscreenUtil_GetCurrentSceneSlot(screen), 5);
}

int AutoExscreenUtil_IsOnlyShowCrossImage(ScreenType screen, int fallback) {
    static const char16_t* const keys[] = {
        u"IsLargePannelOnlyShowCrossImage",
        u"IsSmallPannelOnlyShowCrossImage",
        u"IsWidgetOnlyShowCrossImage"
    };
    const int index = static_cast<int>(screen);
    if (index < 0 || index >= 3) return fallback;
    hsl::Config* config = hsl::HSL::GetConfigHandle();
    if (!config) return fallback;
    const dice::String16 key(reinterpret_cast<const unsigned short*>(keys[index]));
    int value = 99;
    config->GetInt32(key, value, hsl::Config::VALUE_STRATEGY_DEFAULT);
    return value == 99 ? fallback : value;
}

void AutoExscreenUtil_SetMapViewLevelToUiConfig(ScreenType screen, int level) {
    const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
    AutoExscreenUtil_SetMapParamsToUiConfig(
        screen, mode, *AutoExscreenUtil_GetCurrentSceneSlot(screen), 0, level);
}

void AutoExscreenUtil_SetMapViewModeToUiConfig(ScreenType screen, int mode) {
    const int currentMode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
    const int scene = *AutoExscreenUtil_GetCurrentSceneSlot(screen);
    AutoExscreenUtil_SetMapParamsToUiConfig(
        screen, currentMode, scene == 2 ? 1 : scene, 1, mode);
}

void AutoExscreenUtil_SetEagleEyeEnabledToUiConfig(ScreenType screen,
                                                    bool requested) {
    int allModes = 0;
    hsl::Config* config = hsl::HSL::GetConfigHandle();
    if (config) {
        const dice::String16 key(
            reinterpret_cast<const unsigned short*>(u"ControlAllMultiModeFlag"));
        config->GetInt32(key, allModes, hsl::Config::VALUE_STRATEGY_DEFAULT);
    }
    if ((allModes & 1) == 0) {
        const int mode = AutoConfigScreenMode::GetInstance()->GetCurrentScreenMode(screen);
        AutoExscreenUtil_SetMapParamsToUiConfig(screen, mode, 1, 3,
                                                requested ? 1 : 0);
        return;
    }

    std::vector<tagMultiViewModeInfo> modes;
    if (!AutoConfigJsonData::GetInstance()->GetMultiViewModeInfo(screen, modes))
        return;
    for (std::size_t mode = 0; mode < modes.size(); ++mode)
        AutoExscreenUtil_SetMapParamsToUiConfig(screen, static_cast<int>(mode),
                                                1, 3, requested ? modes[mode].value : 0);
}

void AutoExscreenUtil_SetMapParamsToUiConfig(ScreenType screen, int mode,
                                              int scene, int field, int value) {
    const int screenIndex = static_cast<int>(screen);
    if (screenIndex < 0 || screenIndex >= 3 || mode < 0 || mode >= 20 ||
        scene < 0 || scene >= 3 || field < 0 || field >= 6) return;
    hsl::Config* config = hsl::HSL::GetConfigHandle();
    if (!config) return;

    const dice::String16 screenKey(screenKeys[screenIndex]);
    hsl::Bundle existing;
    config->GetBundle(screenKey, existing);
    hsl::Bundle rebuilt;
    for (int modeIndex = 0; modeIndex < 20; ++modeIndex) {
        char modeName[] = "kExScreenModeKey00";
        modeName[16] = static_cast<char>('0' + modeIndex / 10);
        modeName[17] = static_cast<char>('0' + modeIndex % 10);
        if (modeIndex < 10) {
            modeName[16] = modeName[17];
            modeName[17] = 0;
        }
        const dice::String16 modeKey(modeName);
        hsl::Bundle oldMode;
        existing.GetBundle(modeKey, oldMode);
        hsl::Bundle newMode;
        for (int sceneIndex = 0; sceneIndex < 3; ++sceneIndex) {
            const dice::String16 sceneKey(sceneKeys[sceneIndex]);
            hsl::Bundle oldScene;
            oldMode.GetBundle(sceneKey, oldScene);
            hsl::Bundle newScene;
            for (int fieldIndex = 0; fieldIndex < 6; ++fieldIndex) {
                const dice::String16 fieldKey(fieldKeys[fieldIndex]);
                int32_t previous = 99;
                oldScene.GetInt32(fieldKey, previous);
                const int32_t next = modeIndex == mode && sceneIndex == scene &&
                                     fieldIndex == field ? value : previous;
                newScene.PutInt32(fieldKey, next);
            }
            newMode.PutBundle(sceneKey, newScene);
        }
        rebuilt.PutBundle(modeKey, newMode);
    }
    config->SetBundle(screenKey, rebuilt, static_cast<hsl::SetterType_>(1));
}
}
