/* CORRECTION: V21 has 5 init entries, including 0x14e41. The following 4-entry skeleton does not register the actual factories and is not replacement-ready. See BUILD_STATUS.md. */
/*
 * init_array / fini_array reconstruction.
 *
 * The original .init_array has 4 entries (@0x14cf9, 0x14d31, 0x14d69, 0x14d9d)
 * and .fini_array has 2 (@0x14ee0, 0x14ecc). Disassembly shows they:
 *   1. construct GNS_FRAME::ClassNode_CGPackage nodes for the module's
 *      fragments (CGInstrumentPannelLargeCruiseFragment, ...GuideFragment,
 *      ...GuideGuangQiA106Fragment, CGIPLMultiScreen / CGIPLNotify frames),
 *   2. register the module with ExModuleManager (ModuleReg) under the
 *      IMultiScreenInterfaces / IGuideInfoCardAdaptor interfaces,
 *   3. arm __cxa_atexit for each static object.
 * Fragment literal pools recovered from .rodata include:
 *   "CGInstrumentPannelLargeCruiseFragment::OnUpdateUiWidgets(int32_t)"
 *   "NS_MultiScreenBase::AutoCruiseFragment"
 *   "NS_MultiScreenBase::AutoExscreenNotifyObserver::AddMapRenderObserver(...)"
 *   "...OnTimerOpenDynamicLevel(GNS_FRAME::CGUITimer *)"
 * and log format strings:
 *   "...Type:%d, isCurrentDynamicLevel:%d"
 *   "ExtScreenUiStyle:%d, _extScreenProjectionSize:%d"
 *
 * The exact ClassNode factory tables live in the un-reconstructed internal
 * code (the binary carries ~1500 internal functions). This translation unit
 * preserves the constructor/destructor *order and count*; fill the node
 * factories when reimplementing the internal fragment classes.
 */

#include "InstrumentPannelLarge.h"

extern "C" int __cxa_atexit(void (*fn)(void *), void *arg, void *dso);

extern "C" void *__dso_handle;

namespace {

struct StaticFrame { void *node; };

StaticFrame g_frames[4];

void dtor0(void *p) { (void)p; }
void dtor1(void *p) { (void)p; }
void dtor2(void *p) { (void)p; }
void dtor3(void *p) { (void)p; }

/* init_array[0] — original @0x14cf9 */
__attribute__((constructor(101))) static void IPL_Ctor0(void) {
    __cxa_atexit(dtor0, &g_frames[0], &__dso_handle);
}
/* init_array[1] — original @0x14d31 */
__attribute__((constructor(102))) static void IPL_Ctor1(void) {
    __cxa_atexit(dtor1, &g_frames[1], &__dso_handle);
}
/* init_array[2] — original @0x14d69 */
__attribute__((constructor(103))) static void IPL_Ctor2(void) {
    __cxa_atexit(dtor2, &g_frames[2], &__dso_handle);
}
/* init_array[3] — original @0x14d9d */
__attribute__((constructor(104))) static void IPL_Ctor3(void) {
    __cxa_atexit(dtor3, &g_frames[3], &__dso_handle);
}

} /* anonymous namespace */
