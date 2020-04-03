package com.test.emu.app;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.emu.test.R;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class SDCardActivity extends Activity {
	
	private static final int    REQUEST_PERMISSION = 1001;
	public static final  String PACKAGE_NAME       = "package_name";
	public static final  String CLASS_NAME         = "class_name";
	public static final  String TYPE               = "type";
	ListView       mListView;
	File[]         fileList;
	File           rootFile;
	File           currentFile;
	AlertDialog    mAlertDialog;
	FileAdapter    mFileAdapter;
	LayoutInflater mLayoutInflater;
	
	EMU_TYPE type;
	String   packageName;
	String   className;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mListView = new ListView(this);
		mFileAdapter = new FileAdapter();
		mListView.setDividerHeight(1);
		mListView.setDivider(getResources().getDrawable(R.color.color_text));
		mListView.setAdapter(mFileAdapter);
		mLayoutInflater = LayoutInflater.from(this);
		type = EMU_TYPE.valueOf(getIntent().getStringExtra(TYPE));
		packageName = getIntent().getStringExtra(PACKAGE_NAME);
		className = getIntent().getStringExtra(CLASS_NAME);
		setContentView(mListView);
		if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M
				|| checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED) {
			requestPermissionSuccess();
		} else {
			requestPermission();
		}
	}
	
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
		for (int i : grantResults) {
			if (i == PackageManager.PERMISSION_GRANTED) {
				requestPermissionSuccess();
				return;
			}
		}
		mAlertDialog = new AlertDialog.Builder(this)
				.setMessage("请授权读写SD卡权限!")
				.setNegativeButton("确定 ", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						requestPermissionSuccess();
						dialog.dismiss();
					}
				}).setNeutralButton("取消", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
						finish();
					}
				}).show();
	}
	
	@TargetApi(Build.VERSION_CODES.M)
	private void requestPermission() {
		requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, REQUEST_PERMISSION);
	}
	
	private void requestPermissionSuccess() {
		rootFile = Environment.getExternalStorageDirectory();
		show(rootFile);
	}
	
	private void show(File file) {
		currentFile = file;
		File[] files = currentFile.listFiles(new FileFilter() {
			@Override
			public boolean accept(File pathname) {
				return !pathname.getName().startsWith(".");
			}
		});
		List<File> list = new ArrayList<>();
		list.addAll(Arrays.asList(files));
		list.sort(new Comparator<File>() {
			@Override
			public int compare(File o1, File o2) {
				return o1.getName().compareTo(o2.getName());
			}
		});
		fileList = list.toArray(new File[]{});
		mFileAdapter.notifyDataSetChanged();
	}
	
	private class FileAdapter extends BaseAdapter {
		
		@Override
		public int getCount() {
			return fileList != null ? fileList.length : 0;
		}
		
		@Override
		public Object getItem(int position) {
			return fileList != null ? fileList[position] : null;
		}
		
		@Override
		public long getItemId(int position) {
			return position;
		}
		
		@Override
		public View getView(int position, View convertView, ViewGroup parent) {
			final File file = fileList[position];
			if (convertView == null) {
				convertView = mLayoutInflater.inflate(R.layout.activity_file_item, null);
			}
			
			FileHolder fileHolder;
			if (convertView.getTag() == null) {
				fileHolder = new FileHolder();
				fileHolder.mTextView = (TextView) convertView;
				convertView.setTag(fileHolder);
			} else {
				fileHolder = (FileHolder) convertView.getTag();
			}
			fileHolder.mTextView.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					if (file.isDirectory()) {
						show(file);
					} else {
						switch (type) {
							case EMU_PSP:
								onPPSSPPClick(file);
								break;
							case EMU_NES:
								onNesClick(file);
								break;
						}
					}
				}
			});
			fileHolder.mTextView.setText(file.getName());
			return convertView;
		}
	}
	
	class FileHolder {
		TextView mTextView;
	}
	
	@Override
	public void onBackPressed() {
		if (rootFile != null && rootFile.equals(currentFile)) {
			super.onBackPressed();
		} else {
			show(currentFile.getParentFile());
		}
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		if (mAlertDialog != null) {
			mAlertDialog.dismiss();
			mAlertDialog = null;
		}
	}
	
	void showNoSupport() {
		Toast.makeText(this, "不支持此类型游戏", Toast.LENGTH_LONG).show();
	}
	
	void onPPSSPPClick(File file) {
		String name = file.getName().toLowerCase();
		if (name.endsWith(".iso")
				|| name.endsWith(".cso")
				|| name.endsWith(".elf")
		) {
			Intent intent = new Intent();
			intent.setPackage(packageName);
			intent.setClassName(packageName, className);
			intent.setData(Uri.fromFile(file));
			startActivity(intent);
		} else {
			showNoSupport();
		}
	}
	
	void onNesClick(File file) {
		String name = file.getName().toLowerCase();
		if (name.endsWith(".nes")
				|| name.endsWith(".fds")
		) {
			Intent intent = new Intent();
			intent.setClassName(packageName, className);
			intent.setData(Uri.fromFile(file));
			startActivity(intent);
		} else {
			showNoSupport();
		}
	}
}
