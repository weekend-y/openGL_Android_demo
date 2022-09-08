package com.example.opengl_scratchcard;

public class MyNativeRender {

    public static final int SAMPLE_TYPE  =  200;

    public static final int SAMPLE_TYPE_GUAGUALE                = SAMPLE_TYPE + 0;
    public static final int SAMPLE_TYPE_SHOUXIEBAN              = SAMPLE_TYPE + 1;

    static {
        System.loadLibrary("native-lib");
    }

    public native void native_Init();

    public native void native_UnInit();

    public native void native_SetImageData(int format, int width, int height, byte[] bytes);

    public native void native_UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    public native void native_SetParamsInt(int paramType, int value0, int value1);

    public native void native_SetParamsFloat(float value0, float value1);

    public native void native_OnSurfaceCreated();

    public native void native_OnSurfaceChanged(int width, int height);

    public native void native_OnDrawFrame();
}
