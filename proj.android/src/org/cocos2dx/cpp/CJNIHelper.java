package org.cocos2dx.cpp;

import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Pattern;

import android.view.WindowManager;

public class CJNIHelper 
{
	private static final String TAG = "cocos2d-x debug info JAVA";
	public static DanchosServer danchosServer = null;
	public static DanchosClient danchosClient = null;
	public static UDPBroadcaster udpb = null;
    public static UDPReciever udpr = null;
    
    public static Thread serverThread;
	
	public static void StartServer_JAVA(final int port) throws IOException
	{	
		serverThread = new Thread(new StartServer_JAVA());
		serverThread.start();
	}
	
	public static class StartServer_JAVA extends Thread
	{
		public void run()
		{
			try
			{
	    		if(danchosServer != null)
	    		{
	    			AppActivity.MakeToast_JAVA("Server already started!");
	    			return;
	    		}
	    		
				danchosServer = new DanchosServer(7777);
				danchosServer.StartServer();
			}
			catch (IOException e)
			{
				AppActivity.MakeToast_JAVA("Server didn't start!");
			}
		}
	}
	
	public static void ConnectToServer_JAVA(final String IP, final int port) throws IOException
	{
		danchosClient = new DanchosClient(IP, port);
		danchosClient.ConnectToServer();
	}
	
	public static boolean PingIP_JAVA(String IP)
	{
		boolean isAvaivable = false;
		String[] splitIP = IP.split(Pattern.quote("."));
		
		byte[] ipAdress = new byte[4];
		
		ipAdress[0] = (byte)Integer.parseInt(splitIP[0]);
		ipAdress[1] = (byte)Integer.parseInt(splitIP[1]);
		ipAdress[2] = (byte)Integer.parseInt(splitIP[2]);
		ipAdress[3] = (byte)Integer.parseInt(splitIP[3]);
		
		InetAddress adress = null;
		
		try 
		{
			adress = InetAddress.getByAddress(ipAdress);
			isAvaivable = adress.isReachable(1000);
		} 
		catch (Exception e) 
		{
			AppActivity.MakeToast_JAVA("Gyrmi wa");
		}
		
		return isAvaivable;
	}
	
