# Cover initialization pipeline and map registration

initializeConfiguredCover now composes configuration policy, image creation, optional resource-file load, screenshot-source binding and typed map-observer registration. It is an internal staged API with explicit results, not a falsely complete implementation of the original void setConfig.

The entrypoint takes SurfaceConfig by value, preserving the recovered nontrivial argument semantics. The internal prepareConfiguration helper now takes a const reference to avoid an unnecessary second by-value copy; it still performs the original member copy. Dimensions are captured once after policy reads and before external image destruction/creation, then reused for create and final buffer binding. This preserves the original snapshot across possible external callbacks.

Invalid inputs, disabled policy and failed image creation return before registration. Missing/failed optional resource files still proceed to binding and registration, as in the original. Missing image after resource handling is explicitly reported ImageUnavailable by the staged API rather than attempting a null dereference. The success status MapObserverRegistered claims only the completed map-observer registration.

Registration uses the real MapSurfaceViewObserver base pointer after pixel length, buffer, stride and dimensions have been installed. The optional flag at +0x14 is read AFTER registration because that external call may mutate object state. A set flag returns NeedsTopRenderRegistration: the original 20-byte closure/global storage and top-render callback installation are still required. No no-op handler is installed and no optional-completion success is claimed.

Native ASan/UBSan integration tests run the concrete class across policy, host image/file mocks, buffer binding and observer registration. Tests cover invalid/disabled/create-failed suppression, missing and failed optional file continuation, a synchronous map event during registration, and registration-time changes to the optional-render flag. The registered observer sees fully populated screenshot fields. Existing original Thumb arithmetic/state/config trace regressions remain passing. All 36 production units pass ARM Linux musl compile-only checks.

Actual Android configuration/image/map services are not validated by these mocks. Full setConfig wrapper, optional callable/global registration, logging, Packet host-base construction and module factory integration remain incomplete. No Android strict link or usable replacement SO.
