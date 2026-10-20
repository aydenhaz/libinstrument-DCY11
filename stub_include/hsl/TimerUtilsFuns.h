#pragma once
#include <cstdint>
namespace hsl {
class TimerUtilsFuns {
public:
    static void InsertTimerById(int64_t);
    static uint64_t GetElapsedTimeById(int64_t, bool);
};
}
