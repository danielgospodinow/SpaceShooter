package org.cocos2dx.cpp;

import java.io.*;
import java.net.*;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.Iterator;

public class UDPBroadcaster 
{
	public boolean isBroadcasting;
    private String messageToClients;
    private final byte[] buffer;
    private InetAddress address;
    private final int port;
    private DatagramPacket packet;
    private final DatagramSocket socket;
    
    public UDPBroadcaster() throws IOException
    {
    	messageToClients = CJNIHelper.GetWantedIPAdress_JAVA() + "," + CJNIHelper.GetMyShipNameJAVA();
    	buffer = messageToClients.getBytes();
        socket = new DatagramSocket();
        messageToClients = getMyAddress();
        address = InetAddress.getByName(getBroadcastAddress());
        isBroadcasting = true;
        port = 1502;
    }

    public void transmit() 
    {
        try
        {
            while(isBroadcasting)
            {	
                packet = new DatagramPacket(buffer, buffer.length, address, port);
                socket.send(packet);
                
                Thread.sleep(200);
            }

            socket.close();
            CJNIHelper.udpb = null;
            Thread.currentThread().interrupt();
        }
        catch(Exception e){AppActivity.MakeToast_JAVA("Grymna Broadcastera");}
    }
    
    public String getMyAddress()
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
                    String adress = i.getHostAddress().toString();
                    
                    if(adress.contains("192.168.") || adress.startsWith("10."))
                    {
                        wantedIP = i.getHostAddress();
                        break;
                    }
                }
            }
        }
        
        return wantedIP;
    }
    
    @SuppressWarnings("rawtypes")
	public String getBroadcastAddress()
    {
        HashSet<InetAddress> listOfBroadcasts = new HashSet<InetAddress>();
        Enumeration list;
        try 
        {
            list = NetworkInterface.getNetworkInterfaces();

            while(list.hasMoreElements()) 
            {
                NetworkInterface iface = (NetworkInterface) list.nextElement();

                if(iface == null) continue;

                if(!iface.isLoopback() && iface.isUp()) 
                {
					Iterator it = iface.getInterfaceAddresses().iterator();
                    while (it.hasNext()) 
                    {
                        InterfaceAddress address = (InterfaceAddress) it.next();
                        if(address == null) continue;
                        InetAddress broadcast = address.getBroadcast();
                        if(broadcast != null) 
                        {
                            listOfBroadcasts.add(broadcast);
                        }
                    }
                }
            }
        } 
        catch (SocketException ex) {}
        
        if(!listOfBroadcasts.isEmpty())
            return listOfBroadcasts.toArray()[0].toString().split("/")[1];
        return "";
    }
}
