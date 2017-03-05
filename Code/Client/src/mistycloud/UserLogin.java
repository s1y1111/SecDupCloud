package mistycloud;

public class UserLogin {
	private static byte[] b = new byte[512];
	private static int Length;
	public static void loginString(String userName,String userPass)throws Exception
	{
		
		b[0] = 9;//flag!!! 

		UserSign.Hash(userName, userPass);
		byte[] hash = UserSign.HashReturn();
		for(int j=0;j<64;j++){
			b[j+1] = hash[j];
		}
		byte[] temp = userName.getBytes();
		byte length = Util.intToOnebyte(temp.length);
		b[65] = length ;
		for(int i=0;i<temp.length;i++){
			b[66+i]=temp[i];
		}
		
		byte[] pubkey = (SM2Utils.ReturnPub()).getBytes();
		for(int j=0;j<130;j++){
			b[66+temp.length+j]=pubkey[j];
		}
		Length=1+32+32+1+temp.length+130;
		
	}
	
	public static byte[] returnLoginString()
	{
		byte[] loginString = new byte[Length];
		for(int index=0;index<Length;index++){
			loginString[index] = b[index];
		}
		return loginString;
	}

}
