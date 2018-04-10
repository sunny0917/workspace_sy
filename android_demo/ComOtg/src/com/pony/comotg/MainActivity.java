package com.pony.comotg;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Iterator;

import android.app.Activity;
import android.app.PendingIntent;
import android.app.ProgressDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbRequest;
import android.os.Bundle;
import android.os.Environment;
import android.os.Process;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String TAG = "OTG_DEBUG";
    private static final String ACTION_USB_PERMISSION =
            "com.pony.comotg.USB_PERMISSION";
    
    public static String startCmd = "55000a0000000001410000e003"; //55 00 0a 00 00 00 00 01 41 00 00 e0 03
    public static String registered = "5500220000000001430018000000000000000000000000000000000000000000000031e303";
    public static String temp = "5500220000000001480018000000000000000000000000000000000000000000000032eb03";
    public static String user = "5500220000000001450018000000000000000000000000000000000000000000000031e503";
    public static String delete = "5500220000000001440018000000000000000000000000000000000000000000000000d503";

    public static String preview = "55000a0000000001570000f603";
    public static String leftIris = "55000a0000000001580000f903";
    public static String rightIris = "55000a0000000001590000f803";
    public static String hand = "55000b000000000153000100f203";
    
    private static Toast mMyToast = null;
    private boolean mIsStopFlag = false;
    private boolean mErollFlag = false;
    private TextView mShowInfoTxt = null;
    private Button mQueryBtn = null;
    private Button mStartBtn = null;
    private Button mErollBtn = null;
    private Button mSingleSendBtn = null;
    private Button mSendBtn = null;
    private Button mReceiveBtn = null;
    private Button mStopBtn = null;
    private UsbManager mUsbManager = null;
    private UsbDeviceConnection mSendUsbCon = null;
    private UsbInterface mSendUsbIntFace = null;
    private UsbEndpoint mSendUsbEpBulkOut = null;
    private UsbEndpoint mSendUsbEpBulkIn = null;
    private UsbDeviceConnection mRcvUsbCon = null;
    private UsbDeviceConnection mErollUsbCon = null;
    private UsbInterface mRcvUsbIntFace = null;
    private UsbEndpoint mRcvUsbEpBulkOut = null;
    private UsbEndpoint mRcvUsbEpBulkIn = null;
    private UsbInterface mErollUsbIntFace = null;
    private UsbEndpoint mErollUsbEpBulkOut = null;
    private UsbEndpoint mErollUsbEpBulkIn = null;
    private PendingIntent mPermissionIntent = null;
    private Activity mActivity = null;
    private PowerManager mPowerManager = null;
    private WakeLock mWakeLock = null;
    private int previewNub = 0;
    private byte[] bmpMaxPack;

    private String photoFilePath;// 照片存放路径
    private BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (UsbManager.ACTION_USB_DEVICE_ATTACHED.equals(action)) {
                Toast.makeText(context, "设备接入!", Toast.LENGTH_SHORT).show();
            } else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
                Toast.makeText(context, "与设备断开连接!", Toast.LENGTH_SHORT).show();
            } else if (action.equals(ACTION_USB_PERMISSION)) {
                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                    Toast.makeText(context, "权限获取成功!", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(context, "权限被拒绝!", Toast.LENGTH_SHORT).show();
                }
            }
        }
    };
    public static byte[] getPlain(byte[] by, String str) {

        return hexStringToBytes("55aa00010000000000000032000800000000000000000000000000000000000000000000000000000000000000000000000000000000" + bytesToHexString(by) + str + "03");
    }

    public static byte[] startDev() {

        return hexStringToBytes(startCmd);
    }

    public static byte[] regIris() {

        return hexStringToBytes(registered);
    }

