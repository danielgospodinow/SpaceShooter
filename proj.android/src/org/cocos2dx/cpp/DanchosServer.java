package org.cocos2dx.cpp;

import java.net.*;
import java.io.*;

public class DanchosServer 
{
	public ServerSocket serverSocket;
	public Socket socket;
	private DataInputStream dataInputStream;
	private DataOutputStream dataOutputStream;
	
	private int port;

	public boolean serverAlreadyStarted;
	public boolean socketConnected;
	
	private String stringParameter;
	
	public DanchosServer(int port) throws IOException
	{
		this.port = port;
		this.socketConnected = false;
		serverAlreadyStarted = false;
		this.stringParameter = "";
	}
	
	public void StartServer()
	{
		try 
		{
			serverAlreadyStarted = true;
			this.serverSocket = new ServerSocket(this.port);
			this.socket = serverSocket.accept();
			socketConnected = true;
			InitilizeStreams();
			StartServerThread();
			
			CJNIHelper.StopHostGameBroadcastingJAVA();
			AppActivity.GoToGameScene();
		} 
		catch (Exception e)
		{
			socketConnected = false;
		}
	}
	
	public void SendFloat(float floatNumber) throws IOException
	{
		dataOutputStream.writeFloat(floatNumber);
		dataOutputStream.flush();
	}
	
	public void SendString(String string) throws IOException
	{
		dataOutputStream.writeUTF(string);
		dataOutputStream.flush();
	}

	private void RecieveString()
	{
		try {
			stringParameter = dataInputStream.readUTF();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		CJNIHelper.NetworkFunction_JAVA(stringParameter);
		
		stringParameter = "";
	}
	
	private void StartServerThread()
	{
		new Thread()
		{
			public void run()
			{
				while(socketConnected)
				{
					try 
					{
						if(dataInputStream.available() == 0)
							continue;
					} 
					catch (IOException e) 
					{
						e.printStackTrace();
					}
					
					RecieveString();
				}
				
				try {
					dataInputStream.close();
					dataOutputStream.close();
					socket.close();
					serverSocket.close();
				} catch (IOException e) {
					AppActivity.MakeToast_JAVA("grymna zatwarqneto na syrwara");
				}
				
				CJNIHelper.danchosClient = null;
				Thread.currentThread().interrupt();
			}
		}.start();
	}
	
	private void InitilizeStreams() throws IOException
	{
		this.dataInputStream = new DataInputStream(this.socket.getInputStream());
		this.dataOutputStream = new DataOutputStream(this.socket.getOutputStream());
	}
	
	public boolean IsClientConnected()
	{
		return socketConnected;
	}
}
