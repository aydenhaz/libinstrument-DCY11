#include "AutoExscreenUiConfig.h"
#include <DiceString16.h>
#include <hsl/Bundle.h>
#include <hsl/Config.h>
#include <hsl/HSL.h>

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
