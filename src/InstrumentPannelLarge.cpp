/*
 * Reconstruction of the three exported entry points of libInstrumentPannelLarge.so
 *
 * Verified facts (from disassembly):
 *   - All three are Thumb functions, compiled -O2 with stack protector.
 *   - StartupInstrumentPannelLarge @0x312c5, ExitInstrumentPannelLarge @0x313c9,
 *     LogInit @0x31471 (empty, single BX lr).
 *   - Startup returns 1 (MOVS r0,#1 at 0x31388), Exit returns void.
 *
 * Host types are forward-declared; swap in the real GFrame/hsl headers in the
 * product tree. See IMPORTED_SYMBOLS.txt for the full link-time contract.
 */
#include "InstrumentPannelLarge.h"

#include <cstring>

/* Host framework headers. In the product tree these resolve to the real
 * GFrame/hsl headers; here they resolve to the verified stubs in stub_include/.
 * CGString/CGColor/I_* live in libGbl.so; CGIntent/CGPackageMgr/CGFrameSimulation
 * in libGFrame.so; MultiScreenService in libhsl.so. */
#include "GFrame/CGString.h"
#include "GFrame/CGColor.h"
#include "GFrame/GblAlc.h"
#include "GFrame/CGFrameSimulation.h"
#include "GFrame/CGPackageMgr.h"
#include "hsl/MultiScreenService.h"

using GNS_FRAME::CGIntent;
using GNS_FRAME::CGPackageMgr;
using GNS_FRAME::CGFrameSimulation;

/* ------------------------------------------------------------------------
 * Recovered .rodata literals (exact offsets in the original binary)
 * ---------------------------------------------------------------------- */
static const char16_t kPackageName[] = u"InstrumentPannelLargePackage"; // @0x5caae (UTF-16LE)
static const char16_t kKeyFrom[]     = u"fromPackage";                  // UTF-16LE
static const char16_t kKeyType[]     = u"packageType";                  // UTF-16LE
static const char kFromPackage[] = "instrumentPannelLargePackage";            // @0x54e49
static const char kPackageType[] = "com.autonavi.package.GIPLPacket";         // @0x54e66

static const char kAlcModule[] = "HMI_PANNELLARGE";                           // @0x549d3

static const char kStartName[] = "StartupInstrumentPannelLarge";              // @0x54e2c
static const char kStartSig[]  = "bool StartupInstrumentPannelLarge(void *)"; // @0x54e02
static const char kExitName[]  = "ExitInstrumentPannelLarge";
static const char kExitSig[]   = "void ExitInstrumentPannelLarge(void *)";

/* ------------------------------------------------------------------------
 * ALC self-registration block.
 *
 * Original layout (Startup @0x312f6-0x3131a): after the second I_GetAlcPtr
 * call it loads fn = **(void***)alc, builds this 7-word block on the stack
 *   { 0, 0, kAlcModule, <aux>, 25, kStartSig, kStartName }
 * (25 = 0x19 in r12; 56 = 0x38 for Exit) and invokes
 *   fn(alc, 8, 0, &block)            // BLX r3 @0x3131a / BLX lr @0x3141c
 * The exact protocol is host-private; this models it faithfully.
 * ---------------------------------------------------------------------- */
typedef void (*AlcExportFn)(void *alc, int count, int reserved, const void *block);

struct AlcExportBlock {
    const void *zero0;      /* 0        */
    const void *zero1;      /* 0        */
    const char *module;     /* "HMI_PANNELLARGE"          */
    const char *aux;        /* secondary descriptor slot  */
    int         tag;        /* 25 (Startup) / 56 (Exit)   */
    const char *signature;  /* ALC export signature       */
    const char *name;       /* exported function name     */
};

static void AlcRegister(const char *name, const char *sig, int tag) {
    void *alc = I_GetAlcPtr();
    if (alc == 0) {
        return;                                   /* CBZ -> skip block */
    }
    alc = I_GetAlcPtr();                          /* second call, as in original */
    if (alc == 0) {
        return;
    }
    void **table = *reinterpret_cast<void ***>(alc);
    AlcExportFn fn = reinterpret_cast<AlcExportFn>(*table);

    AlcExportBlock block = { 0, 0, kAlcModule, 0, tag, sig, name };
    fn(alc, 8, 0, &block);
}

/* ------------------------------------------------------------------------
 * Entry points
 * ---------------------------------------------------------------------- */
extern "C" int StartupInstrumentPannelLarge(void * /*ctx*/) {
    AlcRegister(kStartName, kStartSig, 25);

    hsl::MultiScreenService *svc = hsl::MultiScreenService::GetInstance();
    if (svc != 0) {
        if (!svc->IsCreatedLargeScreen()) {
            svc->SetCreatedLargeScreen(true);
        }
    }

    GNS_FRAME::CGPackageMgr *pm = GNS_FRAME::CGFrameSimulation::getPackageMgr();
    CGString pkg(reinterpret_cast<const unsigned short*>(kPackageName));
    GNS_FRAME::CGIntent intent;
    intent.putStringExtra(reinterpret_cast<const unsigned short*>(kKeyFrom), kFromPackage);
    intent.putStringExtra(reinterpret_cast<const unsigned short*>(kKeyType), kPackageType);
    pm->loadPackage(pkg, intent, false, false);

    return 1;
}

extern "C" void ExitInstrumentPannelLarge(void * /*ctx*/) {
    AlcRegister(kExitName, kExitSig, 56);

    GNS_FRAME::CGPackageMgr *pm = GNS_FRAME::CGFrameSimulation::getPackageMgr();
    CGString pkg(reinterpret_cast<const unsigned short*>(kPackageName));
    pm->unloadPackage(pkg, false);
}

extern "C" void LogInit(void) {
    /* Original body: single BX lr. */
}

namespace BroadcastExtScreen {
int SwitchRouteMessage::getClassType() {
    return 0;   /* original was an empty stub */
}
}
