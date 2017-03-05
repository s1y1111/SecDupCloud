package mistycloud;

import java.io.IOException;

/*
 * �û���ĳ�����ѹ����ļ���
 * ��־��1��|cookies��32��|�������Ĺ�ϣ��32��|�ļ�����С��1��|�ļ�����*��|�ļ���С��4��|�ļ��Ĺ�ϣ�Ĺ�ϣ��32��
 * |�����û���Կ���ܵ��ļ��Ĺ�ϣ��322��
	�ɹ��򷵻أ�
	��־��1��
 */


public class ShareFile {
	public static byte[] shareTo(byte[] nHash, String Fname,byte[] filecount,
									byte[] Hashhash,byte[] PubKey,byte[] key) 
											throws Exception
	{
		
		// Flag 1
		byte flag =7;
		//Cookie 32
		byte[] cookie = Cookie.returnCookie();
		//FriendHash 32
		byte[] friHash = nHash;
		//length of file name and name 1+len
		byte[] fileName = Fname.getBytes();
		int len = fileName.length;
		byte length = Util.intToOnebyte(len);
		//file size count 4
		
		//Hashhash 32
		
		//encrypthash
		String KeyS = new String(key);
		
		/*String pubkHash = SM2Utils.encrypt(Util.hexToByte(SM2Utils.ReturnPub()), hash.getBytes());
		byte[] pubkeyHash = pubkHash.getBytes(); //322λ*/
		
		byte[] Password = SM2Utils.decrypt
				(Util.hexToByte(SM2Utils.ReturnPri()), Util.hexToByte(KeyS)); 
		
		byte[] pubKey = Util.hexToByte(new String(PubKey));
		String pubkHash = SM2Utils.encrypt(pubKey, Password);
		byte[] encPassword = pubkHash.getBytes();
		
		
		//The length of the 
		int index =1+32+32+1+len+4+32+322;
		byte[] Re= new byte[index];
		Re[0]=flag;
		for(int i=0;i<32;i++){
			Re[1+i]=cookie[i];
		}
		for(int i=0;i<32;i++){
			Re[33+i]=nHash[i];
		}
		Re[65]=length;
		for(int i=0;i<len;i++){
			Re[66+i]=fileName[i];
		}
		for(int i=0;i<4;i++){
			Re[66+len+i]=filecount[i];
		}
		for(int i=0;i<32;i++){
			Re[70+len+i]=Hashhash[i];
		}
		for(int i =0;i<322;i++){
			Re[102+len+i]=encPassword[i];
		}
		
		return Re;
	}
	public static boolean ShareSuccess() throws Exception
	{
		byte[] flag = new byte[1];
		GetSocket.getInStream().read(flag,0,1);
		if(flag[0]==108){
			return true;
		}
		else
			return false;
	}

}
