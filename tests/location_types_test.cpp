#include <bl/LocationTypes.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

int main(int argc, char** argv) {
    const unsigned char fill = argc > 1 ? std::strtoul(argv[1], nullptr, 0) : 0xa5;
    alignas(bl::LocInfo) unsigned char raw[sizeof(bl::LocInfo) + 16];
    std::memset(raw, fill, sizeof raw);
    auto* loc = new (raw) bl::LocInfo;
    assert(loc->text.size() == 0 && loc->text.c_str() && !loc->text.c_str()[0]);
    unsigned char logical[0x690];
    std::memset(logical, fill, sizeof logical);
    std::memcpy(logical, loc->prefix, sizeof loc->prefix);
    const uint32_t normalized_pointer = 0x190000;
    std::memcpy(logical + 0x128, &normalized_pointer, 4);
    std::memcpy(logical + 0x12c, loc->suffix, sizeof loc->suffix);
    std::fwrite(logical, 1, sizeof logical, stdout);
    loc->text = dice::String16("road");
    { bl::LocInfo copy(*loc);
      assert(copy.text.size() == 4 && copy.text.c_str() != loc->text.c_str());
      assert(!std::memcmp(copy.prefix, loc->prefix, sizeof loc->prefix));
      assert(!std::memcmp(copy.suffix, loc->suffix, sizeof loc->suffix));
      bl::LocInfo assigned;
      assigned = copy;
      assert(assigned.text.size() == 4 && assigned.text.c_str() != copy.text.c_str());
    }
    assert(loc->text.size() == 4);
    loc->~LocInfo();
    for (size_t i = sizeof(bl::LocInfo); i < sizeof raw; ++i) assert(raw[i] == fill);
    { bl::CarLoc car;
      assert(car.prefix[0] == 0 && car.prefix[1] == 0);
      assert(!car.locations.data && !car.locations.capacity && !car.locations.count);
      assert(car.locations.allocator.flags == 0x11);
      car.locations.data = car.locations.allocator.allocate(0x68);
      std::memset(car.locations.data, 0x55, 0x68);
      car.locations.count = car.locations.capacity = 1;
    }
    // Borrowed buffers must not be released by the array destructor.
    unsigned char borrowed[0x68] = {};
    { bl::CarLoc car;
      car.locations.data = borrowed;
      car.locations.allocator.flags &= ~0x10;
    }
}
