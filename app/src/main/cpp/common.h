//
// Created by liuxiang on 2017/7/23.
//

#ifndef IDCARD_COMMON_H
#define IDCARD_COMMON_H

#include <jni.h>

#include <android/bitmap.h>

#include <android/log.h>

#include <opencv/cv.hpp>
using namespace cv;
using namespace std;



#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))


//#include "utils.h"

#endif //IDCARD_COMMON_H
