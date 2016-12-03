package org.cocos2dx.cpp;

import java.net.*;
import java.io.*;

import android.text.method.HideReturnsTransformationMethod;

public class DanchosClient 
{
	private Socket socket;
	private DataInputStream dataInputStream;
	private DataOutputStream dataOutputStream;
	
	private String IP;
	private int port;
	
	public boolean socketConnected = false;
	
	private String stringParameter;
	
	public DanchosClient(String IP, int port) throws IOException
	{
		this.IP = IP;
		this.port = port;
		this.stringParameter = "";
	}
	
	public void ConnectToServer()
	{
		try 
		{
			if(socketConnected)
				return;
			
			socket = new Socket();
			socket.connect(new InetSocketAddress(IP, port), 2000);
			socketConnected = true;
			InitilizeStreams();
			StartClientThread();
			
			CJNIHelper.StopHostGamesDiscoveryJAVA();
			CJNIHelper.GoToGameSceneJAVA();
		} 
		catch (Exception e)
		{
			AppActivity.MakeToast_JAVA("Sorry, this game wasn't available! Try again!");
			
			socketConnected = false;
			CJNIHelper.GoToMainMenuSceneJAVA();
		}
	}
	
	public void SendFloat(float floatNumber) throws IOException
	{
		dataOutputStream.writeFloat(floatNumber);
		dataOutputStream.flush();
	}

	private void RecieveString()
	{
		try {
			stringParameter = dataInputStream.readUTF();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		CJNIHelper.NetworkFunction_JAVA(stringParameter);
		
		stringParameter = "";
	}
	
	public void SendString(String string) throws IOException
	{
		dataOutputStream.writeUTF(string);
		dataOutputStream.flush();
	}

	private void StartClientThread()
	{
		Thread clientThread = new Thread(new ClientThread());
		clientThread.start();
	}
	
	class ClientThread implements Runnable
	{
		@Override
		public void run()
		{
			while(socketConnected)
			{
				try 
				{
					if(dataInputStream.available() == 0)
						continue;
				} 
				catch (IOException e1) {}
				
				RecieveString();
			}
			
			try {
				dataInputStream.close();
				dataOutputStream.close();
				socket.close();
			} catch (IOException e) {
				AppActivity.MakeToast_JAVA("grymna zatwarqneto na klienta" );
			}
			
			CJNIHelper.danchosServer = null;
			Thread.currentThread().interrupt();
		}
	}
	
	private void InitilizeStreams() throws IOException
	{
		this.dataInputStream = new DataInputStream(this.socket.getInputStream());
		this.dataOutputStream = new DataOutputStream(this.socket.getOutputStream());
	}
	
	public boolean IsConnectedToServer()
	{
		return socketConnected;
	}
}
