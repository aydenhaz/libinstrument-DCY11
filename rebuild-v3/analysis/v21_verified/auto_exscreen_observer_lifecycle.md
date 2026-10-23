# V21 AutoExscreenNotifyObserver construction and registration

The original ARM32 `libInstrumentPannelLarge.so` has a per-screen factory at
0x296a0. It selects one of three guarded static 0x128-byte observer instances,
passes `ScreenType` to 0x29798 on first use, and publishes that instance only
after construction. This is distinct from the 0x340-byte
`AutoMultiScreenConfig` singleton at 0x1c380.

## Constructor at 0x29798

The following offsets are observed directly in the constructor; the actual
host base classes, mutex wrappers and virtual table relationships still need
reconstruction before a constructor can be safely implemented.

| Object offset | Original operation |
| ---: | --- |
| +0x04 | Calls imported `hsl::ObserverBase::ObserverBase(char const*)` with `AutoExscreenNotifyObserver`. |
| +0x24, +0x48, +0x6c, +0x90 | Initializes four collection locks and their pointer, count, capacity and flag fields; stores collection vtable address points. |
| +0xb4 | Initializes a fifth lock or state holder before timer setup. |
| +0xcc | Stores a newly allocated 0x90-byte timer state; the timer object is constructed at state+0x10. |
| +0xd0, +0xd4 | Stores results of `GAI_GetExtScreenUIStyle` and `GAI_GetExtScreenUISize`. |
| +0xdc | Stores the constructor's `ScreenType`. |
| +0xe0 | Stores the engine mapped from the screen type by 0x16788; the long status callback compares against this word. |
| +0xe4 | Initializes to -1. |
| +0xe8 | Calls imported `hsl::AsyncObject::AsyncObject()`, then changes its virtual table. |
| +0x108 | Initializes a 16-byte `snprintf` label with the engine ID. |

The timer state receives the interval 0x1f40 at +4 and another word 0x118
at +0xc. The host `CGUITimerInstance` constructor receives the subobject
at state+0x10. The callback at 0x2c070 later reads observer+0xcc and uses
the timer-state guards before starting that subobject.

At 0x29a48, construction calls 0x2d264, which obtains the system notification
handle and registers the observer when available. The current source only
declares a padded C++ observer and an inline virtual destructor. It does not
run the original base constructors, initialize these collections or timer,
register the observer, or supply the guarded per-screen factory. Its methods
therefore have the right symbols for the ARM32 link probe but cannot yet be
assumed callable as live V21 observers. Restore the host-derived object and
factory before device validation of the map status callbacks.
