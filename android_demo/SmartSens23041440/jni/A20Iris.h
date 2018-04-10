#ifndef __A20IRIS__
#define __A20IRIS__
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/*******************************库资源接口*********************section***/

/**********************************
description:Iris_InitVeri
功能:	设置Des加密 CBC模式 PKCS5Padding填充方式的初始化向量
输入:	var   8byte 初始化为0
输出:	
		0 成功
**********************************/
int Iris_InitVeri(char* var);

/**********************************
description:Iris_InitVeri
功能:	设置Des加密 CBC模式 PKCS5Padding填充方式的密钥
输入:	keyinput   8byte 必须等于8个字节
输出:	
		0 成功
**********************************/
int Iris_SetKey(char* keyinput);

/**********************************
description:Iris_InitVeri
功能:	Des CBC模式 PKCS5Padding填充方式的加解密
输入:	input   输入二进制流
		type	0为解密 1为加密
		len		输入二进制的字节长度
输出:	
		返回加密或者解密的内容指针 NULL为失败
		len 返回内容的长度
**********************************/
char* Iris_DesProc(char* input,int type,int* len);

/**********************************
description:Iris_ServerInit
功能:	初始化库 该接口应该首先被调用
输入:	无
输出:	
		>=0 库的版本号
		-2 	分配内存失败
		-98 库和硬件不匹配
**********************************/
int Iris_ServerInit(int module);
/**********************************
description:Iris_ServerRelease
功能:	释放库资源
输出:	0 成功
		-2 虹膜库未初始化
		-98 库和硬件不匹配
**********************************/
int Iris_ServerRelease();
/************************end********************************************/
/**********************************
description:Iris_Targetgray
功能:	设置曝光目标值 默认为120
输出:	0 成功
**********************************/
int Iris_Targetgray(int gray);

/*******************************调试接口*********************section***/
/**********************************
:Iris_LogEnable
功能:	打开或者关闭调试信息
输入:	onoff	0	关闭
				1	打开
输出:
		总是0 成功
**********************************/
int Iris_LogEnable(int onoff);
/************************end********************************************/

/*******************************Led控制接口*******************section***/
/**********************************
description:Iris_TurnOffLed
功能:	set turn on/off led
输入:	index	0	左侧红外灯
				1	右侧红外灯
				2	左右红外灯
				3	人脸红外灯
				4	指示灯1 red
				5  	指示灯2 green
				6 	指示灯3	blue
				7 	指示灯4 rgb
				
		onoff	0	关闭
				1 	打开
		value   0	默认值
				其它 1-310
输出:
		总是0 成功
**********************************/
int Iris_TurnOffLed(int index,int onoff,int value);
/************************end********************************************/

/*******************************摄像头接口******************section***/
/**********************************
description:Iris_VideoGain
功能:	设置摄像头的增益
输入:	
		index 	摄像头的索引号
				0: 虹膜摄像头
				1：人脸摄像头
				2：USB摄像头
		adc		摄像头的增益
				1	x1
				2	x2
				3	x3
				4	x4
				5	x5
				6	x6
				7	x7
				8	x8
输出：	0	成功
		-1	失败
**********************************/
int Iris_VideoGain(int index ,int adc);
/************************end********************************************/

/**************************usb camera config******************section***/
/**********************************
description:Iris_OpenUsbCamera
功能:	打开USB摄像头
输出:		0	成功
			-2 	打开摄像头失败
			-98 库和硬件不匹配
**********************************/
int Iris_OpenUsbCamera();

/**********************************
description:Iris_GetUsbFrame
功能:	从USB摄像头获取一帧数据
输入:		frame	数据帧的内存，预分配 大小为640*480*2
			len		数据帧的内存大小，和frame匹配
输出:		0	成功
			-2 	内存空间不匹配
			-98 库和硬件不匹配
			-99	摄像头运行异常，出现此情况需要重新系统让摄像头复位
**********************************/
int Iris_GetUsbFrame(unsigned char* frame,int len);

/**********************************
description:Iris_CloseUsbCamera
功能:	关闭USB摄像头
输出:		0	成功
			-98 库和硬件不匹配
**********************************/
int Iris_CloseUsbCamera();

/************************end********************************************/

/**************************face camera config******************section***/
/**********************************
description:Iris_OpenFaceCamera
功能:	打开人脸摄像头
输出:		0	成功
			-2 	打开摄像头失败
			-98 库和硬件不匹配
**********************************/
int Iris_OpenFaceCamera();

