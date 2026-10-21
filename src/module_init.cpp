#include "../internal/registration/PackageRegistration.h"
#include <stdexcept>
namespace IPL_ABI {
std::unique_ptr<GNS_FRAME::ClassNode_CGPackage> createPackageRegistration(GNS_FRAME::PackageFactory factory) {
    if(!factory) throw std::invalid_argument("Packet factory is required");
    const CGString name(reinterpret_cast<const unsigned short*>(u"CGIPLPacket"));
    const CGString alias(reinterpret_cast<const unsigned short*>(u"com.autonavi.package.GIPLPacket"));
    return std::unique_ptr<GNS_FRAME::ClassNode_CGPackage>(
        new GNS_FRAME::ClassNode_CGPackage(name,factory,alias));
}
}
// No automatic registration yet: Packet construction and the other original
// module initializer objects remain incomplete. The removed four constructors
// only registered empty destructors and provided no actual initialization.
