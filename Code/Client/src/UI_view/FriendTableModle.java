package UI_view;


import javax.swing.table.AbstractTableModel;

import mistycloud.FriendListRe;
import mistycloud.SendByte;
import mistycloud.getFileList;
import mistycloud.getFileListRequest;
import mistycloud.getFriendList;


class FriendTableModle extends AbstractTableModel{
			/**
	 * It is an AbstractTableModel.
	 * There are some methods overridden to suit our thought
	 */
	
	private static final long serialVersionUID = 1L;
	private static String[] columnNames = {"序号","名称","全选","哈希","公钥"};
	//we define it static to visit it in other place to delete file
	
	public static Object[][] tableDatas;
	public static void ObjectGet()throws Exception
	{
		SendByte.Send(FriendListRe.askFriendList());
		int row = getFriendList.fileNum();
		tableDatas= getFriendList.fileList(row);
	}
	/*{{"1","Dan Boneh",false},
			{"2","David Brumley",false},
	{"3","Silverman J H",false},
	{"4","Paterson K G",false},
	{"5","R.Saka",false},
	{"6","M.Kasahara",false},
	{"7","Vanstone S A",false},
	{"8","J.Malone-Lee",false},
	{"9","Xavier Boyen",false},
	{"10","Laurie Law",false}
	};*/
	//隐藏表格列
	/*private void hideTableColumn(JTable table, int column){
	    TableColumnModel tcm = table.getColumnModel();
	    //其实没有移除，仅仅隐藏而已
	    TableColumn tc = tcm.getColumn(column);
	    tcm.removeColumn(tc);		
	}*/
	@Override
	public int getRowCount() {
		// TODO Auto-generated method stub
		return tableDatas.length;
	}

	@Override
	public int getColumnCount() {
		// TODO Auto-generated method stub
		return columnNames.length;
	}

	public Object getValueAt(int rowIndex, int columnIndex) {
		// TODO Auto-generated method stub
			return tableDatas[rowIndex][columnIndex];
	}
	

	public String getColumnName(int col) {
		return columnNames[col];
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
		if (col !=2) {
			return false;
		} else {
			return true;
		}
	}
	
}
