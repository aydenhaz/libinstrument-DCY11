# V21 external-screen message ABI

This checkpoint fixes types and calling conventions, not complete screen-mode
business logic or a runnable replacement library.

## Distinct global payload types

| Type | Recovered extent | Evidence |
| --- | --- | --- |
| ::tagSTExtScreen | 16 bytes, alignment 4 | SetStartOpenScreenMode copies four words by value |
| ::tagSTExtScreenControl | 36 bytes, alignment 4 | original caller initializes nine words at sp+0x44 through sp+0x64; next local starts +0x68; UpdateExScreenView reads +0x20 |

The previous namespace-local, packed 48-byte structure was invented. It is
replaced by the global types; legacy namespace usage names the global types
through using declarations. Unverified semantic field names are replaced by
word arrays. No mode/window/UI/flag interpretation is fabricated.

Control payload initialization observed in OnMapLevelChanged is one zero and
eight 99 sentinels. The host status protocol reads the first eight words; that
does not make the full caller-side type only 32 bytes. Remaining state population
in the rebuilt caller is still incomplete.

## Getter requires a hidden return buffer

Actual host GetStartOpenScreenMode(int), 0x47d568:

- r0: pointer to 16-byte return storage.
- r1: MultiScreenService this.
- r2: lookup key.
- Missing-key result is {0,0,0,-1}; found value copies four words.

The old int-return declaration omitted the hidden return argument and shifted
this/key into the wrong registers. A symbol-name-only check could not find
this error because C++ mangling omits this function's return type.

The return declaration now uses tagSTExtScreen. The ARM probe preserves the
incoming hidden buffer in r0, service in r1, and places key 5 in r2 before
calling the original host symbol.

## Setter passes the structure by value

Actual SetStartOpenScreenMode(int,tagSTExtScreen), 0x47d4b0:

- r0: this; r1: key; r2/r3: first two payload words.
- stack: final two payload words.

The old const-reference declaration was wrong. The ARM probe constructs
{11,22,33,44}; it emits 11/22 in r2/r3 and 33/44 on the stack with key 5 in r1,
matching the host's observed reads.

## Protocol declarations

ExScreenRenderActionHmiToAl takes ::tagSTExtScreen*.
MultiScreenStatusHmiToAl takes mutable ::tagSTExtScreenControl&.
These are separate payloads. ARM probe references for getter, setter and both
protocol calls all match defined symbols in the original libhsl.so, recorded
in v21_verified/external_screen_symbols.json.

## Verification and limits

Native ASan/UBSan test checks all four returned words, by-value isolation,
pointer/reference identity, the nine-word control initialization and mutable
control reference. ARM compile-only probes verify sizes, symbol references and
register/stack argument placement. All 31 source units still compile for ARM
Linux musl. No Android link or device runtime validation passed here.

Existing mode selection, configuration-to-payload population, Packet/Opening
Fragment construction and callbacks remain incomplete. Fixing these ABI types
must not be described as restoring the whole protocol or producing a usable SO.
