/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;
import sonar.systems.framework.SonarFrameworkActivity;

import java.io.IOException;

import org.cocos2dx.lib.*;

import android.content.Context;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

public class AppActivity extends SonarFrameworkActivity
//Cocos2dxActivity 
{
	public static AppActivity currentActivity;
	private static Toast toastObject = null;
	private static DisplayMetrics dm;
	private static Cocos2dxGLSurfaceView glSurfaceView;
	
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		dm = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dm);
		currentActivity = this;
	}
	
	public static boolean isNetworkConnected_JAVA()
	{
		ConnectivityManager cm = (ConnectivityManager)currentActivity.getSystemService(Context.CONNECTIVITY_SERVICE);
		return cm.getActiveNetworkInfo() != null;
	}
	
	public Cocos2dxGLSurfaceView onCreateView()
    {
        glSurfaceView = new Cocos2dxGLSurfaceView(this);
        hideSystemUI();
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        glSurfaceView.setKeepScreenOn(true);

        return glSurfaceView;
    }

	@Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus)
        {
            hideSystemUI();
        }
    }
    
    public static void hideSystemUI()
    {
	    if (Build.VERSION.SDK_INT >= 19) 
	    {
	        glSurfaceView.setSystemUiVisibility(
	                Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE 
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
	    }
    }
	
	public static void hideUI_JAVA()
	{
		currentActivity.runOnUiThread(new Runnable() 
		{
			@Override
			public void run() 
			{
				hideSystemUI();
			}
		});
	}
	
	public static float GetPhysicalSize()
	{
		double wi = Math.pow((float)dm.widthPixels / (int)dm.xdpi, 2);
		double hi = Math.pow((float)dm.heightPixels / (int)dm.xdpi, 2);
		return (float)Math.round(Math.sqrt(wi + hi) * 100) * 0.01f;
	}
	
	public static int GetDPI()
	{
		return (int)dm.xdpi;
	}
	
	public static void MakeToast_JAVA(final String message)
	{
		currentActivity.runOnUiThread(new Runnable() 
		{
			@Override
			public void run() 
			{
				if(toastObject != null)
					toastObject.cancel();
				
				toastObject = Toast.makeText(currentActivity.getApplicationContext(), message, Toast.LENGTH_LONG);
				toastObject.show();
			}
		});
	}
	
	public static void GoToGameScene()
	{
		currentActivity.runOnGLThread(new Runnable() 
		{
			@Override
			public void run() 
			{
				CJNIHelper.GoToGameSceneJAVA();
			}
		});
	}
	
	public static void GoToMainMenuScene()
	{
		currentActivity.runOnGLThread(new Runnable() 
		{
			@Override
			public void run() 
			{
				CJNIHelper.GoToMainMenuSceneJAVA();
			}
		});
	}
}
