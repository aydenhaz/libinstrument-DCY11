# Cover capture result: RGB arithmetic and flags

Original callback at 0x17e9c processes event 0x17. It first clears requested (+0x98), sets state (+0xa0), then reads signed byte stride +0x7c, data pointer +0x88, byte length +0x8c and dimensions +0x80/+0x84. RGB sums are accumulated in single-precision floats from offsets 0/1/2 per pixel; alpha is skipped for stride 4. The divisor is the signed 32-bit product of dimensions, not the number of samples actually visited. Original VCVT.S32.F32 truncates each average toward zero before formatting.

CoverRgbAverage.h recovers arithmetic for explicitly supported, well-formed RGB/RGBA buffers: positive dimensions, positive stride 3/4, exact width*height*stride byte extent and signed-32-bit-safe pixel count. It rejects malformed inputs rather than attempting to reproduce original zero-divisor, negative stride, overflow or invalid-memory behavior. These checks are helper boundaries, not evidence of original callback validation. The flag helper processes only event 0x17.

Validation executes original Thumb/VFP arithmetic in Unicorn with verified original binary SHA256. Thirty combinations cover dimensions 1x1 through 31x17, RGB/RGBA, zero, white and deterministic random buffers. Execution stops before the first external formatter at 0x17f2e. Red is compared after original integer conversion; green/blue original single-precision sums and divisor are compared through equivalent float32 division/truncation. State bytes are checked before/after execution. Native ASan/UBSan and ARM Thumb compile-only checks also pass.

Remaining work: callback formal second-observer signature, three format/string/configuration operations after arithmetic, logging, real CoverUtil class construction/destruction, host service integration. No complete callback is installed, no claim is made that capture results are yet applied by the rebuilt module, and no replacement SO is produced.

Evidence: v21_verified/cover_business_callback.txt. Run tests/cover_rgb_original_differential_test.py with the original SO; it also reruns the earlier 1,002 cover-state cases.
