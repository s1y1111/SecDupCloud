package UI_view;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Downloading extends JFrame {
	public static JFrame f4;
	
    public Downloading() {    	
    	JButton BKDownloading = new JButton();
        ImageIcon ImBKDownloading=new ImageIcon(this.getClass().getResource("/UI_view/Downloading.gif"));
        
    	f4 = new JFrame();	
        f4.setResizable(false);        	
    	f4.setLayout(null);
    	f4.setSize(563,315);

    	BKDownloading.setIcon(ImBKDownloading);  
        BKDownloading.setBorder(null);
        BKDownloading.setContentAreaFilled(false);
        BKDownloading.setBounds(0,0,ImBKDownloading.getIconWidth(), ImBKDownloading.getIconHeight());          
        
        f4.add(BKDownloading);
        f4.setTitle("Downloading ...");
        BKDownloading.setVisible(true);
        f4.setLocationRelativeTo(null);
        f4.setVisible(true);
    }
   
    
}


