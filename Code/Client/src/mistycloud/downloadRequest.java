package mistycloud;

public class downloadRequest {
	public static byte[] downloadFileRequest(byte[] tag)
	{
		byte[] cookie = Cookie.returnCookie();
		byte[] hashhash = tag;
		byte[] request = new byte[65];
		
		byte flag = 6;
		//flag
		request[0] = flag;
		
		for(int i=0;i<32;i++){
			request[i+1] = cookie[i];
		}
		for(int i=0;i<32;i++){
			request[i+33] = hashhash[i];
		}
		
		
		return request;
		
	}

}
