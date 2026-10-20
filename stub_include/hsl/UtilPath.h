#pragma once
#include <DiceString16.h>
namespace hsl {
enum PathType_ : int;
class UtilPath {
public:
    // Return value is not consumed by the recovered caller.
    static void GetPathRelativeToRes(const dice::String16&,dice::String16&,PathType_);
};
}
