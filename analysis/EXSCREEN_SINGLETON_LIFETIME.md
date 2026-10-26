# External-screen singleton lifetime recovery (2026-10-26)

`tests/exscreen_singleton_original_test.py` executes V21 getter `0x296a0`
under Unicorn. It intercepts construction, guard calls, and atexit, so this
is an original-code contract test, not a rebuilt singleton implementation
or a test of concurrent C++ guard behavior.

Three selector orders pass. Selectors 1 and 2 have distinct static objects.
All other selector values share one fallback object. The constructor receives
the first fallback selector unchanged (including its signed 32-bit bit
pattern); subsequent fallback values do not reconstruct or reconfigure it.
Each first access acquires its guard, constructs, registers the same destructor
and DSO identity with `__cxa_atexit`, then releases its guard. Repeated access
returns the same object without those calls.

The constructor's final call at `0x29a4a` enters `0x2d264`. That helper queries
`hsl::HSL::GetSystemNotifyHandle()` and, if present, tail-calls
`hsl::SystemNotify::AddExscreenNotifyObserver(hsl::ExscreenNotifyObserver*)`
through veneer `0x456a4` and PLT `0x14580`. It passes the primary object
address without an adjustment. When the service is absent it takes the
logging branch; the test covers the absent-logger subpath. Both service
cases pass by executing the original helper with host calls intercepted.

This means restoring allocation and fields alone is insufficient: primary
observer callback slots must be valid before the constructor's final host
registration, and guard publication must follow successful construction.
Do not replace the three guarded objects with a selector-indexed map, or
normalize the fallback constructor argument to zero.

Still unfinished: actual owner/base/lock/array/timer construction and matching
destruction, event callback recovery, and connection to the Cruise observer
bindings. Packet registration remains disabled pending its separate derived
virtual subobjects and Opening Fragment lifetime. This change adds evidence
and regression coverage only; it does not produce a newly repaired SO.
