#pragma once
class CGString;
namespace GNS_FRAME {
class CGGFrameCrashLogger {
public:
    static void exportToAndroidLogCat(const char*, const char*, unsigned int, const CGString&);
};
}
