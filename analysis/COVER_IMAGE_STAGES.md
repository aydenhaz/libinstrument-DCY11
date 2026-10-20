# Cover image creation and screenshot-source stages

Added two explicit stages to concrete CoverUtil; neither represents the complete original setConfig.

recreateImageStorage requires an enabled cover, destroys the previous host CGImage, clears its pointer, constructs a new 0x1e0-byte host image, and calls create with SurfaceConfig dimensions, color 0x101 and null initial data. A false create result destroys the new image and clears the pointer. On allocation/construction failure after old-image disposal, the stored pointer remains null. It does not fabricate file-load success, bind a buffer or register observers.

bindScreenshotSource runs after the caller has performed any resource-file load. It takes the final CGSize explicitly, sets the observed screenshot word at +0x78 to 5, stride +0x7c to 4, dimensions at +0x80/+0x84, then calls getDataLen before getData and stores the results at +0x8c/+0x88. It does not allocate or own that returned pixel memory. A missing image returns false without service calls. These early-return stage guards are helper contracts, not a claim that the original full method performs the same checks at these exact points.

A failed replacement preserves prior screenshot metadata, matching the original branch which exits before rebinding. The image pointer is null, so render callbacks remain gated off. CoverUtil destruction still follows the previously recovered string-state destructor rather than introducing unverified image ownership cleanup there; tests explicitly dispose their test images.

Native ASan/UBSan lifecycle tests verify initial disabled/missing-image guards, allocation/create order, successful buffer binding and getter order, dimensions independent of draw/config dimensions, replacement failure cleanup, null image state and subsequent successful recreation. Host image/config services are test doubles. All 34 production units pass ARM Linux musl compile-only checking, and new getter symbol references match supplied host exports. No Android NDK link/runtime validation.

Evidence: prior cover_config_18264.txt allocation/create branch and cover_config_tail_186b6.txt buffer-binding branch, plus image_data_length.txt and image_data_pointer.txt for host getter implementations.

Remaining: resource path resolution, optional file loading and its fallback, observer/top-render registration, full setConfig orchestration and Packet host-base construction. No factory enabled or usable replacement SO.
