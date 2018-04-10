package com.example.mycamera2;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.content.Intent;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

/**
 * 实现自定义相机的核心类
 * 1 实现相机的三个生命周期方法getCamera、setStartPreview、releaseCamera（这三个方法基本是固定的），
 *   并且与之和Activity的生命周期绑定起来
 * 2 实现SurfaceHolder.Callback接口（因为在布局文件中使用了SurfaceView），
 *   注意要在getCamera和releaseCamera方法中分别将Camera对象与布局文件中的SurfaceView进行绑定与取消绑定
 * （以上两布都是对相机的生命周期进行管理）
 * 3 实现点击按钮进行拍照的功能，设置拍照时的参数以及实现AutoFocusCallback接口中onAutoFocus方法的第三个参数（一个回调接口）
 *
 */
public class CustomCamera extends Activity implements SurfaceHolder.Callback {
    private Button customButton;
    private Camera mCamera;
    private SurfaceView mPreview;//用来预览相机的取景
    private SurfaceHolder mHolder;
    private PictureCallback mPictureCallback = new PictureCallback() {

        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            //data保存了拍照之后整个照片完整的数据（而非缩略图的数据）
//          File tempFile=new File("/sdcard/temp.png");//创建一个临时文件用来保存照片，可以自定义其路径
            File tempFile=new File("/storage/emulated/0/temp.png");
            try {
                FileOutputStream fos=new FileOutputStream(tempFile);
                fos.write(data);
                fos.close();
                Intent intent=new Intent(CustomCamera.this,Result.class);
                intent.putExtra("picPath", tempFile.getAbsolutePath());
                startActivity(intent);
                CustomCamera.this.finish();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.custom);
        customButton = (Button) findViewById(R.id.customButton);
        mPreview = (SurfaceView) findViewById(R.id.preview);
        mPreview.setOnClickListener(new OnClickListener() {
            // 实现点击SurfaceView范围内的屏幕时自动对焦
            @Override
            public void onClick(View v) {
                mCamera.autoFocus(null);
            }
        });
        customButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                //首先需要设置相机的一些参数
                Camera.Parameters parameters=mCamera.getParameters();
                parameters.setPictureFormat(ImageFormat.JPEG);//设置拍照的格式
                parameters.setPictureSize(800, 400);//设置拍得的图片的大小
                parameters.setFlashMode(Camera.Parameters.FLASH_MODE_AUTO);//在预览时自动对焦(前提是相机支持自动对焦的)
                mCamera.autoFocus(new Camera.AutoFocusCallback() {//开始相机的自动对焦

                    @Override
                    public void onAutoFocus(boolean success, Camera camera) {
                        //success代表当前对焦是否已经完全准确
                        if(success) {
                            //如果对焦准确就进行拍照
                            mCamera.takePicture(null, null, mPictureCallback);//最后一个参数是一个回调
                        }

                    }
                });//获取最清晰的焦距之后再进行拍照
            }
        });
        mHolder = mPreview.getHolder();
        mHolder.addCallback(this);// 将当前的Activity作为回调设置给mHolder(因为实现了Callback接口)
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mCamera == null) {
            mCamera = getCamera();// 将mCamera的生命周期与此Activity的生命周期进行绑定，保证Camera使用的资源能正确的初始化
            if (mHolder != null) {
                setStartPreview(mCamera, mHolder);
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        releaseCamera();// 将mCamera的生命周期与此Activity的生命周期进行绑定，保证Camera使用的资源能正确的释放
    }

    /**
     * 获取系统的Camera对象
     * 
     * @return
     */
    private Camera getCamera() {
        // 注意：package android.graphics中的Camera是用来实现3D图形变换的
        // android.hardware.Camera才是需要导入的，使用系统硬件的系统相机
        Camera camera = null;
        try {
            camera = Camera.open();//初始化操作
        } catch (Exception e) {
            camera = null;// 释放掉原来的对象引用，使之变成可回收的
            e.printStackTrace();
        }
        return camera;
    }

    /**
     * 开始预览相机内容
     */
    private void setStartPreview(Camera camera, SurfaceHolder holder) {
        // 需要将Camera对象与SurfaceView进行绑定，使得Camera实时预览的效果直接展现在SurfaceView上
        try {
            camera.setPreviewDisplay(holder);

            // 需要注意，系统默认的Camera是横屏的，其预览的图像也是横屏的，因此需要将Camera预览角度进行转换
            camera.setDisplayOrientation(90);// 将整个Camera旋转90°

            camera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 释放相机占用的资源，解除相关的绑定
     */
    private void releaseCamera() {
        if(mCamera!=null) {
            mCamera.setPreviewCallback(null);//将相机的回调置空，取消mPreview与mCamera的关联操作
            mCamera.stopPreview(); //停止相机的取景功能
            mCamera.release();
            mCamera = null;
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        setStartPreview(mCamera, mHolder);//将surfaceCreated与setStartPreview进行绑定
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
        mCamera.stopPreview();//将相机进行关闭
        //然后重启预览功能
        setStartPreview(mCamera, mHolder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        releaseCamera();
    }
}