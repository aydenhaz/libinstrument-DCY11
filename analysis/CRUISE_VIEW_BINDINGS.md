# Cruise view host bindings — 2026-10-26

## Corrected lifecycle contract

Original AutoCruiseFragment::onCreateView and AutoBaseFragment::onCreateView
are void lifecycle methods. At 0x3ebb0, Cruise calls AutoBaseFragment 0x41a50
before registering TestNumObserver. That base wrapper only logs, then tails
through 0x45724 to host CGFragment::onCreateView (PLT 0x1440c). It does not
return a constructed root view. The former reconstructed trailing
inflater(0,parent) was an extra, unsupported layout operation.

Both source copies now declare the Cruise method void and call the real
host base before observer/map work. The CGFragment interface projection is
also corrected to void. The fake trailing inflater is removed. Original
AutoBaseFragment::onDestroyView 0x41b8c likewise only logs before invoking
host CGFragment::onDestroyView; the restored cleanup calls that host last.
Logging-only wrappers are not reproduced.

## Recovered registration and caches

The new ARM32 CruiseViewBindings implementation restores these operations:

| Phase | Original evidence | Behavior |
| --- | --- | --- |
| Begin | 0x3ebac..0x3ebbc | base create, HSL SystemNotify, add observer at Fragment+0x2e4 |
| Guide | 0x3ebcc..0x3ec20 | guide slot +0x20 yields service; service slot +0x18 adds Fragment+0x364 |
| Map cache | 0x3eed4..0x3eee6 | nonnull map at +0x3c0, slot +0x2b4 stored at +0x3c4 |
| Position | 0x3eeea..0x3eefc | GetPosHandle stored at +0x3c8; nonnull Pos slot +0x1c adds Fragment+0x368 |
| Config | 0x3eefe..0x3ef0c | GetConfigHandle stored at +0x3d0; nonnull AddConfigObserver gets Fragment+0x38c |
| Cleanup | 0x3f482..0x3f4c2 | remove test; cached Pos slot +0x20; cached Config remove; re-query Guide service slot +0x1c |

After base/test registration, the engine is still reloaded from +0x308.
Map acquisition/store precedes Guide attachment, which precedes the recovered
location stage. The map/Pos/Config tail follows that stage. Cleanup retains
the original cached Pos/Config handles and original non-clearing behavior.
Guide is re-queried during removal. A null Guide now skips service lookup;
the original creation path assumed nonnull Guide. Observer interfaces must
already be constructed with valid vptrs before any lifecycle call.

## Validation

`cruise_view_bindings_original_test.py --so <fresh ARM SO> --nm <llvm-nm>`
executes original ARM lifecycle slices and the freshly linked bindings.
Sixteen combinations of map/service/Pos/Config presence compare complete
call order, receiver/observer addresses and every byte of the 0x3dc-byte
Fragment fixture. Only the expected cache/engine fields may change. A
mocked synchronous TestNumObserver registration changes the engine to 7.

The test additionally executes the actual rebuilt Cruise onCreateView and
onDestroyView entry points, mocking the separately tested location stage,
and verifies the engine is freshly read and lifecycle stages occur in order.
External hosts/logging are mocked, so this is not a device lifecycle test.
Original-vs-rebuilt map ABI checks, original scene/VFP tests, constructor/
release tests and the six-case Cruise observer/engine regression still pass.
Native sanitizer tests retain the documented detect_leaks=0 limitation.

Official NDK r26c archive SHA256 was reverified. Production overlay:
56/56 units compile; --no-undefined link PASS. Development SO SHA256:
840ff5dc6728c1e8ecf9a99289a1e43d080b55e453b35aa536f17699c584a91e.

## Remaining work

This is a paired host-observer subset of the lifecycle. Broadcast Callable
registration/removal at 0x3ef10/0x3f43a and UI callbacks at +0x390/+0x394/
+0x398/+0x39c remain missing, as do scene transitions and complete resume
behavior. When recovered, their removal must be placed before the final
base destruction, in original order. Full derived construction/vtables and
Packet factory/registration still block a usable replacement. No device
validation or black-screen/crash fix is claimed.
