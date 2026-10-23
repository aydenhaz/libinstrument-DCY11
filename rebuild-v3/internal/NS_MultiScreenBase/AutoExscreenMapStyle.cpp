#include "AutoExscreenMapStyle.h"
#include <cstdint>
#include <cstring>

namespace NS_MultiScreenBase {
namespace {
int32_t readWord(const void* at) {
    int32_t value;
    std::memcpy(&value, at, sizeof(value));
    return value;
}
void writeWord(void* at, int32_t value) {
    std::memcpy(at, &value, sizeof(value));
}
void* virtualSlot(void* view, unsigned slot) {
    return (*reinterpret_cast<void***>(view))[slot / 4];
}
}

void AutoExscreenUtil_ApplyMapStyleWithViews(
    void* own, void* reference, int selector,
    const std::string& dayName, const std::string& nightName) {
    if (!own || !reference || (selector != 0 && selector != 1 && selector != 2))
        return;

    int styleTime = selector == 2 ? 0 : 1;
    const std::string* selected = selector == 2 ? &dayName : &nightName;
    if (selector == 0) {
        // sub_1e668 initializes the host MapStyleParam object. The first
        // 0x10 bytes are zero, +0x10 is one, +0x11..+0x50 are filled with
        // 0x81 and +0x50 is reset to zero. GetMapThemeStyle writes +4.
        alignas(4) unsigned char parameter[0x94] = {};
        parameter[0x10] = 1;
        std::memset(parameter + 0x11, 0x81, 0x40);
        parameter[0x50] = 0;
        typedef int (*GetDeviceId)(void*);
        typedef bool (*IsDeviceAttach)(void*, int);
        typedef void (*GetMapThemeStyle)(void*, void*);
        typedef int (*GetNightMode)(void*);
        const int device = reinterpret_cast<GetDeviceId>(
            virtualSlot(reference, 0xa8))(reference);
        if (reinterpret_cast<IsDeviceAttach>(
                virtualSlot(reference, 0x430))(reference, device)) {
            reinterpret_cast<GetMapThemeStyle>(
                virtualSlot(reference, 0x168))(reference, parameter);
        } else {
            writeWord(parameter + 4, reinterpret_cast<GetNightMode>(
                virtualSlot(reference, 0x16c))(reference) != 0);
        }
        styleTime = readWord(parameter + 4);
        selected = styleTime == 1 && !nightName.empty() ? &nightName : &dayName;
    }

    typedef void (*SetSpecifiedMapStyle)(void*, bool);
    typedef void (*SetMapStyleName)(void*, const char*);
    typedef void (*SetMapStyleTime)(void*, int, bool, bool, bool);
    const bool specified = !selected->empty();
    reinterpret_cast<SetSpecifiedMapStyle>(virtualSlot(own, 0x150))(
        own, specified);
    if (specified)
        reinterpret_cast<SetMapStyleName>(virtualSlot(own, 0x14c))(
            own, selected->c_str());
    reinterpret_cast<SetMapStyleTime>(virtualSlot(own, 0x158))(
        own, styleTime, false, false, false);
}

}
