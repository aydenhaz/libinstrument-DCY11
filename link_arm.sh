#!/bin/sh
# Final ARM link for rebuilt libInstrumentPannelLarge.so — 441/441 symbols closed.
# Requires an Android NDK standalone toolchain (r16 armeabi, or any with soft-float).
set -e
TC=${NDK_TOOLCHAIN:?set NDK_TOOLCHAIN to your standalone toolchain dir}
CXX=$TC/bin/arm-linux-androideabi-g++
$CXX -shared -o libInstrumentPannelLarge.so \
  --sysroot=$TC/sysroot \
  -march=armv5te -mthumb -mfloat-abi=soft -O2 -fPIC \
  -I include -I stub_include \
  src/InstrumentPannelLarge.cpp src/module_init.cpp \
  -L host_libs -Wl,--no-undefined \
  -lGFrame -lhsl -lGbl -lGAdaptor -lGAdaptorInterface -lGPlatformInterface \
  -lCommon -lGSQuirrel -lbase_utils \
  -lc++_shared -lm -ldl -lc
echo "== expected: clean link, zero undefined symbols =="
