# Location buffer layout quarantine — 2026-10-25

The former hsl/HSL.h definitions were unsafe: bl::CarLoc contained two
 doubles (16 bytes) and bl::LocInfo was empty (1 byte). Cruise onCreateView
passed those stack objects into real host APIs even though their results
were unused by the reconstructed flow.

Matching V21 host evidence:
- Context::GetCarPosition at 0x40f418 reads CarLoc+0x10, already outside the
  old 16-byte allocation. It also uses the subobject at +8; a larger plain
  zeroed buffer is not proof of valid construction or ownership.
- PosData::GetLocInfo calls copy routine 0x38f518. That routine copies
  0x128 bytes, assigns an owning field at +0x128, then copies 0x560 bytes
  from +0x12c. Destination extent is at least 0x68c, not one byte.
- Pos::GetLocInfo logging reads destination +0x180/+0x188/+0x198.
- Original instrument routine 0x394ec constructs LocInfo before retrieval;
  it initializes many regions and nontrivial members. The previous
  ConfigBundleZeroInit implementation, memset(0x80), was misidentified.

Changed CarLoc/LocInfo to incomplete declarations, preserving the real
host function signatures while making fake stack/new/sizeof use fail at
compile time. Removed the unsafe unused calls from both Cruise lifecycle
copies, removed the duplicate receiver-less Batch11 helper (already absent
from the production overlay), and withdrew ConfigBundleZeroInit from both
trees. No fabricated default output, resized blob or no-op host API added.

This prevents the known invalid calls; it does not recover car-location
behavior. A future implementation needs exact layouts, construction/copy/
destruction of owning members and the original scene-dependent selection
between GetCarPosition and GetLocInfo. Do not re-enable by casting storage.

Validation: host ELF bounds/symbol checks and positive reference-signature /
negative construction tests pass under native g++ and NDK r26c ARM32.
Original cruise observer/engine regression (six cases) passes. Production
overlay compiles 52/52 and passes strict --no-undefined link. Development SO
SHA256: 34aebceb94a8ffbc01c3067e317c2ffe74c526773dacd379ebe0a24b74f13900.
No device runtime test; Packet factory/registration and complete Fragment
lifecycle remain missing.
