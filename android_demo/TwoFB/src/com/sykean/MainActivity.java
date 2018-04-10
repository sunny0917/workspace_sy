package com.sykean;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;

public class MainActivity extends Activity {

	private static String TAG = "TwoFB";
	private TwoFB twoFb = null;
	private FbThread fbThread = null;
	private int count = 0;
	
	static{
		System.loadLibrary("TwoFB");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		twoFb = new TwoFB();
		fbThread = new FbThread();
		fbThread.start();
	}

	private class FbThread extends Thread {
		private boolean _run = true;
		
		public void stopThread(boolean run) {
	        this._run = !run;
	    }
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			super.run();
			while(_run)
			{
				String name = "/sdcard/fp";
				name = name + count +".bmp";
				Log.d(TAG, "name = " + name);
				twoFb.tofb(name,null);
				count++;
				if(count == 10)
					count = 0;
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
	} 
	

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		fbThread.stopThread(true);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
