# Cover release effects recovered from V21

Original SHA256: 6f5bd54c8e694ac40f02cd4280f6c6a049ea34896703e1eb5547fbc9e41d65e3.
Thumb entry 0x18078, return 0x18184. `releaseConfiguredCover` is a rebuilt descriptive name, not a verified original exported method name. Evidence: v21_verified/cover_release_18078.txt.

The original first requires image +0x9c and enabled +0x70. It clears screenshot bytes/length +0x88/+0x8c, assigns empty String16 +0x18, clears state +0xa0, device/view +0x0c/+0x10 and enabled. It preserves context, request flag, inhibit flag, counter, dimensions and other configuration. Three separate key/value lifetimes write widgetCoverR/G/B = UTF-16 "127". PC-relative string addresses are 0x5c39e, 0x5c3b8, 0x5c3d2 and 0x5c456.

After external configuration writes it rechecks image, removes the map observer, rereads optional flag +0x14 and, if set, clears the workstation callback BEFORE clearing private callback storage. Finally it rereads and deletes the image and stores null. No ownership check against the current global callback target exists in the original. The rebuilt implementation follows this ordering, using the already documented private std::function representation; no C++ closure crosses the host ABI.

Do not move this into the destructor: original destructor does not perform these effects. Do not clear request/inhibit/counter just because doing so looks cleaner. The early return when disabled can leave an image alive; this is original behavior, not a general-purpose cleanup guarantee. Original callers and Packet lifecycle integration remain to be recovered.

Validation: native integration under ASan/UBSan covers RGB reset order, observer removal, optional callback clearing, saved dispatcher after clearing, repeated calls, empty/disabled guards, and preserved fields. LeakSanitizer disabled for environment limitations. ARM Linux musl compile-only passes 38/38; this is not an Android NDK strict-link or device test. No usable replacement SO is produced.

## Caller and original-instruction validation (2026-10-21)

A Thumb BL/B.W scan of executable .text finds a direct call at 0x35bd0. It belongs to AutoOpeningFragment::onDestroyView (entry 0x35a64): load the CoverUtil pointer from Fragment+0x40c, skip if null, call 0x18078. This follows language observer removal and precedes map-view pointer clearing. It is not a direct Packet destructor call. Evidence is cover_release_caller_35bbe.txt. Indirect callers have not been excluded. The current Batch12 AutoOpeningFragment has no recovered storage/base layout and an empty onDestroyView; inserting a raw +0x40c access there would be invalid. This checkpoint deliberately does not enable that path.

New cover_release_original_trace_test.py executes the original 0x18078 with intercepted host services. Five cases verify image/enabled guards, optional callback teardown, configuration callback clearing the image, and every preserved byte of the 0xa8-byte object (String16 assignment itself is intercepted). The source native integration additionally tests image removal during configuration callbacks: subsequent observer/callback removal is skipped as in the original. This validates instruction/control-flow behavior, not Android host behavior.

The obsolete free function CoverUtil_setConfig in Batch11_named.cpp was moved to analysis/rejected. It used a wrong signature, failed to apply configuration and immediately destroyed a newly created empty image. It had no callers. The concrete CoverUtil::setConfig remains the implementation.
