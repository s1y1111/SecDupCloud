package mistycloud;

public class FriendListRe {
	public static byte[] askFriendList()
	{
		byte[] cookie = Cookie.returnCookie();
		byte[] request = new byte[33];
		
		//byte flag = Flag.REQUIRE_FRIENDS;
		//flag
		request[0] =3;
		
		for(int i=0;i<32;i++){
			request[i+1] = cookie[i];
		}
		
		return request;
		
	}

}
