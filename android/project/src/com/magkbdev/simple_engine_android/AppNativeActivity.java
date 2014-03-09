package com.magkbdev.simple_engine_android;

import android.app.NativeActivity;
import android.os.Bundle;

public class AppNativeActivity extends NativeActivity {

	static {
		System.loadLibrary("simple_engine_android");
	}
	
	public void onCreate(Bundle savedInstanceState) {
		
		super.onCreate(savedInstanceState); 
	}
	
}
