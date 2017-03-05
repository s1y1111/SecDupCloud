package mistycloud;
import java.net.*;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextField;



import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.io.*;
import java.awt.*;
public class GetSocket {
	private static DataInputStream SocketIn;
	private static DataOutputStream SocketOut;
	private static String host;
	private static String port;
	private static int flag =0;
	
	
	
	class Address extends JFrame{
		JFrame Ad= new JFrame();
		JTextField IP;
		JTextField Port;	
		JButton OK = new JButton();
		JButton BKAdd = new JButton();
		public Address(){
					
			Ad.setSize(500,501);  
			Ad.setLayout(null);

			
			BKAdd=new JButton(); 
	        ImageIcon ImBKAdd=new ImageIcon(this.getClass().getResource("/UI_view/Ad.png"));
	        BKAdd.setIcon(ImBKAdd);  
	        BKAdd.setBorder(null);
	        BKAdd.setContentAreaFilled(false);
	        Ad.getLayeredPane().add(BKAdd, new Integer(Integer.MIN_VALUE));
	        Ad.setLayout(null);
	        BKAdd.setBounds(0,0,ImBKAdd.getIconWidth(), ImBKAdd.getIconHeight());
	        
			
			OK=new JButton(); 
	        ImageIcon ImOK=new ImageIcon(this.getClass().getResource("/UI_view/OK.png"));
	        OK.setIcon(ImOK);  
	        OK.setBorder(null);
	        OK.setContentAreaFilled(false);
	        Ad.setLayout(null);
	        OK.setBounds(75,356,ImOK.getIconWidth(), ImOK.getIconHeight());
	        
	        
			
	        Port = new JTextField();
			Port.setBounds(80, 205, 343, 49);
			Port.setBackground(null);
			Port.setCaretColor(null);
			Port.setCaretColor(Port.getBackground());
			Port.setBorder(null);
			Port.setText("Port");
			Port.setFont(new Font("黑体",Font.BOLD,20)); 
			Port.setForeground(new Color(155,155,155));
			Port.addFocusListener(new FocusAdapter()
			{
				@Override
				public void focusGained(FocusEvent e)
				{
					Port.setText("");
				}
				@Override
				public void focusLost(FocusEvent e)
				{
					if (Port.getText().isEmpty()){
					Port.setText("Port");
					}
				}
			});
			
			IP = new JTextField();
			IP.setBounds(80, 283, 343, 49);
			IP.setBackground(null);
			IP.setCaretColor(null);
			IP.setCaretColor(IP.getBackground());
			IP.setBorder(null);
			IP.setText("IP");
			IP.setFont(new Font("黑体",Font.BOLD,20)); 
			IP.setForeground(new Color(155,155,155));
			IP.addFocusListener(new FocusAdapter()
			{
				@Override
				public void focusGained(FocusEvent e)
				{
					IP.setText("");
				}
				@Override
				public void focusLost(FocusEvent e)
				{
					if (IP.getText().isEmpty()){
					IP.setText("IP");
					}
				}
			});
			
			
	        Ad.add(IP);
	        Ad.add(Port);
	        Ad.add(OK);
	        Ad.add(BKAdd); 
	        Ad.setLocationRelativeTo(null);
	        Ad.setVisible(true); 
	        Ad.setResizable(false);
		

	     
	    OK.addActionListener(new OKActionListener());
		 }
		class OKActionListener implements ActionListener {

	    	@Override
	        public void actionPerformed(ActionEvent e) {
	    		
	    		//IP与端口赋值  同时设置flag
	    		host = IP.getText();
	    		port = Port.getText();
	    		flag = 1;
	    		//事件要设置关闭这个弹窗，在赋值之后。
	    		Ad.dispose();
	    	}
	    }
	}
	
	/*public static void main(String args[])throws Exception
	{
		run();
		

		
	}*/
	
	

	public static void run()throws Exception
	{
		
		
		new GetSocket().new Address();
		while(flag==0){
			Thread.sleep(1000);
		}
				
		//获得IP，PORT
		try{
			
				Socket socket = new Socket(host,Integer.valueOf(port));
			
				try{
					SocketIn = new DataInputStream(socket.getInputStream());
					SocketOut = new DataOutputStream(socket.getOutputStream());
					
				}catch(Exception e){
					e.printStackTrace();
				}
			
			
		}catch (IOException e){
			e.printStackTrace();
		}
		
	}
	
	public static DataInputStream getInStream()
	{
		return SocketIn;
	}
	
	public static DataOutputStream getOutStream()
	{
		return SocketOut;
	}
	
	

}
