package com.sykean.a210wireless;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class A210Wireless extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public static native int openPort(String path,int baudrate);
    public static native int closePort();
    public static native int enterSleep(int enable);
    public static native int sendData(byte type,byte id,byte port,byte cmd,byte parm);
}
