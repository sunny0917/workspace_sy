package com.sykean;
public class SyA83{
	static {
		System.loadLibrary("SyA83");
	}
	
//IRISCAMERA	=	0;
//FACECAMERA	=	1;
//USBCAMERA		=	2;
/*******************************库资源接口*********************section***/
	/**********************************
	description:Iris_ServerInit
	功能:	初始化库 该接口应该首先被调用
	输入:	module  默认为0 作为模块使用则不启用测距为1
	输出:	
			>=0 库的版本号
			-2 	分配内存失败
			-98 库和硬件不匹配
	**********************************/
	public native int ServerInit(int module);
	/**********************************
	description:Iris_ServerRelease
	功能:	释放库资源
	输出:	0 成功
			-2 虹膜库未初始化
			-98 库和硬件不匹配
	**********************************/
	public native int ServerRelease();
/************************end********************************************/



/*******************************Led控制接口*******************section***/
/**********************************
	description:Iris_TurnOffLed
	功能:	set turn on/off led
	输入:	index	0	左侧红外灯
					1	右侧红外灯
					2	指示灯1
					3  	指示灯2
					4 	指示灯3
				
			onoff	0	关闭
					1 	打开
	输出:
			总是0 成功
	**********************************/
	public native int TurnOffLed(int index,int onoff);
	/************************end********************************************/

	/*******************************摄像头接口******************section***/
	/**********************************
	/**********************************
	description:VideoShutter
	功能:	设置摄像头的曝光时间
	输入:	
			index 	摄像头的索引号
					0: 虹膜摄像头
					1：人脸摄像头
					2：USB摄像头
			shutter	默认800 600--2560
					
	输出：	0	成功
		-1	失败
	**********************************/
	public native int VideoShutter(int index ,int shutter);
	
	/**********************************
	description:VideoGain
	功能:	设置摄像头的增益
	输入:	
			index 	摄像头的索引号
					0: 虹膜摄像头
					1：人脸摄像头
					2：USB摄像头
			adc		默认1-8
					
	输出：	0	成功
		-1	失败
	**********************************/
	public native int VideoGain(int index ,int adc);
	
	/************************end********************************************/
	/**************************iris camera config******************section***/
	/**********************************
	description:Iris_OpenIrisCamera
	功能:	打开虹膜摄像头
	输出:		0	成功
				-2 	打开摄像头失败
				-98 库和硬件不匹配
	**********************************/
	public native int OpenIrisCamera();

	/**********************************
	description:Iris_GetIrisFrame
	功能:	从虹膜摄像头获取一帧数据
	输入:		frame	数据帧的内存，预分配 大小为1920*1080 raw图像

	输出:		0	成功
				-2 	内存空间不匹配
				-98 库和硬件不匹配
				-99	摄像头运行异常，出现此情况需要重新系统让摄像头复位
	**********************************/
	public native int GetIrisFrame(byte[] frame);


	
	/**********************************
	description:Iris_CloseIrisCamera
	功能:	关闭虹膜摄像头
	输出:		0	成功
				-98 库和硬件不匹配
	**********************************/
	public native int CloseIrisCamera();
	/************************end********************************************/

	public native int ToBmp(byte[] frame,int Camindex);
	/**********************************
	description:Iris_RatioImage
	功能：转换显示到位图
	输入：	frame 输入的数据帧
			bitmap 位图对象 大小为 480*270 ARGB8888格式
			index	0显示虹膜图像1920*1080 1显示人脸图像 1280*960 2显示usb图像640*480*2
				-98 The hardware does not match	
				-1	show size is error
				0	success
	**********************************/
	public native int RatioImage(byte[] frame,Object bitmap,int index);
}