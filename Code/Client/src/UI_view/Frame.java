package UI_view;

import java.awt.*; 
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;

import mistycloud.ClientTest;
import mistycloud.Cookie;
import mistycloud.GetSocket;
import mistycloud.SM2Utils;
import mistycloud.SeedProducer;
import mistycloud.SendByte;
import mistycloud.ShareFile;
import mistycloud.Util;
import mistycloud.downloadRequest;
import mistycloud.getFileList;
import mistycloud.getFileListRequest;













//import java.util.*;
import java.io.*;



public class Frame extends JFrame{  
	/**
	 * 
	 */
	
	
	public InputStream sin;
	public OutputStream sout;
	private static final long serialVersionUID = 6438627760150427164L;
    BackgroundPanel bp=new BackgroundPanel();  
    JButton plus,btnUserinfo,btnUploadstuff,btnDownload,btnStorage,btnShare,mdlCloud,mdlGroup,mdlChannel,mdlComingsoon;
    static JTextArea fileList = new JTextArea();
    static JTextArea filesizeList = new JTextArea();
    public static JTable table;
    static JTable Friendlist;
	public static myTableModle myModle;
	static FriendTableModle FriendModle;
	static MyCheckBoxRenderer check;
	public static JFrame f4;
    static JButton cursor;
    JLabel username;
    //static ImageIcon Imcursor=new ImageIcon(Frame.class.getResource("/UI_view/cursor.png"));
    int sharefileflag=0;
	int sharefriendflag=0;
	int checkflag=0;
  
