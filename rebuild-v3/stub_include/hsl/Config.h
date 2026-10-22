#pragma once
#include "Bundle.h"
#include <cstdint>
namespace dice { class String16; }
namespace hsl {
enum SetterType_ : int {};
class Config {
public:
    enum VALUE_STRATEGY { VALUE_STRATEGY_DEFAULT = 0 };
    bool GetInt32(const unsigned short*, int&, VALUE_STRATEGY);
    bool GetInt32(const dice::String16&, int&, VALUE_STRATEGY);
    bool GetString(const dice::String16&, dice::String16&, VALUE_STRATEGY);
    bool GetBool(const dice::String16&, bool&, VALUE_STRATEGY);
    bool GetBundle(const dice::String16&, Bundle&);
    bool SetBundle(const dice::String16&, const Bundle&, SetterType_);
    bool SetInt32(const dice::String16&, int32_t, SetterType_);
    bool SetInt32(const dice::String16& key, int32_t value, int setter) { return SetInt32(key, value, static_cast<SetterType_>(setter)); }
    bool SetBool(const dice::String16&, bool, SetterType_);
    bool SetBool(const dice::String16& key, bool value, int setter) { return SetBool(key, value, static_cast<SetterType_>(setter)); }
    bool SetString(const dice::String16&, const dice::String16&, SetterType_);
    bool SetString(const dice::String16& key, const dice::String16& value, int setter) { return SetString(key, value, static_cast<SetterType_>(setter)); }
};
}
