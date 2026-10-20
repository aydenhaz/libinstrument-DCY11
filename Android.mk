LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE           := InstrumentPannelLarge
LOCAL_SRC_FILES        := src/InstrumentPannelLarge.cpp \
                          src/module_init.cpp
LOCAL_C_INCLUDES       := $(LOCAL_PATH)/include
# Original ABI: armeabi (EABI5, soft-float, Thumb). Requires NDK <= r16 for
# the plain armeabi target; with a newer NDK use:
#   APP_ABI := armeabi-v7a
#   LOCAL_CFLAGS += -mfloat-abi=soft -mthumb
# and make sure every other host library is rebuilt with the same float ABI.
LOCAL_CFLAGS           := -fPIC -mthumb -fvisibility=hidden -fvisibility-inlines-hidden
LOCAL_CPPFLAGS         := -std=c++11 -fno-rtti -fno-exceptions
LOCAL_LDLIBS           := -ldl -llog
# Host libraries the original links against (see IMPORTED_SYMBOLS.txt).
# Provide prebuilt modules or drop in the real shared libs:
LOCAL_SHARED_LIBRARIES := GFrame hsl Gbl GAdaptor GAdaptorInterface GPlatformInterface \
                          Common GSQuirrel GuideInfoCard
include $(BUILD_SHARED_LIBRARY)
