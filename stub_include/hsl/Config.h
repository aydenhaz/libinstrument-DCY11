#pragma once
#include "Bundle.h"
#include <cstdint>
namespace dice { class String16; }
namespace hsl {
class Config {
public:
    enum { VALUE_STRATEGY_DEFAULT = 0 };
    bool GetInt32(const unsigned short*, int&, int);
    bool GetInt32(const dice::String16&, int&, int);
    bool GetString(const dice::String16&, dice::String16&, int);
    bool GetBool(const dice::String16&, bool&, int);
    bool GetBundle(const dice::String16&, Bundle&);
    bool SetInt32(const dice::String16&, int32_t, int);
    bool SetBool(const dice::String16&, bool, int);
    bool SetString(const dice::String16&, const dice::String16&, int);
};
}
