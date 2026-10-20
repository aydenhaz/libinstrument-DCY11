# InstrumentPannelLarge V21 ARM32 reconstruction

This repository contains a partial reconstruction, its supplied host libraries, and verified V21 ABI fixes. **It does not yet build a usable replacement library.** The archive's earlier claims of complete reconstruction and link closure are not build or runtime validation.

See [BUILD_STATUS.md](BUILD_STATUS.md) for current checks, exact remaining failures and evidence. The original instrument binary is retained under reference/v21 for reproducibility. Verified disassembly excerpts are in analysis/v21_verified.

## Build

```sh
sh host_libs/restore_libGbl.sh
python3 build_android.py --ndk /path/to/android-ndk-r27c
```

All internal and expanded translation units are compiled independently. Linking is strict; an unsuccessful build leaves no final .so.

## Host regression checks

```sh
g++ -std=c++11 -Iinclude -Istub_include src/InstrumentPannelLarge.cpp tests/entry_contract_test.cpp -o /tmp/ipl-entry-test
/tmp/ipl-entry-test
g++ -std=c++11 -fsanitize=address,undefined -Istub_include tests/dice_string16_test.cpp -o /tmp/ipl-dice-test
ASAN_OPTIONS=detect_leaks=0 /tmp/ipl-dice-test
```

These tests cover specific reconstructed contracts. They are not Android runtime acceptance tests.
