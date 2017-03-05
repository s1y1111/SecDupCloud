package UI_view;

import java.awt.Color;
import java.io.*;
import java.awt.Font;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

import UI_view.Frame.btnUserinfoActionListener;

import java.awt.event.ActionEvent;
import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import mistycloud.*;
public class LoginFrame extends JFrame{
	/**
	 * 
	 */
	public static LoginFrame loginFrame=new LoginFrame(); 
	public static InputStream sin = GetSocket.getInStream();
	public static OutputStream sout =GetSocket.getOutStream();
	private static final long serialVersionUID = 6466925741741776825L;
	BkLoginPanel bk = new BkLoginPanel();
	JButton btnSignup;
	static JTextField Username;
	static JTextField Password;
	JTextField Caret;
	static int Loginflag=0;
	
	public LoginFrame(){
		setSize(1145, 830);  
        setTitle("M1sty Cloud");
		bk.setLayout(null);
		
		Caret = new JTextField();
		Caret.setBounds(0, 0, 1, 1);
		Caret.setBackground(null);
		Caret.setBorder(null);
		Caret.setCaretColor(Caret.getBackground());
		bk.add(Caret);
		
		Username = new JTextField();
		Username.setBounds(456, 330, 277, 44);
		Username.setBackground(null);
		Username.setCaretColor(null);
		Username.setCaretColor(Username.getBackground());
		Username.setBorder(null);
		Username.setText("Username");
		Username.setFont(new Font("ºÚÌå",Font.BOLD,20)); 
		Username.setForeground(new Color(155,155,155));
		Username.addFocusListener(new FocusAdapter()
		{
			@Override
			public void focusGained(FocusEvent e)
			{
				Username.setText("");
			}
			@Override
			public void focusLost(FocusEvent e)
			{
				if (Username.getText().isEmpty()){
				Username.setText("Username");
				}
			}
		});
		bk.add(Username);
		
		Password = new JPasswordField();
		Password.setBounds(456, 394, 277, 44);
		Password.setBackground(null); 
		Password.setBorder(null);
		Password.setText("Password");
		Password.setFont(new Font("ºÚÌå",Font.BOLD,20)); 
		Password.setForeground(new Color(155,155,155));
		Password.addFocusListener(new FocusAdapter()
		{
			@Override
			public void focusGained(FocusEvent e)
			{
				Password.setText("");
			}
			@Override
			public void focusLost(FocusEvent e)
			{
				if (Password.getText().isEmpty()){
				Password.setText("Password");
				}
			}
		});
		((JPasswordField) Password).setEchoChar('*');
		bk.add(Password);
		
		btnSignup=new JButton(); 
        ImageIcon ImbtnSignup=new ImageIcon(this.getClass().getResource("/UI_view/btnSignup.png"));
        btnSignup.setIcon(ImbtnSignup);  
        btnSignup.setBorder(null);
        btnSignup.setContentAreaFilled(false);
        bk.setLayout(null);
        btnSignup.setBounds(570,471,ImbtnSignup.getIconWidth(), ImbtnSignup.getIconHeight());
        bk.add(btnSignup); 
		
		 add(bk);
	     setLocationRelativeTo(null);
	     setVisible(true); 
	     setResizable(false);
		 setDefaultCloseOperation(EXIT_ON_CLOSE);
	     btnSignup.addActionListener(new btnSignupActionListener());
		
	}
	
	
	
	
	
	
	public static void main(String[] args) throws Exception{  
		GetSocket.run();
		sin = GetSocket.getInStream();
		sout =GetSocket.getOutStream();
		
    }
	class btnSignupActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
        	JLabel Loginerror = new JLabel("Error:Please input again.");
        	JButton btnSign = new JButton();
            ImageIcon ImbtnSign=new ImageIcon(LoginFrame.class.getResource("/UI_view/btnSign.png"));
            btnSign.setIcon(ImbtnSign);  
            btnSign.setBorder(null);
            btnSign.setContentAreaFilled(false);
            btnSign.setBounds(404,471,ImbtnSign.getIconWidth(), ImbtnSign.getIconHeight());
            Loginerror.setBounds(404,444,600,30);
            Loginerror.setForeground(Color.red);
            Loginerror.setFont(new Font("ºÚÌå",Font.BOLD,15));
            bk.add(btnSign);
            add(bk);
            btnSignup.setVisible(false);
   	     	setLocationRelativeTo(null);
   	     	setVisible(true); 
   	     	setResizable(false);
   	     	
   	     	
   	     	
   	     	try{
   	     		UserSign.Hash(Username.getText(), Password.getText());
   	     		SendByte.Send(UserSign.SignIn());
   	     		UserSign.SignSuccess();
   	     		Loginflag = UserSign.Flag();
   	     	}catch(Exception ee){
   	     		ee.printStackTrace();
   	     	}
   	     	
   	     	//System.out.println(Username.getText());
   	     	//System.out.println(Password.getText());
   	     	if (Loginflag==1){
   	     		setVisible(false);
   	     		String uname = Username.getText();
   	     		String upass = Password.getText();
   	     		try {
					Frame f=new Frame(sin,sout,uname,upass);
					f.setDefaultCloseOperation(EXIT_ON_CLOSE);
					
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
   	     		
   	     	}
   	     	if(Loginflag==0){
   	     		setVisible(false);
   	     		bk.add(Loginerror);
   	     		add(bk);
   	     		btnSignup.setVisible(true);
   	     		setLocationRelativeTo(null);
   	     		setVisible(true); 
   	     		setResizable(false);
   	     		
   	     	}
   	     	
        }
    }
}
