# Packet host base and activity receiver correction (2026-10-21)

Host libGFrame SHA256 45252a215427ba52fd0165417de517ee835d079a31bc8eaa8e68e04d841bde35.

Host RTTI: CGFragmentPackage inherits CGPackage. CGPackage inherits CGAsyncObject at 0 and CGObjMem at +0x40. Its extent in Packet is 0xc0, but this does not suffice to reproduce the base's fields or vtable. CGFragmentPackage context constructor 0x1b7610 calls CGPackage constructor 0x1b4434 and installs two vptrs. The package constructor initializes a reference-counted context at +0x64, CGString at +0x6c, CGIntent at +0x8c and other owned state; it cannot be replaced by zeroing memory.

Packet destructor's ARM veneer 0x456b4 resolves to PLT 0x145c8; original Cover destructor runs first. Factory and destructor remain disabled/incomplete in rebuilt Packet. No raw-host-base constructor was introduced at this checkpoint.

## Concrete corrected boundary

`CGFrameSimulation::getFragmentActivity` is an INSTANCE method. Original host entry 0x21a134 saves this, validates its CGObjMem subobject at +0x54, then reads cached CGFragmentActivity at +0x1fc. It may lazily allocate a 0x114-byte CGFragmentActivity if absent and permitted by +0x17c; failure paths abort. Header previously falsely declared a static getter returning CGPackage*. It now returns an opaque CGFragmentActivity* and requires a simulation receiver.

`CGPackage::getHostFragmentActivity` also returns CGFragmentActivity*, not CGPackage*. Host 0x1b71e4 checks its +0x40 CGObjMem, matches its workstation at +0x68 with ownWorkStation, obtains the simulation via workstation+0x138, and tail-calls its getFragmentActivity. CGFragmentActivity RTTI derives from CGActivity, not CGPackage. Do not reinterpret the returned activity as a package.

Packet onInitEnd 0x2ecc6 calls ownWorkStation then getFragmentActivity using the returned r0. It then calls CGPackage::getContext using the PACKET receiver (0x2ecd0), independently of the activity. The concrete call sequence in the partial source is corrected accordingly. Original activity vtable slots 12/25 and complete initialization remain unrecovered. Its legacy fields are still not valid Packet layout, so this change does not enable the factory.

CGPackage::getContext at 0x1b6bc8 reads +0x5c, NOT the context passed to constructor and stored at +0x64. These cannot be substituted. The unused receiver-less activity call in incomplete Batch9 doStartFragment is removed; it had no valid receiver or downstream use.

## Validation

A new Unicorn test executes host instructions for the +0x5c context getter, cached activity getter and package-to-activity chain, mocking object validation and workstation lookup. Lazy allocation and abort paths are NOT tested. ARM signature probe enforces the instance getter and correct pointer types. ARM Linux musl compile-only remains 39/39; no Android strict link or replacement SO. Native source runtime coverage is not claimed for the incomplete Packet constructor/onInitEnd.
