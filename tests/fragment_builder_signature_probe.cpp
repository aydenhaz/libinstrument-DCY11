#include <GFrame/CGFragmentBuilder.h>
#include <GFrame/CGPackage.h>
GNS_FRAME::CGFragmentBuilder* makeBuilder(
    GPI::Closure::Callable<GNS_FRAME::CGFragment*()> create,
    GPI::Closure::Callable<void*()> type, void (*cleanup)(void*),
    int mode, GNS_FRAME::CGContext* context) {
    return new GNS_FRAME::CGFragmentBuilder(create, type, cleanup, mode, context);
}
void registerBuilder(GNS_FRAME::CGPackage* package, const CGString& name,
                     GNS_FRAME::CGFragmentBuilder* builder) {
    package->_registerFragment(name, builder);
}
void destroyBuilder(GNS_FRAME::CGFragmentBuilder* builder) {
    builder->GNS_FRAME::CGFragmentBuilder::~CGFragmentBuilder();
}
