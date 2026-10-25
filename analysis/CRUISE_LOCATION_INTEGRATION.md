# Cruise location branch integration — 2026-10-26

Restored the location branch in both copies of AutoCruiseFragment::onCreateView.
The MapView query result is now stored at +0x3c0 and passed to the recovered
location stage. A null map skips location queries. The rest of this view
lifecycle is still partial and the method comment now says so explicitly.

CruiseLocation.cpp implements original GetNaviType decision order: missing
guide/service -> 2; service slot +0xac nonzero -> 0; otherwise slot +0xc4
nonzero -> 1, else 2. Result 1 invokes GetCarPosition with engine 1. Other
results obtain Pos, construct LocInfo, call GetLocInfo, truncate the speed
float at +0x10 with original ARM saturation/NaN behavior, and convert/append
the count at +0x4b0 from records starting +0x180 with stride 0x110.

Malformed counts above three are rejected before reading beyond the three
inline records (a defensive difference from the original unchecked loop).
Entry padding is zeroed before the conversion helper so no uninitialized
bytes are passed across the host boundary. Missing Pos or empty results
skip map updates. Nonempty results call map slot +0x1a8 with the first
entry's two doubles, followed by integer arguments 0,0,0,1; then obtain
slot +0x2cc's layer and call its slot +8 with the complete CarLoc reference.
LocInfo and CarLoc owning storage remains live for the synchronous calls
and is released by scoped destruction, including exceptions.

The ARM-specific host adapter uses the recovered vtable offsets rather
than inventing incomplete C++ virtual class declarations. No new host ABI
symbols are fabricated. This path assumes the original valid non-null
layer contract, as does the V21 code at 0x3ee26.

Validation:
- Native ASan/UBSan stage test covers scene branches, missing map/guide/
  service/Pos, zero/one/two/three records, corrupt counts, call order,
  direct engine-1 retrieval, coordinates and speed edge cases.
- Original GetNaviType is executed in Unicorn for all 16 guide/service/mode
  combinations, checking both result and short-circuit call order.
- Original VFP instruction tests confirm truncation, saturation and NaN.
- cruise_map_abi_test.py executes original 0x3edec..0x3ee2e and the newly
  linked ARM map adapters, comparing coordinate registers, stack argument
  bytes, layer lookup and CarLoc pointer delivery.
- Existing original constructor/release, record conversion/2200 appends
  and Cruise observer/engine regressions pass.
- NDK r26c production overlay: 55/55 units compile; --no-undefined link PASS.
  Development SO SHA256:
  be00279c424f038857d9afce88c9938bc409dd401b2bcf94d1465730451c4077.

ASan ran with detect_leaks=0 due to the existing /proc task inspection
restriction. There is no device validation. This is only the location
subflow: base-view creation ordering, guide observer registration, cached
map interfaces (+0x3c4), remaining observers and cleanup/resume handling
still require restoration. Packet registration/full derived constructors
remain blockers to a usable replacement library.
