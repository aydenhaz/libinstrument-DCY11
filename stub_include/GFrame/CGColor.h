#pragma once
// Stub of libGbl.so's CGColor (provider: libGbl.so, not uploaded).
class CGColor {
    alignas(4) unsigned char abi_storage_[8]; // V21 0x49914: vptr + RGBA bytes.
public:
    CGColor();
    CGColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    CGColor(unsigned int rgba);
    ~CGColor();
};

static_assert(sizeof(CGColor) == 8, "V21 ARM32 CGColor");
