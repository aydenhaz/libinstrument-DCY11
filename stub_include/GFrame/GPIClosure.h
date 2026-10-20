#pragma once
namespace GPI {
class Closure {
public:
    template<class Signature> class Callable;
    template <typename T, typename M> static Closure bind(T*, M);
};
}
