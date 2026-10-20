#pragma once
class CGString;
namespace GNS_FRAME {
class CGGFrameCrashLogger {
public:
    static void exportToAndroidLogCat(int, const char*, int, CGString&);
};
}
