package com.android.socketclient;

import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

public class settingsDialog extends Activity {
	private EditText mEditIP;
	private EditText mEditPort;  
	private EditText mEditName;
	private EditText mEditUserName;
	private EditText mEditUserPass;
	private EditText mEditBot;
	
	private CheckBox mCheckBox;
	private Button mButtonRegister;
	
	private TCPClient mClientSocket;
	private dbSettings dh;
	private MySingleton sg;	
	private Context context;
	
	
	private	Handler mHandlerMessages;
	
	
	
	private String strTest = null;
	private boolean semafor = false;
	
	
	public settingsDialog(Context contextT){
		context=contextT;
		
	}
    public void settingsDialogInitialise(final Dialog dialog){
    	
     	this.dh = new dbSettings(context);
    	this.sg = MySingleton.getInstance();
    	
    	
    	mEditIP = (EditText) dialog.findViewById(R.id.editText1);
    	mEditPort = (EditText) dialog.findViewById(R.id.editText2);
     	mEditUserName = (EditText) dialog.findViewById(R.id.editText3);
     	mEditUserPass = (EditText) dialog.findViewById(R.id.editText4);
     	mButtonRegister = (Button) dialog.findViewById(R.id.buttonRegister);
    	mCheckBox = (CheckBox) dialog.findViewById(R.id.checkBox1);


    	// update layout elements
    	mEditIP.setText(this.dh.getValue("ID_IP"));
    	mEditPort.setText(this.dh.getValue("ID_PORT"));
    	mEditUserName.setText(this.dh.getValue("ID_THISUSERNAME"));
    	mEditUserPass.setText(this.dh.getValue("ID_PASSWORD"));
    	if (this.dh.getValue("autoConnecting").equals("1")){
    		mCheckBox.setChecked(true);
    	}
    	
    	
    	
    	
    	mCheckBox.setOnClickListener(new OnClickListener() {
    		public void onClick(View v) {
    			if (mCheckBox.isChecked()) {
    				dh.update("autoConnecting", "1");
    				addLOG("Auto connecting was activated.",dialog,"settings");
    			} else {
    				dh.update("autoConnecting", "0");
    				addLOG("Auto connecting was disabled.",dialog,"settings");
    			}
    		}
    	});
    	mButtonRegister.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		
            	final Dialog d = new Dialog(context); //*this* is the Activity
            	Window w = d.getWindow();
            	w.setFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND,
            			WindowManager.LayoutParams.FLAG_BLUR_BEHIND);
            
