package com.example.mycamera;

import com.example.mycamera.R;
import com.sykean.imageproc;

import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity implements OnClickListener {

	private static Button bt_save;
	private static SurfaceView vSurfaceView = null;
	private static SurfaceHolder vSurfaceHolder = null;
	private imageproc imageproc1;
	 public static Bitmap mBitmap = Bitmap.createBitmap(480, 270, Bitmap.Config.ARGB_8888);// 720P;
	
	static{
		System.loadLibrary("MyCamera");
		System.loadLibrary("rawtoBmp");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		initView();
		imageproc1 = new imageproc();
		imageproc1.init();
		while(true)
		{
			byte[] raw= new byte[1920*1080];
			imageproc1.getRawData(raw);
			imageproc1.RatioImage(raw, mBitmap, 0, 1);
			SimpleDraw(mBitmap);
		}
	}

	private void initView() {
		vSurfaceView = (SurfaceView) findViewById(R.id.mySurfaceView);
		vSurfaceHolder = vSurfaceView.getHolder();
		vSurfaceHolder.setFormat(PixelFormat.TRANSLUCENT);
		
		bt_save = (Button) findViewById(R.id.bt_save_bmp);
		bt_save.setOnClickListener(this);
	}

	public void SimpleDraw(Bitmap bmp) {

        try {
            Canvas c;
            c = vSurfaceHolder.lockCanvas(null);
            if (c != null) {
                c.drawColor(Color.BLACK);   //œ‘ æª≠≤º
                c.drawBitmap(bmp, 0, 0, null);// 1080P 60
                vSurfaceHolder.unlockCanvasAndPost(c);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch (arg0.getId()) {
		case R.id.bt_save_bmp:
			
			break;

		default:
			break;
		}
	}

}
