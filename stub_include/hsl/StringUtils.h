#pragma once
#include <DiceString16.h>
namespace hsl { class StringUtils { public: static dice::String16 Format(const unsigned short*, ...); static int FindStr(const dice::String16&, const dice::String16&, unsigned); }; }
