package mistycloud;
import java.io.FileInputStream;
import java.security.MessageDigest;
import java.sql.Time;

public class Sha256File {
	
	/**
	 * return the sha256 of a file
	 * @param String FilePath
	 * @return String sha256 hexformat 64
	 * @author kimi
	 */
	 public static String SHA256; 
	 public static String sha256(String SourceFilePath)throws Exception
	 {
	    	long t1 = System.currentTimeMillis();
		 	MessageDigest md = MessageDigest.getInstance("SHA-256");
	        FileInputStream fis = new FileInputStream(SourceFilePath); //file read in
	        byte[] dataBytes = new byte[1024*512];
	        int nread = 0; 
	        while ((nread = fis.read(dataBytes)) != -1) {
	          md.update(dataBytes, 0, nread);
	        };
	        byte[] mdbytes = md.digest();
	     
	        //convert the byte to hex format method 1
	        StringBuffer sb = new StringBuffer();
	        for (int i = 0; i < mdbytes.length; i++) {
	          sb.append(Integer.toString((mdbytes[i] & 0xff) + 0x100, 16).substring(1));
	        }
	        long t2 = System.currentTimeMillis();
	        SHA256=sb.toString();

	        return sb.toString();
	  }
	 public static String reSha()
	 {
		 return SHA256;
	 }

}
