package UI_view;

import mistycloud.*;
import javax.swing.table.AbstractTableModel;


class myTableModle extends AbstractTableModel{
			/**
	 * It is an AbstractTableModel.
	 * There are some methods overridden to suit our thought
	 */
	
	private static final long serialVersionUID = 1L;
	private static String[] columnNames = {"���","����","��С","ȫѡ","��ϣֵ","��Կ","Size"};
	//we define it static to visit it in other place to delete file
	
	public static Object[][] tableDatas;
	
	public static void ObjectGet()throws Exception
	{
		SendByte.Send(getFileListRequest.askFileList());
		int row = getFileList.fileNum();
		tableDatas= getFileList.fileList(row);
	}		
			/*{{"1","����̽���Ͼ糡��2016�����ڵ�ج��.mp4", "186 M",false,"12sdFf3412","dfaga"},
			{"2","Bu.Liang.Ren.S01.WEB-DL.1080p", "345 KB",false,"3eafr39uefhr29","dfaga"},
	{"3","2016��ȫ�����뼼������������Ŀ", "25 KB",false,"3eafr39uefhr29","dfaga"},
	{"4","Give your answer as a hex number.pptx", "1.5 M",false,"3eafr39uefhr29","dfaga"},
	{"5","mtc3-kitrub-07-sha1crack-en.pdf", "2.7 M",false,"3eafr39uefhr29","dfaga"},
	{"6","3DES Program Problem(1).zip", "31 M",false,"3eafr39uefhr29","dfaga"},
	{"7","PHP�����ֲ�.pdf", "237 KB",false,"3eafr39uefhr29","dfaga"}
	};*/
	//���ر����
	/*private void hideTableColumn(JTable table, int column){
	    TableColumnModel tcm = table.getColumnModel();
	    //��ʵû���Ƴ����������ض���
	    TableColumn tc = tcm.getColumn(column);
	    tcm.removeColumn(tc);		
	}*/
	@Override
	public int getRowCount() {
		// TODO Auto-generated method stub
		if(tableDatas==null)
			return 0;
		else
			return tableDatas.length;
	}

	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return columnNames.length;
	}

	@Override
	public Object getValueAt(int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
			return tableDatas[rowIndex][columnIndex];
	}
	
	public Class<? extends Object> getColumnClass(int c) {
		return getValueAt(0, c).getClass();
	}
	
	public void setValueAt(Object value, int row, int col){
		tableDatas[row][col] = value;
	}
	
	
	public boolean isCellEditable(int row, int col) {
		// Note that the data/cell address is constant,
		// no matter where the cell appears on screen.
		//only the checkBox can be edited
		if (col !=3) {
			return false;
		} else {
			return true;
		}
	}
	
}
