#pragma once
// Stub of libGbl.so's CGString (demangled from libInstrumentPannelLarge imports;
// real provider is libGbl.so which was NOT among the uploaded libs).
class CGString {
    alignas(4) unsigned char abi_storage_[24]; // V21 ctor 0x51e5c writes through +0x17.
public:
    CGString();
    CGString(const unsigned short *utf16);
    CGString(const CGString &other);
    ~CGString();
    CGString &operator=(const unsigned short *utf16);
    CGString &operator=(const CGString &other);
    CGString &operator+=(const unsigned short *utf16);
    bool operator<(const CGString &other) const;
    void Format(const char *fmt, ...);
    void Format(const unsigned short *fmt, ...);
    static CGString &GetString();
};

static_assert(sizeof(CGString) == 24, "V21 ARM32 CGString ABI size");
