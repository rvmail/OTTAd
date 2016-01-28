/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D:\\App\\PassportTemp\\sourcecode\\trunk\\Passport\\src\\com\\konka\\passport\\service\\UserInfo.aidl
 */
package konka;

public interface UserInfo extends android.os.IInterface {
	/** Local-side IPC implementation stub class. */
	public static abstract class Stub extends android.os.Binder implements
			konka.UserInfo {
		private static final java.lang.String DESCRIPTOR = "com.konka.localserver.service.aidl.LocalServer";

		/** Construct the stub at attach it to the interface. */
		public Stub() {
			this.attachInterface(this, DESCRIPTOR);
		}

		/**
		 * Cast an IBinder object into an com.konka.passport.service.UserInfo
		 * interface, generating a proxy if needed.
		 */
		public static konka.UserInfo asInterface(android.os.IBinder obj) {
			if ((obj == null)) {
				return null;
			}
			android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
			if (((iin != null) && (iin instanceof konka.UserInfo))) {
				return ((konka.UserInfo) iin);
			}
			return new konka.UserInfo.Stub.Proxy(obj);
		}

		@Override
		public android.os.IBinder asBinder() {
			return this;
		}

		@Override
		public boolean onTransact(int code, android.os.Parcel data,
				android.os.Parcel reply, int flags)
				throws android.os.RemoteException {
			switch (code) {
			case INTERFACE_TRANSACTION: {
				reply.writeString(DESCRIPTOR);
				return true;
			}
			case TRANSACTION_SendNetRequest: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				int _arg1;
				_arg1 = data.readInt();
				int _arg2;
				_arg2 = data.readInt();
				konka.xmlData _arg3;
				if ((0 != data.readInt())) {
					_arg3 = konka.xmlData.CREATOR.createFromParcel(data);
				} else {
					_arg3 = null;
				}
				int _result = this.SendNetRequest(_arg0, _arg1, _arg2, _arg3);
				reply.writeNoException();
				reply.writeInt(_result);
				if ((_arg3 != null)) {
					reply.writeInt(1);
					_arg3.writeToParcel(reply,
							android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			case TRANSACTION_GetSerial: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _result = this.GetSerial();
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_GetSerialWithCRC: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _result = this.GetSerialWithCRC();
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_GetADOpenFlag: {
				data.enforceInterface(DESCRIPTOR);
				int _result = this.GetADOpenFlag();
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			case TRANSACTION_ReplaceUrlDomain: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				java.lang.String _result = this.ReplaceUrlDomain(_arg0);
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_CheckSignature: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				java.lang.String _arg1;
				_arg1 = data.readString();
				java.lang.String _arg2;
				_arg2 = data.readString();
				boolean _arg3;
				_arg3 = (0 != data.readInt());
				int _result = this.CheckSignature(_arg0, _arg1, _arg2, _arg3);
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			case TRANSACTION_Encode: {
				data.enforceInterface(DESCRIPTOR);
				byte[] _arg0;
				_arg0 = data.createByteArray();
				byte[] _result = this.Encode(_arg0);
				reply.writeNoException();
				reply.writeByteArray(_result);
				reply.writeByteArray(_arg0);
				return true;
			}
			case TRANSACTION_Decode: {
				data.enforceInterface(DESCRIPTOR);
				byte[] _arg0;
				_arg0 = data.createByteArray();
				byte[] _result = this.Decode(_arg0);
				reply.writeNoException();
				reply.writeByteArray(_result);
				reply.writeByteArray(_arg0);
				return true;
			}
			case TRANSACTION_cEncode: {
				data.enforceInterface(DESCRIPTOR);
				byte[] _arg0;
				_arg0 = data.createByteArray();
				byte[] _arg1;
				_arg1 = data.createByteArray();
				byte[] _result = this.cEncode(_arg0, _arg1);
				reply.writeNoException();
				reply.writeByteArray(_result);
				reply.writeByteArray(_arg0);
				reply.writeByteArray(_arg1);
				return true;
			}
			case TRANSACTION_cDecode: {
				data.enforceInterface(DESCRIPTOR);
				byte[] _arg0;
				_arg0 = data.createByteArray();
				byte[] _arg1;
				_arg1 = data.createByteArray();
				byte[] _result = this.cDecode(_arg0, _arg1);
				reply.writeNoException();
				reply.writeByteArray(_result);
				reply.writeByteArray(_arg0);
				reply.writeByteArray(_arg1);
				return true;
			}
			case TRANSACTION_SendNetRequestWithLogin: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				int _arg1;
				_arg1 = data.readInt();
				int _arg2;
				_arg2 = data.readInt();
				konka.xmlData _arg3;
				if ((0 != data.readInt())) {
					_arg3 = konka.xmlData.CREATOR.createFromParcel(data);
				} else {
					_arg3 = null;
				}
				int _result = this.SendNetRequestWithLogin(_arg0, _arg1, _arg2,
						_arg3);
				reply.writeNoException();
				reply.writeInt(_result);
				if ((_arg3 != null)) {
					reply.writeInt(1);
					_arg3.writeToParcel(reply,
							android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			case TRANSACTION_UpdateRequest: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				konka.xmlData _arg1;
				if ((0 != data.readInt())) {
					_arg1 = konka.xmlData.CREATOR.createFromParcel(data);
				} else {
					_arg1 = null;
				}
				int _result = this.UpdateRequest(_arg0, _arg1);
				reply.writeNoException();
				reply.writeInt(_result);
				if ((_arg1 != null)) {
					reply.writeInt(1);
					_arg1.writeToParcel(reply,
							android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			case TRANSACTION_GetPassportId: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _result = this.GetPassportId();
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_GetCrcCode: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _result = this.GetCrcCode();
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_CheckSignatureAndroid: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				java.lang.String _arg1;
				_arg1 = data.readString();
				java.lang.String _arg2;
				_arg2 = data.readString();
				int _result = this.CheckSignatureAndroid(_arg0, _arg1, _arg2);
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			case TRANSACTION_GetUserId: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _result = this.GetUserId();
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_GetCtrlInfoInnerApp: {
				data.enforceInterface(DESCRIPTOR);
				int _arg0;
				_arg0 = data.readInt();
				java.lang.String _result = this.GetCtrlInfoInnerApp(_arg0);
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_GetCtrlInfoThirPartyApp: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				java.lang.String _result = this.GetCtrlInfoThirPartyApp(_arg0);
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			case TRANSACTION_GetPassportInfo: {
				data.enforceInterface(DESCRIPTOR);
				konka.PassportInfo _arg0;
				if ((0 != data.readInt())) {
					_arg0 = konka.PassportInfo.CREATOR.createFromParcel(data);
				} else {
					_arg0 = null;
				}

				int _result = this.GetPassportInfo(_arg0);
				reply.writeNoException();
				reply.writeInt(_result);
				if ((_arg0 != null)) {
					reply.writeInt(1);
					_arg0.writeToParcel(reply,
							android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			case TRANSACTION_SendNetRequestWithFile: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				int _arg1;
				_arg1 = data.readInt();
				int _arg2;
				_arg2 = data.readInt();
				konka.xmlData _arg3;
				if ((0 != data.readInt())) {
					_arg3 = konka.xmlData.CREATOR.createFromParcel(data);
				} else {
					_arg3 = null;
				}
				java.lang.String _arg4;
				_arg4 = data.readString();
				int _result = this.SendNetRequestWithFile(_arg0, _arg1, _arg2,
						_arg3, _arg4);
				reply.writeNoException();
				reply.writeInt(_result);
				if ((_arg3 != null)) {
					reply.writeInt(1);
					_arg3.writeToParcel(reply,
							android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
				} else {
					reply.writeInt(0);
				}
				return true;
			}
			case TRANSACTION_LoginPassport: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				java.lang.String _arg1;
				_arg1 = data.readString();
				int _result = this.LoginPassport(_arg0, _arg1);
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			case TRANSACTION_RegisterPassport: {
				data.enforceInterface(DESCRIPTOR);
				java.lang.String _arg0;
				_arg0 = data.readString();
				java.lang.String _arg1;
				_arg1 = data.readString();
				int _result = this.RegisterPassport(_arg0, _arg1);
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			case TRANSACTION_LogoutPassport: {
				data.enforceInterface(DESCRIPTOR);
				int _result = this.LogoutPassport();
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			case TRANSACTION_GetErrorMessage: {
				data.enforceInterface(DESCRIPTOR);
				int _arg0;
				_arg0 = data.readInt();
				java.lang.String _result = this.GetErrorMessage(_arg0);
				reply.writeNoException();
				reply.writeString(_result);
				return true;
			}
			}
			return super.onTransact(code, data, reply, flags);
		}

		private static class Proxy implements konka.UserInfo {
			private android.os.IBinder mRemote;

			Proxy(android.os.IBinder remote) {
				mRemote = remote;
			}

			@Override
			public android.os.IBinder asBinder() {
				return mRemote;
			}

			public java.lang.String getInterfaceDescriptor() {
				return DESCRIPTOR;
			}

			@Override
			public int SendNetRequest(java.lang.String strParams,
					int iSysTypeId, int iSubTypeId,
					konka.xmlData stXmlData)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strParams);
					_data.writeInt(iSysTypeId);
					_data.writeInt(iSubTypeId);
					if ((stXmlData != null)) {
						_data.writeInt(1);
						stXmlData.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					mRemote.transact(Stub.TRANSACTION_SendNetRequest, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
					if ((0 != _reply.readInt())) {
						stXmlData.readFromParcel(_reply);
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public java.lang.String GetSerial()
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_GetSerial, _data, _reply,
							0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public java.lang.String GetSerialWithCRC()
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_GetSerialWithCRC, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int GetADOpenFlag() throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_GetADOpenFlag, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public java.lang.String ReplaceUrlDomain(java.lang.String strUrl)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strUrl);
					mRemote.transact(Stub.TRANSACTION_ReplaceUrlDomain, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int CheckSignature(java.lang.String strSN,
					java.lang.String strMainFileName,
					java.lang.String strSignatureFile, boolean bCheckSha1)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strSN);
					_data.writeString(strMainFileName);
					_data.writeString(strSignatureFile);
					_data.writeInt(((bCheckSha1) ? (1) : (0)));
					mRemote.transact(Stub.TRANSACTION_CheckSignature, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public byte[] Encode(byte[] baData)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				byte[] _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeByteArray(baData);
					mRemote.transact(Stub.TRANSACTION_Encode, _data, _reply, 0);
					_reply.readException();
					_result = _reply.createByteArray();
					_reply.readByteArray(baData);
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public byte[] Decode(byte[] baData)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				byte[] _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeByteArray(baData);
					mRemote.transact(Stub.TRANSACTION_Decode, _data, _reply, 0);
					_reply.readException();
					_result = _reply.createByteArray();
					_reply.readByteArray(baData);
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public byte[] cEncode(byte[] baData, byte[] baKey)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				byte[] _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeByteArray(baData);
					_data.writeByteArray(baKey);
					mRemote.transact(Stub.TRANSACTION_cEncode, _data, _reply, 0);
					_reply.readException();
					_result = _reply.createByteArray();
					_reply.readByteArray(baData);
					_reply.readByteArray(baKey);
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * AES���� baData -- ����ܵ���� baKey -- ��Կ�����ȱ�����16�ֽ�
			 * ����:�ɹ����ؽ��ܺ����ݣ�ʧ�ܷ���0�ַ�
			 */
			@Override
			public byte[] cDecode(byte[] baData, byte[] baKey)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				byte[] _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeByteArray(baData);
					_data.writeByteArray(baKey);
					mRemote.transact(Stub.TRANSACTION_cDecode, _data, _reply, 0);
					_reply.readException();
					_result = _reply.createByteArray();
					_reply.readByteArray(baData);
					_reply.readByteArray(baKey);
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int SendNetRequestWithLogin(java.lang.String strParams,
					int iSysTypeId, int iSubTypeId,
					konka.xmlData stXmlData)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strParams);
					_data.writeInt(iSysTypeId);
					_data.writeInt(iSubTypeId);
					if ((stXmlData != null)) {
						_data.writeInt(1);
						stXmlData.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					mRemote.transact(Stub.TRANSACTION_SendNetRequestWithLogin,
							_data, _reply, 0);
					_reply.readException();
					_result = _reply.readInt();
					if ((0 != _reply.readInt())) {
						stXmlData.readFromParcel(_reply);
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� - ��ϵͳ����http���� ���� - strParams ������� ���� - stXmlData
			 * ���ڴ洢XML�ַ�Ľṹ�� ���,����Ϊnull ���� - �ɹ�����0��ʧ�ܷ��ش�����
			 */
			@Override
			public int UpdateRequest(java.lang.String strParams,
					konka.xmlData stXmlData)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strParams);
					if ((stXmlData != null)) {
						_data.writeInt(1);
						stXmlData.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					mRemote.transact(Stub.TRANSACTION_UpdateRequest, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
					if ((0 != _reply.readInt())) {
						stXmlData.readFromParcel(_reply);
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� - ��ȡ�û�ͨ��֤ע��id ���� - �� ���� - �ɹ������û����ɹ�����null
			 */
			@Override
			public java.lang.String GetPassportId()
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_GetPassportId, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� - ��ȡ������ ���� - �� ���� - �ɹ������������ַ����ɹ�����null
			 */
			@Override
			public java.lang.String GetCrcCode()
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_GetCrcCode, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� - Ӧ�ó���ǩ����֤(Android) ���� - strPackageName APK���� ���� -
			 * strPackageKeySha1 APK����ǩ��Կ��sha1ֵ ���� - strSignatureFile
			 * ǩ���ļ�(��·��) ���� - �ɹ�����0��ʧ�ܷ��ش�����
			 */
			@Override
			public int CheckSignatureAndroid(java.lang.String strPackageName,
					java.lang.String strPackageKeySha1,
					java.lang.String strSignatureFile)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strPackageName);
					_data.writeString(strPackageKeySha1);
					_data.writeString(strSignatureFile);
					mRemote.transact(Stub.TRANSACTION_CheckSignatureAndroid,
							_data, _reply, 0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� - ��ȡ���Ŷ�Ӧ�ĵ���id ���� - �ɹ������û�id��ʧ�ܷ���null
			 */
			@Override
			public java.lang.String GetUserId()
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_GetUserId, _data, _reply,
							0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� -
			 * ����Ӧ�ü������ӿڣ�ͨ��bussinessId��ȡ���license���������license ����
			 * - BussinessId ҵ��Id ���� - �ɹ�����licence�ַ�ʧ�ܷ���null
			 */
			@Override
			public java.lang.String GetCtrlInfoInnerApp(int BussinessId)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeInt(BussinessId);
					mRemote.transact(Stub.TRANSACTION_GetCtrlInfoInnerApp,
							_data, _reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			/*
			 * ���� - ����Ӧ�ü������ӿڣ�ͨ��apk package
			 * name��ȡ���license���������license ���� - ����Ӧ��apk ��� ���� -
			 * �ɹ�����licence�ַ�ʧ�ܷ���null
			 */
			@Override
			public java.lang.String GetCtrlInfoThirPartyApp(
					java.lang.String AppPackageName)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(AppPackageName);
					mRemote.transact(Stub.TRANSACTION_GetCtrlInfoThirPartyApp,
							_data, _reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int GetPassportInfo(
					konka.PassportInfo passportinfo)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					if ((passportinfo != null)) {
						_data.writeInt(1);
						passportinfo.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					mRemote.transact(Stub.TRANSACTION_GetPassportInfo, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
					if ((0 != _reply.readInt())) {
						passportinfo.readFromParcel(_reply);
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int SendNetRequestWithFile(java.lang.String strParams,
					int iSysTypeId, int iSubTypeId,
					konka.xmlData stXmlData,
					java.lang.String strFilename)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strParams);
					_data.writeInt(iSysTypeId);
					_data.writeInt(iSubTypeId);
					if ((stXmlData != null)) {
						_data.writeInt(1);
						stXmlData.writeToParcel(_data, 0);
					} else {
						_data.writeInt(0);
					}
					_data.writeString(strFilename);
					mRemote.transact(Stub.TRANSACTION_SendNetRequestWithFile,
							_data, _reply, 0);
					_reply.readException();
					_result = _reply.readInt();
					if ((0 != _reply.readInt())) {
						stXmlData.readFromParcel(_reply);
					}
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int LoginPassport(java.lang.String strname,
					java.lang.String strpassword)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strname);
					_data.writeString(strpassword);
					mRemote.transact(Stub.TRANSACTION_LoginPassport, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int RegisterPassport(java.lang.String strname,
					java.lang.String strpassword)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeString(strname);
					_data.writeString(strpassword);
					mRemote.transact(Stub.TRANSACTION_RegisterPassport, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public int LogoutPassport() throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_LogoutPassport, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}

			@Override
			public java.lang.String GetErrorMessage(int iError)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				java.lang.String _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeInt(iError);
					mRemote.transact(Stub.TRANSACTION_GetErrorMessage, _data,
							_reply, 0);
					_reply.readException();
					_result = _reply.readString();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}
		}

		static final int TRANSACTION_SendNetRequest = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
		static final int TRANSACTION_GetSerial = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
		static final int TRANSACTION_GetSerialWithCRC = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
		static final int TRANSACTION_GetADOpenFlag = (android.os.IBinder.FIRST_CALL_TRANSACTION + 3);
		static final int TRANSACTION_ReplaceUrlDomain = (android.os.IBinder.FIRST_CALL_TRANSACTION + 4);
		static final int TRANSACTION_CheckSignature = (android.os.IBinder.FIRST_CALL_TRANSACTION + 5);
		static final int TRANSACTION_Encode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 6);
		static final int TRANSACTION_Decode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 7);
		static final int TRANSACTION_cEncode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 8);
		static final int TRANSACTION_cDecode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 9);
		static final int TRANSACTION_SendNetRequestWithLogin = (android.os.IBinder.FIRST_CALL_TRANSACTION + 10);
		static final int TRANSACTION_UpdateRequest = (android.os.IBinder.FIRST_CALL_TRANSACTION + 11);
		static final int TRANSACTION_GetPassportId = (android.os.IBinder.FIRST_CALL_TRANSACTION + 12);
		static final int TRANSACTION_GetCrcCode = (android.os.IBinder.FIRST_CALL_TRANSACTION + 13);
		static final int TRANSACTION_CheckSignatureAndroid = (android.os.IBinder.FIRST_CALL_TRANSACTION + 14);
		static final int TRANSACTION_GetUserId = (android.os.IBinder.FIRST_CALL_TRANSACTION + 15);
		static final int TRANSACTION_GetCtrlInfoInnerApp = (android.os.IBinder.FIRST_CALL_TRANSACTION + 16);
		static final int TRANSACTION_GetCtrlInfoThirPartyApp = (android.os.IBinder.FIRST_CALL_TRANSACTION + 17);
		static final int TRANSACTION_GetPassportInfo = (android.os.IBinder.FIRST_CALL_TRANSACTION + 18);
		static final int TRANSACTION_SendNetRequestWithFile = (android.os.IBinder.FIRST_CALL_TRANSACTION + 19);
		static final int TRANSACTION_LoginPassport = (android.os.IBinder.FIRST_CALL_TRANSACTION + 20);
		static final int TRANSACTION_RegisterPassport = (android.os.IBinder.FIRST_CALL_TRANSACTION + 21);
		static final int TRANSACTION_LogoutPassport = (android.os.IBinder.FIRST_CALL_TRANSACTION + 22);
		static final int TRANSACTION_GetErrorMessage = (android.os.IBinder.FIRST_CALL_TRANSACTION + 23);
	}

	public int SendNetRequest(java.lang.String strParams, int iSysTypeId,
			int iSubTypeId, konka.xmlData stXmlData)
			throws android.os.RemoteException;

	public java.lang.String GetSerial() throws android.os.RemoteException;

	public java.lang.String GetSerialWithCRC()
			throws android.os.RemoteException;

	public int GetADOpenFlag() throws android.os.RemoteException;

	public java.lang.String ReplaceUrlDomain(java.lang.String strUrl)
			throws android.os.RemoteException;

	public int CheckSignature(java.lang.String strSN,
			java.lang.String strMainFileName,
			java.lang.String strSignatureFile, boolean bCheckSha1)
			throws android.os.RemoteException;

	public byte[] Encode(byte[] baData) throws android.os.RemoteException;

	public byte[] Decode(byte[] baData) throws android.os.RemoteException;

	public byte[] cEncode(byte[] baData, byte[] baKey)
			throws android.os.RemoteException;

	public byte[] cDecode(byte[] baData, byte[] baKey)
			throws android.os.RemoteException;

	public int SendNetRequestWithLogin(java.lang.String strParams,
			int iSysTypeId, int iSubTypeId,
			konka.xmlData stXmlData)
			throws android.os.RemoteException;

	public int UpdateRequest(java.lang.String strParams,
			konka.xmlData stXmlData)
			throws android.os.RemoteException;

	public java.lang.String GetPassportId() throws android.os.RemoteException;

	public java.lang.String GetCrcCode() throws android.os.RemoteException;

	public int CheckSignatureAndroid(java.lang.String strPackageName,
			java.lang.String strPackageKeySha1,
			java.lang.String strSignatureFile)
			throws android.os.RemoteException;

	public java.lang.String GetUserId() throws android.os.RemoteException;

	public java.lang.String GetCtrlInfoInnerApp(int BussinessId)
			throws android.os.RemoteException;

	public java.lang.String GetCtrlInfoThirPartyApp(
			java.lang.String AppPackageName) throws android.os.RemoteException;

	public int GetPassportInfo(
			konka.PassportInfo passportinfo)
			throws android.os.RemoteException;

	public int SendNetRequestWithFile(java.lang.String strParams,
			int iSysTypeId, int iSubTypeId,
			konka.xmlData stXmlData,
			java.lang.String strFilename) throws android.os.RemoteException;

	public int LoginPassport(java.lang.String strname,
			java.lang.String strpassword) throws android.os.RemoteException;

	public int RegisterPassport(java.lang.String strname,
			java.lang.String strpassword) throws android.os.RemoteException;

	public int LogoutPassport() throws android.os.RemoteException;

	public java.lang.String GetErrorMessage(int iError)
			throws android.os.RemoteException;
}
