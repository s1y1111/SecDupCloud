package UI_view;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;

public class Uploading extends JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = -861604319280088002L;
	public static JFrame f2;
    public Uploading() {
    	UploadstuffMainFrom.f1.setVisible(false);
		//DownloadMainForm.fd1.setVisible(false);
    	f2 = new JFrame();
		
        f2.setResizable(false);        	
    	f2.setLayout(null);
    	f2.setSize(563,315);
    	JButton BKUploading = new JButton();
        ImageIcon ImBKUploading=new ImageIcon(this.getClass().getResource("/UI_view/Uploading.gif"));
        BKUploading.setIcon(ImBKUploading);  
        BKUploading.setBorder(null);
        BKUploading.setContentAreaFilled(false);
        BKUploading.setBounds(0,0,ImBKUploading.getIconWidth(), ImBKUploading.getIconHeight());          
        
        //UploadActionListener.setVisible(false);
        f2.add(BKUploading);
        f2.setTitle("Uploading ...");
        
        f2.setLocationRelativeTo(null);
        f2.setVisible(true);
        f2.setAlwaysOnTop(true);
    }
    
}
