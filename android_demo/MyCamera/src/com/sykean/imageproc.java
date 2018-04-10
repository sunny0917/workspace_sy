package com.sykean;

import android.graphics.Bitmap;

public class imageproc {

	public native int RatioImage(byte[] Frame,Bitmap bmp,int index,int level);
	public native int getRawData(byte[] Frame);
	public native int init();
}
