package com.imagine;

import android.app.NativeActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.ShortcutInfo;
import android.content.pm.ShortcutManager;
import android.content.pm.Signature;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Icon;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.View;
import android.view.ViewConfiguration;
import android.view.Window;
import android.widget.PopupWindow;
import android.widget.TextView;

import java.io.FileOutputStream;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;


public final class BaseActivity
		extends NativeActivity
		implements AudioManager.OnAudioFocusChangeListener {
	private static final String logTag                = "BaseActivity";
	private static final Method setSystemUiVisibility = (Build.VERSION.SDK_INT >= 11) ?
			Util.getMethod(View.class, "setSystemUiVisibility", new Class[]{int.class}) : null;
	
	private static final int commonUILayoutFlags = 1536;
	
	
	boolean hasPermanentMenuKey() {
		if (Build.VERSION.SDK_INT < 14) return true;
		boolean hasKey = true;
		
		try {
			Method            hasPermanentMenuKeyFunc = ViewConfiguration.class.getMethod("hasPermanentMenuKey", new Class[0]);
			ViewConfiguration viewConf                = ViewConfiguration.get(this);
			
			try {
				hasKey = ((Boolean) hasPermanentMenuKeyFunc.invoke(viewConf, new Object[0])).booleanValue();
			} catch (IllegalAccessException illegalAccessException) {
			
			
			} catch (InvocationTargetException invocationTargetException) {}
			
			
		} catch (NoSuchMethodException noSuchMethodException) {}
		
		
		return hasKey;
	}
	
	private static final int REQUEST_BT_ON = 1;
	
	static native void onContentRectChanged(long paramLong, int paramInt1, int paramInt2, int paramInt3, int paramInt4, int paramInt5, int paramInt6);
	
	int sigHash() {
		try {
			Signature[] sig = (getPackageManager().getPackageInfo(getPackageName(), 64)).signatures;
			
			return sig[0].hashCode();
		} catch (android.content.pm.PackageManager.NameNotFoundException e) {
			
			return 0;
		}
	}
	
	
	boolean packageIsInstalled(String name) {
		boolean found = false;
		
		try {
			getPackageManager().getPackageInfo(name, 0);
			found = true;
		} catch (android.content.pm.PackageManager.NameNotFoundException nameNotFoundException) {}
		
		
		return found;
	}
	
	
	static boolean gbAnimatesRotation() { return Build.DISPLAY.contains("cyano"); }
	
	
	Display defaultDpy() { return getWindowManager().getDefaultDisplay(); }
	
	
	DisplayMetrics displayMetrics() { return getResources().getDisplayMetrics(); }
	
	
	DisplayMetrics getDisplayMetrics(Display display) {
		DisplayMetrics metrics = new DisplayMetrics();
		display.getMetrics(metrics);
		return metrics;
	}
	
	
	String filesDir() { return getFilesDir().getAbsolutePath(); }
	
	
	String cacheDir() { return getCacheDir().getAbsolutePath(); }
	
	
	static String extStorageDir() { return Environment.getExternalStorageDirectory().getAbsolutePath(); }
	
	
	static String devName() { return Build.DEVICE; }
	
	
	Vibrator systemVibrator() {
		Vibrator vibrator    = (Vibrator) getSystemService("vibrator");
		boolean  hasVibrator = (vibrator != null);
		if (hasVibrator && Build.VERSION.SDK_INT >= 11) {
			
			try {
				
				
				Method hasVibratorFunc = Vibrator.class.getMethod("hasVibrator", new Class[0]);
				
				try {
					hasVibrator = ((Boolean) hasVibratorFunc.invoke(vibrator, new Object[0])).booleanValue();
				} catch (IllegalAccessException illegalAccessException) {
				
				
				} catch (InvocationTargetException invocationTargetException) {}
				
				
			} catch (NoSuchMethodException noSuchMethodException) {}
		}
		
		
		return hasVibrator ? vibrator : null;
	}
	
	
	AudioManager audioManager() { return (AudioManager) getSystemService("audio"); }
	
	
	boolean hasLowLatencyAudio() { return getPackageManager().hasSystemFeature("android.hardware.audio.low_latency"); }
	
	
	public void onAudioFocusChange(int focusChange) {}
	
	
	void setUIVisibility(int mode) {
		if (setSystemUiVisibility == null) {
			return;
		}
		
		
		try {
			int flags = mode | 0x600;
			if ((Build.VERSION.SDK_INT == 16 || Build.VERSION.SDK_INT == 17) && (mode & 0x2) == 0) {
				
				
				flags |= 0x100;
			}
			setSystemUiVisibility.invoke(getWindow().getDecorView(), new Object[]{Integer.valueOf(flags)});
		} catch (IllegalAccessException illegalAccessException) {
		
		
		} catch (InvocationTargetException invocationTargetException) {}
	}
	
	
	void setWinFlags(int flags, int mask) { getWindow().setFlags(flags, mask); }
	
	
	void setWinFormat(int format) { getWindow().setFormat(format); }
	
	
	int winFlags() { return (getWindow().getAttributes()).flags; }
	
	
	int winFormat() { return (getWindow().getAttributes()).format; }
	
	
	void addNotification(String onShow, String title, String message) { NotificationHelper.addNotification(this, onShow, title, message); }
	
	
	void removeNotification() { NotificationHelper.removeNotification(this); }
	
	
	static native void onBTScanStatus(int paramInt);
	
	static native boolean onScanDeviceClass(int paramInt);
	
	static native void onScanDeviceName(String paramString1, String paramString2);
	
	static native void onBTOn(boolean paramBoolean);
	
	BluetoothAdapter btDefaultAdapter() { return Bluetooth.defaultAdapter(); }
	
	
	int btStartScan(BluetoothAdapter adapter) { return Bluetooth.startScan(this, adapter) ? 1 : 0; }
	
	
	void btCancelScan(BluetoothAdapter adapter) { Bluetooth.cancelScan(this, adapter); }
	
	
	BluetoothSocket btOpenSocket(BluetoothAdapter adapter, String address, int ch, boolean l2cap) { return Bluetooth.openSocket(adapter, address, ch, l2cap); }
	
	
	int btState(BluetoothAdapter adapter) { return adapter.getState(); }
	
	
	void btTurnOn() {
		Intent btOn = new Intent("android.bluetooth.adapter.action.REQUEST_ENABLE");
		startActivityForResult(btOn, 1);
	}
	
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == 1) {
			onBTOn((resultCode == -1));
		}
	}
	
	
	public void onGlobalLayout() {}
	
	
	String intentDataPath() {
		String path = null;
		Uri    uri  = getIntent().getData();
		if (uri != null) {
			
			path = uri.getPath();
			
			getIntent().setData(null);
		}
		return path;
	}
	
	
	void addViewShortcut(String name, String path) {
		Intent viewIntent = new Intent(this, BaseActivity.class);
		viewIntent.setAction("android.intent.action.VIEW");
		viewIntent.setData(Uri.parse("file://" + path));
		int icon = getResources().getIdentifier("icon", "drawable", getPackageName());
		if (Build.VERSION.SDK_INT >= 26) {
			
			ShortcutManager shortcutManager = (ShortcutManager) getSystemService(ShortcutManager.class);
			
			
			ShortcutInfo shortcutInfo = (new ShortcutInfo.Builder(this, name)).setShortLabel(name).setIcon(Icon.createWithResource(this, icon)).setIntent(viewIntent).build();
			shortcutManager.requestPinShortcut(shortcutInfo, null);
		} else {
			
			Intent launcherIntent = new Intent();
			launcherIntent.putExtra("android.intent.extra.shortcut.INTENT", viewIntent);
			launcherIntent.putExtra("android.intent.extra.shortcut.NAME", name);
			String EXTRA_SHORTCUT_DUPLICATE = "duplicate";
			launcherIntent.putExtra("duplicate", false);
			launcherIntent.putExtra("android.intent.extra.shortcut.ICON_RESOURCE", Intent.ShortcutIconResource.fromContext(this, icon));
			launcherIntent.setAction("com.android.launcher.action.INSTALL_SHORTCUT");
			sendBroadcast(launcherIntent);
		}
	}
	
	
	protected void onNewIntent(Intent intent) { setIntent(intent); }
	
	
	protected void onCreate(Bundle savedInstanceState) {
		View   contentView;
		Window win = getWindow();
		win.addFlags(65792);
		super.onCreate(savedInstanceState);
		win.setBackgroundDrawable(null);
		win.setSoftInputMode(32);
		if (Build.VERSION.SDK_INT >= 11) {
			
			
			win.setFormat(0);
		}
		
		View nativeActivityView = findViewById(16908290);
		nativeActivityView.getViewTreeObserver().removeGlobalOnLayoutListener(this);
		
		if (Build.VERSION.SDK_INT >= 24) {
			contentView = new ContentViewV24(this);
		} else if (Build.VERSION.SDK_INT >= 16) {
			contentView = new ContentViewV16(this);
		} else {
			contentView = new ContentViewV9(this);
		} setContentView(contentView);
		contentView.requestFocus();
	}
	
	
	protected void onResume() {
		removeNotification();
		super.onResume();
	}
	
	
	protected void onDestroy() {
		System.exit(-1);
		removeNotification();
		super.onDestroy();
	}
	
	
	static native void sysTextInputEnded(String paramString, boolean paramBoolean1, boolean paramBoolean2);
	
	
	static void endSysTextInput(String text, boolean processText, boolean isDoingDismiss) { sysTextInputEnded(text, processText, isDoingDismiss); }
	
	
	void startSysTextInput(String initialText, String promptText, int x, int y, int width, int height, int fontSize) { TextEntry.startSysTextInput(this, initialText, promptText, x, y, width, height, fontSize); }
	
	
	void finishSysTextInput(boolean canceled) { TextEntry.finishSysTextInput(canceled); }
	
	
	void placeSysTextInput(int x, int y, int width, int height) { TextEntry.placeSysTextInput(x, y, width, height); }
	
	
	FontRenderer newFontRenderer() { return new FontRenderer(); }
	
	
	ChoreographerHelper newChoreographerHelper(long timerAddr) {
		if (Build.VERSION.SDK_INT < 16)
			return null;
		return new ChoreographerHelper(timerAddr);
	}
	
	
	InputDeviceHelper inputDeviceHelper() {
		if (Build.VERSION.SDK_INT < 12)
			return null;
		return new InputDeviceHelper();
	}
	
	
	InputDeviceListenerHelper inputDeviceListenerHelper() {
		if (Build.VERSION.SDK_INT < 16)
			return null;
		return new InputDeviceListenerHelper(this);
	}
	
	
	DisplayListenerHelper displayListenerHelper() {
		if (Build.VERSION.SDK_INT < 17)
			return null;
		return new DisplayListenerHelper(this);
	}
	
	
	MOGAHelper mogaHelper(long mogaSystemAddr) { return new MOGAHelper(this, mogaSystemAddr); }
	
	
	PresentationHelper presentation(Display display, long windowAddr) {
		if (Build.VERSION.SDK_INT < 17)
			return null;
		return new PresentationHelper(this, display, windowAddr);
	}
	
	
	UserActivityFaker userActivityFaker() { return new UserActivityFaker(); }
	
	
	StorageManagerHelper storageManagerHelper() {
		if (Build.VERSION.SDK_INT < 24)
			return null;
		return new StorageManagerHelper();
	}
	
	
	void setSustainedPerformanceMode(boolean on) {
		if (Build.VERSION.SDK_INT >= 24) {
			getWindow().setSustainedPerformanceMode(on);
		}
	}
	
	
	Bitmap makeBitmap(int width, int height, int format) {
		Bitmap.Config config = Bitmap.Config.ARGB_8888;
		if (format == 4)
			config = Bitmap.Config.RGB_565;
		return Bitmap.createBitmap(width, height, config);
	}
	
	
	boolean writePNG(Bitmap bitmap, String path) {
		boolean success;
		try {
			FileOutputStream output = new FileOutputStream(path);
			success = bitmap.compress(Bitmap.CompressFormat.PNG, 100, output);
			output.close();
		} catch (Exception e) {
			
			success = false;
		}
		bitmap.recycle();
		return success;
	}
	
	Bitmap bitmapDecodeAsset(String name) {
		InputStream  in;
		AssetManager assets = getAssets();
		
		
		try {
			in = assets.open(name);
		} catch (Exception e) {
			
			return null;
		}
		return BitmapFactory.decodeStream(in);
	}
	
	
	String libDir() { return (getApplicationInfo()).nativeLibraryDir; }
	
	
	String mainSOPath() {
		String libname = "main";
		return libDir() + "/" + System.mapLibraryName(libname);
	}
	
	
	boolean requestPermission(String permission) {
		if (ContextCompat.checkSelfPermission(this, permission) == 0)
			return true;
		ActivityCompat.requestPermissions(this, new String[]{permission}, 0);
		return false;
	}
	
	
	void makeErrorPopup(String text) {
		TextView view = new TextView(this);
		view.setText(text);
		final PopupWindow win         = new PopupWindow(view, -2, -2);
		final View        contentView = findViewById(16908290);
		contentView.post(new Runnable() {
			public void run() { win.showAtLocation(contentView, 17, 0, 0); }
		});
	}
	
	
	void openURL(String url) {
		Intent intent = new Intent("android.intent.action.VIEW", Uri.parse(url));
		if (intent.resolveActivity(getPackageManager()) != null) {
			startActivity(intent);
		}
	}
	
}
