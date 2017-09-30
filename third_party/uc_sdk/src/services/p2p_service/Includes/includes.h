#pragma once

#include <stdio.h>
#include <string>

#define _TEST 1

#ifdef ANDROID
#define LOG_TAG "jni_log"
#include <android/log.h>
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#endif // ANDROID

#ifdef WIN32
#ifdef _DEBUG
#define LOGI(format, ...) printf(format"\n", ##__VA_ARGS__)
#else
// #define LOGI(format, ...)
#endif
#endif // WIN32

#include "UCSDK.h"
#include "UCTemplate.h"
#include "P2PErrorCode.h"