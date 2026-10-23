# Packet and module ALC call boundary (7.5 V21 ARM32)

The original logger calls do not pass a pointer to an eight-word block.
They pass `alc`, level `8` and reserved `0` in `r0`–`r2`; `r3` holds an
ambient value depending on how the indirect call is made. The fields are
ordinary stack arguments: `0x08000000`, zero, module name, empty auxiliary
name, signature, source line, format name, followed by any format values.
The old `AlcExport` passed the address of a local array in `r3` and omitted
the stack fields entirely.

| V21 site | Module | Line | Format and values | `r3` at call |
| --- | --- | ---: | --- | --- |
| `0x2ebe0` Packet `onInitEnd` | `HMI_PANNELLARGE` | `0x12e` | `onInitEnd` | Callback address (`blx r3`) |
| `0x2ec54` after `ModuleReg` | `HMI_COMMON` | `0x22` | `ModuleReg:%p`, module pointer | Empty auxiliary string (`blx lr`) |
| `0x2e00a` after `ModuleUnReg` | `HMI_COMMON` | `0x4b` | `ModuleUnReg:%p`, module pointer | `0x08000000` (`blx r8`) |
| `0x2e088` `QueryInterface` entry | `HMI_COMMON` | `0x9f` | `QueryInterface:%p`, `this` | Callback address |
| `0x2e0d0` after successful output | `HMI_COMMON` | `0xa6` | `QueryInterface:%p %p`, `this` and `this+4` | Callback address |

The second QueryInterface log runs only when the caller supplied output
storage. Original `0x2e08a..0x2e0d8` returns `-1` for a null output; for
non-null output it writes the secondary interface at `this+4`, logs the
result and returns zero. The old code logged both lines before the null
check and used `0x166` for the second line.

`internal/AlcExport.h` now emits stack arguments and allows these observed
`r3` choices. The module implementation uses `HMI_COMMON`, the corrected
lines and the original conditional order. `tests/large_screen_module_contract_test.cpp`
checks the logged fields and branch with a mock ALC receiver under
ASan/UBSan. `tools/inspect_packet_factory.py --check-v21` pins the line
immediates and QueryInterface branch bytes in the original ELF.

This repair concerns the log call boundary. The Packet factory and Opening
Fragment are still missing, so it does not make the rebuild installable.
