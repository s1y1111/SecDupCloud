package UI_view;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;

public class DownloadingSucceed extends JFrame {
	public static JFrame f5;
	
	JButton BKDownloadingSucceed = new JButton();
	ImageIcon ImBKDownloadingSucceed=new ImageIcon(this.getClass().getResource("/UI_view/DSucceed.png"));
	
	public DownloadingSucceed() {
    	//Downloading.f4.setVisible(false);
		Frame.f4.setVisible(false);
    	f5 = new JFrame();
    	
		BKDownloadingSucceed.setIcon(ImBKDownloadingSucceed);  
	    BKDownloadingSucceed.setBorder(null);
	    BKDownloadingSucceed.setContentAreaFilled(false);            
	    BKDownloadingSucceed.setBounds(0,0,ImBKDownloadingSucceed.getIconWidth(), ImBKDownloadingSucceed.getIconHeight());
		
        f5.setResizable(false);        	
    	f5.setLayout(null);
    	f5.setSize(406,315);
    	
        //ImageIcon ImBKDownloadingSucceed=new ImageIcon("Succeed.png");
        
        //Downloading.BKDownloading.setVisible(false);
        f5.add(BKDownloadingSucceed);
        f5.setTitle("Downloading Succeed!");
        
        f5.setLocationRelativeTo(null);
        f5.setVisible(true);
        f5.setAlwaysOnTop(true);
    
    }
}

