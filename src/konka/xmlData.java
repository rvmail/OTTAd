package konka;

import android.os.Parcel;
import android.os.Parcelable;

public class xmlData implements Parcelable {
	private String strXml;

	public xmlData(String str) {
		strXml = str;
	}

	public xmlData() {
	}

	public void SetXmlData(String xmlData) {
		strXml = xmlData;
	}

	public String GetXmlData() {
		return strXml;
	}

	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel out, int flags) {
		out.writeString(strXml);
	}

	public void readFromParcel(Parcel in) {
		strXml = in.readString();
	}

	public static final Parcelable.Creator<xmlData> CREATOR = new Parcelable.Creator<xmlData>() {
		@Override
		public xmlData createFromParcel(Parcel in) {
			return new xmlData(in);
		}

		@Override
		public xmlData[] newArray(int size) {
			return new xmlData[size];
		}
	};

	private xmlData(Parcel in) {
		strXml = in.readString();
	}
}
