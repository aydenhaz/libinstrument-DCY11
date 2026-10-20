# RGB configuration side effects and formatter ABI

Original callback GOT relocation at 0x6e930 resolves hsl::StringUtils::Format(const unsigned short*, ...). It returns dice::String16 through hidden ARM32 r0 storage: r1 format, r2 first integer. libhsl implementation at 0x17a0b9 formats UTF16 into a temporary buffer then constructs the result at the saved r0 address. Existing void Format(const char*,...) declaration was wrong and is corrected.

GOT relocation 0x6e934 resolves CAutoUtil::setSMConfigValue(const dice::String16&, const dice::String16&). The callback passes key/value in r0/r1 with no instance receiver and ignores any return value. The binding declares void because no result is consumed; the original formal return type is not recovered from this caller. Original config key UTF16 literals are at 0x5c39e (widgetCoverR), 0x5c3b8 (widgetCoverG), 0x5c3d2 (widgetCoverB); the format %d is at 0x5c398.

writeCoverRgbConfiguration formats all three numeric values first, writes R/G/B with separate key temporaries, and destroys value temporaries in reverse construction order. It is a helper for future CoverUtil integration, not an installed observer callback.

Validation:

- Native ASan/UBSan mock-host test checks exact format/key/value call sequence.
- ARM Thumb signature probe confirms hidden result pointer + UTF16 format + integer argument and exact host symbol names.
- Original callback is executed through return in Unicorn; external formatter, String16 constructors/destructors and config service are intercepted. Its complete sequence matches format:0, format:127, format:255, widgetCoverR=0, widgetCoverG=127, widgetCoverB=255; temporary cleanup is complete. Three other event IDs return 1 without writes or flag changes. Host service behavior/logging is not validated by these mocks.
- All 31 production units pass ARM Linux musl compile-only checking after the header correction. Not Android NDK link validation.

Full CoverUtil interface signature, construction, vtables and Packet integration remain incomplete. No replacement SO.
