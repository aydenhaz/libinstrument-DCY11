#pragma once
namespace GPI {
class Scheduler {
public:
    static Scheduler *get(int engineId = 0);
};
}
