#pragma once
namespace bl { class PathInfo; }
namespace hsl {
class RouteResultData {
public:
    RouteResultData();
    ~RouteResultData();
    bl::PathInfo *GetCurPath() const;
};
}
