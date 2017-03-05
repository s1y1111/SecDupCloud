package UI_view;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener; 
import javax.swing.JFileChooser;
import javax.swing.filechooser.FileNameExtensionFilter;

public class BrowseAction implements ActionListener {
  @Override
  public void actionPerformed(ActionEvent e) {
    //if (e.getSource().equals(UploadstuffMainFrom.btnBrowseSource)) {
      JFileChooser fcDlg = new JFileChooser();

      fcDlg.setCurrentDirectory(new File("./UploadFile")); 
      fcDlg.addPropertyChangeListener(new PropertyChangeListener(){
    	  public void propertyChange(PropertyChangeEvent arg0) {
    		  fcDlg.setCurrentDirectory(new File("./UploadFile"));
    	  }
      });  
      
      fcDlg.setDialogTitle("Please choose file ...");
     // FileNameExtensionFilter filter = new FileNameExtensionFilter(
      //    "rar", "txt","doc","jpeg","mp4", "kcd","gif","avi","ppt");
      //fcDlg.setFileFilter(filter);
      int returnVal = fcDlg.showOpenDialog(null);
      if (returnVal == JFileChooser.APPROVE_OPTION) {
        String filepath = fcDlg.getSelectedFile().getPath();
        UploadstuffMainFrom.sourcefile.setText(filepath);
      }
    
    }
  
}
