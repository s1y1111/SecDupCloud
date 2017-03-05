package mistycloud;

public class Cookie {
	private static byte[] cookie;
	
	public static void saveCookie(byte[] cookieInitial) 
	{
		cookie = cookieInitial;
	}
	
	public static byte[] returnCookie()
	{
		return cookie;
	}

}
