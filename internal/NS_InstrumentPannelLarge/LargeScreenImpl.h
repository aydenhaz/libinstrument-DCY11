#pragma once
#include <cstdint>
enum MultiScreenFragmentIndex { MSI_PREVIEW=0, MSI_CRUISE=1, MSI_GUIDE=2 };
namespace GNS_FRAME { class CGIntent; class CGFragment; }
namespace NS_InstrumentPannelLarge {

class LargeScreenImpl {
public:
    LargeScreenImpl();
    virtual ~LargeScreenImpl();                                  // vtable[0]
    int32_t QueryInterface(int32_t id, void **out);              // vtable[2]
    void OnChangeScreenMode(int32_t screenType, int32_t mode,
                            const GNS_FRAME::CGIntent &intent);  // vtable[3] legacy name
    int32_t doStartFragment(int index, GNS_FRAME::CGFragment *,
                            const GNS_FRAME::CGIntent &);        // vtable[3] = sub_2e0f8
    bool InitProject();
private:
    void *vptr_;   // = 0x6b79c vtable
};

}
