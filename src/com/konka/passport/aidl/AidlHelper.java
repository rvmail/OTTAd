package com.konka.passport.aidl;

import java.io.StringReader;

import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.InputSource;
import org.xml.sax.XMLReader;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

import com.konka.passport.service.UserInfo;
import com.konka.passport.service.xmlData;

public class AidlHelper {
	private Context mcontext = null;
	private int iErrorCode = -1;

	private static final int WAIT_USERINFO_SERVICE_TIMER = 5000;
	private Object lock = new Object();
	private UserInfo passportaidl = null;
	private ServiceConnection conn = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			synchronized (lock) {
				passportaidl = UserInfo.Stub.asInterface(service);
				lock.notifyAll();
			}
		}

		@Override
		public void onServiceDisconnected(ComponentName name) {
			synchronized (lock) {
				passportaidl = null;
			}
		}
	};

	private boolean BindPassportAidl() {
		if (mcontext == null) {
			return false;
		}
		Intent intent = new Intent();
		intent.setAction("com.konka.passport.service.USERINFO_SERVICE");
		boolean bRet = mcontext.bindService(intent, conn,
				Service.BIND_AUTO_CREATE);
		return bRet;
	}

	private UserInfo GetPassportAidl() {
		synchronized (lock) {
			if (passportaidl == null) {
				if (BindPassportAidl()) {
					try {
						lock.wait(WAIT_USERINFO_SERVICE_TIMER);
					} catch (InterruptedException e) {
						return null;
					}
				} else {
					return null;
				}
			}
			return passportaidl;
		}
	}

	public AidlHelper(Context context) {
		mcontext = context;
		BindPassportAidl();
	}

	public void destroy() {
		if (mcontext != null && conn != null) {
			mcontext.unbindService(conn);
		}
		mcontext = null;
		passportaidl = null;
		iErrorCode = -1;
	}

	public int getErrorCode() {
		return iErrorCode;
	}

	public String getCNTV4License() {
		final int SYS_TYPE_BUSSINESS = 1;
		final int BUSSINESS_CODE = 101;

		iErrorCode = -1;

		String strLicense = null;

		try {
			xmlData xmlInfo = new xmlData();
			iErrorCode = GetPassportAidl().SendNetRequest("getcntvlicense?type=4",
					SYS_TYPE_BUSSINESS, BUSSINESS_CODE, xmlInfo);
			if (iErrorCode != 0 || xmlInfo.GetXmlData() == null) {
				return null;
			}
			ServiceXmlParseHandler xmlhander = new ServiceXmlParseHandler(
					ServiceXmlParseHandler.XMLPARSE_CNTV4);
			SAXParserFactory factory = SAXParserFactory.newInstance();
			XMLReader reader = factory.newSAXParser().getXMLReader();
			reader.setContentHandler(xmlhander);
			reader.parse(new InputSource(new StringReader(xmlInfo.GetXmlData())));
			strLicense = xmlhander.GetCNTV4License();
		} catch (Exception e) {
			return null;
		}

		return strLicense;
	}
}