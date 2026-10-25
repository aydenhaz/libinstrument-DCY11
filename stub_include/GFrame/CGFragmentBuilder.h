#pragma once
#include <GFrame/GPIPointerCallable.h>
namespace GNS_FRAME {
class CGFragment;
class CGContext;
// Host-owned implementation. ARM32 layout: vptr + two 20-byte Callables,
// cleanup function at +0x2c, context at +0x30, launch mode at +0x34.
class CGFragmentBuilder {
    unsigned char hostFields_[0x34];
public:
    CGFragmentBuilder(GPI::Closure::Callable<CGFragment*()>,
                      GPI::Closure::Callable<void*()>, void (*)(void*),
                      int launchMode, CGContext*);
    virtual ~CGFragmentBuilder();
    CGFragmentBuilder(const CGFragmentBuilder&) = delete;
    CGFragmentBuilder& operator=(const CGFragmentBuilder&) = delete;
    CGFragment* createFragment();
    void* getFragmentType();
    CGContext* getContext();
    int getLaunchMode();
    void releaseSelf();
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(CGFragmentBuilder) == 0x38, "V21 builder allocation extent");
static_assert(sizeof(GPI::Closure::Callable<CGFragment*()>) == 20,
              "V21 factory Callable extent");
#endif
}
