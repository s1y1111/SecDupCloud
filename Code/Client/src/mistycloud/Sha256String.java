package mistycloud;
import java.security.MessageDigest;

public class Sha256String {
	/**
	 * sha256 hash algorithm
	 * @param String plaintext
	 * @return String 256
	 */
	public static String sha256(String plain)throws Exception {
		String sha;
		MessageDigest md = MessageDigest.getInstance("SHA-256");
		md.update(plain.getBytes());
		byte[] mdbytes = md.digest();
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < mdbytes.length; i++) {
			sb.append(Integer.toString((mdbytes[i] & 0xff) + 0x100, 16).substring(1));
	    }
		sha = sb.toString();
		return sha;
	}

}
