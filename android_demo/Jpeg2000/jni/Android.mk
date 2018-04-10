# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)


################################################################################
#
################################################################################
include $(CLEAR_VARS)

LOCAL_MODULE := libsykeanjpeg2000

LOCAL_CFLAGS := -Wall #Werror
LOCAL_CFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_CFLAGS += -O3 -fPIC -ftree-vectorize -ftree-loop-optimize
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_CPPFLAGS += -O3 -fPIC -ftree-vectorize -ftree-loop-optimize

LOCAL_LDFLAGS += -Wl,--gc-sections
LOCAL_LDFLAGS += -fPIC -O3
LOCAL_LDFLAGS += -Wall
#LOCAL_LDLIBS := -latomic -lm -llog -ldl
LOCAL_LDLIBS := -lm -llog -ldl

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/sykean_include \
    $(LOCAL_PATH)/lib_jpeg_include

LOCAL_SRC_FILES += \
    lib_jpeg_src/bio.c \
    lib_jpeg_src/cio.c \
    lib_jpeg_src/dwt.c \
    lib_jpeg_src/event.c \
    lib_jpeg_src/function_list.c \
    lib_jpeg_src/image.c \
    lib_jpeg_src/invert.c \
    lib_jpeg_src/j2k.c \
    lib_jpeg_src/jp2.c \
    lib_jpeg_src/mct.c \
    lib_jpeg_src/mqc.c \
    lib_jpeg_src/openjpeg.c \
    lib_jpeg_src/opj_clock.c \
    lib_jpeg_src/opj_malloc.c \
    lib_jpeg_src/pi.c \
    lib_jpeg_src/raw.c \
    lib_jpeg_src/t1.c \
    lib_jpeg_src/t2.c \
    lib_jpeg_src/tcd.c \
    lib_jpeg_src/tgt.c \
    lib_jpeg_src/thread.c \
    sykean_jpeg/sykeanjpeg2000.c \
    sykean_jpeg/sykean_jpeg.c \
    sykeanjpeg2000api.c

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_SHARED_LIBRARY)


################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))
