package com.example.mycamera2;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.os.Bundle;
import android.widget.ImageView;

public class Result extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.result);
        ImageView iv =(ImageView) findViewById(R.id.resultImageView);
        String path=getIntent().getStringExtra("picPath");

//      Bitmap bitmap = BitmapFactory.decodeFile(path);
//      iv.setImageBitmap(bitmap);

        //ֱ�Ӵ�ָ��·�����ͼƬ������������CameraCuston�иı���Ԥ���ĽǶ�,�������յõ�����Ƭ�����Ǻ��õ�
        //����������������Ӵ�����ʾ����Ƭ�ͻ��Ǻ��õģ�����Ϊ������
        FileInputStream fis = null;
        try {                   



            fis = new FileInputStream(path);
            Bitmap bitmap = BitmapFactory.decodeStream(fis);
            Matrix matrix = new Matrix();
            matrix.setRotate(90);//ѡ��90��
            bitmap = Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),matrix, true);//ͨ���任����õ��µ�bitmap

            //���ݵ�ʵ�ʵ��ֻ���������Ҫ��¼�Ƿ���СͼƬ
//          BitmapFactory.Options options=new BitmapFactory.Options();
//          options.inJustDecodeBounds=false;//���Ϊtrue��bitmap����Ϊnull
//          options.inSampleSize=2;//��ͼƬ�ĳߴ���Сһ��
//          Bitmap bitmap = BitmapFactory.decodeStream(fis,null,options); //��CustomCamera�Ѿ����ù���Ƭ�Ĵ�С
            iv.setImageBitmap(bitmap);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } finally {
            try {
                fis.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}