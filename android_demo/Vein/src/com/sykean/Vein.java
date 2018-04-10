package com.sykean;

public class Vein {

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
	
	/*
	 * ��ȡ��ָ������Ӧ����״̬
	 * @return	1:	��ָδ����
	 * 			0:	��ָ����
	 */
	public native int getFingerTpDet();
	
	/*
	 * ���豸
	 * @parm	para:	�̶�Ϊ0x01(��ʾ����)
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int openDev(short para);
	
	/*
	 * �ر��豸
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int closeDev();
	
	/*
	 * ���õ�״̬
	 * @parm	color	0:��ɫ
	 * 					1:��ɫ
	 * 			flush	0:����
	 * 					1:��˸
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int setLedState(short color,short flush);
	
	/*
	 * ��ȡ����״̬
	 * @return	-1:	��ȡ�����쳣
	 * 			0:	δ��
	 * 			1��	����ȥ
	 */
	public native int getButtonState();
	
	/*
	 * ��ģ
	 * @parm	curModel:	��ǰ��ģ����(0~2)
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int rollStep(byte curModel);
	
	/*
	 * ��ɽ�ģ
	 * @parm	id:	0:�豸�Զ�	����ֵ:��λ������
	 * 			modelId:�豸���ص�ģ��ID
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int rollFinish(int id,byte[] modelId);
	
	/*
	 * ��֤
	 * @parm	modelId:������֤�ɹ����豸����ģ��ID
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int identy(byte[] modelId);
	
	/*
	 * ��ָ֤��ID�Ƿ�ģ
	 * @parm	id:	��֤��ID�Ƿ�ģ
	 * 			count:��ģ����
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int getIDExist(int id,byte[] count);
	
	/*
	 * ȡ���ȴ�
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int cancelWait();
	
	/*
	 * ɾ������ģ��
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int clearUser();
	
	/*
	 * ����USBģʽ
	 * @parm	mode: 0:����		1:adb��usbģʽ
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int setUsbMode(byte mode);
	
	/*
	 * ��ȡ��ID
	 * @parm	newId: �豸����һ���յ�ID
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int getEmptyId(byte[] newId);
	
	/*
	 * ɾ��ָ��ģ��
	 * @parm	modelId: ɾ��ָ��id��ģ��
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int delUser(int modelId);
	
	/*
	 * ���ò�����
	 * @parm	speed: ����Э���arBaud���飬speedΪarBaud������±�
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int setBaudRate(byte speed);
	
	/*
	 * ������ʱ
	 * @parm	timeout: ��λΪ��
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int setTimeout(byte timeout);
	
	/*
	 * ��ȡ�û���
	 * @parm	userCount: �����û���
	 * 			modelCount:	����ģ����
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int getUserNum(byte[] userCount,byte[] modelCount);
	
	/*
	 * �ָ���������
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int reset();
	
	/*
	 * ��ȡϵͳ��Ϣ
	 * @parm	systemInfo: ����ϵͳ��Ϣ
	 * 						0:main version
	 *						1:sub version
	 *						2:device id
	 *						3:baud rate
	 *						4~5:level
	 *						6:time out
	 *						7:check dup
	 *						8:check samefinger
	 *						9:usb mode
	 *						10:read error
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int getSystemInfo(byte[] systemInfo);
	
	/*
	 * �ϴ�ģ��
	 * @parm	modelId: ģ��ID
	 * 			data:	ģ������
	 * 			dataSize: ���ݴ�С
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int uploadModel(int modelId,byte[] data,int dataSize);
	
	/*
	 * ����ģ��
	 * @parm	packSize: ÿ�ζ�ȡ��С
	 * 			modelId: ģ��ID
	 * 			outData: ���ص�ģ������
	 * 			outSize: ���ݴ�С
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native int downloadModel(int packSize,/*int modelSize,*/int modelId,byte[] outData,int[] outSize);
	
	/*
	 * �ɼ�ͼ��
	 * @parm	num: �ɼ��ڼ���ͼƬ
	 * @return	-1:	��ȡ�����쳣
	 * 			error code:	��Э���ֲ������
	 */
	public native byte getImage(short num);
}
