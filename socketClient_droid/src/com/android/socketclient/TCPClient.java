package com.android.socketclient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.Button;
import android.widget.Toast;
import android.telephony.TelephonyManager;


public class TCPClient extends Activity{
	private MySingleton sg = MySingleton.getInstance();
	public String slash = "//";
	

	
    private InputStream in;
    private OutputStream out;
    
    private InputStream newIn;
    private OutputStream newOut; 
    
    private Socket remoteSock = null;
    private Socket newSock = null;
    
    private Connect wConnectionClass = new Connect();
    private Thread threadConnection = null;
    private static Handler mBackMainHandler = null;
    private static Handler mBackSettingsHandler = null;
    private static Handler mBackProductsHandler = null;
    
	private Message msgBack = null;
    public boolean IsConnected = false;
    private final ReentrantLock lockGetAllProd = new ReentrantLock();
    private final ReentrantLock lockConnecting = new ReentrantLock();
    private final ReentrantLock lockRegistering = new ReentrantLock();
    

	private static sqliteAPI sql;
	private static Context context;
    private String sendChatToUser = "";
    private String sendChatToGroup = "";


    private long startCounter;    
    private long elapsedTime;


	// one instance
	private static TCPClient mTCPClient = new TCPClient();
	private TCPClient(){
	}
	public static TCPClient getInstance(Handler h, String whichHandler, Context mainContext){
		context = mainContext;
		sql = new sqliteAPI(context);
		if(whichHandler.equals("mainDialog")){
			mBackMainHandler = h;
		}else if(whichHandler.equals("settingsDialog")){
			mBackSettingsHandler = h;
		}else if(whichHandler.equals("productsDialog")){
			mBackProductsHandler = h;
		}
		return mTCPClient;
	}	
	


   
    // global
	private String status, userName, group, toUserName, toGroup;
	private String remoteIPPORTConnected, clientTimeConnected;
	private String typeIMEI, osVersion, name, confirmed, cantitate;
	private String id, oldProdus, oldCategorie, produs, categorie, stoc, pret, addedtime;
    
    

    
    
    public class RecvThread implements Runnable{
        //init
    	byte[] bufferSZ = new byte[5];
        int read,index,size,count; 
        String data;
        String dataSZ;
        
        Socket whichSocket;
        OutputStream output;
        InputStream input;
        
