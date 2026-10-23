#include "../internal/NS_MultiScreenBase/AutoMultiScreenBindingState.h"
#include <cassert>
#include <cstdint>
#include <cstring>

using NS_MultiScreenBase::AutoMapBindingState;
using NS_MultiScreenBase::AutoExscreenUtil_ResolveMapBindingState;

static void put(unsigned char* scene, unsigned offset, int32_t value) {
    std::memcpy(scene + offset, &value, 4);
}

int main() {
    unsigned char scene[0xe4] = {};
    put(scene, 0x4c, 2);
    put(scene, 0x50, 0);
    put(scene, 0x88, 12);
    put(scene, 0x8c, 3);
    put(scene, 0x90, 5);
    scene[0x99] = 0;
    AutoMapBindingState original = {{7, 42, 42, 42, 42, 42, 42, 1}};

    AutoMapBindingState result = AutoExscreenUtil_ResolveMapBindingState(
        scene, -1, 1, 99, 0, original);
    assert(result.words[0] == 7 && result.words[7] == 1);
    assert(result.words[1] == 1 && result.words[2] == 12);
    assert(result.words[3] == 2 && result.words[4] == 2);
    assert(result.words[5] == 2 && result.words[6] == 0);

    result = AutoExscreenUtil_ResolveMapBindingState(scene, -1, 3, 0, 0, original);
    assert(result.words[1] == 3 && result.words[5] == 0);
    put(scene, 0x88, -1);
    result = AutoExscreenUtil_ResolveMapBindingState(scene, -1, 3, 99, 0, original);
    assert(result.words[1] == -1 && result.words[2] == 19);

    put(scene, 0x88, 0);
    put(scene, 0x8c, 0);
    put(scene, 0x4c, -1);
    put(scene, 0x50, 7);
    put(scene, 0x90, -1);
    scene[0x99] = 1;
    result = AutoExscreenUtil_ResolveMapBindingState(scene, 99, 99, 99, 0, original);
    assert(result.words[1] == 0 && result.words[2] == 0);
    assert(result.words[3] == 2 && result.words[4] == 1);
    assert(result.words[5] == 1 && result.words[6] == 1);
    result = AutoExscreenUtil_ResolveMapBindingState(scene, 0, 99, 99, 1, original);
    assert(result.words[2] == -1 && result.words[6] == 1);

    put(scene, 0x88, 2);
    put(scene, 0x8c, 3);
    result = AutoExscreenUtil_ResolveMapBindingState(scene, 99, 99, 99, 0, original);
    assert(result.words[2] == 3 && result.words[1] == -1);
}
