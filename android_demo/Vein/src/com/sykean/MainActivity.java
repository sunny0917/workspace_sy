package com.sykean;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import org.apache.http.util.EncodingUtils;

import android.hardware.Camera.Size;
import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends Activity implements OnClickListener {

	public String TAG = "FingerVein";
	Vein vein = null;
	public Button bt_open_port;
	public Button bt_close_port;
	public Button bt_open_dev;
	public Button bt_close_dev;
	public Button bt_start_eroll;
	public Button bt_identify;
	public Button bt_get_button_state;
	public Button bt_set_led_state;
	public Button bt_get_id_exist;
	public Button bt_cancel_wait;
	public Button bt_clear_user;
	public Button bt_del_user;
	public Button bt_set_usb_mode;
	public Button bt_get_empty_id;
	public Button bt_set_baudrate;
	public Button bt_set_timeout;
	public Button bt_get_user_num;
	public Button bt_reset;
	public Button bt_get_sys_info;
	public Button bt_upload_model;
	public Button bt_download_model;
	public Button bt_grab_image;
	public Button bt_get_tp_det;
	
	public int	arBaud[]={50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200,230400,460800,500000,
			576000,921600,1000000,1152000,1500000,2000000,2500000,3000000,3500000,4000000};
	
	static{
		System.loadLibrary("SykeanFingerVein");
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		vein = new Vein();
		initView();
	}
	
	public void initView() {
		bt_open_port = (Button) findViewById(R.id.bt_open_port);
		bt_close_port = (Button) findViewById(R.id.bt_close_port);
		bt_open_dev = (Button) findViewById(R.id.bt_open_dev);
		bt_close_dev = (Button) findViewById(R.id.bt_close_dev);
		bt_start_eroll = (Button) findViewById(R.id.bt_eroll);
		bt_identify = (Button) findViewById(R.id.bt_identify);
		bt_get_button_state = (Button) findViewById(R.id.bt_get_button_state);
		bt_set_led_state = (Button) findViewById(R.id.bt_set_led_state);
		bt_get_id_exist = (Button) findViewById(R.id.bt_get_id_exist);
		bt_cancel_wait = (Button) findViewById(R.id.bt_cancel_wait);
		bt_clear_user = (Button) findViewById(R.id.bt_clear_user);
		bt_del_user = (Button) findViewById(R.id.bt_del_user);
		bt_set_usb_mode = (Button) findViewById(R.id.bt_set_usb_mode);
		bt_get_empty_id = (Button) findViewById(R.id.bt_get_empty_id);
		bt_set_baudrate = (Button) findViewById(R.id.bt_set_baudrate);
		bt_set_timeout = (Button) findViewById(R.id.bt_set_timeout);
		bt_get_user_num = (Button) findViewById(R.id.bt_get_user_num);
		bt_reset = (Button) findViewById(R.id.bt_reset);
		bt_get_sys_info = (Button) findViewById(R.id.bt_get_sys_info);
		bt_upload_model = (Button) findViewById(R.id.bt_upload_model);
		bt_download_model = (Button) findViewById(R.id.bt_download_model);
		bt_grab_image = (Button) findViewById(R.id.bt_grab_image);
		bt_get_tp_det = (Button) findViewById(R.id.bt_get_tp_det);
		
		bt_open_port.setOnClickListener(this);
		bt_close_port.setOnClickListener(this);
		bt_open_dev.setOnClickListener(this);
		bt_close_dev.setOnClickListener(this);
		bt_start_eroll.setOnClickListener(this);
		bt_identify.setOnClickListener(this);
		bt_get_button_state.setOnClickListener(this);
		bt_set_led_state.setOnClickListener(this);
		bt_get_id_exist.setOnClickListener(this);
		bt_cancel_wait.setOnClickListener(this);
		bt_clear_user.setOnClickListener(this);
		bt_del_user.setOnClickListener(this);
		bt_set_usb_mode.setOnClickListener(this);
		bt_get_empty_id.setOnClickListener(this);
		bt_set_baudrate.setOnClickListener(this);
		bt_set_timeout.setOnClickListener(this);
		bt_get_user_num.setOnClickListener(this);
		bt_reset.setOnClickListener(this);
		bt_get_sys_info.setOnClickListener(this);
		bt_upload_model.setOnClickListener(this);
		bt_download_model.setOnClickListener(this);
		bt_grab_image.setOnClickListener(this);
		bt_get_tp_det.setOnClickListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		byte result = 0x00;
		byte[] modelId = new byte[4];
		int ret = -1;
		switch (arg0.getId()) {
		case R.id.bt_get_tp_det:
			int tpDet = vein.getFingerTpDet();
			if(tpDet < 0) {
				Toast.makeText(getApplication(), "不能获得触摸感应输出", 0).show();
			}if(tpDet == 0) {
				Toast.makeText(getApplication(), "有手指触摸", 0).show();
			}if(tpDet == 1) {
				Toast.makeText(getApplication(), "没有手指触摸", 0).show();
			}
			
			break;
		case R.id.bt_open_port:
			ret = vein.openPort("/dev/ttyS7", 115200);
			if(ret < 0)
			{
				Toast.makeText(getApplication(), "openPort fail:ret = "+ret, 0).show();
			}
			break;
		case R.id.bt_close_port:
			ret = vein.closePort();
			if(ret < 0)
			{
				Toast.makeText(getApplication(), "closePort fail:ret = "+ret, 0).show();
			}
			break;
		case R.id.bt_open_dev:
			result = (byte)vein.openDev((short)0x01);
			Toast.makeText(getApplication(), "openDev: "+(result == 0x00 ? "打开设备成功":"打开设备失败,result = "+result), 0).show();
			break;
		case R.id.bt_close_dev:
			result = (byte)vein.closeDev();
			Toast.makeText(getApplication(), "closeDev: "+result, 0).show();
			break;
		case R.id.bt_eroll:
			for(int i = 0;i < 3;i++)
			{	
				result = (byte)vein.rollStep((byte)i);
				Log.e(TAG, "rollStep "+i+"---result = "+result);
				Toast.makeText(getApplication(), "rollStep: "+result, 0).show();
				if(result != 0x00)
					break;
			}
			Toast.makeText(getApplication(), "rollStep: "+result, 0).show();
			int rollId = 0;
			result = (byte)vein.rollFinish(rollId,modelId);
			Log.e(TAG, "rollFinish:result = "+result+"---modelId = "+bytesToInt(modelId,0));
			break;
		case R.id.bt_identify:
			//byte[] modelId = new byte[4];
			result = (byte)vein.identy(modelId);
			Log.e(TAG, "identy = "+result+"---modelId = "+bytesToInt(modelId,0));
			Toast.makeText(getApplication(), "identy: "+result+"---modelId = "+bytesToInt(modelId,0), 0).show();
			break;
		case R.id.bt_set_led_state:
			short color = 0;
			short flush = 0;
			result = (byte)vein.setLedState(color, flush);
			Toast.makeText(getApplication(), "setLedState: "+result, 0).show();
			break;
		case R.id.bt_get_button_state:
			byte state = (byte)vein.getButtonState();
			Toast.makeText(getApplication(), "button state: "+state, 0).show();
			break;
		case R.id.bt_get_id_exist:
			int id = 2;
			byte[] count = new byte[1];
			result = (byte)vein.getIDExist(id, count);
			Toast.makeText(getApplication(), "id exist: "+result+"---count="+count[0], 0).show();
			break;
		case R.id.bt_cancel_wait:
			result = (byte)vein.cancelWait();
			Toast.makeText(getApplication(), "cancelWait: "+result, 0).show();
			break;
		case R.id.bt_clear_user:
			result = (byte)vein.clearUser();
			Toast.makeText(getApplication(), "clearUser: "+result, 0).show();
			break;
		case R.id.bt_set_usb_mode:
			byte usbMode = 0x00;
			result = (byte)vein.setUsbMode(usbMode);
			Toast.makeText(getApplication(), "setUsbMode: "+result, 0).show();
			break;
		case R.id.bt_get_empty_id:
			result = (byte)vein.getEmptyId(modelId);
			Toast.makeText(getApplication(), "getEmptyId: "+result+" empty ID is "+bytesToInt(modelId, 0), 0).show();
			break;
		case R.id.bt_del_user:
			int ID = 8;
			result = (byte)vein.delUser(ID);
			Toast.makeText(getApplication(), "delUser: "+result, 0).show();
			break;
		case R.id.bt_set_baudrate:
			int baudrate = 115200;
			byte index = 16;
			/*for(int i = 0;i < arBaud.length;i++)
			{
				Log.e(TAG, "baudrate="+baudrate+" arBaud["+i+"]="+arBaud[i]);
				if(baudrate == arBaud[i]);
				{	
					Log.e(TAG, "eauals");
					index = (byte)i;
					break;
				}
			}*/
			Log.e(TAG, "index = "+index);
			result = (byte)vein.setBaudRate(index);
			Toast.makeText(getApplication(), "setBaudRate: "+result, 0).show();
			break;
		case R.id.bt_set_timeout:
			byte timeout = 2;//单位为秒
			result = (byte)vein.setTimeout(timeout);
			Toast.makeText(getApplication(), "setTimeout: "+result, 0).show();
			break;
		case R.id.bt_get_user_num:
			byte[] userCount = new byte[4];
			byte[] modelCount = new byte[4];
			result = (byte)vein.getUserNum(userCount, modelCount);
			Toast.makeText(getApplication(), "getUserNum: "+result+"---userCount:"+bytesToInt(userCount, 0)+"--modelCount:"+bytesToInt(modelCount, 0), 0).show();
			break;
		case R.id.bt_reset:
			result = (byte)vein.reset();
			Toast.makeText(getApplication(), "reset: "+result, 0).show();
			break;
		case R.id.bt_get_sys_info:
			byte[] sysInfo = new byte[11];
			result = (byte)vein.getSystemInfo(sysInfo);
			Toast.makeText(getApplication(), "getSystemInfo: "+result, 0).show();
			if(result == 0x00) {
				Log.e(TAG, "main version:"+sysInfo[0]);
				Log.e(TAG, "sub version:"+sysInfo[1]);
				Log.e(TAG, "device id:"+sysInfo[2]);
				Log.e(TAG, "baud rate:"+sysInfo[3]);
				Log.e(TAG, "level:0x"+sysInfo[5]+sysInfo[4]);
				Log.e(TAG, "time out:"+sysInfo[6]);
				Log.e(TAG, "check dup:"+sysInfo[7]);
				Log.e(TAG, "check same finger:"+sysInfo[8]);
				Log.e(TAG, "usb mode:"+sysInfo[9]);
				Log.e(TAG, "read error:"+sysInfo[10]);
			}
			break;
		case R.id.bt_upload_model:
			byte[] modelData = new byte[4*1024];
			int length = 0;
			try {
				length = readSDFile("/sdcard/model.txt",modelData);
				//writeSDFile("/sdcard/model1.txt", modelData, length);
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
			result = (byte)vein.uploadModel(3, modelData, length);
			Toast.makeText(getApplication(), "uploadModel: "+result, 0).show();
			break;
		case R.id.bt_download_model:
			int packSize = 128;
			//int modelSize = 3352;
			int Id = 3;
			byte[] outData = new byte[4*1024];
			int[] outSize = new int[1];
			result = (byte)vein.downloadModel(packSize,/* modelSize, */Id, outData, outSize);
			try {
				writeSDFile("/sdcard/model.txt", outData,outSize[0]);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			break;
		case R.id.bt_grab_image:
			short currnet = 0;
			result = (byte)vein.getImage(currnet);
			Toast.makeText(getApplication(), "getImage: "+result, 0).show();
			break;
		default:
			break;
		}
	}

	//读文件  
	public int readSDFile(String fileName,byte[] buffer) throws IOException {    
	  
			String res = "";
	        File file = new File(fileName);    
	  
	        FileInputStream fis = new FileInputStream(file);    
	  
	        int length = fis.available();   
	        Log.e(TAG,"length = "+length);
	        // byte [] buffer = new byte[length];   
	         fis.read(buffer);       
	  
	         res = EncodingUtils.getString(buffer, "UTF-8");   
	  
	         fis.close();       
	         return length;    
	}    
	  
	//写文件  
	public void writeSDFile(String fileName, byte[] data,int dataSize) throws IOException{    
	  
	        File file = new File(fileName);    
	  
	        FileOutputStream fos = new FileOutputStream(file);    
	  
	        //byte [] bytes = write_str.getBytes();   
	  
	        fos.write(data, 0, dataSize);
	  
	        fos.close();   
	}  
	
	public static byte[] intToBytes( int value )   
	{   
	    byte[] src = new byte[4];  
	    src[3] =  (byte) ((value>>24) & 0xFF);  
	    src[2] =  (byte) ((value>>16) & 0xFF);  
	    src[1] =  (byte) ((value>>8) & 0xFF);    
	    src[0] =  (byte) (value & 0xFF);                  
	    return src;   
	}  
	
	public static int bytesToInt(byte[] src, int offset) {  
	    int value;    
	    value = (int) ((src[offset] & 0xFF)   
	            | ((src[offset+1] & 0xFF)<<8)   
	            | ((src[offset+2] & 0xFF)<<16)   
	            | ((src[offset+3] & 0xFF)<<24));  
	    return value;  
	}  
}
