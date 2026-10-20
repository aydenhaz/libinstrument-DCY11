#pragma once
#include <cstdint>
namespace GNS_FRAME {
class CGIntent {
public:
    CGIntent();
    ~CGIntent();
    void putStringExtra(const unsigned short *key, const char *value);
    void putInt32Extra(const unsigned short *key, int32_t value);
    void putBoolExtra(const unsigned short *key, bool value);
};
}
