LOCAL_PATH := $(call my-dir)

#
#thrift
#

include $(CLEAR_VARS)
LOCAL_MODULE := thrift
LOCAL_MODULE_FILENAME := libthrift

LOCAL_SRC_FILES := \
../include/thrift/protocol/TBase64Utils.cpp \
../include/thrift/protocol/TDebugProtocol.cpp \
../include/thrift/protocol/TDenseProtocol.cpp \
../include/thrift/transport/TBufferTransports.cpp \
../include/thrift/transport/TTransportException.cpp 

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../include/thrift \
$(LOCAL_PATH)/../include/thrift/protocol \
$(LOCAL_PATH)/../include/thrift/transport \
$(LOCAL_PATH)/../include \
$(LOCAL_PATH)/../../boost_1_58_0

LOCAL_CPPFLAGS += -fexceptions
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)