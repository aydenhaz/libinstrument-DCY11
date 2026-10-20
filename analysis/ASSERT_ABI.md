# I_Assert argument recovery

Original V21 instrument calls at 0x158f6, 0x3296c and 0x32a18 pass a normalized Boolean in r0. Each reads a Fragment byte at +0x214, then CLZ and LSR #5 produce 1 for zero and 0 otherwise. There is no string parameter. The C export cannot encode parameter types in its name; a symbol-name match alone did not catch the erroneous const char* declaration.

GblAlc.h now declares I_Assert(bool). Two invented context-string calls are removed from incomplete LargeScreenImpl flows. The original condition is NOT replaced with ctx != nullptr: it refers to Fragment state, and those flows do not yet have the verified receiver/layout. This is not restoration of the complete startup behavior.

The original libGPlatformInterface.so export is Thumb address 0x5a459, size 2, instruction BX LR. Its release implementation does nothing. The source still declares/imports the host function; no local no-op replacement is added.

Evidence: v21_verified/assert_package_15878.txt, fragment_start_327d0.txt and assert_fragment_32a00.txt. ARM signature probe emits I_Assert with a zero/one r0 argument. All-source compile-only remains ARM Linux musl, not Android NDK link or runtime validation.
