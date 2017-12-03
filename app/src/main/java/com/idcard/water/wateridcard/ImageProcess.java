package com.idcard.water.wateridcard;

import android.graphics.Bitmap;

/**
 * Created by xiang on 2017/7/23.
 */

public class ImageProcess {

    static {
        System.loadLibrary("Imgprocess");
    }

    public static native void findIdNumber(Bitmap src, Bitmap out, Bitmap tpl);
    public static native Bitmap getIdNumber(Bitmap src, Bitmap tpl, Bitmap.Config config);
}