        public RecvThread(Socket whichSocketT, InputStream inT, OutputStream outputT){
        	whichSocket=whichSocketT;
        	input=inT;
        	output=outputT;
        }
        public void run(){
            while ( IsConnected == true ){
                try {
                   read = input.read(bufferSZ); // -1 on lost connection
                   if (read > 0){
                       dataSZ = new String(bufferSZ);
	                   index = 0;
	                   for (int i = 0; i < dataSZ.length(); i++) {
	                	    if (Character.isDigit(dataSZ.charAt(i)))//If we find a non-digit character we return false.
	                	    	index=i; //return false
	                	}
	                   dataSZ = dataSZ.substring(0,index+1);
	                   size = Integer.parseInt(dataSZ);
	                   byte[] buffer = new byte[size];
	                   count=0;
	                   while(count<size){
	                	   read = input.read(buffer,count,size-count);
	                	   count = read; 
	                	  // Log.d("TCPClient", " count " + count + " size "  + size + " " );
	                   }
	                   
	                   data = new String(buffer);
	                   Log.d("TCPClient", "message > " + data);
	                   protocol(new StringBuffer(data));
	                   
	                   
                   }else if (read <= 0){
                	   Log.d("TCPClient", "EXITED OK!");
                	   onCloseConnection(whichSocket,input, output);
                	   break;
                   }
                   
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }           
        }
    }
    public void protocol(StringBuffer bufferTemp){
    	status=bufferTemp.substring(0, bufferTemp.indexOf(slash));
    	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());
    	userName=bufferTemp.substring(0, bufferTemp.indexOf(slash));
    	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());
    	group=bufferTemp.substring(0, bufferTemp.indexOf(slash));
    	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());
    	  	
    	if(status.equals("SEND_MESSAGETOALL")){
        	sg.set("ID_USERNAME", userName);
        	sg.set("ID_AUTH", group);
        	sg.set("bufferTemp",bufferTemp.toString());
        	sg.block();
        	sendHandlerMsgBack(1,"messageToAll",mBackMainHandler);        	
        	 		
    		
 
    	}else if(status.equals("SEND_MESSAGETO")){
    		toUserName=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	toGroup=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	
        	String s="***["+userName+"/"+group+" >> "+toUserName+"/"+toGroup+"] : " + bufferTemp.toString();
        	sendHandlerMsgBack(5,s,mBackMainHandler);        	
	 
    	}else if(status.equals("CLIENT_CONNECT")){ 
    		name=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	typeIMEI=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	osVersion=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	remoteIPPORTConnected=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	clientTimeConnected=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
 

        	sg.set("ID_NAME", name);
        	sg.set("ID_USERNAME", userName);
        	sg.set("ID_AUTH", group);
        	sg.block();
        	sendHandlerMsgBack(1,"newClientConnected",mBackMainHandler);        	
        	
        	
    		if(bufferTemp.toString().equals("CONNECTED1")){
    			sg.set("ID_THISUSERNAME", userName);
    			sg.set("ID_THISAUTH", group);
    			sg.set("ID_THISNAME", name);
    			sendHandlerMsgBack(1,"connected",mBackMainHandler);
    			sendHandlerMsgBack(3,"connected",mBackProductsHandler);
    			
    			IsConnected = true;
    			sendGetAllProducts();
    			
 
    		}

    		if(bufferTemp.toString().equals("CONNECTED2")){
    			
    			sendHandlerMsgBack(1,"CONNECTED2",mBackMainHandler);
    			
    		
    		}
    	}else if(status.equals("CLIENT_DISCONNECT")){
        	sg.set("ID_USERNAME", userName);
        	sg.set("ID_AUTH", group);
        	sendHandlerMsgBack(1,"clientDisconnected",mBackMainHandler);        	
        		
    	}else if(status.equals("CLIENT_REGISTER")){
    		if(bufferTemp.toString().equals("DUPLICATE")){
    		sendHandlerMsgBack(2, "DUPLICATE",mBackSettingsHandler);
    		}else if(bufferTemp.toString().equals("OKEY")){
        		sendHandlerMsgBack(2, "OKEY",mBackSettingsHandler);
        	}

    			
    			
    	}else if(status.equals("DB_GETALLPRODUCTS")){
    		//lockGetAllProd.lock();
    		id=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	produs=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	categorie=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	stoc=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	pret=bufferTemp.toString();
   
        	String cmds = "id='"+id+"',produs='"+produs+"',categorie='"+categorie+"',stoc='"+stoc+"',pret='"+pret+"'";
    		if(sql.dbGetItem("products","id",cmds).isEmpty()==true)
    		sql.dbInsertItem("products",cmds);
  		    //lockGetAllProd.unlock();
    		
    		  
    	}else if(status.equals("DB_UPDATINGALLPRODUCTS")){
    		if(bufferTemp.toString().equals("START")){
    			sendHandlerMsgBack(1, "STARTPRODUCTUPDATE",mBackMainHandler);
    			sql.beginInsertFlux();
    			startCounter = System.currentTimeMillis();
    		}else if(bufferTemp.toString().equals("STOP")){
    			sendHandlerMsgBack(1, "STOPPRODUCTUPDATE",mBackMainHandler);
    			sql.endInsertFlux();
    			elapsedTime = System.currentTimeMillis() - startCounter;
    			String test = "<!> Time: " + elapsedTime;
    			sendHandlerMsgBack(5,test,mBackMainHandler);     
    			
    		}
    	}else if(status.equals("DB_UPDATEPRODUCT")){
       		oldProdus=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());
        	oldCategorie=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
    		
    		id=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	produs=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	categorie=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	stoc=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	pret=bufferTemp.toString();
        	
    		
    		String cmds = "produs='"+produs+"',categorie='"+categorie+"',stoc='"+stoc+"',pret='"+pret+"'";
    		sql.dbUpdateItem("products",id,cmds);	

        	sendHandlerMsgBack(3,"updateAllProducts",mBackProductsHandler);
	

    	}else if(status.equals("DB_DELETEPRODUCT")){
    		id=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	produs=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
        	categorie=bufferTemp.substring(0, bufferTemp.indexOf(slash));
        	bufferTemp.delete(0, bufferTemp.indexOf(slash)+slash.length());	
    		
        	String cmds="id='"+id+"'";
        	sql.dbDeleteItem("products",cmds);
        	
        	sendHandlerMsgBack(4,produs,mBackProductsHandler);
	

    	}else if(status.equals("DB_NEWBUY")){
    		if(bufferTemp.toString().equals("OKEY")){
    			sendHandlerMsgBack(3,"sendCommandsOKEY",mBackProductsHandler);
    		}
    	}
   
    }

    
    
    
    public void setSendChatTo(String userT, String groupT){
       sendChatToUser = userT;
       sendChatToGroup = groupT;
    }
   /* public void sendGetAllProducts(){
    	String lastId = Integer.toString(sql.dbLastRowId("products"));
    	String s = "DB_GETALLPRODUCTS"+slash+sg.get("ID_THISUSERNAME")+slash+sg.get("ID_THISAUTH")+slash+lastId;
    	Send(out,s);
    }
    */
    public void sendNewBuy(String newBuy){
    	String s = "DB_NEWBUY"+slash+
    		sg.get("ID_THISUSERNAME")+slash+
    		sg.get("ID_THISAUTH")+slash+
    		newBuy;
    	Send(out,s);
    }
    public void sendChatMessage(String msg){
    	if( IsConnected==true ){
    		String s = null;
    		if((sendChatToUser.equals("")==true)&&(sendChatToGroup.equals("")==true)){
    			s = "SEND_MESSAGETOALL"+slash+sg.get("ID_THISUSERNAME")+slash+sg.get("ID_THISAUTH")+slash+msg;
    		}else{
    			s = "SEND_MESSAGETO"+slash+sendChatToUser+slash+sendChatToGroup+slash+msg;
    		}
    		Send(out,s);
    	}
    }
    public void sendDisconnect(){
    	if( IsConnected==true ){
    		String s = "CLIENT_DISCONNECT"+slash+sg.get("ID_THISUSERNAME")+slash+sg.get("ID_THISAUTH")+slash+"NULL";
    		Send(out,s);
    	}  	
    }
    public void sendGetAllProducts() {
    	if( IsConnected==true ){
    		try {
    			InetAddress serverAddr = InetAddress.getByName( sg.get("ID_IP") );
    			newSock = new Socket(serverAddr.getHostAddress(), Integer.parseInt( sg.get("ID_PORT") ));
    			if(newSock.isConnected()==true){
    				newIn = newSock.getInputStream();
    				newOut = newSock.getOutputStream();
    				
    				RecvThread recv = new RecvThread(newSock,newIn,newOut); // start recv
    				Thread thread = new Thread(recv);
    				thread.start();

    				String lastId = Integer.toString(sql.dbLastRowId("products"));
    				String s = "DB_GETALLPRODUCTS"+slash+sg.get("ID_THISUSERNAME")+slash+sg.get("ID_THISAUTH")+slash+lastId;
    				Send(newOut,s);		
    			}
    		}catch(Exception e) {
    			Log.e("TCP", "C: Error1", e);
    		}
    	}
	
    }   
    public void sendRegister() {
		lockRegistering.lock(); 
		try {   	
	    	if( IsConnected==false ){
	    		try {
		    		InetAddress serverAddr = InetAddress.getByName( sg.get("ID_IP") );
		    		remoteSock = new Socket(serverAddr.getHostAddress(), Integer.parseInt( sg.get("ID_PORT") ));
					if(remoteSock.isConnected()==true){
						in = remoteSock.getInputStream();
						out = remoteSock.getOutputStream();
		
						IsConnected = true;
						RecvThread recv = new RecvThread(remoteSock,in,out); // start recv
						Thread thread = new Thread(recv);
						thread.start();
		
		
						String s = "CLIENT_REGISTER"+slash+sg.get("ID_THISUSERNAME")+slash+"User"+slash+sg.get("ID_IMEI")+slash+"Android"+slash+sg.get("ID_THISNAME")+slash+sg.get("ID_PASSWORD");
						Send(out,s);
						sendHandlerMsgBack(2,"REQESTSENDED",mBackSettingsHandler);    		
					}
	    		}catch(Exception e) {
					Log.e("TCP", "C: Error1", e);
					sendHandlerMsgBack(2,"NOCONNECTION",mBackSettingsHandler);
				}
	    	}else{
	    		sendHandlerMsgBack(2,"FIRSTDISCONNECT",mBackSettingsHandler);
	    	}
		} finally {
			lockRegistering.unlock();
		}
    }   
    public void startConnecting() {
    	if((sg.getBool()==true)&&(IsConnected == false)){
    		threadConnection = new Thread(wConnectionClass);	
    		threadConnection.start();
    		sendHandlerMsgBack(1,"connecting",mBackMainHandler);	
    	}
    }
    
    public class Connect implements Runnable {
		public void run() {
			lockConnecting.lock(); 
			try {
				while((IsConnected == false)&&(sg.getBool()==true)){
					try {
						Thread.sleep(2000);
						// Establish the remote endpoint for the socket.
						InetAddress serverAddr = InetAddress.getByName( sg.get("ID_IP") );
						remoteSock = new Socket(serverAddr.getHostAddress(), Integer.parseInt( sg.get("ID_PORT") ));
						if(remoteSock.isConnected()==true){
							in = remoteSock.getInputStream();
							out = remoteSock.getOutputStream();
							
							IsConnected = true;
							RecvThread recv = new RecvThread(remoteSock,in,out); // start recv
							Thread thread = new Thread(recv);
							thread.start();
							String s = "CLIENT_CONNECT"+slash+sg.get("ID_THISUSERNAME")+slash+"NULL"+slash+sg.get("ID_IMEI")+slash+"Android"+slash+sg.get("ID_PASSWORD");
							Send(out,s);

				
						}
					}catch(Exception e) {
						Log.e("TCP", "C: Error1", e);
					}
				}
			} finally {
				lockConnecting.unlock();
			}
		}
    }

    public void onCloseConnection(Socket whichSocket, InputStream inT, OutputStream outT) {
    	if ( IsConnected == true ){
	        try {
	            inT.close();
	            outT.close();
	            whichSocket.close();
	            if(whichSocket==remoteSock){
		            IsConnected=false;
		            sendHandlerMsgBack(1,"disconnected",mBackMainHandler);
		            sendHandlerMsgBack(3,"disconnected",mBackProductsHandler);
		            startConnecting();
	            }
	        } catch (IOException e) {
	            e.printStackTrace();
	        }
    	}
    }
    public void Send(OutputStream outT, String data){
        if ( IsConnected == true ){
            try {
            	PrintWriter outL = new PrintWriter( new BufferedWriter( new OutputStreamWriter(outT)),true);
            	outL.println(data.length());   	

    			DataOutputStream dataOutputStream = null;
    			dataOutputStream = new DataOutputStream(outT);
    			dataOutputStream.writeUTF(data);   			
    			
            } catch (IOException e) {
                e.printStackTrace();
            }
        }else{
        	Toast.makeText(context, "Inainte conectati-va!", Toast.LENGTH_SHORT).show();
        }
    }
    public void sendHandlerMsgBack(int what, String object, Handler whichHandler){
    	if(whichHandler!=null){
	    	msgBack = new Message();
	    	msgBack.what = what;
	    	msgBack.obj = object;
	    	whichHandler.sendMessage(msgBack);		
    	}
    }

    
    
}
