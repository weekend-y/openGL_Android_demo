package com.example.opengl_camera2;

import android.Manifest;
import android.os.Bundle;
import android.util.Log;
import android.util.Size;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.RelativeLayout;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import com.example.opengl_camera2.camera.Camera2FrameCallback;
import com.example.opengl_camera2.camera.Camera2Wrapper;
import com.google.android.material.floatingactionbutton.FloatingActionButton;

import static com.example.opengl_camera2.MyNativeRender.IMAGE_FORMAT_I420;


public class MainActivity extends BaseRenderActivity implements Camera2FrameCallback, View.OnClickListener {
    private static final String TAG = "weekend";
    private static final String[] REQUEST_PERMISSIONS = {
            Manifest.permission.CAMERA,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
    };

    private static final int CAMERA_PERMISSION_REQUEST_CODE = 1;

    private RelativeLayout mSurfaceViewRoot;
    private Camera2Wrapper mCamera2Wrapper;
    private ImageButton mSwitchCamBtn;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mCamera2Wrapper != null) {
                    mCamera2Wrapper.capture();
                }
            }
        });

        initViews();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            mCamera2Wrapper.startCamera();
        } else {
            ActivityCompat.requestPermissions(this, REQUEST_PERMISSIONS, CAMERA_PERMISSION_REQUEST_CODE);
        }
        updateTransformMatrix(mCamera2Wrapper.getCameraId());
        if (mSurfaceViewRoot != null) {
            updateGLSurfaceViewSize(mCamera2Wrapper.getPreviewSize());
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (requestCode == CAMERA_PERMISSION_REQUEST_CODE) {
            if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                //mCamera2Wrapper.startCamera();
            } else {
                Toast.makeText(this, "We need the camera permission.", Toast.LENGTH_SHORT).show();
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    @Override
    protected void onPause() {
        if (hasPermissionsGranted(REQUEST_PERMISSIONS)) {
            mCamera2Wrapper.stopCamera();
        }
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mMyGLRender.unInit();
    }

    @Override
    public void onPreviewFrame(byte[] data, int width, int height) {
        Log.d(TAG, "onPreviewFrame() called with: data = [" + data + "], width = [" + width + "], height = [" + height + "]");
        mMyGLRender.setRenderFrame(IMAGE_FORMAT_I420, data, width, height);
        mMyGLRender.requestRender();
    }

    @Override
    public void onCaptureFrame(byte[] data, int width, int height) {
        Log.d(TAG, "暂不做拍照功能");
        Toast.makeText(this, "暂不做拍照功能", Toast.LENGTH_SHORT).show();
    }

    private void initViews() {
        mSwitchCamBtn = (ImageButton) findViewById(R.id.switch_camera_btn);
        mSwitchCamBtn.bringToFront();
        mSwitchCamBtn.setOnClickListener(this);

        mSurfaceViewRoot = (RelativeLayout) findViewById(R.id.surface_root);
        RelativeLayout.LayoutParams p = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                RelativeLayout.LayoutParams.MATCH_PARENT);
        mSurfaceViewRoot.addView(mGLSurfaceView, p);
        mMyGLRender.init(mGLSurfaceView);

        mCamera2Wrapper = new Camera2Wrapper(this);

        ViewTreeObserver treeObserver = mSurfaceViewRoot.getViewTreeObserver();
        treeObserver.addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {
            @Override
            public boolean  onPreDraw() {
                mSurfaceViewRoot.getViewTreeObserver().removeOnPreDrawListener(this);
                mRootViewSize = new Size(mSurfaceViewRoot.getMeasuredWidth(), mSurfaceViewRoot.getMeasuredHeight());
                updateGLSurfaceViewSize(mCamera2Wrapper.getPreviewSize());
                return true;
            }
        });
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.switch_camera_btn:
                String cameraId = mCamera2Wrapper.getCameraId();
                String[] cameraIds = mCamera2Wrapper.getSupportCameraIds();
                if (cameraIds != null) {
                    for (int i = 0; i < cameraIds.length; i++) {
                        if (!cameraIds[i].equals(cameraId)) {
                            mCamera2Wrapper.updateCameraId(cameraIds[i]);
                            updateTransformMatrix(cameraIds[i]);
                            updateGLSurfaceViewSize(mCamera2Wrapper.getPreviewSize());
                            break;
                        }
                    }
                }
                break;
            default:
        }
    }
}