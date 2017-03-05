package UI_view;

import javax.swing.*;

import mistycloud.ClientTest;
import mistycloud.Cookie;
import mistycloud.GetSocket;
import mistycloud.SendByte;
import mistycloud.Sha256File;
import mistycloud.UploadRequest;
import mistycloud.Util;
import mistycloud.getFileList;
import mistycloud.getFileListRequest;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.InputStream;
import java.io.OutputStream;
public class UploadstuffMainFrom extends JFrame {
  /**
	 * 
	 */
	public InputStream in;
	public OutputStream out;
	private static final long serialVersionUID = 6564911253434581984L;
	//public static int uploadingflag=0;
	public static JFrame f1;
	public static JFrame f2;


	//private JFrame mainForm = new JFrame("Upload stuff"); // 主窗体，标题为“TXT文件加密”
	CInstead mainForm=new CInstead();	
  
    
  	static JTextField sourcefile = new JTextField(); // 选择待加密或解密文件路径的文本域  	
  	JButton btnBrowseSource = new JButton(); // 浏览按钮
  	JButton btnUpload = new JButton(); // Upload按钮
  	public UploadstuffMainFrom(InputStream sin,OutputStream sout) throws Exception {  		
  		in=sin;
  		out=sout;
  		mainForm.setLayout(null);
  		sourcefile.setBounds(52, 105, 567, 66);
  		
        ImageIcon ImbtnUpload=new ImageIcon(this.getClass().getResource("/UI_view/btnUpload.png"));
        btnUpload.setIcon(ImbtnUpload);  
        btnUpload.setBorder(null);
        btnUpload.setContentAreaFilled(false);
        btnUpload.setBounds(384,210,ImbtnUpload.getIconWidth(), ImbtnUpload.getIconHeight());
        
        ImageIcon ImbtnBrowseSource=new ImageIcon(this.getClass().getResource("/UI_view/btnBrowse.png"));
        btnBrowseSource.setIcon(ImbtnBrowseSource);  
        btnBrowseSource.setBorder(null);
        btnBrowseSource.setContentAreaFilled(false);
        btnBrowseSource.setBounds(101,210,ImbtnBrowseSource.getIconWidth(), ImbtnBrowseSource.getIconHeight());
        

        btnBrowseSource.addActionListener(new BrowseAction()); // 为源文件浏览按钮绑定监听器，点击该按钮调用文件选择窗口
        btnUpload.addActionListener(new UploadActionListener());
  		sourcefile.setEditable(false);//设置源文件文本域不可手动修改  		
  		mainForm.add(sourcefile);
  		sourcefile.setFont(new Font("黑体",Font.BOLD,30)); 
  		sourcefile.setForeground(new Color(125,195,250));
  		mainForm.add(btnBrowseSource);    
  		mainForm.add(btnUpload);
  		add(mainForm);
  		//setDefaultCloseOperation(WindowConstants.HIDE_ON_CLOSE);// 设置主窗体关闭按钮样式
  		setSize(684, 344);  
  		setName("Upload stuff");
  		setLocationRelativeTo(null);// 设置居于屏幕中央
  		setResizable(false);// 设置窗口不可缩放  		
  		setVisible(true);// 显示窗口
  		
  		
  		
  		
  }
  
  	class CInstead extends JPanel {   
	private static final long serialVersionUID = -3972248762963303072L;
	private Image img = (Image) new ImageIcon(this.getClass().getResource("/UI_view/BKUploadstuff.png")).getImage();  
	protected void paintComponent(Graphics g) {    
        g.drawImage(img, 0, 0, this.getWidth(), this.getHeight(), this);    
    }   
  	}   
  	
    	class UploadActionListener implements ActionListener,Runnable {
    		JLabel label1;
        	JLabel label2;
        	JButton BKDuplicationchecking;
            public void actionPerformed(ActionEvent e) {
        		setVisible(false);
        		f1 = new JFrame();
        		BKDuplicationchecking = new JButton();

          		f1.setResizable(false);        	
            	f1.setLayout(null);
            	f1.setSize(400,230);
            	
                
                ImageIcon ImBKUserInfo=new ImageIcon(this.getClass().getResource("/UI_view/Duplicationchecking.gif"));
                BKDuplicationchecking.setIcon(ImBKUserInfo);  
                BKDuplicationchecking.setBorder(null);
                BKDuplicationchecking.setContentAreaFilled(false);
                BKDuplicationchecking.setBounds(0,0,ImBKUserInfo.getIconWidth(), ImBKUserInfo.getIconHeight());
                
                label1 = new JLabel("正在进行" );
    	        label1.setBounds(50,105,220,30);
    	        label1.setForeground(Color.white);
    	        label1.setFont(new Font("黑体",Font.BOLD,25));
    	        //UploadActionListener.setVisible(false);
    	        f1.add(label1);
    	        

    	        label2 = new JLabel("去重检测" );
    	        label2.setBounds(250,105,220,30);
    	        label2.setForeground(Color.white);
    	        label2.setFont(new Font("黑体",Font.BOLD,25));
    	        //UploadActionListener.setVisible(false);
    	        f1.add(label2);

                f1.setTitle("Duplication checking");
                f1.add(BKDuplicationchecking); 
                f1.setLocationRelativeTo(null);
                f1.setVisible(true);
                Thread a=new Thread(this);
                a.start();
	        	
                
        
                
            }

