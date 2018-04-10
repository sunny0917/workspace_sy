package com.sykean;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import com.example.idcard.R;

import android.R.id;
import android.os.Bundle;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener {

	public Button bt_read_card;
	public Button bt_find_card;
	public Button bt_select_card;
	public Button bt_read;
	public TextView tv_recv_data;
	public ImageView iv_image;
	
	public IDCard idCard;
	
	static{
		System.loadLibrary("IDCard");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		idCard = new IDCard();
		idCard.openPort("/dev/ttyS2", 115200);
		
		initView();
		
	}

	private void initView() {
		bt_read_card = (Button) findViewById(R.id.bt_read_card);
		bt_find_card = (Button) findViewById(R.id.bt_find_card);
		bt_select_card = (Button) findViewById(R.id.bt_select_card);
		bt_read = (Button) findViewById(R.id.bt_read);
		tv_recv_data = (TextView) findViewById(R.id.tv_recv_data);
		iv_image = (ImageView) findViewById(R.id.iv_image);
		
		bt_read_card.setOnClickListener(this);
		bt_find_card.setOnClickListener(this);
		bt_select_card.setOnClickListener(this);
		bt_read.setOnClickListener(this);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		idCard.closePort();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
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
		int len = 0;
		byte[] txt = new byte[256];
		byte[] image = new byte[1024];
		switch (arg0.getId()) {
		case R.id.bt_read_card:
			idCard.oneKeyReadCard(txt,image);
			//tv_recv_data.setText(bytesToHexString(txt,256));
			display(txt);
			break;
		case R.id.bt_find_card:
			byte[] recvFind = new byte[1024];
			len = idCard.findCard(recvFind);
			tv_recv_data.setText(recvFind.toString());
			tv_recv_data.setText(bytesToHexString(recvFind,len));
			break;
		case R.id.bt_select_card:
			byte[] recvSelect = new byte[1024];
			idCard.selectCard(recvSelect);
			break;
		case R.id.bt_read:
			idCard.readCard(txt,image);
			//Bitmap bitmap = BitmapFactory.decodeByteArray(image, 0, 1024);
			Log.e("IDCard","len = "+image.length);
			//Log.e("IDCard",bytesToHexString(image,1024));
			
			//saveBitmap("idcard",bitmap);
			//tv_recv_data.setText(bytesToHexString(image,1024));
			display(txt);
			break;
		default:
			break;
		}
	}
	
	public void display(byte[] txt) {
		String name = new String(txt, 0, 6);
		String sex = new String(txt, 30, 2);
		String nation = new String(txt,32,4);
		String date = new String(txt,36,16);
		String addr = new String(txt,52,70);
		String ID = new String(txt,122,36);
		String police = new String(txt,158,36);
		String startDate = new String(txt,194,10);
		String endDate = new String(txt,204,20);
		String newAddr = new String(txt,224,32);
		
		tv_recv_data.setText("姓名:"+name
							+"\n性别:"+sex
							+"\n民族:"+nation
							+"\n出生日期:"+date
							+"\n住址:"+addr
							+"\n身份证号:"+ID
							+"\n签发机关:"+police
							+"\n有效起始日期:"+startDate
							+"\n有效截止日期:"+endDate
							+"\n最新住址:"+newAddr);
		
	}

	public static void saveBitmap(String picName,Bitmap bm) {
        File f = new File("sdcard/", picName);
        if (f.exists()) {
            f.delete();
        }
        try {
            FileOutputStream out = new FileOutputStream(f);
            bm.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.flush();
            out.close();
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

    }

	public static String bytesToHexString(byte[] src,int len){  
	    StringBuilder stringBuilder = new StringBuilder("");  
	    if (src == null || src.length <= 0) {  
	        return null;  
	    }  
	    for (int i = 0; i < len; i++) {  
	        int v = src[i] & 0xFF;  
	        String hv = Integer.toHexString(v);  
	        if (hv.length() < 2) {  
	            stringBuilder.append(0);  
	        }  
	        stringBuilder.append(hv+" ");  
	    }  
	    return stringBuilder.toString();  
	}  
}
