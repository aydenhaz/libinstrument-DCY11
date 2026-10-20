#pragma once
namespace GPI {
class Closure {
public:
    template <typename T, typename M> static Closure bind(T*, M);
};
}
