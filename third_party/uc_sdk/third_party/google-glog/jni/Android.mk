LOCAL_PATH := $(call my-dir)

#
#googlelog
#
include $(CLEAR_VARS)

LOCAL_MODULE := glog

LOCAL_MODULE_FILENAME := libglog

LOCAL_SRC_FILES := \
../src/raw_logging.cc \
../src/vlog_is_on.cc \
../src/logging.cc \
../src/demangle.cc \
../src/signalhandler.cc \
../src/symbolize.cc \
../src/utilities.cc 

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../include

LOCAL_CFLAGS := -DUNICODE -DGLOG_USE_PTHREADS -DGLOG_HAVE_SYS_TYPES_H -DGLOG_HAVE_SYS_STAT_H -DGLOG_HAVE_STDLIB_H -DGLOG_HAVE_ERROR_H -D__i386__


LOCAL_CPPFLAGS += -fexceptions -frtti

LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)


