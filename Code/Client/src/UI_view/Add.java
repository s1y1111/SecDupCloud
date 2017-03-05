package UI_view;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import mistycloud.AddFirend;
import mistycloud.SendByte;

public class Add extends JFrame {
	  /**
		 * 
		 */
		private static final long serialVersionUID = 6564911253434581984L;
		int Addflag=0;
	    BKAddFriend mainForm=new BKAddFriend();	
	  	static JTextField Friendname = new JTextField(); 
	  	JButton btnAdd = new JButton();	  	
	  	public Add() {  		
	  		mainForm.setLayout(null);
	  		Friendname.setBounds(61, 122, 567, 66);
	  		
	  		 ImageIcon ImbtnAdd=new ImageIcon(this.getClass().getResource("/UI_view/btnAdd.png"));
	            btnAdd.setIcon(ImbtnAdd);  
	            btnAdd.setBorder(null);
	            btnAdd.setContentAreaFilled(false);
	            btnAdd.setBounds(250,215,ImbtnAdd.getIconWidth(), ImbtnAdd.getIconHeight());

	            btnAdd.addActionListener(new btnAddActionListener());
	      			
	      		mainForm.add(Friendname);
	      		Friendname.setBounds(52, 105, 567, 66);
	      		Friendname.setBackground(null);
	    		Friendname.setCaretColor(null);
	    		Friendname.setCaretColor(Friendname.getBackground());
	    		Friendname.setBorder(null);
	      		Friendname.setFont(new Font("黑体",Font.BOLD,30)); 
	      		Friendname.setForeground(new Color(125,195,250));   
	      		mainForm.add(btnAdd);
	      		add(mainForm);
	      		setSize(684, 346);  
	      		setTitle("Add Friend");
	      		setLocationRelativeTo(null);
	      		setResizable(false);		
	      		setVisible(true);
	      		
	    	}
	      
	      	class BKAddFriend extends JPanel{   
	      		private static final long serialVersionUID = -3972248762963303072L;
	      		private Image ImBKAddFriend = (Image) new ImageIcon(this.getClass().getResource("/UI_view/BKAddFriend.png")).getImage();  
	      		protected void paintComponent(Graphics g) {    
	      			g.drawImage(ImBKAddFriend, 0, 0, this.getWidth(), this.getHeight(), this);    
	      		}      		
	        }
	      	class btnAddActionListener implements ActionListener {

	            @Override
	            public void actionPerformed(ActionEvent e) {
	            	JLabel Adderror = new JLabel("Error: This user is not exit. Please input again.");
	            	Adderror.setBounds(65,177,600,30);
	                Adderror.setForeground(Color.red);
	                Adderror.setFont(new Font("黑体",Font.BOLD,20));
	       	     	try{
	       	     		SendByte.Send(AddFirend.addFriendRe(Friendname.getText()));
	       	     		if(AddFirend.addSuccess())
	       	     			Addflag=1;
	       	     		else
	       	     			Addflag=0;
	       	     	}catch(Exception ee){
	       	     		ee.printStackTrace();
	       	     	}
	                
	                
	                if (Addflag==1){
	                	//System.out.println(Friendname.getText());
	                	try {
	                		
	                		byte[] temp=AddFirend.returnPubkey();
							FriendTableModle.ObjectGet();
						} catch (Exception e1) {
							// TODO Auto-generated catch block
							e1.printStackTrace();
						}
      	            	Frame.FriendModle.fireTableDataChanged();
	                	
	                	dispose();
	                	
	       	     	}
	       	     	if(Addflag==0){
	       	     		//setVisible(false);
	       	     		mainForm.add(Adderror);
	       	     		add(mainForm);
	       	     		setLocationRelativeTo(null);
	       	     		setVisible(true); 
	       	     		setResizable(false);
	       	     		
	       	     	}
	       	     	
	            }
	        }
}
