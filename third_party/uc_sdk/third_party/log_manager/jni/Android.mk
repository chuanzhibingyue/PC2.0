LOCAL_PATH := $(call my-dir)

#
#common
#

include $(CLEAR_VARS)
LOCAL_MODULE := logmanager
LOCAL_MODULE_FILENAME := liblogmanager

LOCAL_SRC_FILES := \
../LogManager.cpp 

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../ \
$(LOCAL_PATH)/../../common/include \
$(LOCAL_PATH)/../../google-glog/include \
$(NDK_ROOT)/platforms/android-9/arch-arm/usr/include/android


LOCAL_CPPFLAGS += -lstdc++ -fexceptions -std=c++11
LOCAL_LDLIBS := -llog \
-L$(LOCAL_PATH)/../../common/libs/armeabi-v7a/ -lcommon \
-L$(LOCAL_PATH)/../../google-glog/libs/armeabi-v7a/ -lglog
include $(BUILD_SHARED_LIBRARY)