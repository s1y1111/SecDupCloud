package mistycloud;

import java.io.*;
import java.net.*;
public class SendByte {
	public static void Send(byte[] toSend)throws Exception
	{
		int length = toSend.length;
		DataOutputStream dout = GetSocket.getOutStream();
		dout.write(toSend,0,length);
		dout.flush();
	}

}
