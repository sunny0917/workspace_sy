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
 * ʵ���Զ�������ĺ�����
 * 1 ʵ������������������ڷ���getCamera��setStartPreview��releaseCamera�����������������ǹ̶��ģ���
 *   ������֮��Activity���������ڰ�����
 * 2 ʵ��SurfaceHolder.Callback�ӿڣ���Ϊ�ڲ����ļ���ʹ����SurfaceView����
 *   ע��Ҫ��getCamera��releaseCamera�����зֱ�Camera�����벼���ļ��е�SurfaceView���а���ȡ����
 * �������������Ƕ�������������ڽ��й���
 * 3 ʵ�ֵ����ť�������յĹ��ܣ���������ʱ�Ĳ����Լ�ʵ��AutoFocusCallback�ӿ���onAutoFocus�����ĵ�����������һ���ص��ӿڣ�
 *
 */
public class CustomCamera extends Activity implements SurfaceHolder.Callback {
    private Button customButton;
    private Camera mCamera;
    private SurfaceView mPreview;//����Ԥ�������ȡ��
    private SurfaceHolder mHolder;
    private PictureCallback mPictureCallback = new PictureCallback() {

        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            //data����������֮��������Ƭ���������ݣ���������ͼ�����ݣ�
//          File tempFile=new File("/sdcard/temp.png");//����һ����ʱ�ļ�����������Ƭ�������Զ�����·��
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
            // ʵ�ֵ��SurfaceView��Χ�ڵ���Ļʱ�Զ��Խ�
            @Override
            public void onClick(View v) {
                mCamera.autoFocus(null);
            }
        });
        customButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                //������Ҫ���������һЩ����
                Camera.Parameters parameters=mCamera.getParameters();
                parameters.setPictureFormat(ImageFormat.JPEG);//�������յĸ�ʽ
                parameters.setPictureSize(800, 400);//�����ĵõ�ͼƬ�Ĵ�С
                parameters.setFlashMode(Camera.Parameters.FLASH_MODE_AUTO);//��Ԥ��ʱ�Զ��Խ�(ǰ�������֧���Զ��Խ���)
                mCamera.autoFocus(new Camera.AutoFocusCallback() {//��ʼ������Զ��Խ�

                    @Override
                    public void onAutoFocus(boolean success, Camera camera) {
                        //success����ǰ�Խ��Ƿ��Ѿ���ȫ׼ȷ
                        if(success) {
                            //����Խ�׼ȷ�ͽ�������
                            mCamera.takePicture(null, null, mPictureCallback);//���һ��������һ���ص�
                        }

                    }
                });//��ȡ�������Ľ���֮���ٽ�������
            }
        });
        mHolder = mPreview.getHolder();
        mHolder.addCallback(this);// ����ǰ��Activity��Ϊ�ص����ø�mHolder(��Ϊʵ����Callback�ӿ�)
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mCamera == null) {
            mCamera = getCamera();// ��mCamera�������������Activity���������ڽ��а󶨣���֤Cameraʹ�õ���Դ����ȷ�ĳ�ʼ��
            if (mHolder != null) {
                setStartPreview(mCamera, mHolder);
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        releaseCamera();// ��mCamera�������������Activity���������ڽ��а󶨣���֤Cameraʹ�õ���Դ����ȷ���ͷ�
    }

    /**
     * ��ȡϵͳ��Camera����
     * 
     * @return
     */
    private Camera getCamera() {
        // ע�⣺package android.graphics�е�Camera������ʵ��3Dͼ�α任��
        // android.hardware.Camera������Ҫ����ģ�ʹ��ϵͳӲ����ϵͳ���
        Camera camera = null;
        try {
            camera = Camera.open();//��ʼ������
        } catch (Exception e) {
            camera = null;// �ͷŵ�ԭ���Ķ������ã�ʹ֮��ɿɻ��յ�
            e.printStackTrace();
        }
        return camera;
    }

    /**
     * ��ʼԤ���������
     */
    private void setStartPreview(Camera camera, SurfaceHolder holder) {
        // ��Ҫ��Camera������SurfaceView���а󶨣�ʹ��CameraʵʱԤ����Ч��ֱ��չ����SurfaceView��
        try {
            camera.setPreviewDisplay(holder);

            // ��Ҫע�⣬ϵͳĬ�ϵ�Camera�Ǻ����ģ���Ԥ����ͼ��Ҳ�Ǻ����ģ������Ҫ��CameraԤ���ǶȽ���ת��
            camera.setDisplayOrientation(90);// ������Camera��ת90��

            camera.startPreview();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * �ͷ����ռ�õ���Դ�������صİ�
     */
    private void releaseCamera() {
        if(mCamera!=null) {
            mCamera.setPreviewCallback(null);//������Ļص��ÿգ�ȡ��mPreview��mCamera�Ĺ�������
            mCamera.stopPreview(); //ֹͣ�����ȡ������
            mCamera.release();
            mCamera = null;
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        setStartPreview(mCamera, mHolder);//��surfaceCreated��setStartPreview���а�
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
            int height) {
        mCamera.stopPreview();//��������йر�
        //Ȼ������Ԥ������
        setStartPreview(mCamera, mHolder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        releaseCamera();
    }
}