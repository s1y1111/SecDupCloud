package mistycloud;
import java.io.IOException;  
import java.math.BigInteger;  
import org.bouncycastle.crypto.AsymmetricCipherKeyPair;  
import org.bouncycastle.crypto.params.ECPrivateKeyParameters;  
import org.bouncycastle.crypto.params.ECPublicKeyParameters;  
import org.bouncycastle.math.ec.ECPoint;  
  
public class SM2Utils   
{  
	private static String pubk;
    private static String prik;
	
    
    /**
	 * 生成随机秘钥对  
	 * Generate the public and private key
	 * ATTENTION! need set the seed first
	 */
    public static void generateKeyPair(){  
        SM2 sm2 = SM2.Instance();  
        AsymmetricCipherKeyPair key = sm2.ecc_key_pair_generator.generateKeyPair();  
        ECPrivateKeyParameters ecpriv = (ECPrivateKeyParameters) key.getPrivate();  
        ECPublicKeyParameters ecpub = (ECPublicKeyParameters) key.getPublic();  
        BigInteger privateKey = ecpriv.getD();  
        prik = Util.byteToHex(privateKey.toByteArray());
        
        ECPoint publicKey = ecpub.getQ();  
        pubk=Util.byteToHex(publicKey.getEncoded());
        //System.out.println("公钥: " +pubk);  
        //System.out.println("私钥: " +prik);  
        //System.out.println("公钥: " + Util.byteToHex(publicKey.getEncoded()));  
        //System.out.println("私钥: " + Util.byteToHex(privateKey.toByteArray()));  
    }  
      
    //数据加密  
    public static String encrypt(byte[] publicKey, byte[] data) throws IOException  
    {  
        if (publicKey == null || publicKey.length == 0)  
        {  
            return null;  
        }  
          
        if (data == null || data.length == 0)  
        {  
            return null;  
        }  
          
        byte[] source = new byte[data.length];  
        System.arraycopy(data, 0, source, 0, data.length);  
          
        Cipher cipher = new Cipher();  
        SM2 sm2 = SM2.Instance();  
        ECPoint userKey = sm2.ecc_curve.decodePoint(publicKey);  
          
        ECPoint c1 = cipher.Init_enc(sm2, userKey);  
        cipher.Encrypt(source);  
        byte[] c3 = new byte[32];  
        cipher.Dofinal(c3);  
          
//      System.out.println("C1 " + Util.byteToHex(c1.getEncoded()));  
//      System.out.println("C2 " + Util.byteToHex(source));  
//      System.out.println("C3 " + Util.byteToHex(c3));  
        //C1 C2 C3拼装成加密字串  
        return Util.byteToHex(c1.getEncoded()) + Util.byteToHex(source) + Util.byteToHex(c3);  
          
    }  
      
    /**
     * plainText = new String(SM2Utils.decrypt(Util.hexToByte(prik), Util.hexToByte(cipherText))); 
     * 数据解密  
     */
    public static byte[] decrypt(byte[] privateKey, byte[] encryptedData) throws IOException  
    {  
        if (privateKey == null || privateKey.length == 0)  
        {  
            return null;  
        }  
          
        if (encryptedData == null || encryptedData.length == 0)  
        {  
            return null;  
        }  
        //加密字节数组转换为十六进制的字符串 长度变为encryptedData.length * 2  
        String data = Util.byteToHex(encryptedData);  
        /***分解加密字串 
         * （C1 = C1标志位2位 + C1实体部分128位 = 130） 
         * （C3 = C3实体部分64位  = 64） 
         * （C2 = encryptedData.length * 2 - C1长度  - C2长度） 
         */  
        byte[] c1Bytes = Util.hexToByte(data.substring(0,130));  
        int c2Len = encryptedData.length - 97;  
        byte[] c2 = Util.hexToByte(data.substring(130,130 + 2 * c2Len));  
        byte[] c3 = Util.hexToByte(data.substring(130 + 2 * c2Len,194 + 2 * c2Len));  
          
        SM2 sm2 = SM2.Instance();  
        BigInteger userD = new BigInteger(1, privateKey);  
          
        //通过C1实体字节来生成ECPoint  
        ECPoint c1 = sm2.ecc_curve.decodePoint(c1Bytes);  
        Cipher cipher = new Cipher();  
        cipher.Init_dec(userD, c1);  
        cipher.Decrypt(c2);  
        cipher.Dofinal(c3);  
          
        //返回解密结果  
        return c2;  
    }  
    
    public static String ReturnPub() {
    	generateKeyPair();
    	return pubk;
    } 
    
    public static String ReturnPri() {
    	generateKeyPair();
    	return prik;
    }
    /*public static void main(String[] args) throws Exception   
    {  
        //生成密钥对  
    	SeedProducer.seedPro("4a67d922de76836fbfec337e1da243ee8ec97b99e6f083732e88a2ffa14d4b8c");
        generateKeyPair();  
        //64hash seed 加密 64 hash  322位  
        String plainText = "4a67d922de76836fbfec337e1da243ee8ec97b99e6f083732e88a2ffa14d4b8c";  
        byte[] sourceData = plainText.getBytes();  
          
        //下面的秘钥可以使用generateKeyPair()生成的秘钥内容  
        // 国密规范正式私钥  
        //String prik = "3690655E33D5EA3D9A4AE1A1ADD766FDEA045CDEAA43A9206FB8C430CEFE0D94";
        //String prik = "0096CB4CEDC6D78A4666351950DD1D06AF55611874210658966039DEE8335B88FD";
        // 国密规范正式公钥  
        //String pubk = "04F6E0C3345AE42B51E06BF50B98834988D54EBC7460FE135A48171BC0629EAE205EEDE253A530608178A98F1E19BB737302813BA39ED3FA3C51639D7A20C7391A";  
        //String pubk = "04E98154804CC1287C98E5C734D2ACED67C94EAD299700D73BF010D03E9E0FB6FC45BAC9D21AEA8AF18BA17B5DFD3658084F618F39631A6591CF60CDC42A8CE5B8";
          
        System.out.println("加密: ");  
        String cipherText = SM2Utils.encrypt(Util.hexToByte(pubk), sourceData);  
        System.out.println(cipherText);  
        System.out.println("解密: ");  
        plainText = new String(SM2Utils.decrypt(Util.hexToByte(prik), Util.hexToByte(cipherText)));  
        System.out.println(plainText);  
          
    }  */
}  