/**********************************
description:Iris_GetFaceFrame
功能:	从人脸摄像头获取一帧数据
输入:		frame	数据帧的内存，预分配 大小为1280*960 raw图像
			len		数据帧的内存大小，和frame匹配
输出:		0	成功
			-2 	内存空间不匹配
			-98 库和硬件不匹配
			-99	摄像头运行异常，出现此情况需要重新系统让摄像头复位
**********************************/
int Iris_GetFaceFrame(unsigned char* frame,int len);

/**********************************
description:Iris_FaceAutoExposure
功能:	从人脸摄像头获取一帧数据
输入:		frame	数据帧的内存，预分配 大小为640*480 raw图像
			len		数据帧的内存大小，和frame匹配
输出:		0	成功
			1   图像亮度不满足可以作为显示
			-2 	内存空间不匹配
			-98 库和硬件不匹配
**********************************/
int Iris_FaceAutoExposure(unsigned char* frame,int len);

/**********************************
description:Iris_CloseFaceCamera
功能:	关闭人脸摄像头
输出:		0	成功
			-98 库和硬件不匹配
**********************************/
int Iris_CloseFaceCamera();
/************************end********************************************/

/**************************iris camera config******************section***/
/**********************************
description:Iris_OpenIrisCamera
功能:	打开虹膜摄像头
输出:		0	成功
			-2 	打开摄像头失败
			-98 库和硬件不匹配
**********************************/
int Iris_OpenIrisCamera();

/**********************************
description:Iris_GetIrisFrame
功能:	从虹膜摄像头获取一帧数据
输入:		frame	数据帧的内存，预分配 大小为1920*1080 raw图像
			len		数据帧的内存大小，和frame匹配
输出:		0	成功
			-2 	内存空间不匹配
			-98 库和硬件不匹配
			-99	摄像头运行异常，出现此情况需要重新系统让摄像头复位
**********************************/
int Iris_GetIrisFrame(unsigned char* frame,int len);

/**********************************
description:Iris_IrisAutoExposure
功能:	设置自动曝光
输入:		frame	数据帧的内存，预分配 大小为1920*1080 raw图像
			len		数据帧的内存大小，和frame匹配
输出:		0	成功
			1   图像亮度不满足可以作为显示
			-2 	内存空间不匹配
			-98 库和硬件不匹配
**********************************/
int Iris_IrisAutoExposure(unsigned char* frame,int len);

/**********************************
description:Iris_CloseIrisCamera
功能:	关闭虹膜摄像头
输出:		0	成功
			-98 库和硬件不匹配
**********************************/
int Iris_CloseIrisCamera();
/************************end********************************************/


/**************************测距接口***************************section***/
/**********************************
description:Iris_GetDistance
功能:	获取当前用户离镜头的距离
输出:		>= 0 获取到当前测距的值
			-2	测距模块未初始化
			-98 库和硬件不匹配
**********************************/
int Iris_GetDistance();

/**********************************
description:Iris_GetLightStrong
功能:	获取当前光感强度
输出:		>= 0 获取到当前光感强度的值
			-2	测距模块未初始化
			-98 库和硬件不匹配
**********************************/
int Iris_GetLightStrong();

/**********************************
description:Iris_GetLightStrong
功能:	获取当前光感距离
输出:		>= 0 获取到当前光感距离的值
			-2	测距模块未初始化
			-98 库和硬件不匹配
**********************************/
int Iris_GetLightDistance();

/************************end********************************************/

/*******************************虹膜接口*********************section***/
/**********************************
description:Iris_EnrollConfig
功能:	设置虹膜的注册质量
输入:	TextureFocus	清晰度		default 60
		VisualDegree	张合度		default 40
		Gaze			斜视度		default 50
		foucs		    聚焦度		default 60
		move		    运动模糊 	default 30
输出:	0 总是成功	
**********************************/
int Iris_EnrollConfig(int TextureFocus,int VisualDegree,int Gaze,int foucs,int move);

/**********************************
description:Iris_MatchConfig
功能:	设置虹膜的匹配质量
输入:	TextureFocus	清晰度		default 60
		VisualDegree	张合度		default 30
		Gaze			斜视度		default 50
		foucs   		聚焦度		default 60
		move   			运动模糊 	default 30
输出:	0 总是成功	
**********************************/
int Iris_MatchConfig(int TextureFocus,int VisualDegree,int Gaze,int foucs,int move);

