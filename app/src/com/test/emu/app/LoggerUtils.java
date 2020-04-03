package com.test.emu.app;

import android.os.Bundle;
import android.util.Log;

import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.Set;

public final class LoggerUtils {
	
	public static final String TAG = "LoggerUtils";
	
	private LoggerUtils() {
		super();
	}
	
	public final static void v(Object text) {
		v(TAG, text);
	}
	
	public final static void v(Object tag, Object text) {
		try {
			Log.v(String.valueOf(tag), String.valueOf(text));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public final static void i(Object text) {
		i(TAG, text);
	}
	
	public final static void i(String TAG, Object... objects) {
		
		StringBuffer stringBuffer = new StringBuffer();
		for (Object o : objects) {
			//			 if (o instanceof View) {
			//				View v = (View) o;
			//				o = o.getClass().getSimpleName();
			//			 } else if (o instanceof Class) {
			//				o = o.getClass().getSimpleName();
			//			 }
			stringBuffer.append(String.valueOf(o)).append(" , ");
		}
		Log.i(TAG, stringBuffer.toString());
	}
	
	public final static void logStackTraceElement() {
		StackTraceElement[] traceElements = new Throwable().getStackTrace();
		for (StackTraceElement traceElement : traceElements) {
			Log.i(TAG, traceElement.getMethodName() + "_" + traceElement.getClassName());
		}
	}
	
	//
	public final static void d(Object text) {
		d(TAG, text);
	}
	
	public final static void d(Object TAG, Object str) {
		try {
			Log.d(String.valueOf(TAG), String.valueOf(str));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public final static void e(Object text) {
		e(TAG, text);
	}
	
	public final static void e(Object TAG, Object str) {
		e(TAG, str, null);
	}
	
	public final static void e(Object TAG, Object str, Throwable tr) {
		try {
			Log.e(String.valueOf(TAG), String.valueOf(str), tr);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public final static void w(Object text) {
		w(TAG, text);
	}
	
	public final static void w(Object TAG, Object str) {
		w(TAG, str, null);
	}
	
	public final static void w(Object TAG, Object str, Throwable tr) {
		try {
			Log.w(String.valueOf(TAG), String.valueOf(str), tr);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	public final static void logStackTraceElement(boolean b) {
	}
	
	public final static String showBundle(Bundle bundle) {
		if (bundle == null)
			return "bundle == null";
		Set<String>   set     = bundle.keySet();
		StringBuilder builder = new StringBuilder();
		for (String key : set) {
			builder.append(key).append(" >>>>> ").append(bundle.get(key)).append("\n");
		}
		return builder.toString();
	}
	
	public static String buildFieldToString(Object o) {
		if (o == null) return "o == null";
		StringBuffer buffer = new StringBuffer();
		try {
			Class aClass   = o.getClass();
			Field fields[] = aClass.getDeclaredFields();
			for (Field field : fields) {
				field.setAccessible(true);
				Object value = field.get(o);
				if (value != null)
					if (value.getClass().isArray()) {
						value = Arrays.asList(value);
					}
				buffer.append(field.getName()).append(":").append(value).append("\n");
			}
		} catch (Exception e) {
			//e.printStackTrace()`;
		}
		return buffer.toString();
	}
}