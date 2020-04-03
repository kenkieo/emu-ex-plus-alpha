package com.test.emu.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.emu.test.R;

public class MainActivity extends Activity implements View.OnClickListener {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		findViewById(R.id.activity_main_2600).setOnClickListener(this);
		findViewById(R.id.activity_main_C64).setOnClickListener(this);
		findViewById(R.id.activity_main_GBA).setOnClickListener(this);
		findViewById(R.id.activity_main_GBC).setOnClickListener(this);
		findViewById(R.id.activity_main_MD).setOnClickListener(this);
		findViewById(R.id.activity_main_MSX).setOnClickListener(this);
		findViewById(R.id.activity_main_NEO).setOnClickListener(this);
		findViewById(R.id.activity_main_NES).setOnClickListener(this);
		findViewById(R.id.activity_main_NGP).setOnClickListener(this);
		findViewById(R.id.activity_main_PCE).setOnClickListener(this);
		findViewById(R.id.activity_main_Snes9x).setOnClickListener(this);
		findViewById(R.id.activity_main_Snes9x_143).setOnClickListener(this);
		findViewById(R.id.activity_main_PPSSPSS).setOnClickListener(this);
	}
	
	@Override
	public void onClick(View v) {
		if (v.getId() == R.id.activity_main_2600) {
			on2600Click();
		} else if (v.getId() == R.id.activity_main_NES) {
			onNesClick();
		} else if (v.getId() == R.id.activity_main_PPSSPSS) {
			onPPSSPPClick();
		}
	}
	
	void on2600Click() {
//		Intent intent = new Intent();
//		intent.setClassName("com.explusalpha.NesEmu", "com.imagine.BaseActivity");
//		startActivity(intent);
	}
	
	void onNesClick() {
//		Intent intent = new Intent();
//		intent.setClassName("com.explusalpha.NesEmu", "com.imagine.BaseActivity");
//		startActivity(intent);
	}
	
	void onPPSSPPClick() {
		startSDCardActivity("org.ppsspp.ppssppgold", "org.ppsspp.ppsspp.PpssppActivity", EMU_TYPE.EMU_PSP);
	}
	
	void startSDCardActivity(String packageName, String className, EMU_TYPE emu_type) {
		Intent intent = new Intent(this, SDCardActivity.class);
		intent.putExtra(SDCardActivity.PACKAGE_NAME, packageName);
		intent.putExtra(SDCardActivity.CLASS_NAME, className);
		intent.putExtra(SDCardActivity.TYPE, emu_type.name());
		startActivity(intent);
	}
}
