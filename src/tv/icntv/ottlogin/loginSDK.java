/* Copyright 2014 Future TV, Inc.
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

package tv.icntv.ottlogin;

import java.lang.StringBuffer;

import com.shcmcc.tools.GetSysInfo;

import konka.AidlHelper;

import android.content.Context;
import android.util.Log;

public class loginSDK {
	private static final String LOG_TAG = "ottlogin";
	private static final String VERSION = "V2.0.0";
	private static final int TYPE_KONKA = 1;
	private static final int TYPE_MiGuShiXun = 2;
	private static loginSDK mInstance = null;
	private String mLicense = null;
	private GetSysInfo mSysInfo = null;

	private loginSDK() {
	}
	
	static {
		System.loadLibrary("ottlogin");
	}

	/**
	 * 静态构造方法
	 * 
	 * @return
	 */
	public synchronized static loginSDK getInstance() {
		if (mInstance == null) {
			mInstance = new loginSDK();
		}
		return mInstance;
	}

	/**
	 * SDK初始化
	 * 
	 * @return
	 */
	private synchronized native boolean init(String path, String license);
	public synchronized boolean sdkInit(String path, int type, Context context) {
		Log.i(LOG_TAG, "ottlogin jar VERSION: " + VERSION);
		
		if (type == TYPE_KONKA) {
			if (context == null) {
				Log.e(LOG_TAG, "loginSDK sdkInit context is null");
				return false;
			}
			
			if (mLicense == null || mLicense == "") {
				AidlHelper aidlhelper = new AidlHelper(context);
				mLicense = aidlhelper.getCNTV4License();
				aidlhelper.destroy();
				if (mLicense == null || mLicense == "") {
					Log.e(LOG_TAG, "loginSDK sdkInit getCNTV4License failed");
					return false;
				}
			}
		} else if (type == TYPE_MiGuShiXun) {
			if (context == null) {
				Log.e(LOG_TAG, "loginSDK sdkInit context is null");
				return false;
			}
			
			if (mSysInfo == null) {
				mSysInfo = GetSysInfo.getInstance("10086", "", context);
				if (mSysInfo == null) {
					Log.e(LOG_TAG, "mSysInfo is null");
					return false;
				}
			}
			
			mLicense = mSysInfo.getSnNum();
			if (mLicense == null || mLicense == "") {
				Log.e(LOG_TAG, "loginSDK sdkInit getSnNum failed");
				return false;
			}
			Log.d(LOG_TAG, "mLicense: " + mLicense);
				
			if (mLicense.length() == 32) {
				if (mLicense.substring(6, 8).equals("FF")) {
					mLicense = null;
				} else if (mLicense.substring(2, 4).equals("42") && mLicense.substring(10, 12).equals("01")) {
					mLicense = null;
				}
			} else {
				Log.e(LOG_TAG, "loginSDK sdkInit failed, license error");
				return false;
			}
		} else {
			mLicense = null;
		}
		
		return init(path, mLicense);
	}
	
	/**
	 * SDK退出
	 * 
	 * @return
	 */
	public synchronized native boolean sdkExit();

	/**
	 * 设备认证
	 * 
	 * @param path
	 * @return
	 */
	//private native String login();
	public synchronized native String deviceLogin();

	/**
	 * 获取认证状态
	 * 
	 * @param 
	 * @return 返回认证状态
	 */
	public synchronized native String getLoginStatus();
	
	
	/**
	 * 获取devive ID
	 * 
	 * @param deviceID
	 * @return
	 */
	private native int getDeviceID(TransferData deviceID);

	public int getDeviceID(StringBuffer deviceID) {
		if (deviceID == null)
			deviceID = new StringBuffer();
		deviceID.delete(0, deviceID.length());

		TransferData id = new TransferData();
		int val = getDeviceID(id);

		if (id != null && id.getResult() != null) {
			deviceID.append(id.getResult());
		} else {
			deviceID.append("");
		}

		return val;
	}
	
	/**
	 * 获取user ID
	 * 
	 * @param userID
	 * @return
	 */
	private native int getUserID(TransferData userID);

	public int getUserID(StringBuffer userID) {
		if (userID == null)
			userID = new StringBuffer();
		userID.delete(0, userID.length());

		TransferData id = new TransferData();
		int val = getUserID(id);

		if (id != null && id.getResult() != null) {
			userID.append(id.getResult());
		} else {
			userID.append("");
		}

		return val;
	}

	/**
	 * 获取template ID
	 * 
	 * @param templateID
	 * @return
	 */
	private native int getTemplateID(TransferData templateID);
	
	public int getTemplateID(StringBuffer templateID) {
		if (templateID == null)
			templateID = new StringBuffer();
		templateID.delete(0, templateID.length());

		TransferData id = new TransferData();
		int val = getTemplateID(id);

		if (id != null && id.getResult() != null) {
			templateID.append(id.getResult());
		} else {
			templateID.append("");
		}

		return val;
	}
	
	/**
	 * 获取platform ID
	 * 
	 * @param platformID
	 * @return
	 */
	private native int getPlatformID(TransferData platformID);

	public int getPlatformID(StringBuffer platformID) {
		if (platformID == null)
			platformID = new StringBuffer();
		platformID.delete(0, platformID.length());

		TransferData id = new TransferData();
		int val = getPlatformID(id);

		if (id != null && id.getResult() != null) {
			platformID.append(id.getResult());
		} else {
			platformID.append("");
		}

		return val;
	}
	
	/**
	 * 获取token
	 * 
	 * @param token
	 * @return
	 */
	private native int getToken(TransferData token);

	public int getToken(StringBuffer token) {
		if (token == null)
			token = new StringBuffer();
		token.delete(0, token.length());

		TransferData id = new TransferData();
		int val = getToken(id);

		if (id != null && id.getResult() != null) {
			token.append(id.getResult());
		} else {
			token.append("");
		}

		return val;
	}
	
	/**
	 * 获取服务器地址
	 * 
	 * @param data
	 * @return
	 */
	private native int getServerAddress(String name, TransferData serverAddr);

	public int getServerAddress(String name, StringBuffer serverAddr) {
		if (serverAddr == null)
			serverAddr = new StringBuffer();
		serverAddr.delete(0, serverAddr.length());

		TransferData dataVal = new TransferData();
		int val = getServerAddress(name, dataVal);

		if (dataVal != null && dataVal.getResult() != null) {
			serverAddr.append(dataVal.getResult());
		} else {
			serverAddr.append("");
		}

		return val;
	}
	
	/**
	 * 获取STBext
	 * 
	 * @param
	 * @return
	 */
	private native int getStbext(TransferData data);

	public int getSTBext(StringBuffer data) {
		if (data == null)
			data = new StringBuffer();
		data.delete(0, data.length());

		TransferData buf = new TransferData();
		int val = getStbext(buf);

		if (buf != null && buf.getResult() != null) {
			data.append(buf.getResult());
		} else {
			data.append("");
		}

		return val;
	}
	
	/**
	 * 获取版本信息
	 * 
	 * @param version
	 * @return
	 */
	private native int getVersion(TransferData version);

	public int getVersion(StringBuffer version) {
		if (version == null)
			version = new StringBuffer();
		version.delete(0, version.length());

		TransferData vers = new TransferData();
		int val = getVersion(vers);

		if (vers != null && vers.getResult() != null) {
			version.append(vers.getResult());
		} else {
			version.append("");
		}

		return val;
	}
	
	/**
	 * 日志上传
	 * 
	 * @return
	 */
	public synchronized native int logUpload();
}