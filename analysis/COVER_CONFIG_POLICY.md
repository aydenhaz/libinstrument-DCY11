# Cover configuration prefix and policy

prepareConfiguration is an explicitly partial operation, not a renamed/replacement implementation of the original complete setConfig. It checks all three required context/device/view inputs before changing stored configuration, stores them at the recovered ARM32 offsets +8/+0xc/+0x10, copies SurfaceConfig by value into +0x20, then applies the original policy reads. A missing input returns InvalidInputs without service calls or object-state changes. Disabled means policy rejects this surface. NeedsImageSetup means policy enabled the surface but image/file/buffer setup is still required.

Original UTF16 literals:

| Address | Value |
| --- | --- |
| 0x5c45e | 0 |
| 0x5c462 | sm_enable_widgetCoverForWndId |
| 0x5c49e | 2 |
| 0x5c4a2 | sm_support_widgetCoverMaxFrame |
| 0x5c398 | %d |

The enabled-window string is reset to "0" before its GetString call. Maximum-frame temporary starts at "2". Both GetString return values are ignored, retaining defaults if the service does not update its output. Threshold is passed through host String16Utils::atoi without an added clamp. The surface ID at config scalar[7] is formatted as signed decimal and FindStr searches from offset zero; nonnegative match enables the cover. This is substring matching, so surface ID 1 can match configuration "10". The implementation deliberately preserves that behavior.

State is updated in the original order through references, before subsequent external calls: list defaults/read, threshold conversion, format/find, enabled flag. Context is now represented as a pointer rather than an unnamed integer word; ARM32 layout is unchanged.

Native ASan/UBSan tests cover default enabled surface 0, default-disabled surface 1, explicit substring match, unmodified negative threshold, copied surface ID and invalid-input service suppression. ARM32 layout probe passes and all 33 production units compile for ARM Linux musl. Mock service tests establish forwarding/ordering boundaries, not behavior of a running Android configuration service.

Full setConfig still requires image creation/loading, screenshot-buffer binding, observer registration and optional closure/top-render installation. Packet's host-base construction is also incomplete. No Android strict link or replacement SO.
