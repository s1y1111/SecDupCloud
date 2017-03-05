package mistycloud;



public class getFileListRequest {
	public static byte[] askFileList()
	{
		byte[] cookie = Cookie.returnCookie();
		byte[] request = new byte[33];
		
		//byte flag = Flag.REQUIRE_FILES;
		//flag
		request[0] =2;
		
		for(int i=0;i<32;i++){
			request[i+1] = cookie[i];
		}
		
		return request;
		
	}
	
	

}
