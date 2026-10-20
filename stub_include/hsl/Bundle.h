#pragma once
namespace dice { class String16 { public: String16(const char* = 0) {} const unsigned short* c_str() const { return 0; } }; }
namespace hsl { class Bundle { public: Bundle(); void PutBundle(const dice::String16&, const Bundle&); bool GetBundle(const dice::String16&, Bundle&); }; }
