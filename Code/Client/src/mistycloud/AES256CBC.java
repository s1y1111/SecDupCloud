package mistycloud;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.SecureRandom;
import java.security.Security;

import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.CipherOutputStream;
import javax.crypto.KeyGenerator;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
public class AES256CBC {
	private static final String ALGORITHM = "AES";
	public static final String CIPHER_ALGORITHM="AES/CBC/PKCS7Padding";
	private static final int CACHE_SIZE = 256;
	
	public static void encryptFile(String password,byte [] IV, String sourceFilePath,
			                String destFilePath) throws Exception {
		 File sourceFile = new File(sourceFilePath);
         File destFile = new File(destFilePath); 
         if (sourceFile.exists() && sourceFile.isFile()) {
             if (!destFile.getParentFile().exists()) {
                 destFile.getParentFile().mkdirs();
             }
             destFile.createNewFile();
             InputStream in = new FileInputStream(sourceFile);
             OutputStream out = new FileOutputStream(destFile);
             //Key Generate
             KeyGenerator kgen = KeyGenerator.getInstance(ALGORITHM);
             Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
             kgen.init(256, new SecureRandom(password.getBytes()));  
             SecretKey secretKey = kgen.generateKey();  
             byte[] enCodeFormat = secretKey.getEncoded();  
             SecretKeySpec key = new SecretKeySpec(enCodeFormat, ALGORITHM);              
             
             //Cipher Body
             Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM,"BC");// 创建密码器   
             cipher.init(Cipher.ENCRYPT_MODE, key,new IvParameterSpec(IV));
             CipherInputStream cin = new CipherInputStream(in, cipher);
             byte[] cache = new byte[CACHE_SIZE];
             int nRead = 0;
            //Write IV
             ByteArrayOutputStream b = new ByteArrayOutputStream();
             b.write(IV);
             b.writeTo(out);
             b.close();
             
             while ((nRead = cin.read(cache)) != -1) {
                 out.write(cache, 0, nRead);
                 out.flush();
             }
             out.close();
             cin.close();
             in.close();
         }
     }
	
	public static void decryptFile(String password, String sourceFilePath, 
                                            String destFilePath) throws Exception {
        File sourceFile = new File(sourceFilePath);
        File destFile = new File(destFilePath); 
        if (sourceFile.exists() && sourceFile.isFile()) {
            if (!destFile.getParentFile().exists()) {
                destFile.getParentFile().mkdirs();
            }
            destFile.createNewFile();
            FileInputStream in = new FileInputStream(sourceFile);
            FileOutputStream out = new FileOutputStream(destFile);
            
            KeyGenerator kgen = KeyGenerator.getInstance(ALGORITHM);
            Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
            kgen.init(256, new SecureRandom(password.getBytes()));  
            SecretKey secretKey = kgen.generateKey();  
            byte[] enCodeFormat = secretKey.getEncoded();  
            SecretKeySpec key = new SecretKeySpec(enCodeFormat, ALGORITHM);              
            //Iv storage
            byte [] IV =new byte[16];
            in.read(IV,0,16);
            for(int i=0;i<16;i++)
            	System.out.println(IV[i]);
           
             //Cipher initial
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM,"BC");// 创建密码器   
            cipher.init(Cipher.DECRYPT_MODE, key,new IvParameterSpec(IV));
            CipherOutputStream cout = new CipherOutputStream(out, cipher);
            
            byte[] cache = new byte[CACHE_SIZE];
            int nRead = 0;
            while ((nRead = in.read(cache)) != -1) {
                cout.write(cache, 0, nRead);
                cout.flush();
            }
            cout.close();
            out.close();
            in.close();
        }
    }
        
} 