    public Frame(InputStream in, OutputStream out,String uname,String upass) throws Exception{     	
    
        //SendByte.Send(getFileListRequest.askFileList());
        setSize(1145, 830);  
        setTitle("M1sty Cloud");
        SeedProducer.seedPro(uname, upass);
        sin = in;
        sout =out;
        
        myModle=new myTableModle();
		table=new JTable(myModle);
		
		myTableModle.ObjectGet();
		
		
		DefaultTableCellRenderer r=new DefaultTableCellRenderer();
		r.setHorizontalAlignment(JLabel.CENTER);
		table.setRowHeight (25);
		table.getColumnModel().getColumn(0).setCellRenderer(r);
		table.getColumnModel().getColumn(2).setCellRenderer(r);
		table.getColumnModel().getColumn(0).setPreferredWidth(20);
		table.getColumnModel().getColumn(3).setPreferredWidth(34);
		table.getColumnModel().getColumn(1).setPreferredWidth(306);
		table.getColumnModel().getColumn(2).setPreferredWidth(70);
		table.getColumnModel().getColumn(4).setMinWidth(0);
		table.getColumnModel().getColumn(4).setMaxWidth(0);
		table.getColumnModel().getColumn(4).setPreferredWidth(0);
		table.getColumnModel().getColumn(5).setMinWidth(0);
		table.getColumnModel().getColumn(5).setMaxWidth(0);
		table.getColumnModel().getColumn(5).setPreferredWidth(0);
		table.getColumnModel().getColumn(6).setMinWidth(0);
		table.getColumnModel().getColumn(6).setMaxWidth(0);
		table.getColumnModel().getColumn(6).setPreferredWidth(0);
		
		
		//table.getColumnModel().getColumn(3).setHeaderRenderer(check);
		//table.getTableHeader().addMouseListener(new MouseAdapter() {
		  /*  @Override
		    public void mouseClicked(MouseEvent e){
		        if(table.getColumnModel().getColumnIndexAtX(e.getX())==3){
		        	//if the mouse click the column 3(the checkBox)
		            boolean b = !check.isSelected();
		            check.setSelected(b);
		            if (b==true){
		            	checkflag++;
		                System.out.println(checkflag);
		            }
		            else if (b==false)
		            	checkflag--;
		           // table.getTableHeader().repaint();
		            for(int i=0;i<table.getRowCount();i++){
		                table.getModel().setValueAt(b, i, 3);
		            }
		           // table.getModel().setValueAt(b, , 4);		            
		        }
		    }*/
		//}
		//);
        
        
		table.setSelectionBackground(new Color(145,205,250));
		table.setGridColor (new Color(135,135,135));
		table.setShowVerticalLines (false);
		table.setBounds(286, 346, 500, 427);
        table.setFont(new Font("宋体",Font.BOLD,16)); 
  		table.setForeground(new Color(140,140,140));
        bp.add(table);
        
        
        
        FriendModle=new FriendTableModle();
        Friendlist=new JTable(FriendModle);
        
        FriendTableModle.ObjectGet();
			    
		DefaultTableCellRenderer cr=new DefaultTableCellRenderer();
		cr.setHorizontalAlignment(JLabel.CENTER);
		Friendlist.setRowHeight (20);
		Friendlist.getColumnModel().getColumn(0).setCellRenderer(cr);
		Friendlist.getColumnModel().getColumn(0).setPreferredWidth(20);
		Friendlist.getColumnModel().getColumn(1).setPreferredWidth(243);
		Friendlist.getColumnModel().getColumn(2).setPreferredWidth(34);
		
		Friendlist.getColumnModel().getColumn(3).setMinWidth(0);
		Friendlist.getColumnModel().getColumn(3).setMaxWidth(0);
		Friendlist.getColumnModel().getColumn(3).setPreferredWidth(0);
		Friendlist.getColumnModel().getColumn(4).setMinWidth(0);
		Friendlist.getColumnModel().getColumn(4).setMaxWidth(0);
		Friendlist.getColumnModel().getColumn(4).setPreferredWidth(0);
		
		/*
		Friendlist.getColumn("全选").setHeaderRenderer(check);
		Friendlist.getTableHeader().addMouseListener(new MouseAdapter() {
		    @Override
		    public void mouseClicked(MouseEvent e){
		        if(Friendlist.getColumnModel().getColumnIndexAtX(e.getX())==2){
		        	//if the mouse click the column 3(the checkBox)
		            boolean b = !check.isSelected();
		            check.setSelected(b);
		            Friendlist.getTableHeader().repaint();
		            
		            Friendlist.getModel().setValueAt(b, 1, 0);    
		        }
		    }
		}
		);*/
		
		Friendlist.setSelectionBackground(new Color(145,205,250));
		Friendlist.setGridColor (new Color(135,135,135));
		Friendlist.setShowVerticalLines (false);
		Friendlist.setBounds(806, 346, 317, 427);
		Friendlist.setFont(new Font("宋体",Font.BOLD,16)); 
		Friendlist.setForeground(new Color(140,140,140));
        bp.add(Friendlist);
        
        
        username= new JLabel(uname);
        username.setBounds(1037,19,240,30);
        username.setForeground(new Color(165,165,165));
        username.setFont(new Font("黑体",Font.BOLD,20));
        bp.add(username);
       /* 
        cursor=new JButton();         
        cursor.setIcon(Imcursor);  
        cursor.setBorder(null);
        cursor.setContentAreaFilled(false);
        bp.setLayout(null);
        cursor.setBounds(0,98,Imcursor.getIconWidth(), Imcursor.getIconHeight());
        bp.add(cursor);
       */ 
        btnUserinfo=new JButton(); 
        ImageIcon ImUserinfo=new ImageIcon(this.getClass().getResource("/UI_view/Userinfo.png"));
        btnUserinfo.setIcon(ImUserinfo);  
        btnUserinfo.setBorder(null);
        btnUserinfo.setContentAreaFilled(false);
        bp.setLayout(null);
        btnUserinfo.setBounds(287,100,ImUserinfo.getIconWidth(), ImUserinfo.getIconHeight());
        bp.add(btnUserinfo);  
        
        btnUploadstuff=new JButton(); 
        ImageIcon ImUploadstuff=new ImageIcon(this.getClass().getResource("/UI_view/Uploadstuff.png"));
        btnUploadstuff.setIcon(ImUploadstuff);  
        btnUploadstuff.setBorder(null);
        btnUploadstuff.setContentAreaFilled(false);
        bp.setLayout(null);
        btnUploadstuff.setBounds(453,100,ImUploadstuff.getIconWidth(), ImUploadstuff.getIconHeight());
        bp.add(btnUploadstuff); 
        
        btnDownload=new JButton(); 
        ImageIcon ImDownload=new ImageIcon(this.getClass().getResource("/UI_view/Download.png"));
        btnDownload.setIcon(ImDownload);  
        btnDownload.setBorder(null);
        btnDownload.setContentAreaFilled(false);
        bp.setLayout(null);
        btnDownload.setBounds(619,100,ImDownload.getIconWidth(), ImDownload.getIconHeight());
        bp.add(btnDownload); 
        
        btnStorage=new JButton(); 
        ImageIcon ImStorage=new ImageIcon(this.getClass().getResource("/UI_view/Storage.png"));
        btnStorage.setIcon(ImStorage);  
        btnStorage.setBorder(null);
        btnStorage.setContentAreaFilled(false);
        bp.setLayout(null);
        btnStorage.setBounds(785,100,ImStorage.getIconWidth(), ImStorage.getIconHeight());
        bp.add(btnStorage); 
        
        btnShare=new JButton(); 
        ImageIcon ImShare=new ImageIcon(this.getClass().getResource("/UI_view/Share.png"));
        btnShare.setIcon(ImShare);  
        btnShare.setBorder(null);
        btnShare.setContentAreaFilled(false);
        bp.setLayout(null);
        btnShare.setBounds(951,100,ImShare.getIconWidth(), ImShare.getIconHeight());
        bp.add(btnShare);
        
        plus=new JButton();
        ImageIcon Implus=new ImageIcon(this.getClass().getResource("/UI_view/plus.png"));
        plus.setIcon(Implus);  
        plus.setBorder(null);
        plus.setContentAreaFilled(false);
        bp.setLayout(null);
        plus.setBounds(1081,310,Implus.getIconWidth(), Implus.getIconHeight());
        bp.add(plus);
       
        /*
        mdlCloud=new JButton(); 
        ImageIcon ImmdlCloud=new ImageIcon("mdlCloud.png");
        mdlCloud.setIcon(ImmdlCloud);  
        mdlCloud.setBorder(null);
        mdlCloud.setContentAreaFilled(false);
        bp.setLayout(null);
        mdlCloud.setBounds(20,97,ImmdlCloud.getIconWidth(), ImmdlCloud.getIconHeight());
        bp.add(mdlCloud); 
        
        mdlGroup=new JButton(); 
        ImageIcon ImmdlGroup=new ImageIcon("mdlGroup.png");
        mdlGroup.setIcon(ImmdlGroup);  
        mdlGroup.setBorder(null);
        mdlGroup.setContentAreaFilled(false);
        bp.setLayout(null);
        mdlGroup.setBounds(20,151,ImmdlGroup.getIconWidth(), ImmdlGroup.getIconHeight());
        bp.add(mdlGroup); 
        
        mdlChannel=new JButton(); 
        ImageIcon ImmdlChannel=new ImageIcon("mdlChannel.png");
        mdlChannel.setIcon(ImmdlChannel);  
        mdlChannel.setBorder(null);
        mdlChannel.setContentAreaFilled(false);
        bp.setLayout(null);
        mdlChannel.setBounds(20,207,ImmdlChannel.getIconWidth(), ImmdlChannel.getIconHeight());
        bp.add(mdlChannel); 
        
        mdlComingsoon=new JButton(); 
        ImageIcon ImmdlComingsoon=new ImageIcon("mdlComingsoon.png");
        mdlComingsoon.setIcon(ImmdlComingsoon);  
        mdlComingsoon.setBorder(null);
        mdlComingsoon.setContentAreaFilled(false);
        bp.setLayout(null);
        mdlComingsoon.setBounds(20,253,ImmdlComingsoon.getIconWidth(), ImmdlComingsoon.getIconHeight());
        bp.add(mdlComingsoon); 
       */
       /* 
    	Read.readFileByLines();
        fileList.setBounds(286, 346, 366, 427);
        
        fileList.setFont(new Font("宋体",Font.BOLD,23)); 
  		fileList.setForeground(new Color(140,140,140));
        filesizeList.setBounds(662, 346, 81, 427);
        filesizeList.setFont(new Font("宋体",Font.BOLD,23)); 
  		filesizeList.setForeground(new Color(140,140,140));
        bp.add(filesizeList);
        bp.add(fileList);
        */
        
 
        add(bp);
        setLocationRelativeTo(null);
        setVisible(true); 
        setResizable(false);
        
        
        plus.addActionListener(new plusActionListener());
        btnShare.addActionListener(new btnShareActionListener());
        btnUserinfo.addActionListener(new btnUserinfoActionListener());
        btnUploadstuff.addActionListener(new btnUploadstuffActionListener());
        btnStorage.addActionListener(new btnStorageActionListener());
        btnDownload.addActionListener(new btnDownloadActionListener());
       
        
        /*
         mdlComingsoon.addActionListener(new mdlComingsoonActionListener());
         
        mdlCloud.addActionListener(new mdlCloudActionListener());
        mdlGroup.addActionListener(new mdlGroupActionListener());
        mdlChannel.addActionListener(new mdlChannelActionListener());
        */
    }  
    /*public static void main(String[] args) {  
        Frame Frame=new Frame();  
    } */
    class btnUserinfoActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
        	JFrame f = new JFrame();
        	f.setTitle("User Information");
            JButton BKUserInfo = new JButton();
            ImageIcon ImBKUserInfo=new ImageIcon(this.getClass().getResource("/UI_view/User.png"));
            BKUserInfo.setIcon(ImBKUserInfo);  
            BKUserInfo.setBorder(null);
            BKUserInfo.setContentAreaFilled(false);
            f.setLayout(null);
            BKUserInfo.setBounds(0,0,ImBKUserInfo.getIconWidth(), ImBKUserInfo.getIconHeight());
            f.setSize(700,670);
            f.add(BKUserInfo); 
            f.setLocationRelativeTo(null);
            f.setVisible(true);
        }
    }
    class btnStorageActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
        	JFrame f = new JFrame();
        	f.setTitle("Cloud Storage");
            JButton BKStorageUsage = new JButton();
            ImageIcon ImStorageUsage=new ImageIcon(this.getClass().getResource("/UI_view/StorageUsage.png"));
            BKStorageUsage.setIcon(ImStorageUsage);  
            BKStorageUsage.setBorder(null);
            BKStorageUsage.setContentAreaFilled(false);            
            f.setLayout(null);
            BKStorageUsage.setBounds(0,0,ImStorageUsage.getIconWidth(), ImStorageUsage.getIconHeight());
            f.setSize(540,655);
            f.add(BKStorageUsage); 
            f.setLocationRelativeTo(null);
            f.setVisible(true);
            
            BKStorageUsage.addActionListener(new ActionListener(){
            	public void actionPerformed(ActionEvent e) {
                	f.dispose();
                }
            });
        }         
    }
    
    class btnUploadstuffActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
        	try {
        		UploadstuffMainFrom ff=new UploadstuffMainFrom(sin,sout);
				ff.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
			} catch (Exception e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
        }
        	  
    }
    
    class btnDownloadActionListener implements ActionListener,Runnable {
    	byte[] Hashhash=new byte[32];
    	byte[] Key=new byte[322];
    	byte[] count = new byte[4];
    	String KeyS = null;
    	String Fname = null;
        @Override
        public void actionPerformed(ActionEvent e) {
        	checkflag=0;
        	
        	//String KeyS = null;
        	
        	int size = 0;
        	/*try{
        		myTableModle.ObjectGet();
        	}catch (Exception ee){
        		ee.printStackTrace();
        	}*/
        	for(Object[] oneRow:myTableModle.tableDatas){
				if((boolean) oneRow[3]){
					checkflag++;
					Fname = (String) oneRow[1];
					count = (byte[]) oneRow[6];
					Hashhash= (byte[]) oneRow[4];
					//System.out.println("Hashhash:"+Hashhash);
					Key = (byte[]) oneRow[5];
					KeyS = new String (Key);
					System.out.println("key:"+KeyS);

				}
			}
				if(checkflag==0)
				{
        	
        	        	JFrame f = new JFrame();
        	        	f.setTitle("ERROR");
        	        	JButton BKShowfile = new JButton();
        	        	ImageIcon ImShowfile=new ImageIcon(this.getClass().getResource("/UI_view/Showfile.png"));
        	        	BKShowfile.setIcon(ImShowfile);  
        	        	BKShowfile.setBorder(null);
        	        	BKShowfile.setContentAreaFilled(false);            
        	        	f.setLayout(null);
        	        	BKShowfile.setBounds(0,0,ImShowfile.getIconWidth(), ImShowfile.getIconHeight());
        	        	f.setSize(563,315);
        	        	f.add(BKShowfile); 
        	        	f.setLocationRelativeTo(null);
        	        	f.setVisible(true);
            
        	        	BKShowfile.addActionListener(new ActionListener(){
        	        		public void actionPerformed(ActionEvent e) {
        	        			f.dispose();
        	        		}
        	        	});
        	        }
        	        else{
        				// TODO Auto-generated method stub
        				JButton BKDownloading = new JButton();
        	            ImageIcon ImBKDownloading=new ImageIcon(this.getClass().getResource("/UI_view/Downloading.gif"));
        	            
        	        	f4 = new JFrame();	
        	            f4.setResizable(false);        	
        	        	f4.setLayout(null);
        	        	f4.setSize(406,315);

        	        	BKDownloading.setIcon(ImBKDownloading);  
        	            BKDownloading.setBorder(null);
        	            BKDownloading.setContentAreaFilled(false);
        	            BKDownloading.setBounds(0,0,ImBKDownloading.getIconWidth(), ImBKDownloading.getIconHeight());          
        	            JLabel label1 = new JLabel("正在下载..." );
        		        label1.setBounds(120,135,220,30);
        		        label1.setForeground(Color.white);
        		        label1.setFont(new Font("黑体",Font.BOLD,34));
        		        //UploadActionListener.setVisible(false);
        		        f4.add(label1);

        	            f4.add(BKDownloading);
        	            f4.setTitle("Downloading ...");
        	            BKDownloading.setVisible(true);
        	            f4.setLocationRelativeTo(null);
        	            f4.setVisible(true);
        	        	
        	        	Thread a = new Thread(this);
        	        	a.start();
        	            
        	          // BKDownloading.addActionListener(new ActionListener(){
                      //	public void actionPerformed(ActionEvent e) {
        	        	//Download Request
        	        	
        	        	
                      	//}});
        	        }
        	    	
        		}
		@Override
		public void run() {
			try {
				SendByte.Send(downloadRequest.downloadFileRequest(Hashhash));
			} catch (Exception e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
        	
        	byte[] DownloadFlag=new byte[1];
        	try {
				GetSocket.getInStream().read(DownloadFlag,0,1);
				System.out.println("downloadflag:"+DownloadFlag[0]);
				byte[] password = SM2Utils.decrypt
						(Util.hexToByte(SM2Utils.ReturnPri()), Util.hexToByte(KeyS)); 
				System.out.println("password:"+new String(password));
				try {
					ClientTest.decryptFile(new String(password),
							"./DownloadFile/", Fname,Util.byteToInt(count));
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}
        	
    }
	
    
    class btnShareActionListener implements ActionListener {
    	byte[] nHash;
    	String Fname;
    	byte[] filecount;
    	byte[] Hashhash;
    	byte[] PubKey;
    	byte[] key;
        @Override
        public void actionPerformed(ActionEvent e) {
        	sharefileflag=0;
        	sharefriendflag=0;
        	for(Object[] oneRow:myTableModle.tableDatas){
				if((boolean) oneRow[3]){
					sharefileflag++;
					Fname = (String) oneRow[1];
					filecount = (byte[]) oneRow[6];
					Hashhash = (byte[]) oneRow[4];
					key = (byte[]) oneRow[5];
				}
			}
        	for(Object[] oneRow:FriendModle.tableDatas){
				if((boolean) oneRow[2]){
					sharefriendflag++;
					nHash = (byte[]) oneRow[3];
					PubKey = (byte[]) oneRow[4];
				}
			}
        	
				if(sharefileflag==0||sharefriendflag==0)
				{
					JFrame f = new JFrame();
					f.setTitle("ERROR");
        	       	JButton BKShowfriend = new JButton();
        	       	ImageIcon ImShowfriend=new ImageIcon(this.getClass().getResource("/UI_view/Showfriend.png"));
        	       	BKShowfriend.setIcon(ImShowfriend);  
        	       	BKShowfriend.setBorder(null);
        	       	BKShowfriend.setContentAreaFilled(false);            
        	       	f.setLayout(null);
        	       	BKShowfriend.setBounds(0,0,ImShowfriend.getIconWidth(), ImShowfriend.getIconHeight());
        	       	f.setSize(563,315);
                	f.add(BKShowfriend); 
       	        	f.setLocationRelativeTo(null);
       	        	f.setVisible(true);
            
       	        	BKShowfriend.addActionListener(new ActionListener(){
       	        		public void actionPerformed(ActionEvent e) {
       	        			f.dispose();
       	        		}
       	        	});
       	        }        	        
				else{
					try {
						SendByte.Send(ShareFile.shareTo(nHash, Fname,filecount,
										Hashhash,PubKey,key));
					} catch (Exception e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					try {
						if(ShareFile.ShareSuccess()){
							new ShareSucceed();
						}
					} catch (Exception e12) {
						// TODO Auto-generated catch block
						e12.printStackTrace();
					}
					
       	        }
        	    	
       		}
        	
    }
	/*
    
    class mdlComingsoonActionListener implements ActionListener {

    	@Override
        public void actionPerformed(ActionEvent e) {
    		cursor.setBounds(0,255,Imcursor.getIconWidth(), Imcursor.getIconHeight());
    		JFrame f = new JFrame();
        	f.setTitle("Coming soon ...");
            JButton BKUserInfo = new JButton();
            ImageIcon ImBKUserInfo=new ImageIcon("BKComingsoon.png");
            BKUserInfo.setIcon(ImBKUserInfo);  
            BKUserInfo.setBorder(null);
            BKUserInfo.setContentAreaFilled(false);
            f.setLayout(null);
            BKUserInfo.setBounds(0,0,ImBKUserInfo.getIconWidth(), ImBKUserInfo.getIconHeight());
            f.setSize(556,322);
            f.add(BKUserInfo); 
            f.setLocationRelativeTo(null);
            f.setVisible(true);
        }
    }
    class mdlCloudActionListener implements ActionListener {

    	@Override
        public void actionPerformed(ActionEvent e) {
    		cursor.setBounds(0,98,Imcursor.getIconWidth(), Imcursor.getIconHeight());
    		
        }
    }
    class mdlGroupActionListener implements ActionListener {

    	@Override
        public void actionPerformed(ActionEvent e) {
    		cursor.setBounds(0,152,Imcursor.getIconWidth(), Imcursor.getIconHeight());
    		
        }
    }
    class mdlChannelActionListener implements ActionListener {

    	@Override
        public void actionPerformed(ActionEvent e) {
    		cursor.setBounds(0,207,Imcursor.getIconWidth(), Imcursor.getIconHeight());
    		
        }
    }
    */
    class MyCheckBoxRenderer extends JCheckBox implements TableCellRenderer{
		 
	    /**
		 * Its function is just to get the checkBox replace the "全选" with the image
		 */
		private static final long serialVersionUID = 1L;
		
		public MyCheckBoxRenderer () {
	        this.setBorderPainted(true);
	        
	    }
	    @Override
	    public Component getTableCellRendererComponent(JTable table, Object value,
	            boolean isSelected, boolean hasFocus, int row, int column) {
	        // TODO Auto-generated method stub      
	        return this;
	    }   
	}
    class plusActionListener implements ActionListener {
    	
    	@Override
        public void actionPerformed(ActionEvent e) {
    		new Add();
    	}	
    }
}