package com.sykean;

public class Vein {

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
	
	/*
	 * 获取手指触摸感应引脚状态
	 * @return	1:	手指未按下
	 * 			0:	手指按下
	 */
	public native int getFingerTpDet();
	
	/*
	 * 打开设备
	 * @parm	para:	固定为0x01(表示离线)
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int openDev(short para);
	
	/*
	 * 关闭设备
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int closeDev();
	
	/*
	 * 设置灯状态
	 * @parm	color	0:红色
	 * 					1:绿色
	 * 			flush	0:常亮
	 * 					1:闪烁
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int setLedState(short color,short flush);
	
	/*
	 * 获取触摸状态
	 * @return	-1:	读取数据异常
	 * 			0:	未按
	 * 			1：	按下去
	 */
	public native int getButtonState();
	
	/*
	 * 建模
	 * @parm	curModel:	当前建模步骤(0~2)
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int rollStep(byte curModel);
	
	/*
	 * 完成建模
	 * @parm	id:	0:设备自定	其他值:上位机产生
	 * 			modelId:设备返回的模板ID
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int rollFinish(int id,byte[] modelId);
	
	/*
	 * 认证
	 * @parm	modelId:静脉认证成功后设备返回模板ID
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int identy(byte[] modelId);
	
	/*
	 * 验证指定ID是否建模
	 * @parm	id:	验证该ID是否建模
	 * 			count:建模个数
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int getIDExist(int id,byte[] count);
	
	/*
	 * 取消等待
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int cancelWait();
	
	/*
	 * 删除所有模板
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int clearUser();
	
	/*
	 * 设置USB模式
	 * @parm	mode: 0:串口		1:adb的usb模式
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int setUsbMode(byte mode);
	
	/*
	 * 获取空ID
	 * @parm	newId: 设备返回一个空的ID
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int getEmptyId(byte[] newId);
	
	/*
	 * 删除指定模板
	 * @parm	modelId: 删除指定id的模板
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int delUser(int modelId);
	
	/*
	 * 设置波特率
	 * @parm	speed: 参照协议的arBaud数组，speed为arBaud数组的下标
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int setBaudRate(byte speed);
	
	/*
	 * 设置延时
	 * @parm	timeout: 单位为秒
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int setTimeout(byte timeout);
	
	/*
	 * 获取用户数
	 * @parm	userCount: 返回用户数
	 * 			modelCount:	返回模板数
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int getUserNum(byte[] userCount,byte[] modelCount);
	
	/*
	 * 恢复出厂设置
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int reset();
	
	/*
	 * 获取系统信息
	 * @parm	systemInfo: 返回系统信息
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
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int getSystemInfo(byte[] systemInfo);
	
	/*
	 * 上传模板
	 * @parm	modelId: 模板ID
	 * 			data:	模板数据
	 * 			dataSize: 数据大小
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int uploadModel(int modelId,byte[] data,int dataSize);
	
	/*
	 * 下载模板
	 * @parm	packSize: 每次读取大小
	 * 			modelId: 模板ID
	 * 			outData: 返回的模板数据
	 * 			outSize: 数据大小
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native int downloadModel(int packSize,/*int modelSize,*/int modelId,byte[] outData,int[] outSize);
	
	/*
	 * 采集图像
	 * @parm	num: 采集第几张图片
	 * @return	-1:	读取数据异常
	 * 			error code:	见协议手册错误码
	 */
	public native byte getImage(short num);
}
