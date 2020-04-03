package com.test.emu.app;

import android.os.Parcel;
import android.os.Parcelable;

public enum EMU_TYPE implements Parcelable {
	EMU_2600,
	EMU_C64,
	EMU_GBA,
	EMU_GBC,
	EMU_MD,
	EMU_MSX,
	EMU_NEO,
	EMU_NES,
	EMU_NGP,
	EMU_PCE,
	EMU_Snes9x,
	EMU_Snes9x_143,
	EMU_PSP,
	;
	
	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeString(name());
	}
	
	@Override
	public int describeContents() {
		return 0;
	}
	
	public static final Creator<EMU_TYPE> CREATOR = new Creator<EMU_TYPE>() {
		@Override
		public EMU_TYPE createFromParcel(Parcel in) {
			return EMU_TYPE.valueOf(in.readString());
		}
		
		@Override
		public EMU_TYPE[] newArray(int size) {
			return new EMU_TYPE[size];
		}
	};
}
