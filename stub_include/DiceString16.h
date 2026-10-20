#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>
#include <utility>

namespace dice {
// V21 libhsl 0x13cca0 (UTF-16 ctor), 0x13ce82 (copy),
// 0x13cec4 (allocate), 0x13cd86 (destroy): one owning pointer;
// allocation = {uint32 length, uint32 capacity, UTF-16 payload, NUL}.
class String16 {
    unsigned char* block_;
    void assign_units(const char16_t* text, std::size_t length) {
        if (length > (0x7fffffffu - 15u) / 2u) throw std::bad_alloc();
        const std::size_t bytes = (length * 2u + 15u) & ~std::size_t(3u);
        unsigned char* next = new unsigned char[bytes];
        const uint32_t n = static_cast<uint32_t>(length);
        std::memcpy(next, &n, 4);
        std::memcpy(next + 4, &n, 4);
        if (length) std::memcpy(next + 8, text, length * 2u);
        next[8 + length * 2u] = next[9 + length * 2u] = 0;
        delete[] block_;
        block_ = next;
    }
    static std::u16string decode(const char* input) {
        std::u16string out;
        if (!input) return out;
        const unsigned char* s = reinterpret_cast<const unsigned char*>(input);
        for (std::size_t i = 0; s[i];) {
            const unsigned char first = s[i];
            uint32_t cp = 0; unsigned width = 0;
            if (first < 0x80) { cp = first; width = 1; }
            else if (first >= 0xc2 && first <= 0xdf) { cp = first & 0x1f; width = 2; }
            else if (first >= 0xe0 && first <= 0xef) { cp = first & 0x0f; width = 3; }
            else if (first >= 0xf0 && first <= 0xf4) { cp = first & 7; width = 4; }
            bool valid = width != 0;
            for (unsigned j = 1; valid && j < width; ++j) {
                if ((s[i+j] & 0xc0) != 0x80) valid = false;
                else cp = (cp << 6) | (s[i+j] & 0x3f);
            }
            if (valid && ((width == 2 && cp < 0x80) || (width == 3 && cp < 0x800) ||
                          (width == 4 && cp < 0x10000) || cp > 0x10ffff ||
                          (cp >= 0xd800 && cp <= 0xdfff))) valid = false;
            if (!valid) { out.push_back(0xfffd); ++i; continue; }
            if (cp <= 0xffff) out.push_back(static_cast<char16_t>(cp));
            else { cp -= 0x10000; out.push_back(static_cast<char16_t>(0xd800 + (cp >> 10)));
                   out.push_back(static_cast<char16_t>(0xdc00 + (cp & 0x3ff))); }
            i += width;
        }
        return out;
    }
public:
    String16() : block_(nullptr) { assign_units(nullptr, 0); }
    String16(const char* text) : block_(nullptr) {
        const std::u16string value = decode(text);
        assign_units(value.data(), value.size());
    }
    explicit String16(const unsigned short* text) : block_(nullptr) {
        std::size_t n = 0; if (text) while (text[n]) ++n;
        assign_units(reinterpret_cast<const char16_t*>(text), n);
    }
    String16(const String16& other) : block_(nullptr) {
        assign_units(reinterpret_cast<const char16_t*>(other.c_str()), other.size());
    }
    String16(String16&& other) noexcept : block_(other.block_) { other.block_ = nullptr; }
    String16& operator=(String16 other) { std::swap(block_, other.block_); return *this; }
    ~String16() { delete[] block_; }
    const unsigned short* c_str() const {
        return block_ ? reinterpret_cast<const unsigned short*>(block_ + 8) : nullptr;
    }
    uint32_t size() const { uint32_t n = 0; if (block_) std::memcpy(&n, block_, 4); return n; }
};
static_assert(sizeof(String16) == sizeof(void*), "V21 dice::String16 is one pointer");
}
