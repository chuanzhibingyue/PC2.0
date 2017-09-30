LOCAL_PATH := $(call my-dir)

#
#zlib
#

include $(CLEAR_VARS)
LOCAL_MODULE := z
LOCAL_MODULE_FILENAME := libz

LOCAL_SRC_FILES := \
../adler32.c \
../compress.c \
../crc32.c \
../deflate.c \
../gzclose.c \
../gzlib.c \
../gzread.c \
../gzwrite.c \
../infback.c \
../inffast.c \
../inflate.c \
../inftrees.c \
../trees.c \
../uncompr.c \
../zutil.c

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../

LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)