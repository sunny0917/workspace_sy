package com.sykean;

public class IDCard {

	public native int openPort(String path,int baud);
	public native int closePort();
	public native int getText();
	public native int findCard(byte[] recvData);
	public native int selectCard(byte[] recvData);
	public native int readCard(byte[] txt,byte[] image);
	public native int oneKeyReadCard(byte[] txt,byte[] image);
}
