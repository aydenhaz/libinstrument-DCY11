#include <AbiTypes.h>
#pragma once
#include <GFrame/GPIClosure.h>
#include <memory>
namespace bl {  }
namespace hsl {
class UIObject {
public:
    static void SetUITaskThread(int, int);
    std::shared_ptr<void> RunAsyncDelayByEngineId(
        const GPI::Closure::Callable<void()>&, bl::MapEngineID,
        unsigned int delay, unsigned long long source);
};
}
