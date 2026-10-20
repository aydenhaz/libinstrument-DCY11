#pragma once
#include "Bundle.h"
namespace dice { class String16; }
namespace hsl { class Config { public: enum VALUE_STRATEGY { VALUE_STRATEGY_DEFAULT = 0 }; bool GetInt32(const unsigned short*, int&, VALUE_STRATEGY); bool GetInt32(const dice::String16&, int&, VALUE_STRATEGY); bool GetString(const dice::String16&, dice::String16&, VALUE_STRATEGY); bool GetBool(const dice::String16&, bool&, VALUE_STRATEGY); bool GetBundle(const dice::String16&, Bundle&); }; }
