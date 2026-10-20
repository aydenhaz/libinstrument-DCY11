#include <GFrame/CGPackage.h>
GNS_FRAME::CGContext* packageContext(GNS_FRAME::CGPackage* package){ return package->getContext(); }
GNS_FRAME::CGPackage* packageActivity(GNS_FRAME::CGPackage* package){ return package->getHostFragmentActivity(); }
