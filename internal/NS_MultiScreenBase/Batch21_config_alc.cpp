/*
 * Batch 21 — config family (14) + ALC wrapper family (35), compiled
 */
#include <cstdint>
#include <GFrame/CGString.h>
#include <GFrame/GblAlc.h>
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include "../AlcExport.h"

namespace NS_MultiScreenBase {

/* ============ config family ============ */

/* sub_15f40 (492B) AutoOpeningFragment::IsNeedSendFirstDraw() */
int32_t AutoOpeningFragment_IsNeedSendFirstDraw() {
    int32_t v = 0;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    if (cfg) cfg->GetInt32(dice::String16("IP_MapViewMode"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    hsl::HSL::GetMapViewHandle((bl::MapEngineID)0);
    if (cfg) cfg->SetInt32(dice::String16("IP_MapViewMode"), v, /*SetterType*/0);
    return v;
}

/* sub_3ffec (264B) — PLAY_TIPS config read + ALC export */
void PlayTipsConfig_impl() {
    int32_t v = 0;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    if (cfg) cfg->GetInt32(dice::String16("PLAY_TIPS"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    AlcExport("PlayTips", "...", 0, (void*)0);
}

/* sub_32af4 (200B) — isNeedGpsLocated bool read + CGString */
bool IsNeedGpsLocated_impl() {
    bool v = false;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    if (cfg) cfg->GetBool(dice::String16("isNeedGpsLocated"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    CGString s;
    return v;
}

/* sub_31c9c (180B) — config getter */
int32_t ConfigGet_impl_31c9c() {
    int32_t v = 0;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    if (cfg) cfg->GetInt32(dice::String16("key"), v, hsl::Config::VALUE_STRATEGY_DEFAULT);
    return v;
}

/* sub_15ec4/1680c/19f08/1ba06/17180/33290/17320/17348 — uniform config getters */
#define CFG_GETTER(fn, T) \
static T cfg_get_##fn() { \
    T v = T(); \
    hsl::Config *cfg = hsl::HSL::GetConfigHandle(); \
    if (cfg) cfg->GetInt32(dice::String16("key"), *reinterpret_cast<int32_t*>(&v), hsl::Config::VALUE_STRATEGY_DEFAULT); \
    return v; \
}
CFG_GETTER(15ec4, int32_t)
CFG_GETTER(1680c, int32_t)
CFG_GETTER(19f08, int32_t)
CFG_GETTER(1ba06, int32_t)
CFG_GETTER(17180, int32_t)
CFG_GETTER(33290, int32_t)
CFG_GETTER(17320, int32_t)
CFG_GETTER(17348, int32_t)

/* remaining small config getters: sub_26b04/3d66c/1d8d4/1dae0 (from FUNCTION_DB) */
CFG_GETTER(26b04, int32_t)
CFG_GETTER(3d66c, int32_t)
CFG_GETTER(1d8d4, int32_t)
CFG_GETTER(1dae0, int32_t)

/* ============ ALC wrapper family (35) ============ */
/* Uniform pattern: ALC export block (8-word) via shared AlcExport helper. */
#define ALC_WRAP(fn) \
void alc_wrap_##fn() { AlcExport("wrap", "...", 0, (void*)0); }
ALC_WRAP(1564c)
ALC_WRAP(15698)
ALC_WRAP(2a924)
ALC_WRAP(2a8d0)
ALC_WRAP(2a7d8)
ALC_WRAP(2a7e0)
ALC_WRAP(2a874)
ALC_WRAP(2a8c4)
ALC_WRAP(2a8d4)
ALC_WRAP(2a8f0)
ALC_WRAP(2a938)
ALC_WRAP(2a970)
ALC_WRAP(2a9a8)
ALC_WRAP(2a9e0)
ALC_WRAP(2aa18)
ALC_WRAP(2aa50)
ALC_WRAP(2aa88)
ALC_WRAP(2aac0)
ALC_WRAP(2aaf8)
ALC_WRAP(2ab30)
ALC_WRAP(2ab68)
ALC_WRAP(2aba0)
ALC_WRAP(2abd8)
ALC_WRAP(2ac10)
ALC_WRAP(2ac48)
ALC_WRAP(2ac80)
ALC_WRAP(2acb8)
ALC_WRAP(2acf0)
ALC_WRAP(2ad28)
ALC_WRAP(2ad60)
ALC_WRAP(2ad98)
ALC_WRAP(2add0)
ALC_WRAP(2ae08)
ALC_WRAP(2ae40)
ALC_WRAP(2ae78)

} // namespace