			@Override
			public void run() {
				// TODO Auto-generated method stub
				String SourceFilePath =sourcefile.getText();
          		try {
          			
					SendByte.Send(UploadRequest.upFile(SourceFilePath));
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
          		byte[] flag = new byte[1];
          		byte[] IV = new byte[16];
          		byte[] left =new byte[4];
          		byte[] right =new byte[4];
          		//get iv and left right shift
          		try{
          			in.read(flag,0,1);
              	    in.read(IV,0,16);
              		in.read(left,0,4);
              		in.read(right,0,4);
          		}catch (Exception e2) {
					// TODO Auto-generated catch block
					e2.printStackTrace();
				}
          		//加密
          		if(Util.onebyteToInt(flag[0])!=0){
         
          			
          			//new Uploading();
          			//f1.setVisible(false);
          			//DownloadMainForm.fd1.setVisible(false);
          	    	//f2 = new JFrame();
          			
//          	        f2.setResizable(false);        	
//          	    	f2.setLayout(null);
//          	    	f2.setSize(400,230);
          			/**********Uploading or PoWing ************/
          			
          	    	JButton BKUploading = new JButton();
          	        ImageIcon ImBKUploading=new ImageIcon(this.getClass().getResource("/UI_view/Uploading.gif"));
          	        
          	        BKUploading.setBorder(null);
          	        BKUploading.setContentAreaFilled(false);
          	        BKUploading.setBounds(0,0,ImBKUploading.getIconWidth(), ImBKUploading.getIconHeight());          
          	        
          	        JLabel label3 = new JLabel("正在上传..." );
          	        label3.setBounds(120,70,220,30);
          	        label3.setForeground(Color.black);
          	        label3.setFont(new Font("黑体",Font.BOLD,34));
          	        ImageIcon ImBKUploading2=new ImageIcon(this.getClass().getResource("/UI_view/Powing.gif"));
    		        
    		        BKUploading.setBorder(null);
    		        BKUploading.setContentAreaFilled(false);
    		        BKUploading.setBounds(0,0,ImBKUploading.getIconWidth(), ImBKUploading.getIconHeight());          
    		        JLabel label4 = new JLabel("正在进行所有权证明..." );
    		        label4.setBounds(30,60,420,30);
    		        label4.setForeground(Color.white);
    		        label4.setFont(new Font("黑体",Font.BOLD,30));
    		        

          	      if(Util.byteToInt(left)==0){
          	    	BKUploading.setIcon(ImBKUploading);  
          	    	f1.remove(label1);
          	    	f1.remove(label2);
          	    	f1.remove(BKDuplicationchecking);
          	        f1.add(label3);
          	        f1.add(BKUploading);
          	        f1.setTitle("Uploading ...");
          	      }
          			else{
          				BKUploading.setIcon(ImBKUploading2); 
          				f1.remove(label1);
              	    	f1.remove(label2);
              	    	f1.remove(BKDuplicationchecking);
          				f1.add(label4);
          				f1.add(BKUploading);
          				f1.setTitle("PoWing ...");
          			}
          	      f1.update(getGraphics());
          			//Uploading...
          	        //修改flag 同时运用FrameControl去控制Frame切换
          			UploadRequest.UploadingFlag=1;
          			try {
    					byte[] head =new byte[33];
    					//flag upload file
    					head[0] = 5;
    					for(int i=0;i<32;i++)
    						head[i+1]=Cookie.returnCookie()[i];
    					SendByte.Send(head);
    					if(Util.byteToInt(left)==0){
    						SendByte.Send(IV);
    					}
    					
          				ClientTest.encryptFile(Sha256File.reSha(), IV, SourceFilePath, 
    							GetSocket.getOutStream(),Util.byteToInt(left), Util.byteToInt(right));
          				//jieshou
          				byte[] uploadsuccess = new byte[1];
          	            (GetSocket.getInStream()).read(uploadsuccess,0,1);
          	            System.out.println(uploadsuccess[0]);
          	            if(Util.onebyteToInt(uploadsuccess[0])==107){
          	            	if(Util.byteToInt(left)==0){
          	            		new UploadingSucceed();
          	            	}
          	            	else{
          	            		new POWingSucceed();
          	            	}
          	            	SendByte.Send(getFileListRequest.askFileList());
          	            	int row = getFileList.fileNum();
          	            	myTableModle.tableDatas= getFileList.fileList(row);
          	            	Frame.myModle.fireTableDataChanged();
          	            }
          	            
          	           
    				} catch (Exception e1) {
    					// TODO Auto-generated catch block
    					e1.printStackTrace();
    					/*************PoW 失败***********/
    					//new what
    					new POWfail();
    				}
          		
          	   
          	      }
              	}
			}
    	}
    
    	
    	
    
  

		