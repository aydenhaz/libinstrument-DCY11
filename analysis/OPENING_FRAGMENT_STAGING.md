# Opening Fragment host and activity stages (V21 ARM32)

The original `CGIPLPacket::onInitEnd` calls two activity virtual functions
at byte offsets `0x30` and `0x64` on a constructed
`CGFragmentActivity`. It saves the second result, then tests the pointer at
activity `+8` before allocating its independent `0x440`-byte Opening
Fragment. `internal/abi/OpeningActivityGate.cpp` reproduces only this order
and check. It requires a constructed activity; it is not yet wired to the
partial Packet or to its fatal branch at `0x2f220`.

The Opening base constructor called at `0x41954` starts with imported
`GNS_FRAME::CGFragment` C2, launch mode 0. It invokes imported
`hsl::ObserverBase` C2 on Fragment `+0x2e8` with the **inline** name
`AutoBaseFragment`; its host constructor copies up to 31 bytes and appends
a NUL in the 32-byte field. It then writes engine ID 5 at `+0x308`. The
`HMI_OPENING` tag belongs to the higher-level Opening constructor and is
formatted into the later per-screen label. These are distinct strings.

`internal/abi/OpeningFragmentStages.{h,cpp}` and
`OpeningFragmentHostOps.cpp` reconstruct that first host prefix using the
matching V21 `CGFragment` C2/D2 and `ObserverBase` C2 symbols. The guard
always destroys the host base and frees the allocation, including on a
failed observer constructor. The observer's inline name has no exported D2
in the supplied `libhsl.so`. No activity or GFrame owner receives the
temporary buffer.

`python3 tools/inspect_packet_factory.py --check-v21` pins the activity
gate and base constructor Thumb bytes, the observer name/offset, engine
write, the host D2 symbol and the observer C2 body against the supplied
V21 ELFs. `tests/opening_fragment_stage_test.cpp` and
`tests/opening_activity_gate_test.cpp` check construction, rollback, and
virtual-call order with native ASan/UBSan mocks. These tests establish
staging order, not a complete derived Fragment.

Still needed: derived Opening Fragment and `AutoOpeningFragment` virtual
tables and observer subobjects, embedded `ObjectRuntimeInfo` at `+0x384`,
surface configuration at `+0x3a0`, valid timer and Activity ownership,
CoverUtil binding at Fragment `+0x40c`, and a fully constructed Packet
factory and registration. Until then, the assembled SO cannot replace
the original on a device.
