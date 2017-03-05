package mistycloud;

import java.io.*;
import java.net.*;
import java.security.SecureRandom;
import java.security.Security;

import javax.crypto.Cipher;
import javax.crypto.CipherInputStream;
import javax.crypto.CipherOutputStream;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import UI_view.DownloadingSucceed;

import java.net.InetSocketAddress;
import java.net.Socket;

public class ClientTest {
	private static final String ALGORITHM = "AES";
	public static final String CIPHER_ALGORITHM="AES/CBC/PKCS7Padding";
	private static final int CACHE_SIZE = 256;


    
    public static void encryptFile (String password,byte [] IV, String sourceFilePath,
    		OutputStream DestStream,int left,int right) throws Exception {
		
    	File sourceFile = new File(sourceFilePath);
        //File destFile = new File(destFilePath); 
        if (sourceFile.exists() && sourceFile.isFile()) {
            // if (!destFile.getParentFile().exists()) {
            //   destFile.getParentFile().mkdirs();
            // }
            //destFile.createNewFile();
        	//System.out.println("yes filepath");
            
        	InputStream in = new FileInputStream(sourceFile);
            OutputStream out = DestStream;
            
            

            
           /* File destFile = new File("F:\\CryptoContest\\Code\\AESUTILS_E.java");
            if (!destFile.getParentFile().exists()) {
                destFile.getParentFile().mkdirs();
            }
            destFile.createNewFile();
            OutputStream fout = new FileOutputStream(destFile);*/
            
            //Key Generate
            KeyGenerator kgen = KeyGenerator.getInstance(ALGORITHM);
            Security.addProvider(new org.bouncycastle.jce.provider.BouncyCastleProvider());
            kgen.init(256, new SecureRandom(password.getBytes()));  
            SecretKey secretKey = kgen.generateKey();  
            byte[] enCodeFormat = secretKey.getEncoded();  
            SecretKeySpec key = new SecretKeySpec(enCodeFormat, ALGORITHM);              
            
            //Cipher Body
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);// 创建密码器   
            cipher.init(Cipher.ENCRYPT_MODE, key,new IvParameterSpec(IV));
            CipherInputStream cin = new CipherInputStream(in, cipher);
            
            byte[] cache = new byte[CACHE_SIZE];
            long t1 = System.currentTimeMillis();
            int nRead = 0;
            /*ByteArrayOutputStream b = new ByteArrayOutputStream();
            b.write(flag);
            b.writeTo(out);
            out.flush();
            
            b.write(cookie);
            b.writeTo(out);
            out.flush();
            
            b.write(IV);
            b.writeTo(out);
            out.flush();
            
            b.close();*/
            //System.out.println("while");
            
            
            //简单判断是不是check检验
            if(left == 0){
            	while ((nRead = cin.read(cache)) != -1) {
                    out.write(cache, 0, nRead);
                    
                    out.flush();
                   // fout.write(cache, 0, nRead);
                   // fout.flush();
                }
            	in.close();
            	cin.close();
            	long t2 = System.currentTimeMillis();
            }
            
            else{
            	
            	int index = right+1 - left;
            	long a=(left-1)*16;
            	//System.out.println(a);
            	while(a!=0)
            	{
            		a-=in.skip(a);
            	}
            	//System.out.println(a);
            	//byte[] ccc = new byte[32];
            	//in.read(ccc);
            	//System.out.print(Util.byteToHex(ccc));
           
            	byte[] checkcache = new byte[16];
            	for(int i=0;i<index;i++){
            
            		cin.read(checkcache);
            		//System.out.println(i);
            		out.write(checkcache,0,16);
            		out.flush();
            		
            	}
            	in.close();
            	cin.close();
            }
            
            
         /*   int index=0; //Use the left and right shift to calculate the encrypt part
            while ((nRead = cin.read(cache)) != -1) {
                out.write(cache, 0, nRead);
                out.flush();
                System.out.println(index++);
                //System.out.println(nRead);
            }*/
            //sleep(1000);
            
            /*out.close();
            cin.close();
            in.close();*/
        }
    }
    //String password = SM2Utils.decrypt(Util.hexToByte(SM2Utils.ReturnPri()), byte[] Table[][])
    public static void decryptFile(String password,String destFilePath,String FileName,int count)throws Exception
    {
    	//File sourceFile = new File(sourceFilePath);
        File destFile = new File(destFilePath+FileName); 
        //if (sourceFile.exists() && sourceFile.isFile()) {
            if (!destFile.getParentFile().exists()) {
                destFile.getParentFile().mkdirs();
            }
            destFile.createNewFile();
            DataInputStream in = GetSocket.getInStream();
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
            	System.out.println("IV:"+IV[i]);
           
             //Cipher initial
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM,"BC");// 创建密码器   
            cipher.init(Cipher.DECRYPT_MODE, key,new IvParameterSpec(IV));
            CipherOutputStream cout = new CipherOutputStream(out, cipher);
            
            byte[] cache = new byte[256];
            int nRead = 0;
            count-=1;
            count*=16;      
            int cc=0;
            int number = 1;
            while (count!=0) {
            	nRead = count<256?count:256;
            	number=in.read(cache,0,nRead);	
            	cout.write(cache,0,number);
            	count-=number;
            	cc+=number;
            }
            System.out.println(cc);
            cout.flush();
            cout.close();
            out.flush();
            out.close();
            
            new DownloadingSucceed();
            /*cout.close();
            out.close();
            in.close();*/
        
    }
    
    

}
