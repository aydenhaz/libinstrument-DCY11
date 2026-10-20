#pragma once
class CGString;
class CGIntent;
namespace GNS_FRAME {
class CGExhibition {
public:
    void appendFragment(int, CGString&, const CGIntent&, void*);
};
}
