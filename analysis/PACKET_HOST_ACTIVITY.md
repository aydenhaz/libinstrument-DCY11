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

## Thread getter receiver audit (2026-10-21)

Three more falsely static declarations corrected to instance methods: getId, getRenderThreadExecutorId, getRenderThreadId. getId (host 0x216c16) validates this+0x54 and reads this+0x180. Executor getter (0x2187c0) reads adapter from this+0x218 and its executor at adapter+0x34. Render-thread getter (0x2182d4) reads the same adapter and tail-dispatches through veneer 0x301a68 to CGRenderThreadAdapter::getRenderThreadId (PLT 0x112de0). Adapter-null paths diagnose/abort; they do not yield a default ID. Return declarations retain existing int types; formal signedness is not established by these instruction traces. Packet explicitly converts the returned render-thread bits to uint32_t before widening to the third-manager 64-bit argument, matching original r3=0.

Packet original 0x2ec6e/0x2ec72, 0x2ec88/0x2ec8a, 0x2ec90/0x2ec92 and 0x2ecb8/0x2ecba call ownWorkStation before EACH getter. Partial rebuilt initialization now follows this receiver pattern. Batch11/12 already called ownWorkStation but discarded its return before getId; these are now one receiver-bearing call. No cache of a potentially changing workstation was introduced.

Original host-instruction regression now also covers all three getters with a nonnull adapter, including a high-bit thread ID to ensure all 32 bits are preserved. The adapter's external method is mocked and null-adapter abort branches remain untested. ARM compile-time member-function probes cover all three methods. Complete source compilation remains 39/39 for ARM Linux musl only; no Android strict-link or valid Packet construction. Other static declarations in this header are not automatically verified by this audit.

## Package manager and exhibition receiver audit (2026-10-21)

getPackageMgr and getExhibition were also incorrectly static. Host getPackageMgr at 0x21d090 reads simulation+0x17c then tail-dispatches via veneer 0x301ba8 to CGWorkStation::getPackageMgr (PLT 0x112f6c). The missing-workstation path diagnoses/aborts. Host getExhibition at 0x21a240 calls this->getFragmentActivity, reads this+0x200, writes the activity to exhibition+4 and returns exhibition. This getter has an observable update, not merely a pointer lookup. Both now require simulation receivers. LargeScreenImpl uses ownWorkStation()->getExhibition, matching original calls at 0x2e28a/0x2e28e and 0x2e66a/0x2e66e; the containing legacy business flow remains incomplete.

The entry-point fix is more specific: StartupInstrumentPannelLarge saves its incoming r0 in r9 at 0x312ce, then restores it to r0 at 0x31336 for getPackageMgr. Exit does the same at 0x313d2/0x3141e. Their argument is the simulation receiver, not an ignored context or a reason to fetch ownWorkStation. Source now casts the provided void* and passes it as this. Startup still returns early before manager lookup when the created flag is already set. No new null default was invented.

Validation: entry_contract_test under ASan/UBSan passes with distinct simulation instances for startup and exit, asserts exact getter receivers and manager query counts, and retains existing loading/early-return/ALC assertions. Original host instruction test validates manager receiver forwarding (workstation method intercepted) and actual exhibition activity update. ARM member-function signature probes and 39/39 ARM Linux musl compile-only pass. Null host branches, full Android linking, object construction and device startup remain unvalidated.
