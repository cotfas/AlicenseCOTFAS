package com.android.socketclient;

import java.util.ArrayList;

import com.android.socketclient.SocketClientActivity.GridViewAdapter;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;

public class productsDialog extends Activity {
	private Context context;
	private productsList mListProducts = new productsList();
	private commandsList mListCommands = new commandsList();
	

	private static sqliteAPI sql;
	private TCPClient mClientSocket;
	private	Handler mHandlerMessages = null;
	
	private String slash;
	
	private GridViewAdapter adapterProducts;
	private GridView mGridViewProductsList;
	private GridViewAdapterCommands adapterCommands;
	private GridView mGridViewCommandsList;
	
	private Button mButtonSendCommands;
	private Button mButtonOk;
	private EditText mEditQuantity;
	private TextView mTextViewConnStatus;
	private TextView mTextViewTotalValue;
	

	
	
	
	
	public productsDialog(Context contextT){
		context=contextT;
		sql = new sqliteAPI(context);
		
	}
	
	
	public void productsDialogInitialise(final Dialog dialog){
		mGridViewProductsList = (GridView)dialog.findViewById(R.id.gridViewProductsList); 
		mGridViewCommandsList = (GridView)dialog.findViewById(R.id.gridViewCommandsList); 
		mButtonSendCommands = (Button) dialog.findViewById(R.id.idButtonSendCommand);
		mTextViewConnStatus = (TextView)dialog.findViewById(R.id.textViewConnectionStatus);
		mTextViewTotalValue = (TextView)dialog.findViewById(R.id.textViewTotal);
		
		
		mButtonSendCommands.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		sendCommandList();
        	}
    	});	
		
		adapterProducts = new GridViewAdapter(context);
		mGridViewProductsList.setAdapter(adapterProducts);
		mGridViewProductsList.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View v, final int position, long id) {
				{
					final Dialog d = new Dialog(context); //*this* is the Activity
					Window w = d.getWindow();
					d.requestWindowFeature(Window.FEATURE_LEFT_ICON); // request to show an icon in title
					d.setTitle(R.string.quantityTitle); // set a new title
					d.setContentView(R.layout.getquantity); //your layout defined in an xml file (in res/layout)
					quantityDialog(d,position);
					d.getWindow().setSoftInputMode( WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
					d.show(); 		
					d.setFeatureDrawableResource(Window.FEATURE_LEFT_ICON, android.R.drawable.btn_star); // show the icon in title
					d.setCanceledOnTouchOutside(false);			
				}
			}
		});
		adapterCommands = new GridViewAdapterCommands(context);
		mGridViewCommandsList.setAdapter(adapterCommands);
		mGridViewCommandsList.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
				deleteFromCommandList(position,false);
			}
		});		
		
		updateProductsList();
		updateBuyList();
		
		mHandlerMessages = new Handler() {
    		@Override
    		public void handleMessage(Message msg) {
    			if(msg.what==3){
      				if((boolean)msg.obj.equals("connected")==true){
    					mTextViewConnStatus.setTextColor(android.graphics.Color.GREEN);
    					mTextViewConnStatus.setText("ONLINE");
    					
    				}else if((boolean)msg.obj.equals("disconnected")==true){
    					mTextViewConnStatus.setTextColor(android.graphics.Color.RED);
    					mTextViewConnStatus.setText("OFFLINE");  					
    					
    					
    				}else if((boolean)msg.obj.equals("sendCommandsOKEY")==true){
    					deleteFromCommandList(0,true);		
    					Toast.makeText(context, "Comenzile au fost actualizate cu serverul!", Toast.LENGTH_SHORT).show();
    				}else if((boolean)msg.obj.equals("updateAllProducts")==true){
    					updateProductsList();
    					Toast.makeText(context, "Produse actualizate!", Toast.LENGTH_SHORT).show();
    				}
      				
      				
    			}else if(msg.what==4){
    				int indexPosition = mListProducts.findItemInList("prod", msg.obj.toString());
    				mListProducts.removeItem(indexPosition);
    				adapterProducts.notifyDataSetChanged();		
    			}
    			
    		}
    	};	
      	this.mClientSocket = TCPClient.getInstance(mHandlerMessages,"productsDialog", this);
      	slash=this.mClientSocket.slash;
		if(mClientSocket.IsConnected==true){
			mTextViewConnStatus.setTextColor(android.graphics.Color.GREEN);
			mTextViewConnStatus.setText("ONLINE");
		}
		
		
		
	}
	
	public void quantityDialog(final Dialog d, final int position){
		mButtonOk = (Button) d.findViewById(R.id.buttonOk);
		mEditQuantity = (EditText)d.findViewById(R.id.editTextCantitate);
		mButtonOk.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		if(mEditQuantity.getText().equals("")==false){
        			String id;
        			String cmds = "produs='"+mListProducts.getItem("prod", position)+"'";
        			if(sql.dbGetItem("products", "id", cmds).isEmpty()==false){
        				id = sql.dbGetItem("products", "id", cmds).get(0).get(0);
        				
        				cmds = "idProdus='"+id+"'";
        				if(sql.dbGetItem("buy", "id", cmds).isEmpty()==true){
        					cmds = "idProdus='"+id+"',cantitate='"+mEditQuantity.getText().toString()+"'";
        					sql.dbInsertItem("buy", cmds);
        				}
        			}
        			updateBuyList();
    				d.dismiss();
        		}
        	}
    	});	
	}
	public void sendCommandList(){
		ArrayList<ArrayList<String>> myReturned;
		String returned = "";
		myReturned = sql.dbGetItem("buy","idProdus,cantitate","");
		if(myReturned.isEmpty()==false){
			for(int i = 0; i < myReturned.get(0).size(); i++){
				returned+=myReturned.get(0).get(i)+slash+myReturned.get(1).get(i)+slash;
			}
			mClientSocket.sendNewBuy(returned);
		}	
	}
	public void deleteFromCommandList(int position, boolean allProducts){
		int totalValue = 0;
		if(allProducts==false){
			String cmds;
			String returnedId;
			cmds = "produs='"+mListCommands.getItem("prod", position)+"'";
			if(sql.dbGetItem("products", "id", cmds).isEmpty()==false){
				returnedId = sql.dbGetItem("products", "id", cmds).get(0).get(0);
				cmds = "idProdus='"+returnedId+"'";
				sql.dbDeleteItem("buy", cmds);
			
				totalValue = Integer.parseInt(mTextViewTotalValue.getText().toString()) - (Integer.parseInt(mListCommands.getItem("pret", position))*Integer.parseInt(mListCommands.getItem("cant", position)));
				
			}
			mListCommands.removeItem(position);
			adapterCommands.notifyDataSetChanged(); 	
		}else{
			sql.dbDeleteItem("buy", "");
			mListCommands.removeAllItems();
			adapterCommands.notifyDataSetChanged();		
		}
		mTextViewTotalValue.setText(Integer.toString(totalValue));
	}
	public void updateProductsList(){
		mListProducts.removeAllItems();
		ArrayList<ArrayList<String>> myReturned;
		myReturned = sql.dbGetItem("products","produs,categorie,stoc,pret","");
		if(myReturned.isEmpty()==false){
			for(int i = 0; i < myReturned.get(0).size(); i++){
				mListProducts.addItem(myReturned.get(0).get(i),myReturned.get(1).get(i),myReturned.get(2).get(i),myReturned.get(3).get(i));
			}
		}
		adapterProducts.notifyDataSetChanged();		
	}
	public void updateBuyList(){
		int totalValue = 0;
		
		mListCommands.removeAllItems();
		ArrayList<ArrayList<String>> myReturned = null;
		ArrayList<ArrayList<String>> resultIDS;
		String cmds;
		resultIDS = sql.dbGetItem("buy","idProdus,cantitate","");
		if(resultIDS.isEmpty()==false){
			for(int i = 0; i < resultIDS.get(0).size(); i++){
				cmds="id='"+resultIDS.get(0).get(i)+"'";
				myReturned = sql.dbGetItem("products","produs,categorie,stoc,pret",cmds);
				if(myReturned.isEmpty()==false){
					mListCommands.addItem(myReturned.get(0).get(0),myReturned.get(1).get(0),myReturned.get(2).get(0),myReturned.get(3).get(0),resultIDS.get(1).get(i).toString());
					totalValue += (Integer.parseInt(myReturned.get(3).get(0))*Integer.parseInt(resultIDS.get(1).get(i)));
				}
			}
		}

		mTextViewTotalValue.setText(Integer.toString(totalValue));
		adapterCommands.notifyDataSetChanged();	
	}
	
	
	
	
	
	
	private class GridViewAdapterCommands extends BaseAdapter {
		private Context mContext;

		public GridViewAdapterCommands(Context c) {
			mContext = c;
		}
		public int getCount() {
			return mListCommands.getSize();
		}
		public Object getItem(int position) {
			return position;
		}
		public long getItemId(int position) {
			return position;
		}
		public View getView(int position, View convertView, ViewGroup parent) {
			{         
				View v;         
				if(convertView==null){   
					LayoutInflater li = (LayoutInflater) mContext.getSystemService( Context.LAYOUT_INFLATER_SERVICE );
					v = li.inflate(R.layout.gridcommands, null);
				} else {             
					v = convertView;         
				}    
				// Add The Text!!!
				TextView tv1 = (TextView)v.findViewById(R.id.textView1);
				tv1.setText( mListCommands.getItem("prod", position) );

				TextView tv2 = (TextView)v.findViewById(R.id.textView2);
				tv2.setText( mListCommands.getItem("cat", position) );

				TextView tv3 = (TextView)v.findViewById(R.id.textView3);
				tv3.setText( mListCommands.getItem("stoc", position) );			
				
				TextView tv4 = (TextView)v.findViewById(R.id.textView4);
				tv4.setText( mListCommands.getItem("pret", position) );
				
				TextView tv5 = (TextView)v.findViewById(R.id.textView5);
				tv5.setText( mListCommands.getItem("cant", position) );


				return v;     
			} 
		}
	}
	private class GridViewAdapter extends BaseAdapter {
		private Context mContext;

		public GridViewAdapter(Context c) {
			mContext = c;
		}
		public int getCount() {
			return mListProducts.getSize();
		}
		public Object getItem(int position) {
			return position;
		}
		public long getItemId(int position) {
			return position;
		}
		public View getView(int position, View convertView, ViewGroup parent) {
			{         
				View v;         
				if(convertView==null){   
					LayoutInflater li = (LayoutInflater) mContext.getSystemService( Context.LAYOUT_INFLATER_SERVICE );
					v = li.inflate(R.layout.gridproducts, null);
				} else {             
					v = convertView;         
				}    
				// Add The Text!!!
				TextView tv1 = (TextView)v.findViewById(R.id.textView1);
				tv1.setText( mListProducts.getItem("prod", position) );

				TextView tv2 = (TextView)v.findViewById(R.id.textView2);
				tv2.setText( mListProducts.getItem("cat", position) );

				TextView tv3 = (TextView)v.findViewById(R.id.textView4);
				tv3.setText( mListProducts.getItem("stoc", position) );			
				
				TextView tv4 = (TextView)v.findViewById(R.id.textView5);
				tv4.setText( mListProducts.getItem("pret", position) );


				return v;     
			} 
		}
	}
	
	
	
}
