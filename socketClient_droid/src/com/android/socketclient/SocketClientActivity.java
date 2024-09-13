package com.android.socketclient;

import java.util.ArrayList;



import com.android.socketclient.R;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.location.GpsStatus.Listener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;



public class SocketClientActivity extends Activity {


	private Button mButtonConnect;
	private Button mButtonProducts;
	private Button mButtonChat;
	
	private TCPClient mClientSocket;
	private dbSettings dh;
	private MySingleton sg;
	private settingsDialog mSettings;
	private productsDialog mProductsDialog;

	private Context context;
	private	Handler mHandlerMessages = null;

	private TextView mTextViewConnStatus;
	private TextView mTextViewConnName;
	private TextView mTextViewChat;
	private TextView mTextViewSendChatTo;
	private ScrollView mScrollViewChat;
	
	private EditText meditTextChat;
	
	
	private GridViewAdapter adapter;
	private GridView mGridViewUserList;
	private userList mList = new userList();

	

	
	public SocketClientActivity(){
		context = this;		
	}
	
	
    // Add menu to mainFrame
   public boolean onCreateOptionsMenu(Menu menu){
	   MenuInflater inflater = getMenuInflater();
	   inflater.inflate(R.menu.menu, menu);
	   return true;
   }
   public boolean onOptionsItemSelected (MenuItem item){
	   switch (item.getItemId()){
	   	case R.id.item1: // about
	   	{
			AlertDialog alertDialog;
			alertDialog = new AlertDialog.Builder(this).create();
			alertDialog.setTitle("About");
			alertDialog.setMessage("Created by Cotfas Vlad");
			alertDialog.show();	      		
	   	}	
	   	return true;
	   		
		case R.id.item2: // settings
		{
	       final Dialog d = new Dialog(context); //*this* is the Activity
	        Window w = d.getWindow();
	        w.setFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND,
	                   WindowManager.LayoutParams.FLAG_BLUR_BEHIND);
	       

	        d.requestWindowFeature(Window.FEATURE_LEFT_ICON); // request to show an icon in title
	        d.setTitle(R.string.settingsTitle); // set a new title
	        d.setContentView(R.layout.settings); //your layout defined in an xml file (in res/layout)
	        mSettings.settingsDialogInitialise(d);
	        d.show(); 		
	        d.setFeatureDrawableResource(Window.FEATURE_LEFT_ICON, android.R.drawable.ic_menu_preferences); // show the icon in title
	        d.setCanceledOnTouchOutside(true);
	     
		}
			return true;
		case R.id.item3: // exit
			mClientSocket.sendDisconnect(); // disconnect
			System.exit(0); 
		return true;
		}
    return false;
    }   
    
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	// init
        super.onCreate(savedInstanceState);
        mainDialogInitialise(); // initialise
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	if (keyCode == KeyEvent.KEYCODE_BACK) {
    		
    		return super.onKeyDown(keyCode, event);
    	}
    	return super.onKeyDown(keyCode, event);
    }


    private void ConnectDisconnect(){
    	sg.set("ID_IP", this.dh.getValue("ID_IP").toString());
    	sg.set("ID_PORT", this.dh.getValue("ID_PORT").toString());
    	sg.set("ID_THISUSERNAME", this.dh.getValue("ID_THISUSERNAME").toString());
    	sg.set("ID_PASSWORD", this.dh.getValue("ID_PASSWORD").toString());
    	
    	if(sg.getBool()==false){
    			sg.setBool(true);
    			mClientSocket.startConnecting();
    	}else{
    		sg.setBool(false);
    		mClientSocket.sendDisconnect();
    		mButtonConnect.setText(R.string.textButonConnect);
    	}
    }

  
    
    
    
    

    
    
    
    
    
    
    
    
    

	public class GridViewAdapter extends BaseAdapter {
		private Context mContext;

		public GridViewAdapter(Context c) {
			mContext = c;
		}
		public int getCount() {
			return mList.getSize();
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
					LayoutInflater li = getLayoutInflater();
					v = li.inflate(R.layout.gridconnectedclients, null);
				} else {             
					v = convertView;         
				}    

				// Add The Text!!!
				TextView tv1 = (TextView)v.findViewById(R.id.textView1);
				tv1.setText( mList.getItem("name", position) );

				TextView tv2 = (TextView)v.findViewById(R.id.textView2);
				tv2.setText( mList.getItem("user", position) );

				TextView tv3 = (TextView)v.findViewById(R.id.textView3);
				tv3.setText( mList.getItem("auth", position) );
				
				return v;     
			} 
		}
	}
    public void sendChatMessage(){
    	String temp = meditTextChat.getText().toString();
    	
    	if(mClientSocket.IsConnected==true){
    		if(!temp.equals("")){
    			mClientSocket.sendChatMessage(temp);
    			meditTextChat.setText("");
    		}
    	}else{
    		Toast.makeText(context, "Inainte conectati-va!", Toast.LENGTH_SHORT).show();
    	}
    }
    public void addMessageToChat(String msg) {
    	mTextViewChat.append(msg+"\n");
    	mScrollViewChat.fullScroll(ScrollView.FOCUS_DOWN);
    } 
    
    public void mainDialogInitialise(){

    	requestWindowFeature(Window.FEATURE_NO_TITLE);//Remove title bar
    	//this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN); //Remove notification bar
    	setContentView(R.layout.main);
    	
    	
      	this.dh = new dbSettings(context); // init db
    	this.sg = MySingleton.getInstance();
    	this.mSettings = new settingsDialog(context);
    	this.mProductsDialog = new productsDialog(context);
    	
    	firstStartApplication(); // first run
    	
    	
    	TelephonyManager telephonyManager = (TelephonyManager)getSystemService(Context.TELEPHONY_SERVICE);
    	sg.set("ID_IMEI", telephonyManager.getDeviceId());	
    	
    	mButtonChat = (Button) this.findViewById(R.id.buttonSend);
    	mButtonProducts = (Button) this.findViewById(R.id.idButtonProducts);
    	mButtonConnect = (Button) this.findViewById(R.id.idButtonConnect);
    	mTextViewConnStatus = (TextView)findViewById(R.id.textViewConnectionStatus);
    	mTextViewConnName = (TextView)findViewById(R.id.textViewName);
    	mTextViewConnName.setText(dh.getValue("ID_THISNAME").toString());
    	mTextViewChat = (TextView)findViewById(R.id.textViewChat);
    	mTextViewSendChatTo = (TextView)findViewById(R.id.textViewSendChatTo);
    	meditTextChat = (EditText)findViewById(R.id.editTextChat);
    	mScrollViewChat = (ScrollView) findViewById(R.id.ScrollViewChat);
    	mGridViewUserList = (GridView)findViewById(R.id.gridViewUserList); 
    	//mTextViewChat.setMovementMethod(new ScrollingMovementMethod()); // scrollable effects
    	

    	
    	mHandlerMessages = new Handler() {
    		@Override
    		public void handleMessage(Message msg) {
    			if(msg.what==1){
      				if((boolean)msg.obj.equals("connecting")==true){
    					mButtonConnect.setText("Connecting...");
    					
    				}else if((boolean)msg.obj.equals("connected")==true){
    					mButtonConnect.setText("Connected!");
    					
    					
    					mTextViewConnStatus.setTextColor(android.graphics.Color.GREEN);
    					mTextViewConnStatus.setText("ONLINE");
    					
    					mTextViewConnName.setText(sg.get("ID_THISNAME").toString());
            			dh.update("ID_THISNAME", sg.get("ID_THISNAME").toString());
    					
    					addMessageToChat("Eu ["+ sg.get("ID_THISUSERNAME")+"/"+ sg.get("ID_THISAUTH")+"] m-am conectat!");
        				
    					
    				}else if((boolean)msg.obj.equals("disconnected")==true){
    					mButtonConnect.setText("Disconnected!");
    					
    					mTextViewConnStatus.setTextColor(android.graphics.Color.RED);
    					mTextViewConnStatus.setText("OFFLINE");  					
    					
    					
    					mList.removeAllItems();
    					adapter.notifyDataSetChanged();
    					
    				}else if((boolean)msg.obj.equals("newClientConnected")==true){
    					mList.addItem(sg.get("ID_NAME"),sg.get("ID_USERNAME"),sg.get("ID_AUTH"));
    					sg.unblock();
    					
    					adapter.notifyDataSetChanged();
    				}else if((boolean)msg.obj.equals("clientDisconnected")==true){
    					int index = mList.findItemInList("user", sg.get("ID_USERNAME"));
    					mList.removeItem(index);
    					adapter.notifyDataSetChanged();
    					addMessageToChat("Clientul ["+ sg.get("ID_USERNAME")+"/"+ sg.get("ID_AUTH")+"] sa deconectat!");
    				}else if((boolean)msg.obj.equals("CONNECTED2")==true){
    					addMessageToChat("Clientul ["+ sg.get("ID_USERNAME")+"/"+ sg.get("ID_AUTH")+"] sa conectat!");
    				}else if((boolean)msg.obj.equals("messageToAll")==true){
    					addMessageToChat("["+ sg.get("ID_USERNAME")+"/"+ sg.get("ID_AUTH")+"] " + sg.get("bufferTemp"));
    					sg.unblock();
    				}else if((boolean)msg.obj.equals("STARTPRODUCTUPDATE")==true){
    					mButtonProducts.setText("Products Updating!");
    					mButtonProducts.setEnabled(false);
    					Toast.makeText(context, "Produse in curs de actualizare!", Toast.LENGTH_SHORT).show();
    				}else if((boolean)msg.obj.equals("STOPPRODUCTUPDATE")==true){
    					mButtonProducts.setText("Products Updated!");
    					mButtonProducts.setEnabled(true);
    					Toast.makeText(context, "Produse actualizate cu succes!", Toast.LENGTH_SHORT).show();
    				}
      		
      				
    			}
    			
    			if(msg.what==5){
    				addMessageToChat(msg.obj.toString());
    			}
    		
    		}
    	};	
      	this.mClientSocket = TCPClient.getInstance(mHandlerMessages,"mainDialog", this);//new TCPClient(); // client socket init

		adapter = new GridViewAdapter(context);
		mGridViewUserList.setAdapter(adapter);
		mGridViewUserList.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
			    if(!mTextViewSendChatTo.getText().equals(mList.getItem("user", position).toString())==true){
			    	mTextViewSendChatTo.setText(mList.getItem("user", position).toString());
			    	mClientSocket.setSendChatTo(mList.getItem("user", position),mList.getItem("auth", position));
			    }
			}
		});
		
		
		mTextViewChat.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		mTextViewSendChatTo.setText(" ALL ");
        		mClientSocket.setSendChatTo("","");
        	}
    	});	
		
    	if (this.dh.getValue("autoConnecting").equals("1")){
    		ConnectDisconnect();
    	}   	
    	
    	mButtonConnect.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		ConnectDisconnect();
        	}
    	});	
    	mButtonChat.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		sendChatMessage();
        	}
    	});	
    	
    	mButtonProducts.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		
        		final Dialog d = new Dialog(context); //*this* is the Activity
        		Window w = d.getWindow();
        		w.setFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND,
        				WindowManager.LayoutParams.FLAG_BLUR_BEHIND);


        		d.requestWindowFeature(Window.FEATURE_LEFT_ICON); // request to show an icon in title
        		d.setTitle(R.string.productsTitle); // set a new title
        		d.setContentView(R.layout.products); //your layout defined in an xml file (in res/layout)
    	        d. getWindow().setLayout(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT); // setFullScreen
        		mProductsDialog.productsDialogInitialise(d);
        		d.show(); 		
        		d.setFeatureDrawableResource(Window.FEATURE_LEFT_ICON, android.R.drawable.ic_menu_search); // show the icon in title
        		d.setCanceledOnTouchOutside(true);    		

        	}
    	});	   	
    	
    }
    public void firstStartApplication(){
    	// FIRST START >>>
    	if (dh.getValue("autoConnecting").equals("NULL")){
    		dh.insert("autoConnecting", "0");
    	}
    	if (dh.getValue("ID_IP").equals("NULL")){
    		dh.insert("ID_IP", "riddex.zapto.org");
    	}
    	if (dh.getValue("ID_PORT").equals("NULL")){
    		dh.insert("ID_PORT", "3000");

    	}
    	if (dh.getValue("ID_THISUSERNAME").equals("NULL")){
    		dh.insert("ID_THISUSERNAME", "");

    	}
    	if (dh.getValue("ID_THISNAME").equals("NULL")){
    		dh.insert("ID_THISNAME", "anonymous");

    	}   
    	if (dh.getValue("ID_PASSWORD").equals("NULL")){
    		dh.insert("ID_PASSWORD", "");

    	}
    	// <<< FIRST START 
    }
    
 
    
}







