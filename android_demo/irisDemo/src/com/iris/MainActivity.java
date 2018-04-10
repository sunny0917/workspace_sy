package com.iris;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.sykean.SyA83;

import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity {
    SurfaceView vSurfaceView = null;
    SyA83 iris;
    boolean Running;
    String TAG = "SyA83:";
    Thread Iristhread;
    Thread Iristhreadtwo;
    static private Handler mHandler;
    byte[] frame;
    byte[] Irisframe;
    byte[] Irisbackframe;
    byte[] saveframe;
    byte[] saveframe2;
    byte[] template;
    byte[] backtemplate;
    byte[] iristemplate;
    byte[] tmptemplate;

    byte[] BmpLeft;
    byte[] BmpRight;
    byte[] ErrorCode;

    byte[] BigBmpLeft;
    byte[] BigBmpRight;
    byte[] BigErrorCode;

    private Button enroll;
    private Button match;
    private Button exit;


    private Button photo;
    private Button cancle;
    private Button delet;

    private Button left;
    private Button right;
    private Button Gain;
    private Button Shutter;

    private Button lefta;
    private Button righta;
    private Button Gaina;
    private Button Shuttera;

    private Button Vflip;
    private Button Hflip;

    private Button facelighta;
    private Button facelightb;

    int enLib = 0;
    int gain    = 2;
    int shutter = 800;
    int hflip   = 0;
    int vflip   = 0;
    int camerarun   = 1;
    private TextView info;
    private TextView info2;
    boolean Enroll = false;
    int haveframeone;
    int haveframetwo;

    int tosave = 0;
    int tosave1 = 0;
    int tosave2 = 0;
    int exposure = 800;
    int lastexposure = 800;
    int auto = 1;

    int lightl = 150;
    int lightr = 150;
    int showjin  = 0;
    String dbtext;
    String Bigdbtext;

    static private Bitmap mBitmap;
    static private SurfaceHolder vSurfaceHolder = null;
    Timer timer;
    TimerTask task;
    boolean camerastatus = false;
    int distance = 0;
    boolean isPause = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        vSurfaceView = (SurfaceView) findViewById(R.id.surfaceView);

        vSurfaceHolder = vSurfaceView.getHolder();
        vSurfaceHolder.setFormat(PixelFormat.TRANSLUCENT);

        vSurfaceView.setVisibility(View.VISIBLE);
        vSurfaceView.setZOrderOnTop(true);
        info = (TextView)findViewById(R.id.info);
        info2 = (TextView)findViewById(R.id.info2);
        enroll = (Button)findViewById(R.id.enroll);
        match = (Button)findViewById(R.id.match);
        exit = (Button)findViewById(R.id.exit);


        Vflip = (Button)findViewById(R.id.vflipb);
        Hflip = (Button)findViewById(R.id.hflipb);

        Gain = (Button)findViewById(R.id.gainb);
        Shutter = (Button)findViewById(R.id.shutterb);
        left = (Button)findViewById(R.id.Left);
        right = (Button)findViewById(R.id.Right);

        Gaina = (Button)findViewById(R.id.gaina);
        Shuttera = (Button)findViewById(R.id.shuttera);
        lefta = (Button)findViewById(R.id.Lefta);
        righta = (Button)findViewById(R.id.Righta);
        cancle = (Button)findViewById(R.id.cancle);
        photo = (Button)findViewById(R.id.photo);
        delet = (Button)findViewById(R.id.delet);

        facelighta = (Button)findViewById(R.id.facelighta);
        facelightb = (Button)findViewById(R.id.facelightb);

        photo.setText("显示近");
        photo.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (showjin == 1) {
                    showjin = 0;
                    photo.setText("显示近");
                    Log.i(TAG, "显示近");
                } else {
                    photo.setText("显示远");
                    Log.i(TAG, "显示远");
                    showjin = 1;
                }
            }
        });

        cancle.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if(tosave == 1){
                    tosave = 0;
                    tosave1 = 2;
                    tosave2 = 2;
                    cancle.setText("虹膜开始拍照");
                }
                else
                {
                    cancle.setText("虹膜拍照中");
                    tosave = 1;
                    tosave1 = 0;
                    tosave2 = 0;
                }
            }
        });

        delet.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Arrays.fill(iristemplate, (byte) 0x00);
                Arrays.fill(backtemplate, (byte) 0x00);
            }
        });

        Shutter.setText("曝光减:800");
        Shutter.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (exposure > 80) {
                    exposure -= 40;
                    Shutter.setText("曝光减:" + exposure);
                    Shuttera.setText("曝光加:" + exposure);
                    iris.VideoShutter(0, exposure);
                    iris.VideoShutter(1, exposure);
                }

            }
        });

        Shuttera.setText("曝光加:800");
        Shuttera.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (exposure < 1600) {
                    exposure += 40;
                    Shuttera.setText("曝光加:" + exposure);
                    Shutter.setText("曝光减:" + exposure);
                    iris.VideoShutter(0, exposure);
                    iris.VideoShutter(1, exposure);
                }
            }
        });
        Gain.setText("增益减:2");
        Gain.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (gain > 1) {
                    gain--;
                    Gain.setText("增益减:" + gain);
                    Gaina.setText("增益加:" + gain);
                    iris.VideoGain(0, gain);
                    iris.VideoGain(1, gain);
                }
            }
        });

        Gaina.setText("增益加:2");
        Gaina.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (gain < 8) {
                    gain++;
                    Gaina.setText("增益加:" + gain);
                    Gain.setText("增益减:" + gain);
                    iris.VideoGain(0, gain);
                    iris.VideoGain(1, gain);
                }
            }
        });

        left.setText("左灯减:150");
        left.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                    iris.TurnOffLed(0, 0);
                    left.setText("左灯减:" + lightl);
                    lefta.setText("左灯加:" + lightl);
            }
        });

        right.setText("右灯减:150");
        right.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                    iris.TurnOffLed(1, 0);
                    right.setText("右灯减:" + lightr);
                    righta.setText("右灯加:" + lightr);
            }
        });

        lefta.setText("左灯加:150");
        lefta.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                    iris.TurnOffLed(0, 1);
                    lefta.setText("左灯加:" + lightl);
                    left.setText("左灯减:" + lightl);
            }
        });

        righta.setText("右灯加:150");
        righta.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                    iris.TurnOffLed(1, 1);
                    righta.setText("右灯加:" + lightr);
                    right.setText("右灯减:" + lightr);
            }
        });

        Vflip.setText("自动曝光开:");
        Shutter.setEnabled(false);
        Shuttera.setEnabled(false);
        Vflip.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                if (auto == 0) {
                    auto = 1;
                    exposure = 800;
                    iris.VideoShutter(0, lastexposure);
                    iris.VideoShutter(1, lastexposure);
                    Shuttera.setText("曝光加:" + exposure);
                    Shutter.setText("曝光减:" + exposure);
                    Shutter.setEnabled(false);
                    Shuttera.setEnabled(false);
                } else {
                    auto = 0;
                    Vflip.setText("自动曝光关:");
                    exposure = 800;
                    iris.VideoShutter(0, exposure);
                    iris.VideoShutter(1, exposure);
                    Shuttera.setText("曝光加:" + exposure);
                    Shutter.setText("曝光减:" + exposure);
                    Shutter.setEnabled(true);
                    Shuttera.setEnabled(true);
                }

            }
        });
        Hflip.setText("摄像头运行");
        Hflip.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                int ret = 0;
                isPause = true;
                if(camerarun == 1)
                {
                    camerarun = 2;
                    Hflip.setText("摄像头停止");
                }
                else
                {
                    camerarun = 3;
                    Hflip.setText("摄像头运行");
                }
                //ret = iris.VideoHflip(1, hflip);
                Log.i(TAG,"set iris camrea camerarun "+camerarun);
                ///do samething
                isPause = false;

            }
        });
        //Hflip.setEnabled(false);
        enroll.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Enroll = true;
                haveframeone = 0;
                //haveframetwo = 0;

            }
        });
        match.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                Enroll = false;
                haveframeone = 0;
                haveframetwo = 0;
            }
        });

        facelighta.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                iris.TurnOffLed(3,1);
            }
        });

        facelightb.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                iris.TurnOffLed(3,0);
            }
        });

        exit.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                haveframeone = 2;
                haveframetwo = 2;
                finish();
            }
        });
    }



    public void SimpleDraw(Bitmap bmp) {
        Canvas c = null;
        c = vSurfaceHolder.lockCanvas(null);
        c.drawColor(Color.BLACK);
        c.drawBitmap(bmp, 0, 0, null);//1080P  60
        vSurfaceHolder.unlockCanvasAndPost(c);
    }

    public void ClearDraw() {
        Canvas c = null;
        c = vSurfaceHolder.lockCanvas(null);
        c.drawColor(Color.BLACK);// 娓呴櫎鐢诲竷
        vSurfaceHolder.unlockCanvasAndPost(c);
    }

    protected void onResume() {
        super.onResume();
        iris = new SyA83();
        int ret = iris.ServerInit(0);
        Log.i(TAG, "init iris return " + ret);
        Running = true;
        haveframeone = 2;
        haveframetwo = 2;
        mBitmap = Bitmap.createBitmap(480, 270, Bitmap.Config.ARGB_8888);//720P
        frame = new byte[1920*1080];
        Irisframe = new byte[1920*1080];
        Irisbackframe = new byte[1920*1080];


        saveframe = new byte[1920*1080];
        saveframe2 = new byte[1920*1080];

        template = new byte[1024];
        backtemplate = new byte[1024];
        tmptemplate = new byte[1024];
        iristemplate = new byte[1024];
        BmpLeft = new byte[360*360];
        BmpRight = new byte[360*360];
        ErrorCode = new byte[2];

        BigBmpLeft = new byte[360*360];
        BigBmpRight = new byte[360*360];
        BigErrorCode = new byte[2];

        Iristhread = new Thread() {
            int ret = 0;
            public void run() {
                while(Running==true){
                    try{
                        if(tosave == 1)
                        {
                            if(tosave1 == 1)
                            {
                                iris.ToBmp(saveframe,0);
                                tosave1 = 0;
                            }
                        }

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        Iristhread.start();
        timer = new Timer();
        task = new TimerTask() {
            @Override
            public void run() {
                int ret = 0;
                if(true == isPause)
                {
                    return;
                }
                if(enLib == -2)
                {
                    mHandler.sendEmptyMessageDelayed(17, 0);
                    isPause = true;
                }
                distance = 21;//iris.GetDistance();
                if(distance > 20)
                {
                    if(camerastatus == false){
                        ret = iris.OpenIrisCamera();
                        Log.i(TAG,"open Iris camrea "+ret);
                        if(ret != 0) {
                            Log.i(TAG, "open iris fail ");
                        }
                        camerastatus = true;
                    }


                    if(camerastatus == true){
                        Log.i(TAG,"get iris frame");
                        ret = iris.GetIrisFrame(frame);
                        Log.i(TAG,"get iris frame finish "+ret);
                        if(ret==0)
                        {
                            if(ret == 0){
                                if(tosave == 1)
                                {
                                    if(tosave1 == 0) {
                                        System.arraycopy(frame, 0, saveframe, 0, frame.length);
                                        tosave1 = 1;
                                    }
                                }
                            }
                            if(showjin == 0) {
                                Log.i(TAG,"RatioImage iris ");
                                ret = iris.RatioImage(frame, mBitmap, 2);
                                Log.i(TAG,"RatioImage iris finish");
                                SimpleDraw(mBitmap);
                                Log.i(TAG, "SimpleDraw iris finish");
                            }
                        }
                        else
                        {
                            Log.i(TAG,"get frame fail ");
                        }
                    }
                }
                else
                {
                    if(camerastatus == true)
                    {
                        iris.CloseIrisCamera();
                        Log.i(TAG, "close iris camrea " + ret);
                        ClearDraw();
                        camerastatus = false;
                    }
                }
            }
        };
        timer.schedule(task, 200, 30);
        mHandler = new Handler(){
            public void handleMessage(Message msg){
                int count = 0;
                int index = 0;
                switch (msg.what)
                {
                    case 1:
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 2:
                        dbtext +="注册失败\n";
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 3:
                        dbtext +="单眼注册成功\n";
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 4:
                        dbtext +="双眼注册成功\n";
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 5:
                        dbtext +="识别成功\n";
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 6:
                        dbtext +="识别失败\n";
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 7:
                        dbtext +="重复注册\n";
                        count = info.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(dbtext);
                            dbtext = dbtext.substring(index+1);
                        }
                        info.setText(dbtext);
                        break;
                    case 8:
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                    case 9:
                        Bigdbtext +="注册失败\n";
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                    case 10:
                        Bigdbtext +="单眼注册成功\n";
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                    case 11:
                        Bigdbtext +="双眼注册成功\n";
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                    case 12:
                        Bigdbtext +="识别成功\n";
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                    case 13:
                        Bigdbtext +="识别失败\n";
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                    case 14:
                        Bigdbtext +="重复注册\n";
                        count = info2.getLineCount();
                        if(count >= 6)
                        {
                            index = FindFirstLine(Bigdbtext);
                            Bigdbtext = Bigdbtext.substring(index+1);
                        }
                        info2.setText(Bigdbtext);
                        break;
                }
            }
        };
    }

    private int FindFirstLine(String str)
    {
        char[] chs = str.toCharArray();
        for(int i = 0; i < chs.length; i++) {
            if(chs[i] == '\n')
            {
                return i;
            }
        }
        return 0;
    }

    private void mRebootShow() {
        AlertDialog.Builder builder;
        AlertDialog mydialog;
        builder = new AlertDialog.Builder(MainActivity.this);
        mydialog = builder
                .setTitle("库已过期，请重启系统，重新授权:")
                .setPositiveButton("确定",
                        new DialogInterface.OnClickListener() {

                            @Override
                            public void onClick(DialogInterface arg0, int arg1) {
                                // TODO Auto-generated method stub
                                finish();
                                //String[] commands = new String[] {"adb reboot" };
                                //ShellUtils.execCommand(commands, true);
                            }

                        }).create();
        mydialog.setCancelable(false);
        mydialog.show();
    }

    protected void onPause() {
        Log.e(TAG, String.format("onPause: ready to pause app\n"));
        if (timer != null) {
            timer.cancel();
            Log.e(TAG, String.format("onPause: timer finish\n"));
            timer = null;
        }
        if (task != null) {
            task.cancel();
            Log.e(TAG, String.format("onPause: task finish\n"));
            task = null;
        }

        Running = false;
        try {

            Iristhread.join(); // wait for secondary to finish
            Log.e(TAG, String.format("onPause: Iristhread finish\n"));
            Iristhread = null;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        iris.TurnOffLed(0,0);
        iris.TurnOffLed(1,0);
        iris.TurnOffLed(2,0);
        iris.TurnOffLed(3,0);
        iris.TurnOffLed(4,0);
        iris.CloseIrisCamera();
        iris.ServerRelease();
        iris = null;

        frame = null;
        Irisframe = null;
        Irisbackframe = null;
        saveframe = null;

        template = null;
        backtemplate = null;
        tmptemplate = null;
        iristemplate = null;
        BmpLeft = null;
        BmpRight = null;
        ErrorCode = null;

        BigBmpLeft = null;
        BigBmpRight = null;
        BigErrorCode = null;

        mBitmap.recycle();
        mBitmap = null;
        super.onPause();
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.

        return true;
    }

}
