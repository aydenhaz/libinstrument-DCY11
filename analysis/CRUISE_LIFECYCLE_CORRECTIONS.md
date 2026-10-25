# Cruise engine query and resume ownership — 2026-10-25

Original onCreateView 0x3ebc0 reads the 32-bit engine word at Fragment+0x308
and 0x3ebc4 calls HSL::GetMapViewHandle. This follows TestNumObserver
registration, so a registration callback can change the engine before the
read. Replaced the fabricated constant zero in both lifecycle source copies
with a fresh four-byte memcpy read at that point. No unaligned aliasing or
new host object layout is introduced.

Original code stores the returned map pointer at +0x3c0 and continues a
larger service/UI flow. That full flow is still not reconstructed by the
partial onCreateView; this correction only supplies the real query argument.
In particular the separate GetCarPosition branch is not the map query and
must not blindly receive the same argument: original 0x3ec52 passes 1 on
that conditional branch. Its existing incomplete path remains to recover.

The log signature used by 0x3f080 resolves via 0x3f0dc plus the literal at
0x3f310 to "virtual void NS_MultiScreenBase::AutoCruiseFragment::onResume()".
The function reads Fragment fields +0x3cc/+0x3c0, invokes AutoBaseFragment
resume 0x41abc, and registers further observer subobjects. It is not a
CGIPLPacket method (Packet allocation is only 0x16c). Removed the erroneous
Packet declaration and pseudo-body in both source copies rather than
leaving a misleading map-service call under the wrong lifecycle owner.
This is withdrawal of a wrong implementation, not complete Cruise resume.

Validation: original Thumb prefix emulation covers ALC on/off and engine
0,5,-1, with engine updated during mocked observer registration. All six
cases query the updated engine. The same test pins the original resume
signature. Native Packet lifecycle ASan/UBSan test passes (vptr and leak
sanitizers disabled for mocked host boundaries). NDK r26c compiles 52/52
and strictly links; development SO SHA256:
e3a69f77933559e154baa207764f340612d724003164eabcc3f503f67b976c17.
No Android device test or complete Fragment factory/registration success.
