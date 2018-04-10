package com.sykean;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class SykeanJpeg2000 extends Activity {

	static{
		System.loadLibrary("sykeanjpeg2000");
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		allocSize(1024,1024,2*1024);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public native int allocSize(int w,int h,int cmprsSizeByte);
	
	public native int jpegCompress(byte[] rawBuf,int w,int h,byte[] outBuf,int[] outSize,int cmprsSizeByte);
}