/**********************************
description:Iris_Reset
功能:	重设虹膜服务
输入:	无
输出:	0 成功
		-2 虹膜库未初始化
		-98 库和硬件不匹配
**********************************/
int Iris_Reset();
/*********************************
description:Iris_GetLeftQuality
功能:	获取左眼的虹膜质量
输入:	TextureFocus	清晰度		
		VisualDegree	张合度		
		Gaze			斜视度		
		Focus   		聚焦度		
		Move   			运动模糊 
		Ratio   		虹膜比例 
		Composite   	注册质量 			
输出:	0 成功
		-2 虹膜库未初始化
		-98 库和硬件不匹配
**********************************/
int Iris_GetLeftQuality(int* TextureFocus,int* VisualDegree,int* Gaze,int* Focus,int* Move,
						int* Ratio,int* Composite);
					
/**********************************
description:Iris_GetRightQuality
功能:	获取右眼的虹膜质量
输入:	TextureFocus	清晰度		
		VisualDegree	张合度		
		Gaze			斜视度		
		Focus   		聚焦度		
		Move   			运动模糊 
		Ratio   		虹膜比例 
		Composite   	注册质量 			
输出:	0 成功
		-2 虹膜库未初始化
		-98 库和硬件不匹配
**********************************/
int Iris_GetRightQuality(int* TextureFocus,int* VisualDegree,int* Gaze,int* Focus,int* Move,
						int* Ratio,int* Composite);

/**********************************
description:Iris_GetError
功能:	获取当前注册或者识别的错误码
输入：	lefterr		左眼的错误码
		righterr	右眼的错误码	
输出:	0 成功
		-2 虹膜库未初始化
		-98 库和硬件不匹配
**********************************/
int Iris_GetError(int* lefterr,int* righterr);

/**********************************
description:    Iris_EnrollEye
功能：获取虹膜模板
输入：	frame 1920*1080的图像
		ListTemplate 虹膜列表 1024的整数倍
		Cnt			用户的个数
		Template    保存返回的模板 1024个byte
		BmpLeft		存返回的左眼图像 256*256大小
		BmpRight	存返回的右眼图像 256*256大小
输出：  	3 重复注册
			2  获取成功
			1  获取单眼成功
			0  继续调用
			-1 获取失败
			-3 输入图像格式错误
			-4 输入模板内存错误
********注册虹膜***********/
int Iris_EnrollEye(unsigned char* Frame,unsigned char* ListTemplate,int Cnt,unsigned char* Template,
					unsigned char* BmpLeft, unsigned char* BmpRight);
				
/**********************************
description:Iris_MatchEye
功能：匹配虹膜模板
输入：	Frame 1920*1080的gray图像
   		irislist    存虹膜的列表 1024的整数倍
		nubmber    用户的个数
		BmpLeft		存返回的左眼图像 256*256大小
		BmpRight	存返回的右眼图像 256*256大小
输出：  	>0  匹配成功返回对应的用户索引
			0  继续调用
			-1 匹配失败
			-2 库未初始化
			-3 输入图像格式错误
			-4 输入模板内存错误
			-5 用户不存在
**********************************/
int Iris_MatchEye(unsigned char* Frame,unsigned char* irislist,int nubmber,
					unsigned char* BmpLeft, unsigned char* BmpRight);
					
/************************end********************************************/
/**********************************
description:Iris_RatioImage
功能：转换显示图像
输入：	frame 	1920*1080的gray图像
   		lensrc    frame的内存大小 1920*1080
		bitmap    位图对象 大小为 480*270 ARGB8888格式
		lendst	   bitmap的内存大小 480*270*4	
		index	0显示虹膜图像画框1920*1080 1显示人脸图像 640*480 2 显示usb图像640*480*2  3 显示画圆的图像 4 显示方形 亮度降低 5 显示圆形 亮度降低
输出：  -2 传入内存错误
		-98 库和硬件不匹配
		0 成功
**********************************/
int Iris_RatioImage(unsigned char* frame,int lensrc ,unsigned char* bitmap,int lendst,int index);

int Iris_ToBmp(unsigned char* frame,int lensrc ,int Camindex);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
