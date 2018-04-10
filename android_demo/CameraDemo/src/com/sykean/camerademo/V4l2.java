package com.sykean.camerademo;

public class V4l2 {

	static
	{
		System.loadLibrary("V4l2");
	}

	public native int InitCamera(int id);
	public native int GetFrame(int id,byte[] frame);

	public native int CloseCamera(int id);
}
