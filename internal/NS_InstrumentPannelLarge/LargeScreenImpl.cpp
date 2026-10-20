/*
 * Batch 1 — LargeScreenImpl (IBaseModuleEx implementation)
 * Reconstructed from sub_2dfa4 / sub_2e02c / sub_2e03c / sub_2e0f8.
 *
 * Vtable @0x6b79c (LargeScreenImpl, 4 slots):
 *   [0] sub_2dfa4  ~LargeScreenImpl            -> ModuleUnReg(0x1fa) + ALC export tag 0x4b
 *   [1] sub_2e02c  deleting-dtor thunk -> [0]
 *   [2] sub_2e03c  int32_t QueryInterface(int32_t, void**)  -> ALC export tags 0x9f, 0x166
 *   [3] sub_2e0f8  OnChangeScreenMode(...)      -> 2548B state machine (below)
 *
 * ALC export block (8 words, passed by value as 4th arg):
 *   { 0, 0, module, aux, tag, signature, this, name }   fn(alc, 8, 0, block)
 * Verified strings:
 *   name/signature pairs @0x54980/0x549c4/0x54936/0x54969 (see analysis/)
 */
#include "LargeScreenImpl.h"
#include "../AlcExport.h"
#include <GFrame/CGString.h>
#include <GFrame/GblAlc.h>
#include <GFrame/CGFrameSimulation.h>
#include <GFrame/CGPackage.h>
#include <GFrame/CGExhibition.h>
#include <hsl/MultiScreenService.h>
#include <hsl/HSL.h>
#include <hsl/Config.h>
#include <ExModuleManager.h>

namespace NS_InstrumentPannelLarge {

static const char kSigDtor[]  = "virtual NS_InstrumentPannelLarge::LargeScreenImpl::~LargeScreenImpl()";
static const char kNameDtor[] = "ModuleUnReg:%p";
static const char kSigQI[]    = "virtual int32_t NS_InstrumentPannelLarge::LargeScreenImpl::QueryInterface(int32_t, void **)";
static const char kNameQI[]   = "QueryInterface:%p %p";


LargeScreenImpl::LargeScreenImpl() {
    /* vtable store observed at 0x2dfb2: vptr=0x6b79c(+0x1c region), count 8 */
}

LargeScreenImpl::~LargeScreenImpl() {
    /* sub_2dfa4 */
    ExModuleManager::GetInstance()->ModuleUnReg((ModuleInterfaceId)0x1fa /*506*/);
    AlcExport(kNameDtor, kSigDtor, 0x4b, this);
}

int32_t LargeScreenImpl::QueryInterface(int32_t id, void **out) {
    /* sub_2e03c — two ALC exports, no ModuleReg here */
    AlcExport(kNameQI, kSigQI, 0x9f /*159*/, this);
    AlcExport(kNameQI, kSigQI, 0x166 /*358*/, this);
    if (out) *out = this;
    return 0;
}

/*
 * sub_2e0f8 — screen-mode change handler (2548B).
 * Flow verified from call trace:
 *   1. Config::GetInt32(String16 key, int&, VALUE_STRATEGY) reads target mode
 *   2. For each of 4 screen-mode branches:
 *        CGPackage::getHostFragmentActivity() / getContext()
 *        I_Assert(ctx) ; fragment = tryStartExistFragment(intent, 2, ctx)
 *        frag = __dynamic_cast(frag, CGFragment -> concrete fragment class)
 *        if frag: CGFrameSimulation::ownWorkStation()/getExhibition()
 *                 CGString name(u"..."); exhibition->appendFragment(3, name, intent, 0)
 *   3. Fallback (no existing fragment): operator new fragment; frag->setContext(ctx);
 *      ...; frag->startNewFragmentUnrecovered(parent, 0, intent, 2)
 *   Log sites interleave CGFrameAlcLog::getInstance() + I_GetAlcDevPtr() pairs.
 */
void LargeScreenImpl::OnChangeScreenMode(int32_t screenType, int32_t mode, const GNS_FRAME::CGIntent &intent) {
    (void)screenType;(void)mode;(void)intent;
    int cur = 0;
    hsl::Config *cfg = hsl::HSL::GetConfigHandle();
    /* key literal pool @0x2e16e-0x2e198 region (String16) */
    /* cfg->GetInt32(key, cur, hsl::Config::VALUE_STRATEGY_DEFAULT); */

    GNS_FRAME::CGPackage *pkg = GNS_FRAME::CGPackage::getHostFragmentActivity();
    GNS_FRAME::CGContext *ctx = GNS_FRAME::CGPackage::getContext();
    /* Original assertion requires the real Fragment state, not a context string.
       Restore it together with the correct receiver/layout. */

    /* branch per (screenType, mode): see analysis log strings
       "largeScreenMode:%d,curLargeScreenMode:%d" @0x5365c, "largeScreenMode same as before" @0x53685 */
    GNS_FRAME::CGFragment *frag = GNS_FRAME::CGFragment::tryStartExistFragmentUnrecovered(intent, 2, ctx);
    /* __dynamic_cast to one of the 10 concrete CGInstrumentPannelLarge*Fragment types */

    if (frag) {
        GNS_FRAME::CGExhibition *exh = GNS_FRAME::CGFrameSimulation::getExhibition();
        CGString name(reinterpret_cast<const unsigned short*>(u""));  /* per-branch name, pool @0x2e2d6 */
        exh->appendFragment(3, name, intent, 0);
        return;
    }

    /* fallback: create new fragment for this mode */
    /* frag = new CGInstrumentPannelLargeXxxFragment(...); frag->setContext(ctx);
       frag->startNewFragmentUnrecovered(parent, 0, intent, 2); */
}

} // namespace NS_InstrumentPannelLarge
