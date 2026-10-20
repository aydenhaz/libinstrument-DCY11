#pragma once
#include <cstdint>
namespace NS_MultiScreenBase {
#pragma pack(push,1)
struct tagSTExtScreen {
    int32_t mode;
    int32_t screenType;
    int32_t extScreenUiStyle;
    int32_t projectionSize;
    int32_t flags;
    char reserved[28];
};
#pragma pack(pop)
}
