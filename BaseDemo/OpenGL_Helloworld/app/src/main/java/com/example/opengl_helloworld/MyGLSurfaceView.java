package com.example.opengl_helloworld;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MyGLSurfaceView extends GLSurfaceView {
    public MyGLSurfaceView(Context context) {
        super(context);
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        //1.自定义的 Render,在 Render中实现绘制
        MyGLRender myGlRender = new MyGLRender();
        //2.调用 GLSurfaceView的setRenderer方法
        setRenderer(myGlRender);
    }
}
