package UI_view;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;

public class ShareSucceed extends JFrame {
	JFrame f6;
    public ShareSucceed() {
    	
		f6 = new JFrame();
		JButton BKShareSucceed = new JButton();
		
        f6.setResizable(false);        	
    	f6.setLayout(null);
    	f6.setSize(406,315);
    	
        ImageIcon ImBKShareSucceed=new ImageIcon(this.getClass().getResource("/UI_view/SSucceed.png"));
        BKShareSucceed.setIcon(ImBKShareSucceed);  
        BKShareSucceed.setBorder(null);
        BKShareSucceed.setContentAreaFilled(false);            
        BKShareSucceed.setBounds(0,0,ImBKShareSucceed.getIconWidth(), ImBKShareSucceed.getIconHeight());
        
        //Share.BKShare.setVisible(false);
        f6.add(BKShareSucceed);
        f6.setTitle("Share Succeed!");
        
        f6.setLocationRelativeTo(null);
        f6.setVisible(true);
        f6.setAlwaysOnTop(true);
    }
}

