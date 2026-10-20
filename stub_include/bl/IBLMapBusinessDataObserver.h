#pragma once
namespace bl {
// V21 libGbl MapBusinessDataObserver bridge 0x973b94 forwards the dice
// callback's r1/r2/r3 unchanged to slot 2 of this interface. Destructor slots 0/1.
class IBLMapBusinessDataObserver {
public:
    virtual ~IBLMapBusinessDataObserver() = default;
    virtual bool onBusinessDataObserver(int, const void*, void*) = 0;
};
}
