package com.example.opengl_pbo;

public class MyNativeRender {

    static {
        System.loadLibrary("native-lib");
    }

    public native void native_Init();

    public native void native_UnInit();

    public native void native_OnSurfaceCreated();

    public native void native_OnSurfaceChanged(int width, int height);

    public native void native_SetImageData(int format, int width, int height, byte[] bytes);

    public native void native_OnDrawFrame();
}
