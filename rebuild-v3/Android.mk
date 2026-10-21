LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE           := InstrumentPannelLarge
LOCAL_SRC_FILES        := src/InstrumentPannelLarge.cpp \
                          src/module_init.cpp
LOCAL_C_INCLUDES       := $(LOCAL_PATH)/include
# Target ABI: armeabi-v7a (ARMv7-A, Thumb, softfp calling convention). The
# supplied V21 host DSOs are ELF32 ARMv7 with the EABI soft-float tag.
LOCAL_ARM_MODE         := thumb
LOCAL_CFLAGS           := -fPIC -march=armv7-a -mfloat-abi=softfp -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CPPFLAGS         := -std=c++11 -fno-rtti -fno-exceptions
LOCAL_LDLIBS           := -ldl -llog
# Host libraries the original links against (see IMPORTED_SYMBOLS.txt).
# Provide prebuilt modules or drop in the real shared libs:
LOCAL_SHARED_LIBRARIES := GFrame hsl Gbl GAdaptor GAdaptorInterface GPlatformInterface \
                          Common GSQuirrel GuideInfoCard
include $(BUILD_SHARED_LIBRARY)
