# V21 LargeScreenImpl module interface boundary (2026-10-23)

The original ARM32 `onInitEnd` checks a module singleton at BSS `0x6fd4c`,
allocates **0x10** bytes only when empty (`0x2ebea`), writes two virtual
pointers, clears a halfword at `+8` and a word at `+0xc`, registers the
module with `ExModuleManager::ModuleReg(0x1f40, object)` at `0x2ec16`,
then publishes it at `0x2ec56`. This is distinct from the Packet singleton
at BSS `0x6fd48`. The former source's `legacy_.impl_` wrote into Packet
offset zero, which would overwrite its host package vptr.

| Evidence | Original V21 | Current assembled source |
| --- | --- | --- |
| Class extent | `new(0x10)` | ARM32 `sizeof(LargeScreenImpl)==0x10` |
| Primary table | `0x6b79c`, address point `0x6b7a4`, four slots | D1, D0, QueryInterface, doStartFragment |
| Secondary table | header `0x6b7b4`, address point `0x6b7bc`, offset-to-top `-4`, three slots | `-4` destructor pair and doStartFragment thunk |
| RTTI | `0x6b7c8`, public `IBaseModuleEx` at 0 and `IMultiScreenInterfaces` at `+4` | Both base RTTI entries and offsets `0x2` / `0x402` |
| Module ID | `ModuleReg` and destructor `ModuleUnReg` use `0x1f40` | Registration on construction, unregistration in D1 |
| QueryInterface | `0x2e090..0x2e0d8`: output is `this+4`, result zero; missing output returns `-1` | `IMultiScreenInterfaces*` output and matching results |

The source previously used `0x9f` for `ModuleReg`, confusing an ALC log
tag with the module ID, and used `0x1fa` for `ModuleUnReg`. It also had a
single-vptr class with an extra `vptr_` data field. The restored two-base
declaration now produces a 44-byte virtual table, a 32-byte RTTI object
and all three `-4` thunks in the Android NDK r26c linked SO. It restores
the original `IBaseModuleEx` and `IMultiScreenInterfaces` typeinfo/type-name
pairs, reducing missing visible original exports from 34 to 30.

`python3 tests/large_screen_abi_test.py --so /path/to/linked/libInstrumentPannelLarge.so`
compares the original and built tables, thunks and RTTI. The separate
native `tests/large_screen_module_contract_test.cpp` drives module
registration, both virtual dispatch paths, QueryInterface and deletion
through the secondary base with mock services under ASan/UBSan. The full
Android build still compiles 48/48 units and strictly links against nine
hosts after the change.

The Packet's `onInitEnd` now keeps this module in an independent static
slot and holds the third-party handle and context in local variables.
The invalid `legacy_` Packet fields and reads have since been removed.
Original `onInitEnd` allocates a separate `0x440`-byte Opening Fragment
at `0x2ecfc`; its base construction, observers and ownership remain to
be restored. The factory is not registered and this development SO remains
unsuitable for installation. See `analysis/OPENING_FRAGMENT_ONINIT_BOUNDARY.md`.

The original Packet `onDestroy` clears only the Packet singleton at `0x6fd48`.
Its final PC-relative store at `0x2f424` does not reach the independent module
slot at `0x6fd4c`. The rebuilt teardown accordingly leaves the module alone;
its eventual disposal path has not been established. The original-ELF check
now pins the two slots and the store bytes.

The module and Packet ALC calls now use the original ARM32 stack-field
calling convention. The module's logger category is `HMI_COMMON`, and the
successful QueryInterface result line is `0xa6`; see
`analysis/MODULE_ALC_ABI.md`. Native sanitizer tests cover the corrected
call order and output values.
