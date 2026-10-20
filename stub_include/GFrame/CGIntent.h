#pragma once
namespace GNS_FRAME {
class CGIntent {
public:
    CGIntent();
    ~CGIntent();
    void putStringExtra(const unsigned short *key, const char *value);
};
}
