LOCAL_PATH := $(call my-dir)

#
#common
#

include $(CLEAR_VARS)
LOCAL_MODULE := common
LOCAL_MODULE_FILENAME := libcommon

LOCAL_SRC_FILES := \
../src/dllmain.cpp \
../src/StringConvert.cpp \
../src/ThreadMessage.cpp \
../src/ZipWrapper.cpp \
../src/minizip/ioapi.c \
../src/minizip/mztools.c \
../src/minizip/unzip.c \
../src/minizip/zip.c \
../src/modp_base64/modp_b64.cc

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../include \
$(LOCAL_PATH)/../include/common \
$(LOCAL_PATH)/../../zlib-1.2.8 \
$(LOCAL_PATH)/../../boost_1_58_0 \
$(LOCAL_PATH)/../../boost_1_58_0/libs \
$(LOCAL_PATH)/../../boost_1_58_0\boost\system\detail \
$(NDK_ROOT)/platforms/android-9/arch-arm/usr/include/android


LOCAL_CPPFLAGS += -lstdc++ -fexceptions -std=c++11 -pthread
LOCAL_SHARED_LIBRARIES := libc libstdc++ libstlport
LOCAL_LDLIBS := -llog \
-landroid \
-L$(LOCAL_PATH)/../../zlib-1.2.8/libs/armeabi-v7a/ -lz \
-L$(LOCAL_PATH)/../../boost_1_58_0/libs/armeabi-v7a/ -lboost
include $(BUILD_SHARED_LIBRARY)
