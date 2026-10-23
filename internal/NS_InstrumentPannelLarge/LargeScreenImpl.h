#pragma once
#include <cstdint>
enum MultiScreenFragmentIndex { MSI_PREVIEW=0, MSI_CRUISE=1, MSI_GUIDE=2 };
namespace GNS_FRAME { class CGIntent; class CGFragment; }
// Both interfaces are explicit bases in the V21 LargeScreenImpl RTTI.
// Their virtual destructors and shared doStartFragment slot yield the
// secondary -4 adjustor thunks in the original 0x6b79c table.
class IBaseModuleEx {
public:
    virtual ~IBaseModuleEx() = default;
    virtual int32_t QueryInterface(int32_t, void**) = 0;
    virtual int32_t doStartFragment(int, GNS_FRAME::CGFragment*,
                                    const GNS_FRAME::CGIntent&) = 0;
};
class IMultiScreenInterfaces {
public:
    virtual ~IMultiScreenInterfaces() = default;
    virtual int32_t doStartFragment(int, GNS_FRAME::CGFragment*,
                                    const GNS_FRAME::CGIntent&) = 0;
};
namespace NS_InstrumentPannelLarge {

class LargeScreenImpl : public IBaseModuleEx, public IMultiScreenInterfaces {
public:
    LargeScreenImpl();
    ~LargeScreenImpl() override;                                 // slots 0/1
    int32_t QueryInterface(int32_t id, void **out) override;     // primary slot 2
    int32_t doStartFragment(int index, GNS_FRAME::CGFragment *,
                            const GNS_FRAME::CGIntent &) override; // both slot 3/2
    // Provisional non-virtual business flow; not an extra original vtable slot.
    void OnChangeScreenMode(int32_t screenType, int32_t mode,
                            const GNS_FRAME::CGIntent &intent);
    bool InitProject();
private:
    uint16_t projectFlags_ = 0;
    uint16_t reserved_;
    void* runtime_ = nullptr;
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(LargeScreenImpl)==0x10,"V21 module allocation extent");
#endif

}
