#pragma once
namespace GPI { class Closure; }
namespace bl { typedef int MapEngineID; }
namespace hsl {
class UIObject {
public:
    static void SetUITaskThread(int, int);
    static void RunAsyncDelayByEngineId(const GPI::Closure&, bl::MapEngineID, int);
};
}
