package mistycloud;
import java.io.*;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;

import UI_view.UploadstuffMainFrom;

public class UploadRequest {
	public static JFrame f2;
	public static int UploadingFlag =0;
	/**
	 * ATTENTION!  need to call seed producer first
	 * so that the SM2 can work
	 * 
	 * @param String filepath
	 * @return byte[] UpFile Request with control flag
	 */
	public static byte[] upFile(String FilePath)throws Exception
	{
		//Thread.sleep(5000);
		//UploadstuffMainFrom.uploadingflag = 1;
		//Thread.sleep(5000);
		//UploadstuffMainFrom.uploadingflag = 2;
		
		byte[] flag = {4}; //CHECK_FILE  1位
		
		File sourceFile = new File(FilePath);
		byte[] cookie = Cookie.returnCookie(); //32位
		
		String nameTemp = sourceFile.getName();
		byte[] fileName = nameTemp.getBytes(); //nameLengthTemp 位
		
		
		int namelengthTemp = fileName.length;
		byte nameLength = Util.intToOnebyte(namelengthTemp);// 1位
		
		//System.out.println(Util.onebyteToInt(nameLength));
		
		double sizettt = Math.ceil((double)((double)sourceFile.length()/16))+1;
		long sizeTemp = (long)sizettt;
		if(sourceFile.length()%16==0)
			sizeTemp++;
		byte[] fileSize = Util.longToBytes(sizeTemp);  //4位
		//System.out.println(Util.byteToInt(fileSize));
		
		String hash = Sha256File.sha256(FilePath);
		byte[] hashhash = Util.hexToByte(Sha256String.sha256(hash)); //32
		
		
		//System.out.println("公钥："+SM2Utils.ReturnPub());
		String pubkHash = SM2Utils.encrypt(Util.hexToByte(SM2Utils.ReturnPub()), hash.getBytes());
		byte[] pubkeyHash = pubkHash.getBytes(); //322位
		//System.out.println(pubkeyHash.length);
		
		int l = 1+32+namelengthTemp+1+4+32+322;
		byte[] request = new byte[l];
		
		//flag 0
		request[0] = flag[0];
		
		//cookie 1-32
		for(int i=0;i<32;i++){
			request[i+1] = cookie[i];
		}
		
		//nameLength 33
		request[33] = nameLength;
		
		
		// fileName 34-33+namelengthTemp
		for(int i=0;i<namelengthTemp;i++){
			request[34+i] = fileName[i];
		}
		
		//fileSize 34+namelengthTemp-37+namelengthTemp
		for(int i=0;i<4;i++){
			request[34+namelengthTemp+i] = fileSize[i];
		}
		
		//hashhash 38+namelengthTemp - 69+namelengthTemp
		for(int i=0;i<32;i++){
			request[38+namelengthTemp+i] = hashhash[i];
		}
		
		//pubkeyhash 70+namelengthTemp - 391+namelengthTemp
		for(int i=0;i<322;i++){
			request[70+namelengthTemp+i] = pubkeyHash[i];
		}
		
		return request;
	}
	
	
	
	
	 
}
