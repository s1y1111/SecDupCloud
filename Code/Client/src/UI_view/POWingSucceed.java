package UI_view;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;

public class POWingSucceed {
	/**
	 * 
	 */
	private static final long serialVersionUID = 6285691773834398641L;
	public static JFrame f3;
    public POWingSucceed() {
    	UploadstuffMainFrom.f1.setVisible(false);
		f3 = new JFrame();
		JButton BKUploadingSucceed = new JButton();
		
        f3.setResizable(false);        	
    	f3.setLayout(null);
    	f3.setSize(400,230);
    	
        ImageIcon ImBKUploadingSucceed=new ImageIcon(this.getClass().getResource("/UI_view/PSucceed.png"));
        BKUploadingSucceed.setIcon(ImBKUploadingSucceed);  
        BKUploadingSucceed.setBorder(null);
        BKUploadingSucceed.setContentAreaFilled(false);            
        BKUploadingSucceed.setBounds(0,0,ImBKUploadingSucceed.getIconWidth(), ImBKUploadingSucceed.getIconHeight());
        
        //Uploading.BKUploading.setVisible(false);
        f3.add(BKUploadingSucceed);
        f3.setTitle("PoWing Succeed!");
        
        f3.setLocationRelativeTo(null);
        f3.setVisible(true);
        f3.setAlwaysOnTop(true);
    }
}
