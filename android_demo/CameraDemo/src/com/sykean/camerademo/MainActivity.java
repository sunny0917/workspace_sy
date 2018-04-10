package com.sykean.camerademo;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.util.Log;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class MainActivity extends Activity {

	V4l2 v4l2 = null;
	SurfaceView vSurfaceView1 = null;
	SurfaceView vSurfaceView2 = null;
	boolean Running = false;
	int ret1 = -1;
	int ret2 = -1;

    Thread Camera1thread;
    Thread Camera2thread;
	byte[] frame1;
	byte[] frame2;
    static private Bitmap mBitmap1;
    static private Bitmap mBitmap2;
    static private SurfaceHolder vSurfaceHolder1 = null;
    static private SurfaceHolder vSurfaceHolder2 = null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		vSurfaceView1 = (SurfaceView) findViewById(R.id.camera1);
		vSurfaceView2 = (SurfaceView) findViewById(R.id.camera2);
		assert vSurfaceView1 != null;
		assert vSurfaceView2 != null;
		vSurfaceHolder1 = vSurfaceView1.getHolder();
        vSurfaceHolder1.setFormat(PixelFormat.TRANSLUCENT);

        vSurfaceView1.setVisibility(View.VISIBLE);
        vSurfaceView1.setZOrderOnTop(true);
        

		vSurfaceHolder2 = vSurfaceView2.getHolder();
        vSurfaceHolder2.setFormat(PixelFormat.TRANSLUCENT);

        vSurfaceView2.setVisibility(View.VISIBLE);
        vSurfaceView2.setZOrderOnTop(true);
		
	}

	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();

        Running = true;
		v4l2 = new V4l2();
		v4l2.InitCamera(0);
		v4l2.InitCamera(1);
		//mBitmap1 = Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888);//720P
		//mBitmap2 = Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888);//720P
		
		frame1 = new byte[640*480*3+1024+54+2];
		frame2 = new byte[640*480*3+1024+54+2];
		
		Camera1thread = new Thread() {
            int ret = 0;
            String bmpname;
            public void run() {
                while(Running==true){
                    try{
                        ret1 = v4l2.GetFrame(0, frame1);
                        if (ret1 == 0)
                        {
                        	bmpname = "video0_"+ret+++".bmp";
                        	mBitmap1 = BitmapFactory.decodeByteArray(frame1, 0, frame1.length);
                        	//Log.e("haha","frame1.length="+frame1.length+"   "+mBitmap1);
                        	//saveBitmap(mBitmap1, bmpname);
                        	//saveFile("sdcard/pic/"+bmpname, frame2, frame2.length);
                        	SimpleDraw1(mBitmap1);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        Camera1thread.start();
        
        Camera2thread = new Thread() {
            int ret = 0;
            String bmpname;
            public void run() {
                while(Running==true){
                    try{
                        ret2 = v4l2.GetFrame(1, frame2);
                        if (ret2 == 0)
                        {
                        	bmpname = "video1_"+ret+++".bmp";
                        	mBitmap2 = BitmapFactory.decodeByteArray(frame2, 0, frame2.length);
                        	//Log.e("haha","frame2.length="+frame2.length+"   "+mBitmap2);
                        	//saveFile("sdcard/"+bmpname, frame2, frame2.length);
                        	//saveBitmap(mBitmap2, bmpname);
                        	SimpleDraw2(mBitmap2);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        Camera2thread.start();
	}
	
	 public static int saveFile(String filename, byte[] fileBytes, int file_len) {
	        FileOutputStream fos = null;

	        try {
	            fos = new FileOutputStream(filename);
	            fos.write(fileBytes, 0, file_len);
	            fos.flush();
	            fos.close();

	            return 0;
	        } catch (FileNotFoundException e) {
	            e.printStackTrace();
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
	        return -1;
	    }
	
	public static void saveBitmap(Bitmap bm, String picName) {
        File f = new File("sdcard/pic/", picName);
        if (f.exists()) {
            f.delete();
        }
        try {
            FileOutputStream out = new FileOutputStream(f);
            bm.compress(Bitmap.CompressFormat.PNG, 90, out);
            out.flush();
            out.close();
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
	public void SimpleDraw1(Bitmap bmp) {
        Canvas c = null;
        try {
            c = vSurfaceHolder1.lockCanvas(null);
            if (c != null) {
                c.drawBitmap(bmp, new Rect(0, 0, bmp.getWidth(), bmp.getHeight()),
                        new Rect(0, 0, c.getWidth(), c.getHeight()), null);

                vSurfaceHolder1.unlockCanvasAndPost(c);
            }
        } finally {
            //vSurfaceHolder1.unlockCanvasAndPost(c);
        }
    }
	

	public void SimpleDraw2(Bitmap bmp) {
        Canvas c = null;
        try {
            c = vSurfaceHolder2.lockCanvas(null);
            if (c != null) {
                c.drawBitmap(bmp, new Rect(0, 0, bmp.getWidth(), bmp.getHeight()),
                        new Rect(0, 0, c.getWidth(), c.getHeight()), null);

                vSurfaceHolder2.unlockCanvasAndPost(c);
            }
        } finally {
            //vSurfaceHolder1.unlockCanvasAndPost(c);
        }
    }
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
