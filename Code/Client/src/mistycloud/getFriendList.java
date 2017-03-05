package mistycloud;

import java.io.DataInputStream;

public class getFriendList {
	public static Object[][] fileList(int index)throws Exception{
		DataInputStream in = GetSocket.getInStream();
		if(index==0){
			Object[][]list ={};
			return list;
		}
		else{
			
			// name  false hash publickey
			Object[][] list = new Object[index][5];
			for(int i=0;i<index;i++){
				//0
				list[i][0]=i+1;
				//1
				byte[] length = new byte[1];
				in.read(length,0,1);
				int len = Util.onebyteToInt(length[0]);
				byte[] name = new byte[len];
				in.read(name,0,len);
				String Name = new String(name);
				list[i][1]=Name;
						
				//2
				list[i][2]= false;		
						
				//3
				byte[] nHash = new byte[32];
				in.read(nHash,0,32);
				list[i][3]=nHash;
				
				//4
				byte[] PubKey = new byte[130];
				int count = 0;
				while(count!=130){
					count+=in.read(PubKey,count,130-count);
				}
				list[i][4]=PubKey;
				
			}
			return list;
		}
		
	}
	
	public static int fileNum() throws Exception {
		DataInputStream in = GetSocket.getInStream();
		//receive the flag
		byte[] flag = new byte[1];
		in.read(flag,0,1); //103
		//System.out.println(Util.onebyteToInt(flag[0]));
		
		//number of Files
		byte[] n = new byte[4];
		in.read(n, 0, 4);
		int num = Util.byteToInt(n);
		return num;
	
	}
}
