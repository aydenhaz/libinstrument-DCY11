# Cover resource image load stage

loadConfiguredResourceImage reconstructs the resource branch after initial image creation. It resolves HmiResPath with path type 3 and directly appends sm_cover.png. It does not insert an extra directory separator, normalize the path, or ignore file existence.

If the file exists, the original sets inhibitCapture (+0x15) before testing/loading the image. It constructs CGString from the UTF16 path and calls CGImage::loadImageFromFile(path,-1,false,false,nullptr); the final pointer parameter is CGString*, not CGImage*. The return value updates state (+0xa0). A failed load leaves inhibitCapture set and does not pretend success or recreate another image. If the image pointer is missing after file discovery, the helper reports NoImage while retaining the original inhibit flag update.

If the file is missing, byte +0x91 is set to 1. Other prior inhibit/ready state is preserved; the original does not clear these flags in this branch. This byte remains an opaque screenshot/style state marker rather than being assigned a guessed semantic name. The helper reports MissingFile independently of a successful load. Its enum is a new internal stage result, not the original setConfig return type.

The host path resolver return type is not recovered/consumed by this caller; the binding consumes its output String16 and preserves the verified argument ABI. File-existence and image-load bool returns are consumed. Exact symbol references are checked against libhsl/libGFrame exports (cover_resource_symbols.txt).

Native ASan/UBSan tests cover resolved path/key/type, direct filename append, correct load arguments, temporary CGString scope, success/failure flags, missing-file state preservation and missing-image path. All 35 production units pass ARM Linux musl compile-only checks. Host file/image services are mocked; no real resource file was decoded by this test.

This stage still needs orchestration with prepareConfiguration, recreateImageStorage and bindScreenshotSource, followed by map-observer registration and optional top-render closure installation. Packet and host package construction remain incomplete. No Android strict link or replacement SO.
