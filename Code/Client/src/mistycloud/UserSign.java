package mistycloud;
import java.io.*;

public class UserSign {
	private static String nameHash;
	private static String passwordHash;
	private static int SignSuccess;
	public static void Hash(String userName, String userPass)throws Exception
	{
		
		nameHash = Sha256String.sha256(userName);
		passwordHash = Sha256String.sha256(userPass);
	}
	
	public static byte[] HashReturn() 
	{
		return Util.hexStringToBytes(nameHash+passwordHash);
	}
	
	public static void SignSuccess() throws Exception
	{
		DataInputStream in =GetSocket.getInStream();
		byte[] flag = new byte[1];
		in.read(flag,0,1);
		if(Util.onebyteToInt(flag[0]) == 101){
			byte[] cookie = new byte[32];
			in.read(cookie,0,32);
			Cookie.saveCookie(cookie);
			SignSuccess = 1;
		}
		else
			SignSuccess = 0;
		
	}
	public static int Flag()
	{
		return SignSuccess;
	}
	
	
	/**
	 * return the byte[65] with the flag byte
	 * Used to sign in with the server
	 * ATTENTION need call UserSign.Hash(name, pass) first
	 * to initialize
	 * 
	 */
	public static byte[] SignIn()
	{
		byte[] temp = Util.hexStringToBytes(nameHash+passwordHash);
		byte[] r = new byte[65];
		r[0] = 8; ///flag!!!!
		for(int index=0;index<64;index++){
			r[index+1]=temp[index];
		}
		return r;
	}
	
	/*public static int returnSignflag()
	{
		
	}*/

}
