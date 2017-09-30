LOCAL_PATH := $(call my-dir)

#
#sqlite3
#

include $(CLEAR_VARS)
LOCAL_MODULE := sqlite3
LOCAL_MODULE_FILENAME := libsqlite3

LOCAL_SRC_FILES := \
../sqlite3.c

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../

LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)