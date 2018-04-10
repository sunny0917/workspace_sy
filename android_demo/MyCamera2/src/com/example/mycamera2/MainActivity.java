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

    private String photoFilePath;// ��Ƭ���·��

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
        photoFilePath = Environment.getExternalStorageDirectory().getPath();// ��ȡsd��Ŀ¼��·��
        photoFilePath += "/test.jpg";
        startCamera.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);// ָ��action����ʽIntent
                startActivityForResult(intent, REQ_1);// ��ָ�����µ�Activity�����󷵻ش�Activity�У����ҷ���������
                // REQ_1�൱��һ����ǣ���ʱָ����Activity������᷵��REQ_1��onActivityResult�е�requestCode
                // ��������Ҫ��Activity���ص�����
            }
        });
        
        startCamera2.setOnClickListener(new OnClickListener() {
            // ����Ƭ�洢��ϵͳĿ¼��ȥ��ȡԭͼ
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                Uri photoUri = Uri.fromFile(new File(photoFilePath));// �����������
                // ���ȸ���ָ��·������һ���ļ�����
                // Ȼ���������ļ�������Uri
                intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);// ����EXTRA_OUTPUT������ϵͳ��������ڻ�ȡ��Ƭ�������Ӧ��Uri�洢��Ƭ
                // һ��Ҫע�⣬��uri�������ļ���·���ַ�����������������������

                startActivityForResult(intent, REQ_2);
            }
        });

        customCamera.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                //ע�⣺�������ֱ����intent(this,XXX.class)����Ϊ������һ���������У����ֱ��this����ָ�ĵ�ǰ�������࣬����MainActivity.this
                startActivity(new Intent(MainActivity.this,CustomCamera.class));
            }
        });
	}
	@Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) {// �ж���������Activity�Ƿ�˳������
            if (requestCode == REQ_1) {
                Bundle bundle = data.getExtras();// Bundle����һ��key-value��,����activity֮���ͨѶ����ͨ��bundle����ʵ��
                // data�е�ͼƬ����������ͼ��������ԭͼ�����imageView����ʾ��ͼƬ������ԭͼ����������
                // ��Ϊ�������ԭͼ���ܻ�������̫�����������

                // Bitmap��Androidϵͳ�е�ͼ���������Ҫ��֮һ
                Bitmap bitmap = (Bitmap) bundle.get("data");// ���õ���ͼƬ���ݴ洢��bitmap��

                if(bitmap!=null) startCamera.setText(""+bitmap.getWidth()+" "+bitmap.getHeight());
                else startCamera.setText("null");

                imageView.setImageBitmap(bitmap);
            } else if (requestCode == REQ_2) {
                FileInputStream fis = null;// �ֲ�������Ҫ��ʼ��
                try {
                    fis = new FileInputStream(photoFilePath);

                    Bitmap bitmap = BitmapFactory.decodeStream(fis);
                    //�ڵ�һ�׶��У����������ͼƬ�ߴ�������������⣬��Ҫע��һ��

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
