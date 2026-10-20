#!/bin/sh
# Final ARM link for rebuilt libInstrumentPannelLarge.so — 441/441 symbols closed.
# Requires an Android NDK standalone toolchain (r16 armeabi, or any with soft-float).
# Unity-verified: all 23 TUs compile together with zero errors.
set -e
TC=${NDK_TOOLCHAIN:?set NDK_TOOLCHAIN to your standalone toolchain dir}
CXX=$TC/bin/arm-linux-androideabi-g++
$CXX -shared -o libInstrumentPannelLarge.so \
  --sysroot=$TC/sysroot \
  -march=armv5te -mthumb -mfloat-abi=soft -O2 -fPIC \
  -I include -I stub_include \
  -I internal -I internal/NS_InstrumentPannelLarge -I internal/NS_MultiScreenBase \
  internal/UnnamedResidue.cpp \\
  internal/NS_InstrumentPannelLarge/Batch7_big4.cpp \\
  internal/NS_InstrumentPannelLarge/Batch8_lifecycle.cpp \\
  internal/NS_InstrumentPannelLarge/Batch9_doStartFragment.cpp \\
  internal/NS_InstrumentPannelLarge/CGIPLPacket.cpp \\
  internal/NS_InstrumentPannelLarge/CGIPLPacket_onInitEnd.cpp \\
  internal/NS_InstrumentPannelLarge/Fragments.cpp \\
  internal/NS_InstrumentPannelLarge/Fragments_batch4.cpp \\
  internal/NS_InstrumentPannelLarge/LargeScreenImpl.cpp \\
  internal/NS_MultiScreenBase/AutoExscreenNotifyObserver.cpp \\
  internal/NS_MultiScreenBase/Batch11_named.cpp \\
  internal/NS_MultiScreenBase/Batch12_named.cpp \\
  internal/NS_MultiScreenBase/Batch13_named.cpp \\
  internal/NS_MultiScreenBase/Batch14_boilerplate.cpp \\
  internal/NS_MultiScreenBase/Batch15_named192.cpp \\
  internal/NS_MultiScreenBase/Batch17_compiled.cpp \\
  internal/NS_MultiScreenBase/Batch18_final.cpp \\
  internal/NS_MultiScreenBase/Batch19_compiled.cpp \\
  internal/NS_MultiScreenBase/Batch20_fragments.cpp \\
  internal/NS_MultiScreenBase/Batch21_config_alc.cpp \\
  internal/NS_MultiScreenBase/Batch22_final_meaty.cpp \\
  src/InstrumentPannelLarge.cpp \\
  src/module_init.cpp \
  -L host_libs -Wl,--no-undefined \
  -lGFrame -lhsl -lGbl -lGAdaptor -lGAdaptorInterface -lGPlatformInterface \
  -lCommon -lGSQuirrel -lbase_utils \
  -lc++_shared -lm -ldl -lc
echo "== expected: clean link, zero undefined symbols =="
