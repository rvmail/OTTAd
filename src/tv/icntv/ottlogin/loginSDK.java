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

import android.util.Log;

public class loginSDK {
	private static loginSDK sdk;// 实例
	private boolean isInit = false;

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
		if (sdk == null) {
			sdk = new loginSDK();
		}
		return sdk;
	}

	/**
	 * SDK初始化
	 * 
	 * @return
	 */
	public synchronized native boolean sdkInit(String path);
	
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
	 * @return true: 已认证；false：未认证
	 */
	public synchronized native boolean getLoginStatus();
	
	
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
	 * 获取服务器地址
	 * 
	 * @param data
	 * @return
	 */
	private native int getServerAddress(String type, TransferData serverAddr);

	public int getServerAddress(String type, StringBuffer serverAddr) {
		if (serverAddr == null)
			serverAddr = new StringBuffer();
		serverAddr.delete(0, serverAddr.length());

		TransferData dataVal = new TransferData();
		int val = getServerAddress(type, dataVal);

		if (dataVal != null && dataVal.getResult() != null) {
			serverAddr.append(dataVal.getResult());
		} else {
			serverAddr.append("");
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
}