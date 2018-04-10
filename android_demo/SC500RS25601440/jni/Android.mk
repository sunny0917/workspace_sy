LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CFLAGS 	:= -fPIC -DANDROID -DHAVE_PTHREADS -O3  -mfloat-abi=softfp -mfpu=neon -ftree-vectorize -ffast-math
LOCAL_MODULE    := SyA83
LOCAL_SRC_FILES := A20Iris.c
LOCAL_SRC_FILES += A20V4l2.c
LOCAL_SRC_FILES += A20Gpio.c
LOCAL_SRC_FILES += imagejni.c

LOCAL_LDLIBS    := -lm -llog -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
