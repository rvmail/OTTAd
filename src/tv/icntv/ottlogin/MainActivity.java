package tv.icntv.ottlogin;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
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

public class MainActivity extends Activity {
	private int errorCount = 0;
	private int okCount = 0;
	private Timer timer = null;
	Context mContext;
	
	Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			if (msg.what == 1) {
				//Integer result =  (Integer) msg.obj;
				String result = (String) msg.obj;
				EditText txt = (EditText)findViewById(R.id.txtlogin);
				txt.setText(result);
			} else if (msg.what == 6) {
				Integer arg1 = (Integer) msg.arg1;
				Integer arg2 = (Integer) msg.arg2;
				String error = (String)msg.obj;
				EditText txt = (EditText)findViewById(R.id.txtauto);
				txt.setText("<okCount=" + arg1.toString() + ">, <errorCount=" + arg2.toString() + ", lastErrCode=" + error + ">");
				Log.d("", "ui thread id = " + Thread.currentThread().getId());
			}
		}
	};
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.testui);
		
		// copy configure file
		String configPath = getFilesDir() + "/ini";
		File configFile = new File(configPath + "/DeviceInfo.ini");
		if (!configFile.exists()) {
			Log.d("ottlogin", "configFile : " + configFile);
			copyFileToDst("ini/DeviceInfo.ini", configFile.getPath());
		}
		
		//loginSDK.getInstance().sdkInit(getFilesDir().getPath());
		mContext= getApplicationContext();
		
		//getLoginStatus
		Button status = (Button)findViewById(R.id.buttonstatus);
		status.setOnClickListener(new Click_status());
		
		//deviceLogin
		Button login = (Button)findViewById(R.id.buttonlogin);
		login.setOnClickListener(new Click_login());
		
		//sdkExit
		Button exit = (Button)findViewById(R.id.buttonexit);
		exit.setOnClickListener(new Click_exit());
		
		//getDeviceID
		Button device = (Button)findViewById(R.id.buttondevice);
		device.setOnClickListener(new Click_device());
		
		//getPlatformID
		Button platform = (Button)findViewById(R.id.buttonplatform);
		platform.setOnClickListener(new Click_platform());
		
		//getUserID
		Button userid = (Button)findViewById(R.id.buttonuser);
		userid.setOnClickListener(new Click_userid());
		
		//auto
		Button auto = (Button)findViewById(R.id.buttonauto);
		auto.setOnClickListener(new Click_auto());
		
		//sdkInit
		Button init = (Button)findViewById(R.id.buttoninit);
		init.setOnClickListener(new Click_init());
		
		//getTemplateID
		Button template = (Button)findViewById(R.id.buttonTemplate);
		template.setOnClickListener(new Click_template());
		
		//getToken
		Button token = (Button)findViewById(R.id.buttonToken);
		token.setOnClickListener(new Click_token());
		
		//getToken
		Button STBext = (Button)findViewById(R.id.buttonSTBext);
		STBext.setOnClickListener(new Click_common());
		
		//logUpload
		Button log = (Button)findViewById(R.id.buttonlog);
		log.setOnClickListener(new Click_log());
		
		//version
		Button version = (Button)findViewById(R.id.buttonversion);
		version.setOnClickListener(new Click_version());
	}
	
	//getLoginStatus
	class Click_status implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtstatus);
			txt.setText("getLoginStatus...");
			
			String ret = loginSDK.getInstance().getLoginStatus();
			
			txt.setText(ret);
		}
	}
	
	//deviceLogin
	class Click_login implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtlogin);
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
	class Click_device implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtdevice);
			txt.setText("getDeviceID...");
			
			StringBuffer tf  = new StringBuffer();
			int ret = loginSDK.getInstance().getDeviceID(tf);
			
			txt.setText("getDeviceID return " + ret + ", and get data: " + tf.toString());
		}
	}
	
	//getPlatformID
	class Click_platform implements View.OnClickListener {
		public void onClick(View v)
		{
			EditText txt = (EditText)findViewById(R.id.txtplatform);
			txt.setText("getPlatformID...");
			
			StringBuffer tf  = new StringBuffer();
			int ret = loginSDK.getInstance().getPlatformID(tf);
			
			txt.setText("getPlatformID return " + ret + ", and get data: " + tf.toString());
		}
	}
	
	//getUserID
	class Click_userid implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtuser);
			txt.setText("getUserID...");
			
			StringBuffer tf  = new StringBuffer();
			int ret = loginSDK.getInstance().getUserID(tf);
			
			txt.setText("getUserID return " + ret + ", and get data: " + tf.toString());
		}
	}
	
	//sdkExit
	class Click_exit implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtexit);
			txt.setText("sdkExit...");
			
			boolean ret = loginSDK.getInstance().sdkExit();
			
			txt.setText("sdkExit return " + ret);
		}
	}
	
	//auto test
	class Click_auto implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtauto);
			Button b6 = (Button)findViewById(R.id.buttonauto);
			
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
					loginSDK.getInstance().sdkInit(getFilesDir().getPath(), 0, null);
					
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
	class Click_init implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtinit);
			txt.setText("sdkInit...");
			
			boolean ret = loginSDK.getInstance().sdkInit(getFilesDir().getPath(), 0, mContext);
			
			txt.setText("" + ret);
		}
	}
	
	//getTemplateID
	class Click_template implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtTemplate);
			txt.setText("getTemplateID...");
			
			StringBuffer tf  = new StringBuffer();
			int ret = loginSDK.getInstance().getTemplateID(tf);
			
			txt.setText("getTemplateID return " + ret + ", and get data: " + tf.toString());
		}
	}
	
	//getToken
	class Click_token implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtToken);
			txt.setText("getToken...");
			
			StringBuffer tf  = new StringBuffer();
			int ret = loginSDK.getInstance().getToken(tf);
			
			txt.setText("return " + ret + ", and get data: " + tf.toString());
		}
	}
	
	//logUpload
	class Click_log implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtlog);
			txt.setText("logUpload...");
			
			int ret = loginSDK.getInstance().logUpload();
			
			txt.setText("return " + ret);
		}
	}
	
	class Click_version implements View.OnClickListener {
		public void onClick(View v) {
			EditText txt = (EditText)findViewById(R.id.txtversion);
			txt.setText("getVersion...");
			
			StringBuffer tf  = new StringBuffer();
			int ret = loginSDK.getInstance().getVersion(tf);
			
			txt.setText(tf.toString());
		}
	}
	
	class Click_common implements View.OnClickListener {
		public void onClick(View v) {
			if (v.getId() == R.id.buttonSTBext) {
				EditText txt = (EditText)findViewById(R.id.txtSTBext);
				txt.setText("getSTBext...");
				
				StringBuffer tf  = new StringBuffer();
				int ret = loginSDK.getInstance().getSTBext(tf);
				
				txt.setText("return " + ret + ", and get data: " + tf.toString());
			}
		}
	}
	
	protected void onDestroy() {
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
