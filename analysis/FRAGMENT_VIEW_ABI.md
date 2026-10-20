# Fragment view receiver ABI

Three previously static declarations are instance methods, established by original libGFrame.so machine code:

| Method | Thumb symbol | Required receiver and arguments |
| --- | --- | --- |
| inflater(int, CGViewGroup*) | 0x296681 | r0 Fragment, r1 resource ID, r2 parent |
| findViewById(int) | 0x2963c7 | r0 Fragment, r1 view ID |
| isTopFragment() | 0x297d99 | r0 Fragment |

findViewById saves r0 as its receiver and reads its root at +0x180; isTopFragment checks its object at +0xb0, reads +0x18c and forwards this to the host stack. inflater saves r0/r1/r2 separately and reads receiver fields +0x214 and +0x258. A static declaration emits the same mangled name but shifts or omits these arguments, so a successful symbol resolution cannot validate the call.

The header now declares member functions. Receiver-less placeholder flows in Batch11/17/18/20 explicitly use undefined Unrecovered names rather than passing invented this pointers. GuideGuangQiA106Fragment in Batch18 declares CGFragment as its base and now forwards its implicit this to inflater and findViewById. Its resource IDs, widget wiring and base object construction remain unverified; this does not restore the complete onCreateView behavior.

The ARM signature probe tail-calls all three host symbols without shifting incoming r0/r1/r2. Original instructions and probe assembly are in v21_verified/fragment_{find_view,is_top,inflater_prefix,view_probe_arm} files. All 31 source units compile for ARM Linux musl. No Android link or usable replacement SO.
