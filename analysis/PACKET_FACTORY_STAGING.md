# V21 Packet construction stages (2026-10-23)

The original factory at `0x2eb08` allocates `0x16c` bytes, invokes
`CGFragmentPackage(CGContext*)` C2, constructs `CoverUtil` at `+0xc0`,
clears the ARM32 proxy word at `+0x168`, installs four Packet vptrs and
publishes its singleton to `0x6fd48`. See
`rebuild-v3/analysis/v21_verified/packet_factory_runtime_table.md`.

`internal/abi/PacketFactoryStages.{h,cpp}` implements the **unpublished**
portion through the proxy clear and an optional derived-vptr stage. Its `PendingPacket` guard
owns the allocation and always destroys CoverUtil, then the GFrame host base,
then frees the allocation. If CoverUtil construction throws, it destroys the
completed host base; if the host constructor throws, it only frees the raw
allocation. No pointer is transferred to GFrame or the Packet singleton.
Four nonzero ARM32 address points must be supplied together; the guard
writes them in original order at `+0xc0`, `+0xc4`, `+0x40`, then `+0`.
The original store instructions at `0x2eb36..0x2eb42` are pinned by
`tools/inspect_packet_factory.py --check-v21`. The native test supplies
mock words, not real production RTTI/vtable pointers. Partial or repeated
installation fails before another store.

On ARM32, `internal/abi/PacketFactoryHostOps.cpp` supplies actual callback
bindings to the matching `libGFrame.so` C2/D2 symbols and the reconstructed
CoverUtil constructor/destructor. The host exports were verified by `nm -D`
and `tools/inspect_packet_factory.py --check-v21`. The callbacks are not
called by Startup or the registration helper. The host destructor is used
only on a completed base subobject. Original CoverUtil D1 at `0x17dfc`
destroys `SurfaceConfig::name` at `+0x44`, then `text_` at `+0x18`; default C++
member destruction provides that order. The original also logs, which the
local destructor does not reproduce. Image and observer release occur in the
lifecycle path. The host callback uses a qualified destructor call, which
produces a direct CoverUtil D1 reference instead of dispatching through a
potentially replaced virtual pointer. This guard must not escape into a live
Packet.

Host regression:

```sh
g++ -std=c++11 -O1 -g -fsanitize=address,undefined \
  -fno-omit-frame-pointer -Iinternal -Istub_include \
  internal/abi/PacketFactoryStages.cpp \
  internal/NS_InstrumentPannelLarge/CGIPLPacket.cpp \
  tests/packet_factory_stage_test.cpp -o /tmp/ipl-packet-stage-test
ASAN_OPTIONS=detect_leaks=0 /tmp/ipl-packet-stage-test
```

The test checks allocation extent, host/Cover order, four-byte proxy clear,
the four derived-vptr offsets, partial/repeated installation rejection,
no singleton publication, reverse cleanup, and exception rollback. It passed
with ASan/UBSan. LeakSanitizer is disabled because this environment cannot
inspect `/proc` under the test runner.

This is a prerequisite for the live factory, **not** an operational factory
or a replacement SO. The real derived Packet vptrs, RTTI, virtual callbacks,
Fragment construction, CoverUtil ownership, and module registration are
still absent. Installing four mock address points in a rollback guard does
not produce a live factory.

An ARM GNU compile of `PacketFactoryHostOps.cpp` confirms direct references
to the host C2/D2 and local CoverUtil C1/D1 symbols. That compiler's
hard-float GNU sysroot is not the Android NDK: a complete assembled
compile-only run reaches 47/48 units, with the remaining intentional
`sizeof(std::string)==12` assertion observing GNU's 24-byte string instead
of V21's NDK layout. A softfp full-source attempt stops even earlier because
the hard-float GNU sysroot has no `gnu/stubs-soft.h`. This is toolchain
evidence only, not Android link or runtime validation.
