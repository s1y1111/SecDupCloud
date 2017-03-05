package mistycloud;
public class SeedProducer {
	private static byte[] seed = new byte[64];
	/**
	 *  depend on the user info such as the user name+password
	 *  produce the hash of it
	 * @param String userinfo
	 * @return void
	 */
	public static void seedPro(String userName,String userPass)throws Exception {
		seed = Sha256String.sha256(Sha256String.sha256(userName)+
				Sha256String.sha256(userPass)).getBytes();
		
	}
	/**
	 * 
	 * @return byte[] seed
	 */
	public static byte[] returnSeed() {
		return seed;
	}
}
