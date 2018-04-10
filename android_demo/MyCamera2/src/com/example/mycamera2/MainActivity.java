package com.example.mycamera2;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends Activity {
	private Button startCamera;
    private Button startCamera2;
    private Button customCamera;
    private ImageView imageView;

    private String photoFilePath;// 照片存放路径

    private static final int REQ_1 = 1;
    private static final int REQ_2 = 2;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		startCamera = (Button) findViewById(R.id.startCamera);
        startCamera2 = (Button) findViewById(R.id.startCamera2);
        customCamera=(Button) findViewById(R.id.customCamera);
        imageView = (ImageView) findViewById(R.id.imageView);
        photoFilePath = Environment.getExternalStorageDirectory().getPath();// 获取sd卡目录的路径
        photoFilePath += "/test.jpg";
        startCamera.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);// 指定action，隐式Intent
                startActivityForResult(intent, REQ_1);// 在指定的新的Activity结束后返回此Activity中，并且返回了数据
                // REQ_1相当于一个标记，当时指定的Activity结束后会返回REQ_1给onActivityResult中的requestCode
                // 表明是需要的Activity返回的数据
            }
        });
        
        startCamera2.setOnClickListener(new OnClickListener() {
            // 从照片存储的系统目录中去获取原图
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                Uri photoUri = Uri.fromFile(new File(photoFilePath));// 这里包含两步
                // 首先根据指定路径创建一个文件对象
                // 然后根据这个文件对象获得Uri
                intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);// 根据EXTRA_OUTPUT参数，系统相机将会在获取照片后根据相应的Uri存储照片
                // 一定要注意，是uri，不是文件的路径字符串！！！！！！！！！！

                startActivityForResult(intent, REQ_2);
            }
        });

        customCamera.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                //注意：这儿不能直接用intent(this,XXX.class)，因为这是在一个匿名类中，如果直接this就是指的当前的匿名类，而非MainActivity.this
                startActivity(new Intent(MainActivity.this,CustomCamera.class));
            }
        });
	}
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {// 判断新启动的Activity是否顺利结束
            if (requestCode == REQ_1) {
                Bundle bundle = data.getExtras();// Bundle类是一个key-value对,两个activity之间的通讯可以通过bundle类来实现
                // data中的图片数据是缩略图，而不是原图（因此imageView中显示的图片不会像原图那样清晰）
                // 因为如果返回原图可能会数据量太大导致数据溢出

                // Bitmap是Android系统中的图像处理的最重要类之一
                Bitmap bitmap = (Bitmap) bundle.get("data");// 将得到的图片数据存储在bitmap中

                if(bitmap!=null) startCamera.setText(""+bitmap.getWidth()+" "+bitmap.getHeight());
                else startCamera.setText("null");

                imageView.setImageBitmap(bitmap);
            } else if (requestCode == REQ_2) {
                FileInputStream fis = null;// 局部变量需要初始化
                try {
                    fis = new FileInputStream(photoFilePath);

                    Bitmap bitmap = BitmapFactory.decodeStream(fis);
                    //在第一阶段中，有提过关于图片尺寸过大而引起的问题，需要注意一下

                    if(bitmap!=null) startCamera2.setText(""+bitmap.getWidth()+" "+bitmap.getHeight());
                    else startCamera2.setText("null");

                    imageView.setImageBitmap(bitmap);
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
    }
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
