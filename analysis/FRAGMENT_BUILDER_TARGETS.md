# Post-load builder targets — 2026-10-25

Read from the original V21 ARM32 ELF, including PC-relative callback words.
Addresses below are Thumb code addresses with the low bit cleared.

| Registration suffix | Create callback | Allocation | Type callback | Marker returned | Builder launch mode |
| --- | ---: | ---: | ---: | ---: | ---: |
| largecuise | 0x305ac | 0x3dc | 0x305c0 | 0x30711 | 2 |
| largeguide | 0x302f0 | 0x8a0 | 0x30308 | 0x30459 | 2 |
| largepreview | 0x2fcc4 | 0x380 | 0x2fdd8 | 0x30111 | 0 |

Cruise tail-calls 0x3e608 after allocation. Guide tail-calls 0x4281c with
argument 1; that constructor argument is distinct from builder launch mode 2.
Preview calls AutoBaseFragment 0x41954 and then constructs its derived
interfaces and service fields inline. None can be replaced by a raw buffer.
The three marker addresses designate BX LR functions and serve as type
identities. They are not heap objects and must not be sent to the cleanup
callback as if they were Fragment instances.

All three builders receive cleanup 0x2fde4. It allows null, otherwise reads
the object's primary vptr and tail-dispatches slot 1 (deleting destructor).
It neither invokes slot 0 alone nor separately calls operator delete.

Added FragmentBuilderCallbacks.{h,cpp}: three local type-identity getters and
this cleanup behavior. The local keys use three distinct writable bytes to
avoid folding identical marker functions. These preserve stable identity
semantics, not the numeric addresses in the original DSO. Future rebuilt
Fragment getClassType implementations must reuse these getters; mixing
these keys with original-image class identity checks is not supported.

The helpers are not registered with GFrame. Factories remain incomplete,
and cleanup is only valid for genuinely constructed polymorphic Fragments,
not PendingOpeningHost storage. No new fake constructor or no-op factory
has been introduced.

Validation: original-ELF target/marker/allocation/mode/deletion regression
passes; native ASan/UBSan cleanup tests pass for null, primary base, and a
secondary base needing a deleting-destructor adjustment, with exactly one
destruction and deallocation. Previous pointer Callable regression passes.
LeakSanitizer is disabled (detect_leaks=0); deallocation counts are explicit.
The temporary environment was cleared and the repository was recovered
from GitHub. NDK was not recovered in this session; no fresh Android compile,
link or device test is claimed. Previous 51-unit build predates this new
translation unit and is not validation of this revision.
