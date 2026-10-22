# AutoExscreen remaining callbacks

The V21 original is ARM32. This file records verified call paths and
implementation limits; a strict link alone cannot establish replacement
behavior.

| Entry | RVA | Current state |
| --- | ---: | --- |
| OnSetExternalScreenCrossControlUi(int,bool) | 0x2bcdc | Restored UI config write and CrossControl observer dispatch |
| OnSetExternalScreenMapStatusUi(int,int,int,int) | 0x2c070 | Not implemented |
| OnSetExScreenMapStatusUi(tagSTExtScreenControl const&) | 0x2ace0 | Not implemented |

## Map status entry at 0x2c070

Arguments after `this` are map engine ID, requested level, requested mode,
and requested map position on the stack. It first calls
`HSL::GetMapViewHandle(mapEngineId)`; when null it leaves without changing
state. It looks up scene with screen type at `this+0xdc`, reads current
`AutoMultiScreenConfig` scene record, and copies exactly 0xe4 bytes at
0x17a6c. Configured level and view mode are at copied record offsets
+0x88 and +0x8c.

The level path checks 0x17ad0, and for requested level other than 99 calls
0x173fc (field 0 UI config) then 0x1e084 (instance SetMapLevel). For requested
mode other than 99 it calls 0x171b8 (field 1; preview scene 2 maps to scene 1),
then may invoke map-view virtual slot +0x3e8, conditionally start the timer
at `this+0xcc`, and call instance SetMapViewMode at 0x1def4. It finally
calls ResetMapBindingList at 0x1e790 using the scene ID. The current source's
Batch12 SetMapLevel is an incorrect static pseudo-implementation, and Batch13
ResetMapBindingList is a placeholder. These dependencies need repair before
installing the callback.

## Control record entry at 0x2ace0

The source withdrew a wrongly assigned body. The original is 3788 bytes,
accepts a 36-byte tagSTExtScreenControl record and drives several screen
state transitions. Its branches, concrete object ownership, and downstream
callbacks still require recovery.

The observer constructor at 0x29798 and registrations are also incomplete.
The CrossControl member implementation assumes its original object layout;
it has not been exercised on a constructed device observer.

The three scheduling entries at 0x2a5a8, 0x2a7e0 and 0x2aa28 bind the above
callbacks with captured objects of 24, 32 and 52 bytes respectively. The
source's previous zero-argument ActionA/B/C methods were invented. The
reconstructed GPI Callable and UIObject member import clear those incorrect
link errors in the ARM32 Linux probe. Its source token is currently zero,
and scheduling/cancellation has not been validated on Android.
