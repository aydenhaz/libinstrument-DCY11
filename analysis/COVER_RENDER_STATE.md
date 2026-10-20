# CoverUtil render-state decisions

Recovered helpers in internal/abi/CoverRenderState.h are prerequisites for the real CoverUtil implementation. They are not wired into Packet and do not construct a host observer or install any vtable.

Original isUIFirstRenderFinish at 0x18d9c returns false exactly when image (+0x9c) is non-null, enabled byte (+0x70) and state byte (+0xa0) are nonzero, and the signed counter (+0xa4) is <= signed threshold (+0x1c). All other paths return true. The signed BLE and equality case are preserved; naming alone is not a reliable guide to this predicate.

Original MapSurfaceViewStatusChange at 0x17d14 resets counter +0xa4 to zero only for configured surface (+0x3c), event 1, non-null image, and enabled byte. Other events and surfaces preserve the counter. Logging is outside the extracted helper.

Constructor evidence at 0x17cac establishes threshold=2, image=null, enabled=false, state=false, counter=0; nested SurfaceConfig begins +0x20 and its surface ID scalar[7] is at +0x3c. A dice::String16 object is constructed at +0x18. The original also clears exactly 18 bytes at +0x80, not an inferred larger block. Complete constructor, destructor, second observer signature, actual virtual tables and registration remain incomplete.

Validation:

- Native ASan/UBSan test covers all Boolean gates, signed extrema, equal threshold, and reset/non-reset branches.
- ARM Thumb compile-only succeeds for the helper test.
- Unicorn differential test executes the actual original V21 instructions and compares with the compiled C++ helpers: 392 predicate cases + 32 event cases = 424 matches. It verifies the original SO SHA256 before execution. The external I_GetAlcPtr call is mocked to return null, so enabled logging paths are not covered. It does not emulate Android, host service implementations or object construction.

Reproduce with pyelftools, unicorn and g++ available:

```sh
python3 tests/cover_original_differential_test.py /path/to/original/libInstrumentPannelLarge.so
```

Original instruction evidence is stored in v21_verified/cover_constructor.txt, cover_first_render_finish.txt and cover_surface_status.txt. No strict Android link or replacement SO is claimed.

## Top render callback transitions

Original 0x18898 checks signed counter <= threshold before drawing (only when state +0xa0 is set). With image and enabled gates satisfied, count==10 requests a capture only if inhibit byte +0x15 and requested byte +0x98 are both clear. It sets +0x98 before calling device vtable +0x128, passing the secondary business observer at CoverUtil+4. Every active paint increments counter, even beyond threshold. Increment is wrapping 32-bit ARM ADDS; the helper avoids C++ signed-overflow UB.

Draw and capture are external effects and can mutate state synchronously. The original reloads counter after draw before capture gating, and again after capture before increment. A caller must re-evaluate the decision helper after drawing and reload state after capture; computing all effects from one initial snapshot is incorrect.

Extended differential validation passes 576 paint cases plus two synchronous mutation cases (capture changes counter to 201, draw changes it from 2 to 10). Total with prior checks: 1,002 comparisons/cases. Canvas draw and device capture methods are intercepted; this validates decisions, observer adjustment, flag ordering and counter updates, not the real graphics/capture services. Full TopRenderCoverCallback is not yet wired into a real CoverUtil class.
