#pragma once
#include "CGIntent.h"
class CGString;
namespace GNS_FRAME {
class CGPackageMgr {
public:
    bool loadPackage(CGString &pkg, CGIntent &intent, bool, bool);
    bool unloadPackage(CGString &pkg, bool);
};
}
