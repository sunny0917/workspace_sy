package com.sykean.A210Iris;

import android.util.Log;

public class Iris{

    static {
    	System.loadLibrary("A210Iris");
    }
    
    /**
     * ����JNI�ص�����
     * @param obj	������
     * @param func	�ص�������(recvCallback)
     */
    public native void setJNIEnv(Object obj,String func);
    
    /**
     * ��ʼ����Դ(�򿪴��ڣ������߳�)
     * @param path	������
     * @param baudrate	������(115200)
     * @return	0����ʼ���ɹ�	-1:��ʼ������ʧ��	-2:�����߳�ʧ��
     */
    public static native int Init(String path,int baudrate);
    
    /**
     * �ͷ���Դ(�رմ��ڣ�ֹͣ�߳�)
     * @return	0:�ɹ�	-1:�ͷŴ���ʧ��
     */
    public static native int Destory();
    
    /**
     * �����ź�
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Start();

    /**
     * 
     * ����ʶ��
     * @param userId	�û�ID(useridȫΪ0���ʾ1��Nʶ�𣨵�ͨ����ഥ���Զ�ʶ��ʱ���豸Ĭ��Ϊ1��Nʶ�𣬲�����ʶ��������useridΪȫFF��ʾѭ��ʶ�𣬷���ֻ�� userid ����1:1ʶ��)
     * @param nLen		24λ�û�ID
     * @return			0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Recog(byte[] userId,int nLen);

    /**
     * ����ע��
     * @param userId	�û�ID
     * @param nLen		24λ�û�ID
     * @return			0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Register(byte[] userId,int nLen);

    /**
     * ɾ��ģ��
     * @param userId	�û�ID(useridΪ24��0ɾ��ȫ�����ش洢�ĺ�Ĥ��Ϣ������ɾ����Ӧuserid�ĺ�Ĥ��Ϣ)
     * @param nLen		24λ�û�ID
     * @return			0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Delete(byte[] userId,int nLen);
    
    /**
     * ��ȡ�����û�
     * @param userId	�û�ID(����24λuserid���ص�ǰ�û���ȫ0����ȫ���û�)
     * @param nLen		24λ�û�ID
     * @return			0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Read(byte[] userId,int nLen);
    
    /**
     * ȡ����ǰ����
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Cancle();
    
    /**
     * ��ȡ��ǰ�豸״̬
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Sync();
    
    /**
     * ��ȡģ��
     * @param userId	�û�ID
     * @param nLen		24λ�û�ID
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Get(byte[] userId,int nLen);
    
    /**
     * �·�ģ��
     * @param data	userId+1024bytes��Ĥģ������
     * @param nLen	���ݳ���
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Down(byte[] app,int nLen);
    
    /**
     * ����Ӧ��
     * @param app	appdata+crc
     * @param nLen	Ӧ�ó���+4byteCRC
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int App(byte[] app,int nLen);
    
    /**
     * �л�ʶ��ģʽ
     * @param mode	0x01:�л�����ʶ��ģʽ	0x02:�л�������ʶ��ģʽ
     * @param nLen	0x01
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Mode(byte mode,int nLen);
    
    /**
     * ������
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int ReadCard();
    
    /**
     * ��׽ͼ��
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Image();
    
    /**
     * �·�������Ϣ
     * @param config	������Ϣ(ǰ3��byte��ʾע���������ֱ�Ϊ���ź϶ȣ�Ĭ��50���������ȣ�Ĭ��60����б�Ӷȣ�Ĭ��50��������3������ȡֵ��Χ��40----90֮�䣬���·�Ϊ FF FF FF ���ʾʹ��Ĭ��ֵ�����������ã�
	 *							��3��byte��ʾʶ���������ֱ�Ϊ�ź϶ȣ�Ĭ��50���������ȣ�Ĭ��40����б�Ӷȣ�Ĭ��50��������3������ȡֵ��Χ��40----90֮�䣬���·�ΪFF FF FF ���ʾʹ��Ĭ��ֵ�����������ã�)
     * @param nLen	0x06
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Config(byte[] config,int nLen);
    
    /**
     * ��ʼ����ģ��
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int BuildStart();
    
    /**
     * ��ȡʵʱģ��
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int Build();
    
    /**
     * ��������ʵʱģ��
     * @return	0:д���ڳɹ�	-1:д����ʧ��
     */
    public static native int BuildEnd();
}
