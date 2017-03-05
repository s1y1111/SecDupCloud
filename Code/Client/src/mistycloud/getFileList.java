package mistycloud;
import java.io.*;
import java.net.*;

public class getFileList {
		
	public static Object[][] fileList(int index)throws Exception {
		DataInputStream in = GetSocket.getInStream();
		if(index==0){
			Object[][]list ={};
			return list;
		}
		else{
		Object[][] list = new Object[index][7];
		for(int i=0;i<index;i++) {
			/*The Object[] format is
			 * 
			 * number£¬1 file Name£¬ file size, false, 4tag , 5key
			 */
			
			//number
			list[i][0]=i+1;
			
			//file name
			byte[] nz = new byte[1];
			in.read(nz,0,1);
			
			int nameSize=Util.onebyteToInt(nz[0]);
			byte[] name = new byte[nameSize];
			int n =0;
			while(n!=nameSize){
				n+=in.read(name,n,nameSize-n);
			}
			System.out.println(nameSize);
			
			
			String fileName = new String(name);
			list[i][1]=fileName;
			/*byte[] zeroCorrect =new byte[1];
			in.read(zeroCorrect,0,1);*/
			
			//file size   KB
			byte[] size = new byte[4];
			
			in.read(size,0,4);
			String FileSize;
			int fileSize = (Util.byteToInt(size)*16)/1024;
			if(fileSize>9999){
				fileSize /= 1024;
				FileSize = fileSize +"MB";
			}
			else{
				FileSize =fileSize+"KB";
			}
			list[i][2]=FileSize;
			
			
			//System.out.println(Util.byteToInt(size));
			//boolean
			list[i][3]=false;
			
			//tag
			byte[] tag = new byte[32];
			in.read(tag,0,32);
			//String fileTag = Util.byteToHex(tag);
			list[i][4]=tag;
			//System.out.println(Util.byteToHex(tag));
			//key
			byte[] key = new byte[322];
			int keyN = 0;
			while(keyN!=322){
				keyN+=in.read(key,keyN,322-keyN);
			}
			
			list[i][5] = key;
			//System.out.println(key);
			
			byte[] count = new byte[4];
			count = size;
			list[i][6] = count;
		}
		
		return list;
		}
	}
	
	public static int fileNum() throws Exception {
		DataInputStream in = GetSocket.getInStream();
		//receive the flag
		byte[] flag = new byte[1];
		in.read(flag,0,1);
		//System.out.println(Util.onebyteToInt(flag[0]));
		
		//number of Files
		byte[] n = new byte[4];
		in.read(n, 0, 4);
		int num = Util.byteToInt(n);
		//System.out.println(num);
		return num;
	
	}
	

}
