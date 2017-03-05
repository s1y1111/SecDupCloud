package mistycloud;

import java.io.IOException;
import java.io.InputStream;

public class AddFirend {
	public static byte[] addFriendRe(String Fname) throws Exception
	{
		byte[] Re =new byte[65];
		Re[0]=10;
		byte[] cookie = Cookie.returnCookie();
		for(int i=0;i<32;i++){
			Re[1+i]=cookie[i];
		}
		byte[] nHash = Util.hexToByte(Sha256String.sha256(Fname));
		for(int j=0;j<32;j++){
			Re[33+j]=nHash[j];
		}
		return Re;
		
	}
	
	public static boolean addSuccess() throws IOException
	{
		byte[] flag =new byte[1];
		GetSocket.getInStream().read(flag,0,1);
		if(Util.onebyteToInt(flag[0])==106){
			return true;
		}
		else
			return false;
	}
	
	public static byte[] returnPubkey() throws Exception
	{
		
			byte[] PubKey = new byte[130];
			int count=0;
			InputStream in =GetSocket.getInStream();
			while(count!=130){
				count+=in.read(PubKey, count, 130-count);
			}
			return PubKey;
	
	}

}
