#pragma once
#include <memory>
#include <GFrame/ClassNode_CGPackage.h>
namespace IPL_ABI {
// Caller retains the node for the entire registered-factory lifetime.
// Only call after supplying a fully implemented Packet factory.
std::unique_ptr<GNS_FRAME::ClassNode_CGPackage> createPackageRegistration(GNS_FRAME::PackageFactory);
}
