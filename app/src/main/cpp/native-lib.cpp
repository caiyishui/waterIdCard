#include <jni.h>
#include <string>
#include "common.h"
#define LOG_TAG "opencv"


#define DEFAULT_CARD_WIDTH 640
#define DEFAULT_CARD_HEIGHT 400
#define  FIX_IDCARD_SIZE Size(DEFAULT_CARD_WIDTH,DEFAULT_CARD_HEIGHT)
#define FIX_TEMPLATE_SIZE  Size(153, 28)




extern "C"
extern JNIEXPORT void JNICALL Java_org_opencv_android_Utils_nBitmapToMat2
        (JNIEnv *env, jclass, jobject bitmap, jlong m_addr, jboolean needUnPremultiplyAlpha);
extern "C"
extern JNIEXPORT void JNICALL Java_org_opencv_android_Utils_nMatToBitmap
        (JNIEnv *env, jclass, jlong m_addr, jobject bitmap);

jobject createBitmap(JNIEnv *env, Mat srcData, jobject config) {
    // Image Details
    int imgWidth = srcData.cols;
    int imgHeight = srcData.rows;
    int numPix = imgWidth * imgHeight;
    jclass bmpCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapMid = env->GetStaticMethodID(bmpCls, "createBitmap",
                                                       "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jobject jBmpObj = env->CallStaticObjectMethod(bmpCls, createBitmapMid, imgWidth, imgHeight,
                                                  config);
    Java_org_opencv_android_Utils_nMatToBitmap(env, 0, (jlong) &srcData, jBmpObj);
//    mat2Bitmap(env, srcData, jBmpObj);
    return jBmpObj;
}



extern "C"
JNIEXPORT jstring

JNICALL
Java_com_idcard_water_wateridcard_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_idcard_water_wateridcard_ImageProcess_findIdNumber(JNIEnv *env, jclass type, jobject src,
                                                            jobject out, jobject tpl) {

    // TODO

}extern "C"
JNIEXPORT jobject JNICALL
Java_com_idcard_water_wateridcard_ImageProcess_getIdNumber(JNIEnv *env, jclass type, jobject src,
                                                           jobject tpl, jobject config) {

    //原始图
    Mat img_src;
    //灰度图 需要拿去模版匹配
    Mat img_gray;
    //二值图 进行轮廓检测
    Mat img_threshold;
    // Gaussian 图片
    Mat img_gaussian;
    // Canny 图片
    Mat img_canny;
    //模版
    Mat img_tpl;
    //获得的身份证图
    Mat img_idCard;
    //获得的身份证号码图
    Mat img_idNumber;
    Java_org_opencv_android_Utils_nBitmapToMat2(env, type, src, (jlong) &img_src, 0);
    Java_org_opencv_android_Utils_nBitmapToMat2(env, type, tpl, (jlong) &img_tpl, 0);

    cvtColor(img_src, img_gray, COLOR_BGRA2GRAY);
    imwrite("sdcard/gray.png", img_gray);
    threshold(img_gray, img_threshold, 195, 255, THRESH_TRUNC);
    imwrite("sdcard/threshold.png", img_threshold);

    //高斯模糊让图片变的更加平滑
    GaussianBlur(img_threshold, img_gaussian, Size(3, 3), 0);
    imwrite("sdcard/gaussian.png", img_gaussian);

    //canny, 将边缘进行增强
    Canny(img_gaussian, img_canny, 180, 255);
    imwrite("sdcard/img_canny.png", img_canny);


    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    //轮廓检测 只检测外轮廓 并压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，比如矩形就是存储四个点
    // 第一个参数是原图，
    // 第二个是获取轮廓的点的保存向量，是找到的各个轮廓
    // 第三个是 层次结构，存放了轮廓同一等级的前后轮廓的索引，不同等级的父亲轮廓和孩子轮廓的索引
    // 第四个参数 mode :CV_RETR_EXTERNAL找到的轮廓里面没有小轮廓（洞），CV_RETR_LIST找到的轮廓中可以包括小轮廓
    // http://blog.csdn.net/corcplusplusorjava/article/details/20536251
    findContours(img_canny, contours, hierachy, RETR_LIST, CHAIN_APPROX_SIMPLE);
    int width = img_src.cols >> 1;
    int height = img_src.rows >> 1;

    vector<Rect> roiArea;
    Rect rectMin;
    for (int i = 0; i < contours.size(); i++) {
        vector<Point> v = (vector<Point> &&) contours.at(i);
        Rect rect = boundingRect(v);
        rectangle(img_threshold, rect, Scalar(255, 255, 255));
        if (rect.width >= width && rect.height >= height) {
            roiArea.push_back(rect);
        }
    }
    if (roiArea.size() > 0) {
        rectMin = roiArea.at(0);
        for (int i = 0; i < roiArea.size(); i++) {
            Rect temp = roiArea.at(i);
            if (temp.area() < rectMin.area()) {
                rectMin = temp;
            }
        }
//        rectangle(img_threshold, rectMin, Scalar(255, 255, 255));
    } else {
        rectMin = Rect(0, 0, img_gray.cols, img_gray.rows);
    }
    imwrite("sdcard/img_contours.png", img_threshold);

    img_idCard = img_gray(rectMin);
    imwrite("sdcard/img_idCard.png", img_idCard);

    resize(img_idCard, img_idCard, FIX_IDCARD_SIZE);
    resize(img_tpl, img_tpl, FIX_TEMPLATE_SIZE);
    cvtColor(img_tpl, img_tpl, COLOR_BGRA2GRAY);
    int cols = img_idCard.cols - img_tpl.cols + 1;
    int rows = img_idCard.rows - img_tpl.rows + 1;
    //创建输出图像，输出图像的宽度 = 被查找图像的宽度 - 模版图像的宽度 + 1
//    Mat match(rows, cols, CV_32F);
//        TM_SQDIFF 平方差匹配法
//        TM_CCORR 相关匹配法
//        TM_CCOEFF 相关系数匹配法
//        TM_SQDIFF_NORMED
//        TM_CCORR_NORMED
//        TM_CCOEFF_NORMED
    // 对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值代表更高的匹配结果. 而对于其他方法, 数值越大匹配越好
    Mat match;
    matchTemplate(img_idCard, img_tpl, match, TM_CCORR_NORMED);
    //归一化
    normalize(match, match, 0, 1, NORM_MINMAX, -1);
    Point maxLoc;
    minMaxLoc(match, 0, 0, 0, &maxLoc);
    //计算 [身份证(模版):号码区域]
    //号码区域:
    //x: 身份证(模版)的X+宽
    //y: 身份证(模版)Y
    //w: 全图宽-(身份证(模版)X+身份证(模版)宽) - n(给个大概值)
    //h: 身份证(模版)高
    Rect rect(maxLoc.x + img_tpl.cols + 10, maxLoc.y - 5,
              img_idCard.cols - (maxLoc.x + img_tpl.cols) - 50,
              img_tpl.rows + 15);
    //拿二值的号码
//    resize(img_threshold, img_threshold, FIX_IDCARD_SIZE);
//    img_idNumber = img_threshold(rect);
    img_idNumber = img_idCard(rect);
    imwrite("/sdcard/abc0.png", img_idNumber);//备注这个可以写文件到sdcard
    jobject result = createBitmap(env, img_idNumber, config);

    img_src.release();
    img_gray.release();
    img_threshold.release();
    img_idCard.release();
    img_idNumber.release();
    img_tpl.release();
    match.release();

    return result;

}