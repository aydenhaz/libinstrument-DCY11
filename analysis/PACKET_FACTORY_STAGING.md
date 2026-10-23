# V21 Packet construction stages (2026-10-23)

The original factory at `0x2eb08` allocates `0x16c` bytes, invokes
`CGFragmentPackage(CGContext*)` C2, constructs `CoverUtil` at `+0xc0`,
clears the ARM32 proxy word at `+0x168`, installs four Packet vptrs and
publishes its singleton to `0x6fd48`. See
`rebuild-v3/analysis/v21_verified/packet_factory_runtime_table.md`.

`internal/abi/PacketFactoryStages.{h,cpp}` now implements only the
**unpublished** portion through the proxy clear. Its `PendingPacket` guard
owns the allocation and always destroys CoverUtil, then the GFrame host base,
then frees the allocation. If CoverUtil construction throws, it destroys the
completed host base; if the host constructor throws, it only frees the raw
allocation. No pointer is transferred to GFrame or the Packet singleton.

On ARM32, `internal/abi/PacketFactoryHostOps.cpp` supplies actual callback
bindings to the matching `libGFrame.so` C2/D2 symbols and the reconstructed
CoverUtil constructor/destructor. The host exports were verified by `nm -D`
and `tools/inspect_packet_factory.py --check-v21`. The callbacks are not
called by Startup or the registration helper. The host destructor is used
only on a completed base subobject. The local CoverUtil destructor is still
incomplete relative to V21, so this stage must not escape into a live Packet.

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
no singleton publication, reverse cleanup, and exception rollback. It passed
with ASan/UBSan. LeakSanitizer is disabled because this environment cannot
inspect `/proc` under the test runner.

This is a prerequisite for the live factory, **not** an operational factory
or a replacement SO. The derived Packet vptrs, RTTI, virtual callbacks,
Fragment construction, CoverUtil ownership, and module registration are
still absent. No Android NDK compile/link or device run was performed for
these new files.
