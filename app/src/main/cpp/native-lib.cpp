#include <jni.h>
#include <string>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <android/log.h>

#include "DarknetAPI.h"


#define  LOG_TAG    "Darknet"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif
/*
* Class:     wildsheep_darknet_DarknetUtils
* Method:    inference
* Signature: (Ljava/lang/String;)[Lwildsheep/darknet/Result;
*/
JNIEXPORT jobjectArray JNICALL Java_x_leon_DarknetDao_inference
        (JNIEnv *, jobject, jstring);

/*
 * Class:     wildsheep_darknet_DarknetUtils
 * Method:    load
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_x_leon_DarknetDao_load
        (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     wildsheep_darknet_DarknetUtils
 * Method:    unload
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_x_leon_DarknetDao_unload
        (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif

static DarknetAPI * api;
image readRawImageData(std::string image_path);
//image convertImage(int width, int height, byte[] elements);


/*
* Class:     wildsheep_darknet_DarknetUtils
* Method:    inference
* Signature: (Ljava/lang/String;)[Lwildsheep/darknet/Result;
*/
JNIEXPORT jobjectArray JNICALL Java_x_leon_DarknetDao_inference
        (JNIEnv * env, jobject, jstring image_name_j) {
    LOGI("JNI: Inference starting");

    std::string image_name  = env->GetStringUTFChars(image_name_j, (jboolean)false);
    image im = readRawImageData(image_name);
    //image im2 = copy_image(im);
    std::vector<Result> results = api->detect(im);
    LOGI("JNI: Detection results count: %d" , results.size());

    jclass c = (*env).FindClass("x/leon/Result");
    jmethodID mid = NULL;
    jsize size = results.size();
    LOGI("JNI: FindClass: %d" , (c==NULL));

    jobjectArray returnArr = (*env).NewObjectArray(size, c, NULL);
    if (NULL != c) {
        mid = (*env).GetMethodID(c, "<init>", "(IIIIFLjava/lang/String;)V");
        if (NULL != mid) {
            for (int i = 0; i < results.size(); ++i) {
                Result r = results[i];
                jint l = r.left;
                jint jr = r.right;
                jint t = r.top;
                jint b = r.bot;
                jstring jlabel = env->NewStringUTF(r.label.c_str());
                jfloat conf = r.confidence;

                jobject jObj = (*env).NewObject(c, mid, l, t, jr, b, conf, jlabel);
                (*env).SetObjectArrayElement(returnArr, i, jObj);
                LOGI("JNI: %s %d %d %d %d %f" ,r.label.c_str() ,  r.left , r.top , r.right , r.bot , r.confidence);
                //std::cout << "JNI: " << r.label << " " << r.left << " " << r.top << " " << r.right << " " << r.bot << " " << r.confidence << std::endl;
                //draw_box(im2, r.left, r.top, r.right, r.bot, 255, 0, 0);
            }
        }
    }
    //save_image_png(im2, "test");
    //free_image(im2);
    LOGI("JNI: Inference routine complete");
    return returnArr;
}

int readIntFromByteArr(char * buf, int offset, int len) {
    int sum = 0;

    for (int i = 0, shift = 0; i < len; ++i, shift+=8) {
        sum += int((unsigned char )buf[offset + i] << shift);
    }
    return sum;
}

const static int PixelFormat_AndroidAPI19_RGB_565 = 4;
const static int PixelFormat_AndroidAPI19_TRANSLUCENT = -3;
const static int PixelFormat_AndroidAPI19_RGBA_8888 = 1;
const static int PixelFormat_AndroidAPI19_RGBX_8888 = 2;
const static int PixelFormat_AndroidAPI19_RGB_888 = 3;


unsigned int *  bandmask(int pixelFormat) {
    unsigned int* temp;
    switch (pixelFormat) {
        case PixelFormat_AndroidAPI19_RGBA_8888:
            temp = new unsigned int[4];
            temp[0] = 0xFF000000;
            temp[1] = 0xFF0000;
            temp[2] = 0xFF00;
            temp[3] = 0xFF;
            return temp;
        case PixelFormat_AndroidAPI19_RGB_888:
            temp = new unsigned int[3];
            temp[0] = 0xFF0000;
            temp[1] = 0xFF00;
            temp[2] = 0xFF;
            return temp;
        case PixelFormat_AndroidAPI19_RGB_565:
            temp = new unsigned int[3];
            temp[0] = 0xF80000;
            temp[1] = 0x7E0;
            temp[2] = 0x1F;
            return temp;
    }
}

