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

        //直接从指定路径获得图片，但是由于在CameraCuston中改变了预览的角度,但是拍照得到的照片依旧是横置的
        //所以在这里如果不加处理，显示的照片就会是横置的，以下为处理方法
        FileInputStream fis = null;
        try {                   



            fis = new FileInputStream(path);
            Bitmap bitmap = BitmapFactory.decodeStream(fis);
            Matrix matrix = new Matrix();
            matrix.setRotate(90);//选择90°
            bitmap = Bitmap.createBitmap(bitmap,0,0,bitmap.getWidth(),bitmap.getHeight(),matrix, true);//通过变换矩阵得到新的bitmap

            //根据的实际的手机，还是需要考录是否缩小图片
//          BitmapFactory.Options options=new BitmapFactory.Options();
//          options.inJustDecodeBounds=false;//如果为true，bitmap将会为null
//          options.inSampleSize=2;//把图片的尺寸缩小一半
//          Bitmap bitmap = BitmapFactory.decodeStream(fis,null,options); //在CustomCamera已经设置过照片的大小
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