
LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := boost

LOCAL_MODULE_FILENAME := libboost

  
LOCAL_SRC_FILES += \
  ../libs/chrono/src/chrono.cpp \
  ../libs/chrono/src/process_cpu_clocks.cpp \
  ../libs/chrono/src/thread_clock.cpp \
  \
  ../libs/date_time/src/gregorian/date_generators.cpp \
  ../libs/date_time/src/gregorian/greg_month.cpp \
  ../libs/date_time/src/gregorian/greg_weekday.cpp \
  ../libs/date_time/src/gregorian/gregorian_types.cpp \
  ../libs/date_time/src/posix_time/posix_time_types.cpp \
  \
  ../libs/filesystem/src/path.cpp \
  ../libs/filesystem/src/path_traits.cpp \
  ../libs/filesystem/src/operations.cpp \
  ../libs/filesystem/src/codecvt_error_category.cpp \
  ../libs/filesystem/src/portability.cpp \
  ../libs/filesystem/src/utf8_codecvt_facet.cpp \
  \
  ../libs/regex/src/cpp_regex_traits.cpp \
  ../libs/regex/src/cregex.cpp \
  ../libs/regex/src/c_regex_traits.cpp \
  ../libs/regex/src/fileiter.cpp \
  ../libs/regex/src/icu.cpp \
  ../libs/regex/src/instances.cpp \
  ../libs/regex/src/internals.hpp \
  ../libs/regex/src/posix_api.cpp \
  ../libs/regex/src/regex.cpp \
  ../libs/regex/src/regex_debug.cpp \
  ../libs/regex/src/regex_raw_buffer.cpp \
  ../libs/regex/src/regex_traits_defaults.cpp \
  ../libs/regex/src/static_mutex.cpp \
  ../libs/regex/src/usinstances.cpp \
  ../libs/regex/src/w32_regex_traits.cpp \
  ../libs/regex/src/wc_regex_traits.cpp \
  ../libs/regex/src/wide_posix_api.cpp \
  ../libs/regex/src/winstances.cpp \
  \
  ../libs/system/src/error_code.cpp \
  \
  ../libs/thread/src/future.cpp \
  ../libs/thread/src/pthread/thread.cpp \
  ../libs/thread/src/pthread/once.cpp

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../

LOCAL_CFLAGS += -fPIC -frtti -fexceptions
LOCAL_SHARED_LIBRARIES := libstlport

include $(BUILD_SHARED_LIBRARY)