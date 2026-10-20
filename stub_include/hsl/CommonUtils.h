#pragma once
namespace hsl {
enum UI_TASK_DEVICE_ID : int { UI_TASK_DEVICE_INSTRUMENT = 2 };
class CommonUtils {
public:
    static int GetEngineIdByDeviceId(UI_TASK_DEVICE_ID);
    static int GetEngineIdByDeviceId(int device) { return GetEngineIdByDeviceId(static_cast<UI_TASK_DEVICE_ID>(device)); }
};
}
