#pragma once
#include "CommonUtils.h"
namespace hsl {
class IUITaskThread;
class ThirdManager {
public:
    void SetUITaskThread(UI_TASK_DEVICE_ID, int, long long, bool, IUITaskThread*);
};
}
