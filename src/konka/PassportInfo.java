package konka;

import android.graphics.Bitmap;
import android.os.Parcel;
import android.os.Parcelable;

public class PassportInfo implements Parcelable {
	private String PassportId;
	private String IpAddress;
	private Bitmap HeadPic;

	public PassportInfo(String passportid, String ipaddress, Bitmap headpic) {
		PassportId = passportid;
		IpAddress = ipaddress;
		HeadPic = headpic;
	}

	public PassportInfo() {
	}

	public void SetPassportId(String passportid) {
		PassportId = passportid;
	}

	public String GetPassportId() {
		return PassportId;
	}

	public void SetIpAddress(String ipaddress) {
		IpAddress = ipaddress;
	}

	public String GetIpAddress() {
		return IpAddress;
	}

	public void SetHeadPic(Bitmap headpic) {
		HeadPic = headpic;
	}

	public Bitmap GetHeadPic() {
		return HeadPic;
	}

	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel out, int flags) {
		out.writeString(PassportId);
		out.writeString(IpAddress);
		out.writeParcelable(HeadPic, flags);
	}

	public void readFromParcel(Parcel in) {
		PassportId = in.readString();
		IpAddress = in.readString();
		HeadPic = in.readParcelable(Bitmap.class.getClassLoader());
	}

	public static final Parcelable.Creator<PassportInfo> CREATOR = new Parcelable.Creator<PassportInfo>() {
		@Override
		public PassportInfo createFromParcel(Parcel in) {
			return new PassportInfo(in);
		}

		@Override
		public PassportInfo[] newArray(int size) {
			return new PassportInfo[size];
		}
	};

	private PassportInfo(Parcel in) {
		PassportId = in.readString();
		IpAddress = in.readString();
		HeadPic = in.readParcelable(Bitmap.class.getClassLoader());
	}
}
