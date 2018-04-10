package com.sykean;

public class Iris{

	/*
	 * 打开串口
	 * @parm	path:	串口路径
	 * 			baudrate:	波特率(默认115200)
	 * @return	0:	打开成功
	 * 			-1:	打开失败
	 */
	public native int openPort(String path,int baudrate);
	
	/*
	 * 关闭串口
	 * @return	0:	关闭成功
	 * 			-1:	关闭失败
	 */
	public native int closePort();
	
	/**
	 * 读取串口数据
	 * @param recvData	读取到的数据
	 * @param recvLen	读取到的数据长度
	 * @return	0：读取成功
	 * 			-1：读取失败
	 */
	public native int recvData(byte[] recvData,int recvLen);
	/**
	 * 启动信号
	 * @return	0：启动信号发送成功
	 * 			-1：启动信号发送失败
	 */
	public native int startSignal();

	public native int startIdentify(byte[] userId);

	public native int startRegister(byte[] userId);

	public native int delModel(byte[] userId);

	public native int getLocalUser(byte[] userId);

	public native int cancelAction();
	
	public native int getDevState();

	public native int getModel(byte[] userId);

	public native int downloadModel(byte[] iris,int irisLen);

	public native int upgradeApp(byte[] appData,int appLen);

	public native int switchIdentifyMode(byte mode);

	public native int getCardId();

	public native int capture();

	public native int downloadProfile(byte rZhd,byte rQxd,byte rXsd,byte vZhd,byte vQxd,byte vXsd);

	public native int startCreateModel();

	public native int getRealtimeModel();

	public native int stopCreateModel();


}
