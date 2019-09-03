#define LOG_TAG "QREncode"

#include <string.h>
#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <malloc.h>
#include "libqrencode/qrencode.h"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

const int error = -1;

const int ok = 0;

QRcode *qrCode = NULL;

const int margin = 2;

const int bytesPerPixel = 4;

const unsigned char foregroundPixelColor[4] = { 0, 0, 0, 0xff };

JNIEXPORT jint JNICALL
Java_com_example_timtest_QREncode_nativeInit(JNIEnv *env, jobject instance,
                                                      jstring content_) {
    const char *content = (*env)->GetStringUTFChars(env, content_, 0);
    qrCode = QRcode_encodeString(content, 0, QR_ECLEVEL_H, QR_MODE_8, 1);
    (*env)->ReleaseStringUTFChars(env, content_, content);

    if (qrCode != NULL) {
        return qrCode->width + margin * 2;
    } else {
        LOGE("QRCode_encodeString failed!");
        return error;
    }
}

JNIEXPORT jint JNICALL
Java_com_example_timtest_QREncode_nativeCreateQRCodeBitmap(JNIEnv *env, jobject instance,
                                                                    jobject bitmap) {
    LOGE("gQRCode size: %d x %d", qrCode->width, qrCode->width);
    LOGE("gQRCode version is: %d", qrCode->version);
    int ret;
    void* pixels;
    AndroidBitmapInfo info;
    unsigned char* p = qrCode->data;

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("error: AndroidBitmap_lockPixels failed! error = %d", ret);
        return error;
    }

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("error: AndroidBitmap_getInfo failed! error = %d", ret);
        return error;
    }

    if (p == NULL) {
        LOGE("error: Get QRCode data failed!");
    }

    unsigned char *line = (unsigned char*) pixels;
    unsigned char* row = (unsigned char*)malloc(info.width * bytesPerPixel);

    memset(line, 0xff, info.width * info.height * bytesPerPixel);

    int x; int y;
    for (y = 0; y < qrCode->width; ++y) {
        memset(row, 0xff, info.width * bytesPerPixel);
        for (x = 0; x < qrCode->width; ++x) {
            if (*p & 1) {
                memcpy(&row[(x + margin) * bytesPerPixel], foregroundPixelColor, bytesPerPixel);
            }
            p++;
        }

        memcpy(&line[(y + margin) * info.width * bytesPerPixel], row, info.width * bytesPerPixel);
    }

    AndroidBitmap_unlockPixels(env, bitmap);
    free(row);

    return ok;
}

JNIEXPORT void JNICALL
Java_com_example_timtest_QREncode_nativeFinalize(JNIEnv *env,jobject instance) {
    QRcode_free(qrCode);
}