//    public static byte[] postHand() {
//
//        return hexStringToBytes(hand);
//    }

    public static byte[] getTemp() {

        return hexStringToBytes(temp);
    }

    public static byte[] getUser() {

        return hexStringToBytes(user);
    }

    public static byte[] deleteUser() {

        return hexStringToBytes(delete);
    }

    public static byte[] preView() {
        return hexStringToBytes(preview);
    }

    public static byte[] getLeftIris() {
        return hexStringToBytes(leftIris);
    }

    public static byte[] getRightIris() {
        return hexStringToBytes(rightIris);
    }
    
    public static byte[] hexStringToBytes(String hexString) {
        if (hexString == null || hexString.equals("")) {
            return null;
        }
        hexString = hexString.toUpperCase();
        int length = hexString.length() / 2;
        char[] hexChars = hexString.toCharArray();
        byte[] d = new byte[length];
        for (int i = 0; i < length; i++) {
            int pos = i * 2;
            d[i] = (byte) (charToByte(hexChars[pos]) << 4 | charToByte(hexChars[pos + 1]));
        }
        return d;
    }
    private static byte charToByte(char c) {
        return (byte) "0123456789ABCDEF".indexOf(c);
    }
    public static String bytesToHexString(byte[] src) {
        StringBuffer sb = new StringBuffer();
        if (src == null || src.length <= 0) {
            return null;
        }

        for (byte aSrc : src) {
            int v = aSrc & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                sb.append(0);
            }
            sb.append(hv);
        }
        return sb.toString();
    } 
    public static String bytesToHexString(byte[] src,int len) {
        StringBuffer sb = new StringBuffer();
        if (src == null || src.length <= 0) {
            return null;
        }

        for (int i = 0;i < len;i++) {
            int v = src[i] & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                sb.append(0);
            }
            sb.append(hv);
        }
        return sb.toString();
    } 
    public static int saveFile(String filename, byte[] fileBytes, int file_len) {
        FileOutputStream fos = null;

        try {
            fos = new FileOutputStream(filename,true);
            fos.write(fileBytes, 0, file_len);
            fos.flush();
            fos.close();

            return 0;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return -1;
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mActivity = this;
        mShowInfoTxt = (TextView) findViewById(R.id.showInfoTxt);
        mShowInfoTxt.setMovementMethod(new ScrollingMovementMethod());
        mQueryBtn = (Button) findViewById(R.id.queryBtn);
        mStartBtn = (Button) findViewById(R.id.start);
        mErollBtn = (Button) findViewById(R.id.eroll);
        mSingleSendBtn = (Button) findViewById(R.id.singleBtn);
        mSendBtn = (Button) findViewById(R.id.sendBtn);
        mReceiveBtn = (Button) findViewById(R.id.receiveBtn);
        mStopBtn = (Button) findViewById(R.id.stopBtn);
        bmpMaxPack = new byte[1024*105];
        mPermissionIntent = PendingIntent.getBroadcast(this, 0,
                new Intent(ACTION_USB_PERMISSION), 0);
        mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);

        mQueryBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();

                if (!(deviceList.isEmpty())) {
                    Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                    StringBuilder devInfo = new StringBuilder();

                    while (deviceIterator.hasNext()) {
                        UsbDevice device = deviceIterator.next();

                        devInfo.append("\ndev name: " + device.getDeviceName()
                                + "\ndev id:" + device.getDeviceId()
                                + "\ndev product id:" + device.getProductId()
                                + "\ndev vendor id:" + device.getVendorId()
                                + "\ndev class id:" + device.getDeviceClass()
                                + "\ndev sub class id:" + device.getDeviceSubclass()
                                + "\ndev protocol:" + device.getDeviceProtocol()
                                + "\ndev interface count: " + device.getInterfaceCount());

                        for (int itfIdx = 0; itfIdx < device.getInterfaceCount(); itfIdx++) {
                            UsbInterface usbInterface = device.getInterface(itfIdx);

                            devInfo.append("\ninterface class: " + usbInterface.getInterfaceClass()
                                    + "\ninterface sub class: " + usbInterface.getInterfaceSubclass()
                                    + "\nendpoint count:" + usbInterface.getEndpointCount());

                            for (int epIdx = 0; epIdx < usbInterface.getEndpointCount(); epIdx++) {
                                UsbEndpoint ep = usbInterface.getEndpoint(epIdx);
                                devInfo.append("\nendpoint type: " + ep.getType()
                                        + "\nendpoint dir:" + ep.getDirection()
                                        + "\nendpoint size:" + ep.getMaxPacketSize());
                            }
                        }
                    }

                    mShowInfoTxt.setText(devInfo.toString());
                } else {
                    mShowInfoTxt.setText("请连接USB设备至OTG!");
                    Context context = getApplicationContext();
                    Toast.makeText(context, "请连接USB设备至OTG!", Toast.LENGTH_SHORT).show();
                }
            }
        });

        mStartBtn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Context context = getApplicationContext();
                HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
                UsbDevice comDevice = null;

                if (!(deviceList.isEmpty())) {
                    Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                    boolean isHid = false;
                    boolean isComm = false;
                    boolean isSerial = false;
                    boolean isCdcData = false;

                    while (deviceIterator.hasNext()) {
                        UsbDevice device = deviceIterator.next();

                        if (device.getVendorId() == 4292 && device.getProductId() == 60000) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42151) {
                            comDevice = device;
                            isCdcData = true;
                            //isComm = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42150) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1155 && device.getProductId() == 22352) {
                            comDevice = device;
                            isHid = true;
                            break;
                        }
                    }

                    if (comDevice != null) {
                        UsbInterface usbInterface = null;
                        UsbEndpoint epBulkOut = null;
                        UsbEndpoint epBulkIn = null;
                        UsbEndpoint epIntOut = null;
                        UsbEndpoint epIntIn = null;

                        for (int itfIdx = 0; itfIdx < comDevice.getInterfaceCount(); itfIdx++) {
                            usbInterface = comDevice.getInterface(itfIdx);
                            boolean isFound = false;

                            if (isSerial && UsbConstants.USB_CLASS_VENDOR_SPEC == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            } else if (isCdcData && UsbConstants.USB_CLASS_CDC_DATA == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            } else if (isComm && UsbConstants.USB_CLASS_COMM == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            } else if (isHid && UsbConstants.USB_CLASS_HID == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            }
                            if (!isFound) {
                                continue;
                            }

                            for (int epIdx = 0; epIdx < usbInterface.getEndpointCount(); epIdx++) {
                                UsbEndpoint ep = usbInterface.getEndpoint(epIdx);

                                if (UsbConstants.USB_ENDPOINT_XFER_BULK == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        epBulkOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        epBulkIn = ep;
                                    }
                                }

                                if (UsbConstants.USB_ENDPOINT_XFER_INT == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        epIntOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        epIntIn = ep;
                                    }
                                }
                            }
                        }
                        
                        if (null != epBulkOut || null != epBulkIn) {
                            UsbDeviceConnection conn = null;

                            if (mUsbManager.hasPermission(comDevice)) {
                                conn = mUsbManager.openDevice(comDevice);

                                if (conn == null) {
                                    Toast.makeText(context, "打开USB设备失败!", Toast.LENGTH_SHORT).show();
                                } else {
                                    if (conn.claimInterface(usbInterface, true)) {
                                        /*byte[] startByte = new byte[14];
                                        startByte[0]  = 0x55; startByte[1]  = 0x00; startByte[2]  = 0x0A; startByte[3]  = 0x00;
                                        startByte[4]  = 0x00; startByte[5]  = 0x00; startByte[6]  = 0x00; startByte[7]  = 0x01;
                                        startByte[8]  = 0x41; startByte[9]  = 0x00; startByte[10] = 0x00; startByte[11] = (byte)0xE0;
                                        startByte[12] = 0x03;*/
                                        int ret = conn.bulkTransfer(epBulkOut, startDev(), startDev().length, 200);
                                        if (ret < 0) {
                                            Toast.makeText(context, "发送消息失败(" + ret + ")!", Toast.LENGTH_SHORT).show();
                                        } else {
                                            Toast.makeText(context, "发送消息成功!", Toast.LENGTH_SHORT).show();

                                            if (epBulkIn != null) {
                                                byte[] readByte = new byte[epBulkIn.getMaxPacketSize()];
                                                ret = conn.bulkTransfer(epBulkIn, readByte, readByte.length, 600);
                                                if (ret < 0) {
                                                    Toast.makeText(context, "读取消息失败(" + ret + ")!", Toast.LENGTH_SHORT).show();
                                                } else {
                                                    //mShowInfoTxt.setText(bytesToHexString(readByte));
                                                	showToast(mActivity, "消息(" + bytesToHexString(readByte,ret) + ")!");
                                                }
                                            }
                                        }
                                    } else {
                                        Toast.makeText(context, "无法打开连接通道!", Toast.LENGTH_SHORT).show();
                                    }
                                    conn.releaseInterface(usbInterface);
                                    conn.close();
                                }
                            } else {
                                mUsbManager.requestPermission(comDevice, mPermissionIntent);
                                Toast.makeText(context, "无法USB权限，向用户获取!", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(context, "没有找到Bulk端点!", Toast.LENGTH_SHORT).show();
                        }
			}
                }
               }
		});
        mErollBtn.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Context context = getApplicationContext();
                HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
                UsbDevice comDevice = null;

                if (mIsStopFlag) {
                    Toast.makeText(context, "已设置停止状态!", Toast.LENGTH_SHORT).show();
                }

                if (!(deviceList.isEmpty())) {
                    Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                    boolean isHid = false;
                    boolean isComm = false;
                    boolean isSerial = false;
                    boolean isCdcData = false;

                    while (deviceIterator.hasNext()) {
                        UsbDevice device = deviceIterator.next();

                        if (device.getVendorId() == 4292 && device.getProductId() == 60000) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42151) {
                            comDevice = device;
                            isCdcData = true;
                            //isComm = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42150) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1155 && device.getProductId() == 22352) {
                            comDevice = device;
                            isHid = true;
                            break;
                        }
                    }

                    if (comDevice != null) {
                        for (int itfIdx = 0; itfIdx < comDevice.getInterfaceCount(); itfIdx++) {
                            mErollUsbIntFace = comDevice.getInterface(itfIdx);
                            boolean isFound = false;

                            if (isSerial && UsbConstants.USB_CLASS_VENDOR_SPEC == mErollUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isCdcData && UsbConstants.USB_CLASS_CDC_DATA == mErollUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isComm && UsbConstants.USB_CLASS_COMM == mErollUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isHid && UsbConstants.USB_CLASS_HID == mErollUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            }
                            if (!isFound) {
                                continue;
                            }

                            for (int epIdx = 0; epIdx < mErollUsbIntFace.getEndpointCount(); epIdx++) {
                                UsbEndpoint ep = mErollUsbIntFace.getEndpoint(epIdx);

                                if (UsbConstants.USB_ENDPOINT_XFER_BULK == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        mErollUsbEpBulkOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        mErollUsbEpBulkIn = ep;
                                    }
                                }
                            }
                        }

                        if (null != mErollUsbEpBulkIn) {
                            if (mUsbManager.hasPermission(comDevice)) {
                                mErollUsbCon = mUsbManager.openDevice(comDevice);

                                if (mErollUsbCon == null) {
                                    Toast.makeText(context, "打开USB设备失败!", Toast.LENGTH_SHORT).show();
                                } else {
                                    //ProgressDialog.show(mActivity, "OTG:", "读取消息中", false, true);
                                	photoFilePath = Environment.getExternalStorageDirectory().getPath();// 获取sd卡目录的路径
                                    photoFilePath += "/test.txt";
                                    new Thread(new Runnable() {
                                        @Override
                                        public void run() {
                                        int ret = mErollUsbCon.bulkTransfer(mErollUsbEpBulkOut, regIris(), regIris().length, 200);
                                        if (ret < 0) {
                                        	showToast(mActivity, "发送注册失败(" + ret + ")!");
                                        } else {
                                        	//showToast(mActivity, "读取消息成功(" + ret + ")!");
                                            int ret1 = mErollUsbCon.bulkTransfer(mErollUsbEpBulkOut, preView(), preView().length, 200);
                                            if (ret1 < 0) {
                                            	showToast(mActivity, "发送消息失败(" + ret + ")!");
                                            } else {
                                            	showToast(mActivity, "发送预览消息成功!");
                                            }

                                            Process.setThreadPriority(Process.THREAD_PRIORITY_URGENT_AUDIO);
                                            if (mErollUsbCon.claimInterface(mErollUsbIntFace, true)) {
                                                showToast(mActivity, "开始读取消息");
                                                byte[] readByte = new byte[mErollUsbEpBulkIn.getMaxPacketSize()];
                                                int num = 0;
                                                int count = 0;
                                                int ret2 = 0;
                                                int get = 0;
                                                int maxLen = 0;
                                                int infolen = 0;
                                                int bmpLen = 0;
                                                byte[] bmp = new byte[499*104+382];
                                                String bmpname = "/sdcard/test0.bmp";
                                                FileOutputStream outputStream = null;
                                                String msg = null;
                                                try {
                                                    outputStream = openFileOutput("/sdcard/test.txt", Context.MODE_APPEND);
                                                } catch (Exception e) {
                                                    e.printStackTrace();
                                                }
                                                while (true) {
                                                	ret2 = mErollUsbCon.bulkTransfer(mErollUsbEpBulkIn, readByte, readByte.length, 0);
                                                    if (ret2 <= 0) {
                                                        //showToast(mActivity, "读取消息失败(" + ret2 + ")!");
                                                    } else {
                                                    	num += ret2;
                                                    	//showToast(mActivity, num+"读取("+get+")结果("+ret2+")(" +bytesToHexString(readByte, ret2) + ")!");
                                                    	if(ret2 == 512 || ret2 == 395)
                                                    	{
                                                    		maxLen = Integer.parseInt(bytesToHexString(readByte).substring(10, 14), 16);
                                                    		infolen = Integer.parseInt(bytesToHexString(readByte).substring(18, 22), 16);
                                                    	}else if(ret2 == 409)
                                                    	{
                                                    		maxLen = Integer.parseInt(bytesToHexString(readByte).substring(10, 14), 16);
                                                    		infolen = Integer.parseInt(bytesToHexString(readByte).substring(18, 22), 16);
                                                    	}else if(ret2 == 14)
                                                    	{
                                                    		maxLen = Integer.parseInt(bytesToHexString(readByte).substring(10, 14), 16);
                                                    		infolen = Integer.parseInt(bytesToHexString(readByte).substring(18, 22), 16);
                                                    	}
                                                    	//showToast(mActivity, ret2+"读取("+maxLen+")结果("+infolen+")(" +bytesToHexString(readByte, ret2) + ")!");
                                                    	//msg = ret2+"("+get+")("+maxLen+")("+infolen+")(" +bytesToHexString(readByte, ret2) + " \n";
                                                    	System.arraycopy(readByte, 11, bmp, 0, infolen);
                                                    	//msg = bytesToHexString(readByte, ret2) + " \n";
                                                    	//saveFile("/sdcard/test.txt",msg.getBytes(),msg.length());
                                                    	
                                                    	saveFile(bmpname,bmp,infolen);
                                                       /*try {
                                                            outputStream.write(msg.getBytes());
                                                            outputStream.flush();
                                                        } catch (Exception e) {
                                                            e.printStackTrace();
                                                        }*/
                                                    	/*if(ret2 == 14)
                                                    	{
                                                    		mErollFlag = true;
                                                    		//showToast(mActivity, "注册结果(" +bytesToHexString(readByte, ret2).substring(790, 818) + ")!");
                                                    	}else*/{
	                                                    	//byte[] newBy = new byte[ret2 - 11 - 2];
	                                                    	get = previewNub++;
	                                                     
	                                                        /*System.arraycopy(readByte, 11, newBy, 0, newBy.length);
	                                                        System.arraycopy(newBy, 0, bmpMaxPack, newBy.length * get, newBy.length);*/
	                                                        if(get == 104) {
	                                                        	//saveFile("/sdcard/test.bmp",bmp,bmp.length);
	                                                        	/*try {
																	outputStream.close();
																} catch (IOException e) {
																	// TODO Auto-generated catch block
																	e.printStackTrace();
																}*/
	                                                        	bmpname = "/sdcard/test"+(++count)+".bmp";
	                                                        	//return;
	                                                        	if(mErollFlag)
	                                                        		break;
	                                                        	showToast(mActivity, num+"读取("+count+++")结果("+ret2+")");
	                                                        	previewNub = 0;
	                                                        	num = 0;
		                                                        ret1 = mErollUsbCon.bulkTransfer(mErollUsbEpBulkOut, preView(), preView().length, 200);
	                                                            if (ret1 < 0) {
	                                                            	showToast(mActivity, "发送消息失败(" + ret + ")!");
	                                                            } else {
	                                                            	//showToast(mActivity, "发送预览消息成功!");
	                                                            }
	                                                        }
                                                    	}
                                                    }
                                                }
                                                //showToast(mActivity, "停止读取消息");
                                            } else {
                                                showToast(mActivity, "无法打开连接通道!");
                                            }
                                            mRcvUsbCon.releaseInterface(mRcvUsbIntFace);
                                            mRcvUsbCon.close();
                                            mRcvUsbIntFace = null;
                                            mRcvUsbCon = null;
                                        }
                                        }
                                    }, "Receive").start();
                                }
                            } else {
                                mUsbManager.requestPermission(comDevice, mPermissionIntent);
                                Toast.makeText(context, "无法USB权限，向用户获取!", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(context, "没有找到Bulk端点!", Toast.LENGTH_SHORT).show();
                        }
                    } else {
                        Toast.makeText(context, "没有找到串口设备!", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(context, "请连接USB设备至OTG!", Toast.LENGTH_SHORT).show();
                }
            }
		});
        mSingleSendBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Context context = getApplicationContext();
                HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
                UsbDevice comDevice = null;

                if (!(deviceList.isEmpty())) {
                    Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                    boolean isHid = false;
                    boolean isComm = false;
                    boolean isSerial = false;
                    boolean isCdcData = false;

                    while (deviceIterator.hasNext()) {
                        UsbDevice device = deviceIterator.next();

                        if (device.getVendorId() == 4292 && device.getProductId() == 60000) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42151) {
                            comDevice = device;
                            isCdcData = true;
                            //isComm = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42150) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1155 && device.getProductId() == 22352) {
                            comDevice = device;
                            isHid = true;
                            break;
                        }
                    }

                    if (comDevice != null) {
                        UsbInterface usbInterface = null;
                        UsbEndpoint epBulkOut = null;
                        UsbEndpoint epBulkIn = null;
                        UsbEndpoint epIntOut = null;
                        UsbEndpoint epIntIn = null;

                        for (int itfIdx = 0; itfIdx < comDevice.getInterfaceCount(); itfIdx++) {
                            usbInterface = comDevice.getInterface(itfIdx);
                            boolean isFound = false;

                            if (isSerial && UsbConstants.USB_CLASS_VENDOR_SPEC == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            } else if (isCdcData && UsbConstants.USB_CLASS_CDC_DATA == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            } else if (isComm && UsbConstants.USB_CLASS_COMM == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            } else if (isHid && UsbConstants.USB_CLASS_HID == usbInterface.getInterfaceClass()) {
                                isFound = true;
                            }
                            if (!isFound) {
                                continue;
                            }

                            for (int epIdx = 0; epIdx < usbInterface.getEndpointCount(); epIdx++) {
                                UsbEndpoint ep = usbInterface.getEndpoint(epIdx);

                                if (UsbConstants.USB_ENDPOINT_XFER_BULK == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        epBulkOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        epBulkIn = ep;
                                    }
                                }

                                if (UsbConstants.USB_ENDPOINT_XFER_INT == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        epIntOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        epIntIn = ep;
                                    }
                                }
                            }
                        }

                        if (null != epBulkOut) {
                            UsbDeviceConnection conn = null;

                            if (mUsbManager.hasPermission(comDevice)) {
                                conn = mUsbManager.openDevice(comDevice);

                                if (conn == null) {
                                    Toast.makeText(context, "打开USB设备失败!", Toast.LENGTH_SHORT).show();
                                } else {
                                    if (conn.claimInterface(usbInterface, true)) {
                                        //String msgStr = "Hello, World!\n";
                                        //byte[] msgByte = msgStr.getBytes();
                                        //55 00 0a 00
                                        //00 00 00 01
                                        //41 00 00 e0
                                        //03
                                        //byte[] msgByte = new byte[13];
                                        //msgByte[0]  = 0x55; msgByte[1]  = 0x00; msgByte[2]  = 0x0A; msgByte[3]  = 0x00;
                                        //msgByte[4]  = 0x00; msgByte[5]  = 0x00; msgByte[6]  = 0x00; msgByte[7]  = 0x01;
                                        //msgByte[8]  = 0x41; msgByte[9]  = 0x00; msgByte[10] = 0x00; msgByte[11] = (byte)0xE0;
                                        //msgByte[12] = 0x03;
                                        //55 00 0b 00
                                        //00 00 00 01
                                        //53 00 01 00
                                        //f2 03
                                        byte[] msgByte = new byte[14];
                                        msgByte[0]  = 0x55; msgByte[1]  = 0x00; msgByte[2]  = 0x0B; msgByte[3]  = 0x00;
                                        msgByte[4]  = 0x00; msgByte[5]  = 0x00; msgByte[6]  = 0x00; msgByte[7]  = 0x01;
                                        msgByte[8]  = 0x53; msgByte[9]  = 0x00; msgByte[10] = 0x01; msgByte[11] = 0x00;
                                        msgByte[12] = (byte)0xF2; msgByte[13] = 0x03;
                                        int ret = conn.bulkTransfer(epBulkOut, msgByte, msgByte.length, 200);
                                        if (ret < 0) {
                                            Toast.makeText(context, "发送消息失败(" + ret + ")!", Toast.LENGTH_SHORT).show();
                                        } else {
                                            Toast.makeText(context, "发送消息成功!", Toast.LENGTH_SHORT).show();

                                            if (epBulkIn != null) {
                                                byte[] readByte = new byte[epBulkIn.getMaxPacketSize()];
                                                ret = conn.bulkTransfer(epBulkIn, readByte, readByte.length, 600);
                                                if (ret < 0) {
                                                    Toast.makeText(context, "读取消息失败(" + ret + ")!", Toast.LENGTH_SHORT).show();
                                                } else {
                                                    mShowInfoTxt.setText(new String(readByte));
                                                }
                                            }
                                        }
                                    } else {
                                        Toast.makeText(context, "无法打开连接通道!", Toast.LENGTH_SHORT).show();
                                    }
                                    conn.releaseInterface(usbInterface);
                                    conn.close();
                                }
                            } else {
                                mUsbManager.requestPermission(comDevice, mPermissionIntent);
                                Toast.makeText(context, "无法USB权限，向用户获取!", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(context, "没有找到Bulk端点!", Toast.LENGTH_SHORT).show();
                        }

                        if (null != epIntOut) {
                            UsbDeviceConnection conn = null;

                            if (mUsbManager.hasPermission(comDevice)) {
                                conn = mUsbManager.openDevice(comDevice);

                                if (conn == null) {
                                    Toast.makeText(context, "打开USB设备失败!", Toast.LENGTH_SHORT).show();
                                } else {
                                    if (conn.claimInterface(usbInterface, true)) {
                                        String msgStr = "Hello, World!\n";
                                        byte[] msgByte = msgStr.getBytes();
                                        int ret = conn.bulkTransfer(epIntOut, msgByte, msgByte.length, 200);
                                        if (ret < 0) {
                                            Toast.makeText(context, "发送消息失败(" + ret + ")!", Toast.LENGTH_SHORT).show();
                                        } else {
                                            Toast.makeText(context, "发送消息成功!", Toast.LENGTH_SHORT).show();

                                            if (epIntIn != null) {
                                                byte[] readByte = new byte[epIntIn.getMaxPacketSize()];
                                                ret = conn.bulkTransfer(epIntIn, readByte, readByte.length, 1000);
                                                if (ret < 0) {
                                                    Toast.makeText(context, "读取消息失败(" + ret + ")!", Toast.LENGTH_SHORT).show();
                                                } else {
                                                    mShowInfoTxt.setText(new String(readByte));
                                                }
                                            }
                                        }
                                    } else {
                                        Toast.makeText(context, "无法打开连接通道!", Toast.LENGTH_SHORT).show();
                                    }
                                    conn.releaseInterface(usbInterface);
                                    conn.close();
                                }
                            } else {
                                mUsbManager.requestPermission(comDevice, mPermissionIntent);
                                Toast.makeText(context, "无法USB权限，向用户获取!", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(context, "没有找到Int端点!", Toast.LENGTH_SHORT).show();
                        }
                    } else {
                        Toast.makeText(context, "没有找到串口设备!", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(context, "请连接USB设备至OTG!", Toast.LENGTH_SHORT).show();
                }
            }
        });

        mSendBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Context context = getApplicationContext();
                HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
                UsbDevice comDevice = null;

                if (mIsStopFlag) {
                    Toast.makeText(context, "已设置停止状态!", Toast.LENGTH_SHORT).show();
                }

                if (!(deviceList.isEmpty())) {
                    Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                    boolean isHid = false;
                    boolean isComm = false;
                    boolean isSerial = false;
                    boolean isCdcData = false;

                    while (deviceIterator.hasNext()) {
                        UsbDevice device = deviceIterator.next();

                        if (device.getVendorId() == 4292 && device.getProductId() == 60000) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42151) {
                            comDevice = device;
                            isCdcData = true;
                            //isComm = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42150) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1155 && device.getProductId() == 22352) {
                            comDevice = device;
                            isHid = true;
                            break;
                        }
                    }

                    if (comDevice != null) {
                        for (int itfIdx = 0; itfIdx < comDevice.getInterfaceCount(); itfIdx++) {
                            mSendUsbIntFace = comDevice.getInterface(itfIdx);
                            boolean isFound = false;

                            if (isSerial && UsbConstants.USB_CLASS_VENDOR_SPEC == mSendUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isCdcData && UsbConstants.USB_CLASS_CDC_DATA == mSendUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isComm && UsbConstants.USB_CLASS_COMM == mSendUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isHid && UsbConstants.USB_CLASS_HID == mSendUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            }
                            if (!isFound) {
                                continue;
                            }

                            for (int epIdx = 0; epIdx < mSendUsbIntFace.getEndpointCount(); epIdx++) {
                                UsbEndpoint ep = mSendUsbIntFace.getEndpoint(epIdx);

                                if (UsbConstants.USB_ENDPOINT_XFER_BULK == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        mSendUsbEpBulkOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        mSendUsbEpBulkIn = ep;
                                    }
                                }
                            }
                        }

                        if (null != mSendUsbEpBulkOut) {
                            if (mUsbManager.hasPermission(comDevice)) {
                                mSendUsbCon = mUsbManager.openDevice(comDevice);

                                if (mUsbManager == null) {
                                    Toast.makeText(context, "打开USB设备失败!", Toast.LENGTH_SHORT).show();
                                } else {
                                    ProgressDialog.show(mActivity, "OTG:", "发送消息中", false, true);
                                    new Thread(new Runnable() {
                                        @Override
                                        public void run() {
                                            Process.setThreadPriority(-19);
                                            if (mSendUsbCon.claimInterface(mSendUsbIntFace, true)) {
                                                showToast(mActivity, "开始发送消息!");
                                                int sendSize = mSendUsbEpBulkOut.getMaxPacketSize();
                                                byte[] sendByte = new byte[sendSize];
                                                for (int i = 0; i < sendSize; i++) {
                                                    sendByte[i] = 0x31;
                                                }
                                                sendByte[sendSize - 2] = '\n';
                                                sendByte[sendSize - 1] = '\n';
                                                while (true) {
                                                    if (mIsStopFlag) {
                                                        break;
                                                    }
                                                    int ret = mSendUsbCon.bulkTransfer(mSendUsbEpBulkOut, sendByte, sendByte.length, 1000);
                                                    if (ret < 0) {
                                                        showToast(mActivity, "发送消息失败(" + ret + ")!");
                                                    } else {
                                                        //showToast(mActivity, "发送消息成功!");
                                                    }
                                                }
                                                showToast(mActivity, "停止发送消息!");
                                            } else {
                                                showToast(mActivity, "无法打开连接通道!");
                                            }
                                            mSendUsbCon.releaseInterface(mSendUsbIntFace);
                                            mSendUsbCon.close();
                                            mSendUsbIntFace = null;
                                            mSendUsbCon = null;
                                        }
                                    }, "Send").start();
                                }
                            } else {
                                mUsbManager.requestPermission(comDevice, mPermissionIntent);
                                Toast.makeText(context, "无法USB权限，向用户获取!", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(context, "没有找到Bulk端点!", Toast.LENGTH_SHORT).show();
                        }
                    } else {
                        Toast.makeText(context, "没有找到串口设备!", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(context, "请连接USB设备至OTG!", Toast.LENGTH_SHORT).show();
                }
            }
        });

        mReceiveBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Context context = getApplicationContext();
                HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
                UsbDevice comDevice = null;

                if (mIsStopFlag) {
                    Toast.makeText(context, "已设置停止状态!", Toast.LENGTH_SHORT).show();
                }

                if (!(deviceList.isEmpty())) {
                    Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
                    boolean isHid = false;
                    boolean isComm = false;
                    boolean isSerial = false;
                    boolean isCdcData = false;

                    while (deviceIterator.hasNext()) {
                        UsbDevice device = deviceIterator.next();

                        if (device.getVendorId() == 4292 && device.getProductId() == 60000) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42151) {
                            comDevice = device;
                            isCdcData = true;
                            //isComm = true;
                            break;
                        }

                        if (device.getVendorId() == 1317 && device.getProductId() == 42150) {
                            comDevice = device;
                            isSerial = true;
                            break;
                        }

                        if (device.getVendorId() == 1155 && device.getProductId() == 22352) {
                            comDevice = device;
                            isHid = true;
                            break;
                        }
                    }

                    if (comDevice != null) {
                        for (int itfIdx = 0; itfIdx < comDevice.getInterfaceCount(); itfIdx++) {
                            mRcvUsbIntFace = comDevice.getInterface(itfIdx);
                            boolean isFound = false;

                            if (isSerial && UsbConstants.USB_CLASS_VENDOR_SPEC == mRcvUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isCdcData && UsbConstants.USB_CLASS_CDC_DATA == mRcvUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isComm && UsbConstants.USB_CLASS_COMM == mRcvUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            } else if (isHid && UsbConstants.USB_CLASS_HID == mRcvUsbIntFace.getInterfaceClass()) {
                                isFound = true;
                            }
                            if (!isFound) {
                                continue;
                            }

                            for (int epIdx = 0; epIdx < mRcvUsbIntFace.getEndpointCount(); epIdx++) {
                                UsbEndpoint ep = mRcvUsbIntFace.getEndpoint(epIdx);

                                if (UsbConstants.USB_ENDPOINT_XFER_BULK == ep.getType()) {
                                    if (UsbConstants.USB_DIR_OUT == ep.getDirection()) {
                                        mRcvUsbEpBulkOut = ep;
                                    } else if (UsbConstants.USB_DIR_IN == ep.getDirection()) {
                                        mRcvUsbEpBulkIn = ep;
                                    }
                                }
                            }
                        }

                        if (null != mRcvUsbEpBulkIn) {
                            if (mUsbManager.hasPermission(comDevice)) {
                                mRcvUsbCon = mUsbManager.openDevice(comDevice);

                                if (mRcvUsbCon == null) {
                                    Toast.makeText(context, "打开USB设备失败!", Toast.LENGTH_SHORT).show();
                                } else {
                                    //ProgressDialog.show(mActivity, "OTG:", "读取消息中", false, true);
                                    new Thread(new Runnable() {
                                        @Override
                                        public void run() {
                                            Process.setThreadPriority(Process.THREAD_PRIORITY_URGENT_AUDIO);
                                            if (mRcvUsbCon.claimInterface(mRcvUsbIntFace, true)) {
                                                showToast(mActivity, "开始读取消息");
                                                byte[] readByte = new byte[mRcvUsbEpBulkIn.getMaxPacketSize()];
                                                while (true) {
                                                    if (mIsStopFlag) {
                                                        break;
                                                    }

                                                    /*
                                                    int inMax = mRcvUsbEpBulkIn.getMaxPacketSize();
                                                    ByteBuffer byteBuffer = ByteBuffer.allocate(inMax);
                                                    UsbRequest usbRequest = new UsbRequest();
                                                    usbRequest.initialize(mRcvUsbCon, mRcvUsbEpBulkIn);
                                                    usbRequest.queue(byteBuffer, inMax);
                                                    if (mRcvUsbCon.requestWait() == usbRequest) {
                                                        //
                                                    }
                                                    */

                                                    int ret = mRcvUsbCon.bulkTransfer(mRcvUsbEpBulkIn, readByte, readByte.length, 200);
                                                    if (ret <= 0) {
                                                        showToast(mActivity, "读取消息失败(" + ret + ")!");
                                                    } else {
                                                    	byte[] newBy = new byte[ret - 11 - 2];
                                                        int get = previewNub++;
                                                     
                                                        System.arraycopy(readByte, 11, newBy, 0, newBy.length);
                                                        System.arraycopy(newBy, 0, bmpMaxPack, newBy.length * get, newBy.length);
                                                        if(get == 104)
                                                        	previewNub = 0;
                                                        showToast(mActivity, get+"消息("+ret+")(" + bytesToHexString(readByte,ret) + ")!");
                                                    }
                                                }
                                                showToast(mActivity, "停止读取消息");
                                            } else {
                                                showToast(mActivity, "无法打开连接通道!");
                                            }
                                            mRcvUsbCon.releaseInterface(mRcvUsbIntFace);
                                            mRcvUsbCon.close();
                                            mRcvUsbIntFace = null;
                                            mRcvUsbCon = null;
                                        }
                                    }, "Receive").start();
                                }
                            } else {
                                mUsbManager.requestPermission(comDevice, mPermissionIntent);
                                Toast.makeText(context, "无法USB权限，向用户获取!", Toast.LENGTH_SHORT).show();
                            }
                        } else {
                            Toast.makeText(context, "没有找到Bulk端点!", Toast.LENGTH_SHORT).show();
                        }
                    } else {
                        Toast.makeText(context, "没有找到串口设备!", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(context, "请连接USB设备至OTG!", Toast.LENGTH_SHORT).show();
                }
            }
        });

        mStopBtn.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Context context = getApplicationContext();

                if (mIsStopFlag) {
                    mIsStopFlag = false;
                    Toast.makeText(context, "已设置开始状态!", Toast.LENGTH_SHORT).show();
                } else {
                    mIsStopFlag = true;
                    Toast.makeText(context, "已设置停止状态!", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
    
    private void showToast(final Activity ctx, final String msg) {
        // 判断是在子线程，还是主线程
        if ("main".equals(Thread.currentThread().getName())){
            if (mMyToast == null) {
                mMyToast = Toast.makeText(ctx, msg, Toast.LENGTH_SHORT);
            } else {
                mMyToast.setText(msg);
            }
            mMyToast.show();
        } else {
            // 子线程
            ctx.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (mMyToast == null) {
                        mMyToast = Toast.makeText(ctx, msg, Toast.LENGTH_SHORT);
                    } else {
                        mMyToast.setText(msg);
                    }
                    mMyToast.show();
                }
            });
        }
    }

    private void sendToUsb(String content) {
//        sendbytes = content.getBytes();
//        int ret = -1;
        // 发送准备命令
//        ret = mDeviceConnection.bulkTransfer(usbEpOut, sendbytes, sendbytes.length, 5000);
//        showTmsg("指令已经发送！");
        // 接收发送成功信息(相当于读取设备数据)
//        receiveytes = new byte[128];   //根据设备实际情况写数据大小
//        ret = mDeviceConnection.bulkTransfer(usbEpIn, receiveytes, receiveytes.length, 10000);
//        result_tv.setText(String.valueOf(ret));
//        Toast.makeText(this, String.valueOf(ret), Toast.LENGTH_SHORT).show();
    }

    private void readFromUsb() {
        //读取数据2
//        int outMax = usbEpOut.getMaxPacketSize();
//        int inMax = usbEpIn.getMaxPacketSize();
//        ByteBuffer byteBuffer = ByteBuffer.allocate(inMax);
//        UsbRequest usbRequest = new UsbRequest();
//        usbRequest.initialize(mDeviceConnection, usbEpIn);
//        usbRequest.queue(byteBuffer, inMax);
//        if (mDeviceConnection.requestWait() == usbRequest) {
//            byte[] retData = byteBuffer.array();
//            try {
//                showTmsg("收到数据：" + new String(retData, "UTF-8"));
//            } catch (UnsupportedEncodingException e) {
//                e.printStackTrace();
//            }
//        }
    }

    private void acquireWakeLock() {
        final int wakeFlag = PowerManager.PARTIAL_WAKE_LOCK | PowerManager.ON_AFTER_RELEASE;
        mPowerManager = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock = mPowerManager.newWakeLock(wakeFlag, this.getClass().getCanonicalName());
        if (mWakeLock != null) {
            mWakeLock.acquire();
        }
    }

    private void releaseWakeLock() {
        if (mWakeLock != null && mWakeLock.isHeld()) {
            mWakeLock.release();
            mWakeLock = null;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        mIsStopFlag = false;
        acquireWakeLock();
        IntentFilter usbFilter = new IntentFilter();
        usbFilter.addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED);
        usbFilter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
        usbFilter.addAction(ACTION_USB_PERMISSION);
        registerReceiver(mUsbReceiver, usbFilter);
    }

    @Override
    protected void onPause() {
        super.onPause();

        mIsStopFlag = true;
        releaseWakeLock();
        unregisterReceiver(mUsbReceiver);
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
