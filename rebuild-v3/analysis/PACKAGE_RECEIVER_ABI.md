# Package context/activity receiver ABI

Original libGFrame.so getContext at Thumb address 0x1b6bc9 consists of LDR r0,[r0,#0x5c]; BX LR. It requires a package pointer in r0. The previous static declaration omitted this and could read arbitrary memory.

getHostFragmentActivity at 0x1b71e5 likewise saves this, validates the subobject at +0x40, and reads the workstation at +0x68. Original instrument caller 0x158bc..0x158c8 passes the same package (r6) to both methods. It does not pass the returned activity as a package to getContext.

Both declarations are now instance methods. The legacy return type CGPackage* on getHostFragmentActivity remains unverified: this pointer-valued result represents an activity, and must not be used as a package receiver. The signature probe verifies only incoming r0 forwarding and pointer return ABI.

The incomplete Packet is not derived from a constructed CGPackage; LargeScreenImpl does not carry the original package receiver; CGFrameSimulation::getFragmentActivity is also not a verified substitute. Thus existing receiver-less calls explicitly reference undefined Unrecovered methods. No casts, new pointer guesses or local host getter implementations are used to bypass this.

Evidence is saved in v21_verified/package_context.txt, package_activity_prefix.txt, package_receiver_probe_arm.s, and assert_package_15878.txt. All 31 production translation units pass ARM Linux musl compile-only checking. Android link and complete package construction remain outstanding; no replacement SO.

## LargeScreenImpl shared package slot follow-up

Further V21 disassembly shows `sub_2e0f8` loads its `CGPackage*` receiver from
one shared position-relative BSS slot at RVA `0x6fd48` (read sites:
`0x2e20e`, `0x2e2fa`, `0x2e36e`, `0x2e3ea`). A separate path at `0x158aa`
reads the same slot. This confirms `LargeScreenImpl::this` is not the package
receiver, while also showing that the original obtains a valid shared receiver.
The code or lifecycle action that initializes the slot remains unrecovered.
The evidence is in `v21_verified/large_screen_package_slot.txt`.
