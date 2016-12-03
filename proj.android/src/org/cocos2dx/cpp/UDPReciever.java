package org.cocos2dx.cpp;

import java.net.*;

public class UDPReciever 
{
	public boolean isRecieving;
    public InetAddress address;
    public byte[] buffer;
    public String args;
    public DatagramPacket packet;
    public MulticastSocket socket;
    public int port;
    
    public UDPReciever() throws Exception
    {
    	port = 1502;
        socket = new MulticastSocket(port);
        isRecieving = true;
    }

    public void transmit() 
    {
        try
        {
            address = InetAddress.getByName("233.0.0.1");
            socket.joinGroup(address);
            
            Thread.sleep(500);
            
            while(isRecieving)
            {
                buffer = new byte [31];
                packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);
                args = new String(packet.getData());
                
                if(args != "") //testwai qko tuka
                {
                	AppActivity.currentActivity.runOnGLThread(new Runnable() 
        			{
        				@Override
        				public void run() 
        				{
                        	CJNIHelper.AddHostGameJAVA(args);
        				}
        			});
                }

            	Thread.sleep(1000); //moje bi 500
            }
            
            socket.leaveGroup(address);
            socket.close();
            CJNIHelper.udpr = null;
            Thread.currentThread().interrupt();
        }
        catch(Exception e){AppActivity.MakeToast_JAVA("Grymna Recievera");}
    }
}