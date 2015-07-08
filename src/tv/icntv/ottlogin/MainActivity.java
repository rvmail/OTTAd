package tv.icntv.ottlogin;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SimpleAdapter.ViewBinder;
import tv.icntv.ottlogin.loginSDK;

public class MainActivity extends Activity
{	
	private int errorCount = 0;
	private int okCount = 0;
	private Timer timer = null;
	
	Handler handler = new Handler()
	{
		public void handleMessage(Message msg)
		{
			if (msg.what == 1)
			{
				//Integer result =  (Integer) msg.obj;
				String result = (String) msg.obj;
				EditText txt = (EditText)findViewById(R.id.txt1);
	        	txt.setText(result);
			}
			else if (msg.what == 6)
			{
				Integer arg1 = (Integer) msg.arg1;
				Integer arg2 = (Integer) msg.arg2;
				String error = (String)msg.obj;
				EditText txt = (EditText)findViewById(R.id.txt6);
	        	txt.setText("<okCount=" + arg1.toString() + ">, <errorCount=" + arg2.toString() + ", lastErrCode=" + error + ">");
			    Log.d("", "ui thread id = " + Thread.currentThread().getId());
			}
		}
	};
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.testui);
        
        // copy configure file
        String configPath = getFilesDir() + "/ini";
        File configFile = new File(configPath + "/DeviceInfo.ini");
        if (!configFile.exists())
        {
            Log.d("ottlogin", "configFile : " + configFile);
            copyFileToDst("ini/DeviceInfo.ini", configFile.getPath());
        }
        
        loginSDK.getInstance().sdkInit(getFilesDir().getPath());
        
        //getLoginStatus
        Button bn0 = (Button)findViewById(R.id.bn0);
        bn0.setOnClickListener(new ClickB0());
        
        //deviceLogin
        Button bn1 = (Button)findViewById(R.id.bn1);
        bn1.setOnClickListener(new ClickB1());
        
        //sdkExit
        Button bn2 = (Button)findViewById(R.id.bn2);
        bn2.setOnClickListener(new ClickB2());
        
        //getDeviceID
        Button bn3 = (Button)findViewById(R.id.bn3);
        bn3.setOnClickListener(new ClickB3());
        
        //getPlatformID
        Button bn4 = (Button)findViewById(R.id.bn4);
        bn4.setOnClickListener(new ClickB4());
        
        //getUserID
        Button bn5 = (Button)findViewById(R.id.bn5);
        bn5.setOnClickListener(new ClickB5());
        
        //auto
        Button bn6 = (Button)findViewById(R.id.bn6);
        bn6.setOnClickListener(new ClickB6());
        
        //sdkInit
        Button init = (Button)findViewById(R.id.buttoninit);
        init.setOnClickListener(new Click_init());
        
        //getTemplateID
        Button template = (Button)findViewById(R.id.buttonTemplate);
        template.setOnClickListener(new Click_template());
        
        
    }
    
    //getLoginStatus
    class ClickB0 implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt0);
        	txt.setText("getLoginStatus...");
        	
            String ret = loginSDK.getInstance().getLoginStatus();
            
            txt.setText(ret);
        }
    }
    
    //deviceLogin
    class ClickB1 implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt1);
        	txt.setText("deviceLogin...");
        	new Thread(new Runnable() {
    			public void run() {
    	            String ret = loginSDK.getInstance().deviceLogin();
    	            Message response = Message.obtain();
    	            response.what = 1;
    	            //response.obj = Integer.valueOf(ret);
    	            response.obj = ret;
    	            handler.sendMessage(response);
    			}
    		}).start();
        	
        }
    }
    
    //getDeviceID
    class ClickB3 implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt3);
        	txt.setText("getDeviceID...");
        	
        	StringBuffer tf  = new StringBuffer();
            int ret = loginSDK.getInstance().getDeviceID(tf);
            
            txt.setText("getDeviceID return " + ret + ", and get data: " + tf.toString());
        }
    }
    
    //getPlatformID
    class ClickB4 implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt4);
        	txt.setText("getPlatformID...");
        	
        	StringBuffer tf  = new StringBuffer();
            int ret = loginSDK.getInstance().getPlatformID(tf);
            
            txt.setText("getPlatformID return " + ret + ", and get data: " + tf.toString());
        }
    }
    
    //getUserID
    class ClickB5 implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt5);
        	txt.setText("getUserID...");
        	
        	StringBuffer tf  = new StringBuffer();
            int ret = loginSDK.getInstance().getUserID(tf);
            
            txt.setText("getUserID return " + ret + ", and get data: " + tf.toString());
        }
    }
    
    //sdkExit
    class ClickB2 implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt2);
        	txt.setText("sdkExit...");
        	
            boolean ret = loginSDK.getInstance().sdkExit();
            
            txt.setText("sdkExit return " + ret);
        }
    }
    
    //auto test
    class ClickB6 implements View.OnClickListener
    {
    	public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txt6);
        	Button b6 = (Button)findViewById(R.id.bn6);
        	
        	if (timer == null) {
        		timer = new Timer();
        		txt.setText("start...");
        		b6.setText("auto on");
        		
        	} else {
        		txt.setText("Press [auto] to start");
        		timer.cancel();
        		timer = null;
        		b6.setText("auto off");
        		return;
        	}
        	
        	timer.schedule(new TimerTask() {
				@Override
				public void run() {
					Message resp = Message.obtain();
					resp.what = 6;
					
					loginSDK.getInstance().sdkExit();
					loginSDK.getInstance().sdkInit(getFilesDir().getPath());
					
					String ret = loginSDK.getInstance().deviceLogin();
					if (ret.compareTo("111") == 0 || ret.compareTo("110") == 0){
						resp.arg1 = ++okCount;
						resp.arg2 = errorCount;
					}
					else {
						resp.arg1 = okCount;
						resp.arg2 = ++errorCount;
					}
					Log.d("", "sub thread id = " + Thread.currentThread().getId());
					//resp.obj = Integer.valueOf(ret);
					resp.obj = ret;
					handler.sendMessage(resp);
				}
			}, 0, 5000);
            
        }
    }
    
    //sdkInit
    class Click_init implements View.OnClickListener
    {
        public void onClick(View v)
        {
        	EditText txt = (EditText)findViewById(R.id.txtinit);
        	txt.setText("sdkInit...");
        	
            boolean ret = loginSDK.getInstance().sdkInit(getFilesDir().getPath());
            
            txt.setText("" + ret);
        }
    }
    
    //getTemplateID
    class Click_template implements View.OnClickListener
    {
    	public void onClick(View v)
    	{
    		EditText txt = (EditText)findViewById(R.id.txtTemplate);
        	txt.setText("getTemplateID...");
        	
        	StringBuffer tf  = new StringBuffer();
            int ret = loginSDK.getInstance().getTemplateID(tf);
            
            txt.setText("getTemplateID return " + ret + ", and get data: " + tf.toString());
    	}
    	
    }
    
    protected void onDestroy() 
    {
    	Log.d("loginsdk", "onDestroy");
    	loginSDK.getInstance().sdkExit();
    	if (timer != null){
    		timer.cancel();
        	timer = null;
    	}
    	
    	super.onDestroy();
    }
    
    public void copyFileToDst(String srcPath, String dstPath) {
		if (dstPath == null) {
		    return;
		}
		
		AssetManager mAssetManager = null;
		if (mAssetManager == null) {
			mAssetManager = getAssets();
		}
		 
		BufferedOutputStream bos = null;
		InputStream is = null;
		try {
			// 源文件插上管子，准备读取内容
			is = mAssetManager.open(srcPath);
			// 目的文件插上管道，准备写入文件
			File dstFile = new File(dstPath);
			dstFile.getParentFile().mkdirs();
			bos = new BufferedOutputStream(new FileOutputStream(dstFile), 1024);
		    int read = 0, offset = 0;
			byte[] buffer = new byte[1024];
		    while ((read = is.read(buffer, 0, 1024)) > 0) {
		        bos.write(buffer, offset, read);	
		        offset += read;
		    }
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				bos.flush();
				bos.close();
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}