unsigned int * bitshift(int pixelFormat) {
    unsigned int* temp;
    switch (pixelFormat) {
        default:
        case PixelFormat_AndroidAPI19_RGBA_8888:
            temp = new unsigned int[4];
            temp[0] = 24;
            temp[1] = 16;
            temp[2] = 8;
            temp[3] = 0;
            return temp;
        case PixelFormat_AndroidAPI19_RGB_888:
            temp = new unsigned int[3];
            temp[0] = 16;
            temp[1] = 8;
            temp[2] = 0;
            return temp;
        case PixelFormat_AndroidAPI19_RGB_565:
            temp = new unsigned int[3];
            temp[0] = 19;
            temp[1] = 5;
            temp[2] = 0;
            return temp;
    }
}

image readRawImageData(std::string image_path) {
    std::ifstream infile;
    char buf[4];
    infile.open(image_path.c_str(), std::ios::binary);
    //infile.seekg(std::ifstream::end);
    //const size_t filesize = infile.tellg();
    //const size_t pixel_data_size = filesize - 12;
    infile.seekg(std::ifstream::beg);


    //infile.read(buf,4);
    int width = 768;//readIntFromByteArr(buf, 0, 4);
    //infile.read(buf, 4);
    int height= 576;//readIntFromByteArr(buf, 0, 4);
    //infile.read(buf, 4);
    int pixelFormat = 1;//readIntFromByteArr(buf, 0, 4);

    image im = make_image(width, height, 3);

    //"JNI: %s: Predicted in %f seconds.\n", std::string(input).c_str(), (elapsed_secs)
    LOGI("JNI: %d x %d x %d ,pixel: %d",im.c ,im.w ,im.h,pixelFormat);

    char * pixel;
    int pixel_size = -1;
    switch (pixelFormat) {
        default:
        case PixelFormat_AndroidAPI19_RGB_888:
            pixel = new char[3];
            pixel_size = 3;
            break;
        case PixelFormat_AndroidAPI19_RGBA_8888:
            pixel = new char[4];
            pixel_size = 4;
            break;
        case PixelFormat_AndroidAPI19_RGB_565:
            pixel = new char[2];
            pixel_size = 2;
            break;
    }


    unsigned int * bandmasks = bandmask(pixelFormat);
    unsigned int * bitshifts = bitshift(pixelFormat);
    int pixel_idx = 0;

    LOGI("JNI: pixelFormat: %d",pixelFormat);
    LOGI("JNI: bytes per pixel: %d",pixel_size);

    while (infile.peek() != EOF){
        infile.read(pixel, pixel_size);
        int value = readIntFromByteArr(pixel, 0, pixel_size);
        int r = (bandmasks[3] & value) >> bitshifts[3];
        int g =( bandmasks[2] & value) >> bitshifts[2];
        int b = (bandmasks[1] & value) >> bitshifts[1];
        int a = (bandmasks[0] & value) >> bitshifts[0];
        int x = pixel_idx % width;
        int y = (pixel_idx - x) / width;
        //std::cout << x << " " << y << std::endl;
        //break;
        //std::cout << r << " " << g << " " << b  << " " << a << std::endl;
        set_pixel(im, x, y, 0, ((float)r)/255 );
        set_pixel(im, x, y, 1, ((float)g)/255 );
        set_pixel(im, x, y, 2, ((float)b)/255 );
        ++pixel_idx;
    }
    LOGI("JNI: %f" ,get_pixel(im, 10, 10, 2));
    //std::cout << "JNI: " << get_pixel(im, 10, 10, 2) << std::endl;
    infile.close();

    //print_image(im);

    delete[] (bandmasks);
    delete[] (bitshifts);
    //delete(buf);
    delete[](pixel);
    return im;
}


/*
* Class:     wildsheep_darknet_DarknetUtils
* Method:    load
* Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z
*/
JNIEXPORT jboolean JNICALL Java_x_leon_DarknetDao_load
        (JNIEnv * env, jobject obj, jstring name_list_file_j, jstring cfgfile_j, jstring weightfile_j) {

    LOGI("JNI: Loading network and weights..");

    std::string name_list_file = env->GetStringUTFChars(name_list_file_j, (jboolean)false);
    std::string cfgfile = env->GetStringUTFChars(cfgfile_j, (jboolean)false);
    std::string weightfile = env->GetStringUTFChars(weightfile_j, (jboolean)false);

    api = new DarknetAPI(&name_list_file[0u], &cfgfile[0u], &weightfile[0u]);
    assert(api && "Failed to initialize DarknetAPI");
    jboolean b = 1;
    LOGI("JNI: Done.");
    return b;
}