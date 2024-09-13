package com.android.socketclient;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import com.android.socketclient.TCPClient.Connect;

public class MySingleton{
	// pt o singura instanta
	private static MySingleton m_sg = new MySingleton();
	private MySingleton(){}
	
	//metoda returneaza adresa instantei clasei ca sa nu poata fi accesata numai o data
	public static MySingleton getInstance(){
		return m_sg;
	}
	

	// Global
	private static String ID_IP;
	private static String ID_PORT;
	private static String ID_IMEI;
	private static String ID_NAME;
	private static String ID_USERNAME;
	private static String ID_PASSWORD;
	private static String ID_AUTH;
	private static String ID_THISNAME;
	private static String ID_THISUSERNAME;
	private static String ID_THISAUTH;
	private static String bufferTemp;
	private static boolean buttonConnecting;
	
	private boolean blocking=false;
	

	
	public void block(){
		blocking=true;
		
	}
	public void unblock(){
		blocking=false;
		
	}
	public void set(String who, String value){
		while(blocking==true);

		if(who.equals("ID_IP")){
			ID_IP = value;
		}else if(who.equals("ID_PORT")){
			ID_PORT = value;
		}else if(who.equals("ID_IMEI")){
			ID_IMEI = value;
		}else if(who.equals("ID_NAME")){
			ID_NAME = value;
		}else if(who.equals("ID_USERNAME")){
			ID_USERNAME = value;
		}else if(who.equals("ID_PASSWORD")){
			ID_PASSWORD = value;
		}else if(who.equals("ID_AUTH")){
			ID_AUTH = value;
		}else if(who.equals("ID_THISNAME")){
			ID_THISNAME = value;
		}else if(who.equals("ID_THISUSERNAME")){
			ID_THISUSERNAME = value;
		}else if(who.equals("ID_THISAUTH")){
			ID_THISAUTH = value;
		}else if(who.equals("bufferTemp")){
			bufferTemp = value;
		}
	}
	
	public String get(String who){
		if(who.equals("ID_IP")){
			return ID_IP;
		}else if(who.equals("ID_PORT")){
			return ID_PORT;
		}else if(who.equals("ID_IMEI")){
			return ID_IMEI;
		}else if(who.equals("ID_NAME")){
			return ID_NAME;
		}else if(who.equals("ID_USERNAME")){
			return ID_USERNAME;
		}else if(who.equals("ID_PASSWORD")){
			return ID_PASSWORD;
		}else if(who.equals("ID_AUTH")){
			return ID_AUTH;
		}else if(who.equals("ID_THISNAME")){
			return ID_THISNAME;
		}else if(who.equals("ID_THISUSERNAME")){
			return ID_THISUSERNAME;
		}else if(who.equals("ID_THISAUTH")){
			return ID_THISAUTH;
		}else if(who.equals("bufferTemp")){
			return bufferTemp;
		}
		return null;
	}
	
	public void setBool(boolean value){
		buttonConnecting = value;
	}
	public boolean getBool(){
		return buttonConnecting;
	}


	
}