	public static void NetworkFunction_JAVA(final String parameters)
	{
		final List<String> parametersList = Arrays.asList(parameters.split(","));
		
		if(NetworkFunctions.SpawnEnemyShipBlast.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					float spawnXCoordInPercent = Float.parseFloat(parametersList.get(1));
					float blastAngle = (Float.parseFloat(parametersList.get(2)));
					int stacker = Integer.parseInt(parametersList.get(3));

					SpawnEnemyShipBlastJAVA(spawnXCoordInPercent, blastAngle, stacker);
				}
			});
		}
		else if(NetworkFunctions.RestartOpponentGame.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					RestartGameJAVA();
				}
			});
		}
		else if(NetworkFunctions.SendMyShipType.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					SetOpponentShipTypeJAVA(parametersList.get(1));
				}
			});
		}
		else if(NetworkFunctions.SendMyScreenDetails.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					SetOpponentScreenDetailsJAVA(Integer.parseInt(parametersList.get(1)), Integer.parseInt(parametersList.get(2)), Integer.parseInt(parametersList.get(3)));
				}
			});
		}
		else if(NetworkFunctions.SendMyName.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					SetOpponentNameJAVA(parametersList.get(1));
				}
			});
		}
		else if(NetworkFunctions.SetOpponentIsReady.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					if(parametersList.get(1).equals("true"))
						SetOpponentReadyBoolJAVA(true);
					else if(parametersList.get(1).equals("false"))
						SetOpponentReadyBoolJAVA(false);
				}
			});
		}
		else if(NetworkFunctions.LeaveOpponentGame.toString().equals(parametersList.get(0)))
		{
			AppActivity.currentActivity.runOnGLThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					LeaveGameSceneJAVA();
				}
			});
		}
	}
	
	public static void CloseServerClientConnectionJAVA()
	{
		if(danchosServer != null)
		{
			danchosServer.socketConnected = false;
			danchosServer = null;
		}
		else if(danchosClient != null)
		{
			danchosClient.socketConnected = false;
			danchosClient = null;
		}
	}
	
	public static void NetworkFunctionOnOpponent_JAVA(final String parameters) throws IOException
	{
		if(danchosServer != null)
		{
			danchosServer.SendString(parameters);
		}
		else if(danchosClient != null)
		{
			danchosClient.SendString(parameters);
		}
	}
	
	public static void StartHostGamesDiscoveryJAVA() throws Exception
	{
		new Thread()
		{
		    public void run() 
		    {
		    	try
				{
		    		if(udpr != null)
		    			return;

		    		udpr = new UDPReciever();
		    		udpr.transmit();
				}
				catch (Exception e)
				{
					AppActivity.MakeToast_JAVA("Failed to start discovering");
				}
		    }
		}.start();
	}
	
	public static void StartHostGameBroadcastingJAVA() throws Exception
	{
		new Thread()
		{
		    public void run() 
		    {
		    	try
				{
		    		if(udpb != null)
		    			return;
		    		
		    		udpb = new UDPBroadcaster();
		    		udpb.transmit();
				}
				catch (Exception e)
				{
					AppActivity.MakeToast_JAVA("Failed to start broadcasting");
				}
		    }
		}.start();
	}
	
	public static void StopHostGamesDiscoveryJAVA()
	{
		if(udpr != null)
			udpr.isRecieving = false;
	}
	
	public static void StopHostGameBroadcastingJAVA() throws Exception
	{
		udpb.isBroadcasting = false;
	}
	
	public static void StopServerThreadJAVA() throws Exception
	{
		if(serverThread != null)
		{
			if(danchosServer.socketConnected == false)
			{
				danchosServer.serverSocket.close();
				serverThread.interrupt();
				serverThread = null;
				danchosServer = null;
			}
			else
			{
				danchosServer.socketConnected = false;
				danchosServer = null;
			}
		}
	}
	
	public static void StopClientThreadJAVA()
	{
		danchosClient.socketConnected = false;
	}
	
	public static void ToastAllIPAdresses_JAVA()
	{
		AppActivity.MakeToast_JAVA(GetIPAdresses_JAVA());
	}
	
	public static void ToastWantedAdress_JAVA()
	{
		AppActivity.MakeToast_JAVA(GetWantedIPAdress_JAVA());
	}
	
	public static String IntToString_JAVA(int number)
	{
		return Integer.toString(number);
	}
	
	public static String FloatToString_JAVA(float number)
	{
		return Float.toString(number);
	}
	
	public static String GetIPAdresses_JAVA()
    {
        String allIPAdresses = "";
        Enumeration<NetworkInterface> e = null;
        
        try
        {
            e = NetworkInterface.getNetworkInterfaces();
        }
        catch (SocketException e1)
        {
            e1.printStackTrace();
        }

        if(e != null) 
        {
            while (e.hasMoreElements()) 
            {
                NetworkInterface n = (NetworkInterface) e.nextElement();
                Enumeration<InetAddress> ee = n.getInetAddresses();
                while (ee.hasMoreElements()) 
                {
                    InetAddress i = (InetAddress) ee.nextElement();
                    allIPAdresses += (i.getHostAddress() + "\n");
                }
            }
        }
        
        return allIPAdresses;
    }
	
	public static String GetWantedIPAdress_JAVA()
	{
        String wantedIP = "";
        Enumeration<NetworkInterface> e = null;
        
        try
        {
            e = NetworkInterface.getNetworkInterfaces();
        }
        catch (SocketException e1)
        {
            e1.printStackTrace();
        }

        if(e != null) 
        {
            while (e.hasMoreElements()) 
            {
                NetworkInterface n = (NetworkInterface) e.nextElement();
                Enumeration<InetAddress> ee = n.getInetAddresses();
                while (ee.hasMoreElements()) 
                {
                    InetAddress i = (InetAddress) ee.nextElement();
                    if(i.getHostAddress().toString().contains("192.168.") || i.getHostAddress().toString().startsWith("10."))
                    {
                    	wantedIP = i.getHostAddress();
                    	break;
                    }
                }
            }
        }
        
    	return wantedIP;
	}
	
	public static native void GoToGameSceneJAVA();
	public static native void GoToMainMenuSceneJAVA();
	public static native void LeaveGameSceneJAVA();
	public static native void RestartGameJAVA();
	public static native void SpawnEnemyShipBlastJAVA(float xCoord, float blastAngle, int stacker);
	public static native float GetScreenWidthJAVA();
	public static native String GetMyShipNameJAVA();
	public static native void SetOpponentShipTypeJAVA(String shipTypeString);
	public static native void SetOpponentScreenDetailsJAVA(int resolutionWidth, int resolutionHeight, int dpi);
	public static native void SetOpponentNameJAVA(String name);
	public static native void SetOpponentReadyBoolJAVA(boolean isReady);
	public static native void AddHostGameJAVA(String args);
}
