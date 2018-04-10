package com.sykean;

public class Iris{

	/*
	 * �򿪴���
	 * @parm	path:	����·��
	 * 			baudrate:	������(Ĭ��115200)
	 * @return	0:	�򿪳ɹ�
	 * 			-1:	��ʧ��
	 */
	public native int openPort(String path,int baudrate);
	
	/*
	 * �رմ���
	 * @return	0:	�رճɹ�
	 * 			-1:	�ر�ʧ��
	 */
	public native int closePort();
	
	/**
	 * ��ȡ��������
	 * @param recvData	��ȡ��������
	 * @param recvLen	��ȡ�������ݳ���
	 * @return	0����ȡ�ɹ�
	 * 			-1����ȡʧ��
	 */
	public native int recvData(byte[] recvData,int recvLen);
	/**
	 * �����ź�
	 * @return	0�������źŷ��ͳɹ�
	 * 			-1�������źŷ���ʧ��
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
