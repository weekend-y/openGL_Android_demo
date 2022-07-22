package com.example.opengl_yuvrender;

import android.Manifest;
import android.content.pm.PackageManager;

import android.hardware.SensorManager;
import androidx.annotation.*;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import android.os.Bundle;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.RelativeLayout;
import android.widget.Toast;
import java.io.IOException;
import java.io.InputStream;


import static com.example.opengl_yuvrender.MyNativeRender.SAMPLE_TYPE;
import static com.example.opengl_yuvrender.MyNativeRender.SAMPLE_TYPE_YUV_TEXTURE_MAP;
import static android.opengl.GLSurfaceView.RENDERMODE_CONTINUOUSLY;
import static com.example.opengl_yuvrender.MyGLSurfaceView.IMAGE_FORMAT_NV21;

public class MainActivity extends AppCompatActivity implements ViewTreeObserver.OnGlobalLayoutListener {
    private static final String TAG = "MainActivity";
    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO,
    };
    private static final int PERMISSION_REQUEST_CODE = 1;

    private com.example.opengl_yuvrender.MyGLSurfaceView mGLSurfaceView;
    private ViewGroup mRootView;
    private int mSampleSelectedIndex = SAMPLE_TYPE_YUV_TEXTURE_MAP - SAMPLE_TYPE;;
    private com.example.opengl_yuvrender.MyGLRender mGLRender = new com.example.opengl_yuvrender.MyGLRender();
    private SensorManager mSensorManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mRootView = (ViewGroup) findViewById(R.id.rootView);
        mRootView.getViewTreeObserver().addOnGlobalLayoutListener(this);
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        mGLRender.init();
        loadNV21Image();
    }

    @Override
    public void onGlobalLayout() {
        mRootView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);
        mGLSurfaceView = new com.example.opengl_yuvrender.MyGLSurfaceView(this, mGLRender);
        mRootView.addView(mGLSurfaceView, lp);
        mGLSurfaceView.setRenderMode(RENDERMODE_CONTINUOUSLY);

    }



    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                Toast.makeText(this, "We need the permission: WRITE_EXTERNAL_STORAGE", Toast.LENGTH_SHORT).show();
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        mGLRender.unInit();
    }

    private void loadNV21Image() {
        InputStream is = null;
        try {
            is = getAssets().open("YUV_Image_840x1074.NV21");
        } catch (IOException e) {
            e.printStackTrace();
        }

        int lenght = 0;
        try {
            lenght = is.available();
            byte[] buffer = new byte[lenght];
            is.read(buffer);
            mGLRender.setImageData(IMAGE_FORMAT_NV21, 840, 1074, buffer);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }

    }

    protected boolean hasPermissionsGranted(String[] permissions) {
        for (String permission : permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                    != PackageManager.PERMISSION_GRANTED) {
                return false;
            }
        }
        return true;
    }

}
