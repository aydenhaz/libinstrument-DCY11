#pragma once
class CGString;

namespace GNS_FRAME {
class CGIntent; class CGExclusionInfo;
class CGExhibition {
public:
    void appendFragment(int, const CGString&, const CGIntent&, CGExclusionInfo*);
};
}
