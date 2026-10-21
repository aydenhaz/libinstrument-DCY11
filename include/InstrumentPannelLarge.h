#ifndef LIBINSTRUMENTPANNELLARGE_H
#define LIBINSTRUMENTPANNELLARGE_H

/*
 * libInstrumentPannelLarge.so — interface-compatible reconstruction
 * Target ABI: ELF32 / ARM EABI5 / soft-float (armeabi) / Thumb
 *
 * Reverse-engineered from the stripped binary. All three exported entry
 * points and their exact call sequences verified by disassembly.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* @0x312c5 (Thumb). ALC export signature string recovered from .rodata:
 *   "bool StartupInstrumentPannelLarge(void *)"
 * Registers itself with the host ALC table, marks the large screen created
 * in hsl::MultiScreenService, then loads the "InstrumentPannelLargePackage"
 * UI package with two string extras (fromPackage / packageType).           */
bool StartupInstrumentPannelLarge(void *ctx);

/* @0x313c9 (Thumb). ALC export signature:
 *   "void ExitInstrumentPannelLarge(void *)"
 * Unregisters from the host ALC table and unloads the package.             */
void ExitInstrumentPannelLarge(void *ctx);

/* @0x31471 (Thumb). Body is a single `BX lr` in the original — no-op.      */
void LogInit(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* Exported weak symbol in the original:
 *   _ZN18BroadcastExtScreen18SwitchRouteMessage12getClassTypeEv
 * Original body at 0x3d8b0 is an empty stub (BX lr).                        */
namespace BroadcastExtScreen {
class SwitchRouteMessage {
public:
    static int getClassType();
};
}
#endif

#endif /* LIBINSTRUMENTPANNELLARGE_H */