            	d.requestWindowFeature(Window.FEATURE_LEFT_ICON);
            	d.setTitle(R.string.registerTitle);
            	d.setContentView(R.layout.register); //your layout defined in an xml file (in res/layout)
            	registerFormInitialise(d);
            	d.show();
    	        d.setFeatureDrawableResource(Window.FEATURE_LEFT_ICON, android.R.drawable.ic_menu_add); 
    	        d.setCanceledOnTouchOutside(true);
             }
    	});
    
    	
    	
    	
    	mEditIP.addTextChangedListener(new TextWatcher() {
    		public void  afterTextChanged (Editable s){
    			Log.d("seachScreen", "afterTextChanged");
    			dh.update("ID_IP", mEditIP.getText().toString());
    	    	sg.set("ID_IP",mEditIP.getText().toString());		
    		}
    		public void  beforeTextChanged  (CharSequence s, int start, int count, int after){}
    		public void  onTextChanged  (CharSequence s, int start, int before, int count) {}
    	});
    	mEditPort.addTextChangedListener(new TextWatcher() {
    		public void  afterTextChanged (Editable s){
    			Log.d("seachScreen", "afterTextChanged");
    			dh.update("ID_PORT", mEditPort.getText().toString());
    			sg.set("ID_PORT",mEditPort.getText().toString());	
    		}
    		public void  beforeTextChanged  (CharSequence s, int start, int count, int after){}
    		public void  onTextChanged  (CharSequence s, int start, int before, int count) {}
    	});   	
    	mEditUserName.addTextChangedListener(new TextWatcher() {
    		public void  afterTextChanged (Editable s){
    			Log.d("seachScreen", "afterTextChanged");
    			dh.update("ID_THISUSERNAME", mEditUserName.getText().toString());
    			sg.set("ID_THISUSERNAME",mEditUserName.getText().toString());	
    		}
    		public void  beforeTextChanged  (CharSequence s, int start, int count, int after){}
    		public void  onTextChanged  (CharSequence s, int start, int before, int count) {}
    	});    	
    	mEditUserPass.addTextChangedListener(new TextWatcher() {
    		public void  afterTextChanged (Editable s){
    			Log.d("seachScreen", "afterTextChanged");
    			dh.update("ID_PASSWORD", mEditUserPass.getText().toString());
    			sg.set("ID_PASSWORD",mEditUserPass.getText().toString());	
    		}
    		public void  beforeTextChanged  (CharSequence s, int start, int count, int after){}
    		public void  onTextChanged  (CharSequence s, int start, int before, int count) {}
    	});  	

 
    }
    
    public void addLOG(String msg, Dialog dialog, String wichLog) {
    	TextView t = null;
    	if(wichLog=="settings"){
    		t = (TextView)dialog.findViewById(R.id.TextViewLogSettings);
    		t.append(msg+"\n");
    	}else{
    		t = (TextView)dialog.findViewById(R.id.TextViewLogRegister);
    		if(wichLog=="register"){
    			t.setText(msg);
    		}else if(wichLog=="registerAppend"){
    			t.append(msg+"\n");
    		}
    	}
    }   
    public void registerFormInitialise(final Dialog dialog){
    	final String keyValue = genKeyP();
    	sg.set("ID_IP", this.dh.getValue("ID_IP").toString());
    	sg.set("ID_PORT", this.dh.getValue("ID_PORT").toString());
    	
    	
    	mHandlerMessages = new Handler() {
    		@Override
    		public void handleMessage(Message msg) {
    			if(msg.what==2){
    				if((boolean)msg.obj.equals("DUPLICATE")==true){

    					addLOG("Duplicate user name, choise another one!", dialog, "registerAppend");
    				}else if((boolean)msg.obj.equals("REQESTSENDED")==true){

    					addLOG("Request for register sended!", dialog, "registerAppend");
    				}else if((boolean)msg.obj.equals("OKEY")==true){

    					addLOG("Registred okey, press back button, and wait for account confirmation!", dialog, "registerAppend");
    				}else if((boolean)msg.obj.equals("NOCONNECTION")==true){

    					addLOG("No connetion, check connection details.", dialog, "registerAppend");
    				}else if((boolean)msg.obj.equals("FIRSTDISCONNECT")==true){

    					addLOG("First disconnect!", dialog, "registerAppend");
    				}
    				
    				

    			}
    		}
    	};	    	
    	
    	mClientSocket = TCPClient.getInstance(mHandlerMessages,"settingsDialog", this);// client socket init

    	mEditName = (EditText) dialog.findViewById(R.id.EditTextName);
     	mEditUserName = (EditText) dialog.findViewById(R.id.editTextUserName);
     	mEditUserPass = (EditText) dialog.findViewById(R.id.editTextPassword);
     	mEditBot = (EditText) dialog.findViewById(R.id.editTextBot);
     	mButtonRegister = (Button) dialog.findViewById(R.id.buttonRegister);
    	  
     	mEditBot.setHint(mEditBot.getHint()+keyValue);
     	
    	mButtonRegister.setOnClickListener(new OnClickListener() {
        	public void onClick(View v) {
        		strTest="";
        		semafor=false;
        		
        		if(verificareString(mEditName.getText().toString(),false,dialog)==false){
        			strTest+="*Introduceti un nume/prenume format doar din numere/litere.\r\n";
        			semafor=true;
        		}
        		if(verificareString(mEditUserName.getText().toString(),false,dialog)==false){
        			strTest+="*Introduceti un nume de utilizator format doar din numere/litere.\r\n";
        			semafor=true;
        		}
        		if(verificareString(mEditUserPass.getText().toString(),false,dialog)==false){
        			strTest+="*Introduceti o parola formata doar din numere/litere.\r\n";
        			semafor=true;
        		}
        		if(!mEditBot.getText().toString().equals(keyValue)){
        			strTest+="*Codul introdus este gresit.\r\n";
        			semafor=true;
        		}		
        		
            	if(semafor==false){
            		sg.set("ID_THISNAME", mEditName.getText().toString());
                   	sg.set("ID_THISUSERNAME", mEditUserName.getText().toString());
                   	sg.set("ID_PASSWORD", mEditUserPass.getText().toString());
                   	
                   	dh.update("ID_THISUSERNAME", mEditUserName.getText().toString());
        			dh.update("ID_PASSWORD", mEditUserPass.getText().toString());
        			
            		mClientSocket.sendRegister();
            		
            	}else{
            		addLOG(strTest, dialog, "register");
            	}
        	}
    	});     	

    	
    	
    	
    	
    }
    

	private boolean verificareString(String stringTest, boolean isNumbers, Dialog dialog){
		if(!stringTest.equals("")){
			if(isNumbers==true){
				for(int i = 0; i < stringTest.length(); i++){
					if(!(stringTest.charAt(i)>=48 && stringTest.charAt(i)<=57)){
						return false;
					}
				}	
			}else{
				for(int i = 0; i < stringTest.length(); i++){
					if(!((stringTest.charAt(i)>=65 && stringTest.charAt(i)<=90)
							||(stringTest.charAt(i)>=97 && stringTest.charAt(i)<=122)
							||(stringTest.charAt(i)>=48 && stringTest.charAt(i)<=57)
							||(stringTest.charAt(i)==' ')
							)
							){
						return false;
					}
				}
			}
			return true;
		}else{
			return false;
		}
		
    }
    
	private String genKeyP(){
    	int min = 1000;
    	int max = 9999;
    	Random r = new Random();
    	int i1 = r.nextInt(max - min + 1) + min;
    	return Integer.toString(i1);
    }
    
}
