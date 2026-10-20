#pragma once
namespace bl {
class PathInfo {
public:
    bool GetSegmentInfo(unsigned, class SegmentInfo&) const;
};
class SegmentInfo {
public:
    SegmentInfo();
    ~SegmentInfo();
    bool IsValid() const;
    bool GetLinkInfo(unsigned, class LinkInfo&) const;
};
class LinkInfo {
public:
    LinkInfo();
    ~LinkInfo();
    bool IsValid() const;
    int GetLength() const;
};
}
