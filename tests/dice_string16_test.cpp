#include <DiceString16.h>
#include <cassert>
#include <cstring>
int main() {
    dice::String16 empty;
    assert(empty.size() == 0 && empty.c_str() && empty.c_str()[0] == 0);
    dice::String16 key("IP_MapViewMode");
    assert(key.size() == 14);
    const char16_t* expected = u"IP_MapViewMode";
    assert(std::memcmp(key.c_str(), expected, 30) == 0);
    dice::String16 copied(key);
    assert(copied.c_str() != key.c_str() && copied.size() == key.size());
    dice::String16 unicode("\xe4\xbb\xaa\xe8\xa1\xa8\xf0\x9f\x9a\x97");
    assert(unicode.size() == 4);
    assert(unicode.c_str()[0] == 0x4eea && unicode.c_str()[1] == 0x8868);
    assert(unicode.c_str()[2] == 0xd83d && unicode.c_str()[3] == 0xde97);
    dice::String16 truncated("\xf0");
    assert(truncated.size() == 1 && truncated.c_str()[0] == 0xfffd);
    dice::String16 assigned; assigned = copied;
    assert(assigned.size() == 14 && assigned.c_str() != copied.c_str());
    // Emulate the host replacing the owning block through a String16& output.
    dice::String16 host_output;
    unsigned char* block = new unsigned char[16]{};
    uint32_t length = 1;
    std::memcpy(block, &length, 4); std::memcpy(block + 4, &length, 4);
    block[8] = 'A';
    unsigned char* old;
    std::memcpy(&old, &host_output, sizeof(old)); delete[] old;
    std::memcpy(&host_output, &block, sizeof(block));
    assert(host_output.size() == 1 && host_output.c_str()[0] == 'A');
}
