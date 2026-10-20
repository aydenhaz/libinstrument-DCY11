# Concrete top-render callback

CoverUtil::TopRenderCoverCallback is now implemented in the concrete observer class. It preserves original image/enabled gates, signed threshold comparison, ready-state draw gate, counter-10 capture condition, inhibit/requested flags, and wrapping increment. State is re-read after external draw/capture calls, including synchronous mutations. A false capture-service return is ignored as in the original.

CGCanvas vtable +0xcc is independently resolved from the host exported vtable to drawImgEx(CGImage*, const GPI::CGRect&, EGDrawImageType_, void*). The original call passes enum zero and style pointer at CoverUtil+0x90. CGRect RTTI proves a polymorphic CGRect with STGRect_ at +4; its four coordinates are populated as 0,width,0,height. The rebuilt class reproduces that ARM32 layout rather than using a plain 16-byte rectangle for drawing.

Screenshot requests use the device virtual slot at +0x128, with a plain dice::Rect<int32_t> (0,width,0,height), ScreenShotInfo reference at CoverUtil+0x74, and the business observer subobject at +4. Original host GetScreenShotInfo wrapper and log signature establish these parameter types and bool return. The screenshot state projection groups the known fields into a real typed subobject, retaining the same offsets; opaque fields/padding retain observed widths. It does not allocate screenshot memory or establish unverified ownership semantics.

Native ASan/UBSan tests call the concrete callback against explicit fake canvas/device vtables. They verify rectangle contents, enum/style argument, actual screenshot-state address, secondary observer pointer, request-flag ordering, ignored false result, threshold/equality and request guards, signed overflow wrapping, and synchronous draw/capture counter mutation. Existing class configuration/observer tests still pass. ARM32 layout probe and all 32 production source compile-only checks pass; prior original Thumb state/trace regression tests still pass.

Virtual dispatch helpers are deliberately limited to verified slots. They do not define a fabricated full CGCanvas/IMapView class vtable. No host drawing service is exercised by these native mocks, and current compilation remains ARM Linux musl, not Android NDK validation.

Remaining: setConfig/init paths that provide real image/device/buffer state, logging, top-render callback registration and Packet host-base construction/integration. No factory is enabled, no strict Android link succeeds, and no usable replacement SO is claimed.
