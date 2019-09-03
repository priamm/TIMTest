package com.example.timtest;

import android.graphics.Bitmap;

class QREncode {

    Bitmap createQRCodeBitmap(String content, int width) throws Exception {
        int origWidth = nativeInit(content);
        if (origWidth < 0) {
            throw new Exception("Can't get original QRCode width");
        }

        Bitmap origBitmap = Bitmap.createBitmap(origWidth, origWidth, Bitmap.Config.ARGB_8888);
        int ret = nativeCreateQRCodeBitmap(origBitmap);
        if (ret < 0) {
            throw new Exception("Can't get original QRCode bitmap");
        }
        Bitmap scaledBitmap = Bitmap.createScaledBitmap(origBitmap, width, width, false);
        nativeFinalize();

        return scaledBitmap;
    }

    private native int nativeInit(String content);
    private native int nativeCreateQRCodeBitmap(Bitmap bitmap);
    private native void nativeFinalize();

    static {
        System.loadLibrary("qrencode");
    }
}