package com.example.display;

import android.hardware.display.DisplayManager;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.view.Display;
import android.view.Menu;
import android.view.WindowManager;

public class MainActivity extends Activity {
	
	DisplayManager  mDisplayManager;//屏幕管理类
	Display[]  displays;//屏幕数组
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mDisplayManager = (DisplayManager)getApplicationContext().getSystemService(Context.DISPLAY_SERVICE);
	    displays =mDisplayManager.getDisplays();
	    
	    DifferentDislay  mPresentation =new DifferentDislay (getApplicationContext(),displays[1]);//displays[1]是副屏
	    mPresentation.getWindow().setType(
	    WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
	    mPresentation.show();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
