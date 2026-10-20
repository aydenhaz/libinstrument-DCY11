cmake_minimum_required(VERSION 3.10)
project(InstrumentPannelLarge CXX)

set(CMAKE_CXX_STANDARD 11)

# ABI note: the original is 32-bit ARM EABI soft-float (armeabi).
# With the NDK toolchain file use:
#   -DANDROID_ABI=armeabi-v7a -DCMAKE_ANDROID_ARM_MODE=ON
#   -DCMAKE_CXX_FLAGS="-mfloat-abi=soft -mthumb"
add_library(InstrumentPannelLarge SHARED
    src/InstrumentPannelLarge.cpp
    src/module_init.cpp)
target_include_directories(InstrumentPannelLarge PRIVATE include)
target_compile_options(InstrumentPannelLarge PRIVATE -fvisibility=hidden -mthumb)
set_target_properties(InstrumentPannelLarge PROPERTIES OUTPUT_NAME "InstrumentPannelLarge")
