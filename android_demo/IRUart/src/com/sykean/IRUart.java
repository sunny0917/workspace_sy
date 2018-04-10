package com.sykean;

public class IRUart{

	/**
	 * 
	 * @param path	:serial name
	 * @param baudrate	:serial speed
	 * @return 0:success -1:error
	 */
	public static native int openPort(String path,int baudrate);
    /**
     * none
     * @return	0:success -1:error
     */
	public static native int closePort();
    /**
     * 
     * @param sn	:Gate number
     * @param cmd	:command code
     * @param parm	:command parameter
     * @return	return write number on success,return < 0 on error
     */
    public static native int sendData(byte sn,byte cmd,byte[] parm);
    /**
     * 
     * @param recvData	:data with read
     * @return	0:success	-1:error
     */
    public static native int recvData(byte[] recvData);
}
