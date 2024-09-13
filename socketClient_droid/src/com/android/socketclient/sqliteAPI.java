package com.android.socketclient;

import java.util.ArrayList;
import java.util.Vector;

import android.R.string;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.os.Handler;
import android.util.Log;

public class sqliteAPI extends SQLiteOpenHelper  {

	// All Static variables
	private static final int DATABASE_VERSION = 1;   // Database Version
	private static final String DATABASE_NAME = "database.db";   // Database Name

	private static SQLiteDatabase dbFlux = null;
	private boolean isFluxActive = false;
	

	public sqliteAPI(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
	}
	@Override
	public void onCreate(SQLiteDatabase db) {
		// Creating Tables
		String s;
		s = "CREATE TABLE products (id INTEGER PRIMARY KEY AUTOINCREMENT, produs varchar(255), categorie varchar(255), stoc INTEGER, pret INTEGER);";
		db.execSQL(s);
		s = "CREATE TABLE buy (id INTEGER PRIMARY KEY AUTOINCREMENT, idProdus varchar(255), cantitate INTEGER);";
		db.execSQL(s);	
		
	}
	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		// Drop older table if existed
		db.execSQL("DROP TABLE IF EXISTS products");
		onCreate(db);  // Create tables again		   
		db.execSQL("DROP TABLE IF EXISTS buy");
		onCreate(db);  // Create tables again

	}
	public void dbInsertItem(String dbTable, String cmds){
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;
		String dbColumnsName = "";
		String dbValues = "";

		StringBuffer tempCmds = new StringBuffer(cmds);
		{
			tempCmds.append(",");
			while(tempCmds.toString().equals("")==false){//
				if((dbColumnsName!="")&&(dbValues!="")){
					dbColumnsName += ",";
					dbValues += ",";
				}
				dbColumnsName += tempCmds.substring(0,tempCmds.indexOf("=")).toString();
				tempCmds.delete(0,tempCmds.indexOf("=")+1);
				dbValues += tempCmds.substring(0,tempCmds.indexOf(",")).toString();
				tempCmds.delete(0,tempCmds.indexOf(",")+1);
			}
		}
		
		String s = "INSERT INTO "+dbTable+" (" +dbColumnsName+ ") VALUES(" +dbValues+ ");";
		db.execSQL(s);
		if(isFluxActive==false)
			db.close(); 
	}
	public void dbUpdateItem(String dbTable, String id, String cmds){
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;
		String s = "UPDATE "+dbTable+" SET "+cmds+" WHERE id = '"+id+"';";
		db.execSQL(s);
		if(isFluxActive==false)
			db.close(); 
	}
	public void dbDeleteItem(String dbTable, String deleteItemID){
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;
		String whereCmds="";
		if(deleteItemID!=""){
			deleteItemID=deleteItemID.replaceAll(",", " AND ");
			whereCmds=" WHERE "+deleteItemID;
		}
		String s = "DELETE FROM "+dbTable+whereCmds+";";
		db.execSQL(s);
		if(isFluxActive==false)
			db.close();  
	}
	public ArrayList<ArrayList<String>> dbGetItem(String dbTable, String selCol, String cmds){
		ArrayList<ArrayList<String>> myList = new ArrayList<ArrayList<String>>();
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;	
		Cursor cursor = null;
		 String whereCMD = "";
		 if(!cmds.equals("")){
			 cmds= cmds.replaceAll(",", " AND ");
			 whereCMD = " WHERE " + cmds;
		 }
		 String s = "SELECT "+selCol+" FROM "+dbTable+whereCMD+";";
		 cursor = db.rawQuery(s,null);
		
		 if (cursor != null){
			 cursor.moveToFirst();
			 if(cursor.getCount()!=0){
				 for(int i=0;i<cursor.getColumnCount();i++){
					 myList.add(new ArrayList<String>());
					 myList.get(i).add(new String(cursor.getString(i)));
				 }
	
				 while(cursor.moveToNext()){
					 for(int i=0;i<cursor.getColumnCount();i++){
						 myList.get(i).add(new String(cursor.getString(i)));
					 }
				 }
			 }
		 }
		if(isFluxActive==false)
			db.close(); 
		return myList;
	}
	public int dbLastRowId(String dbTable){
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;
		Cursor cursor = null;
		int returned = 0;
		String s = "SELECT id FROM "+dbTable+" ORDER BY id DESC LIMIT 1";
		cursor = db.rawQuery(s,null);
		
		if (cursor != null){
			 cursor.moveToFirst();
			 if(cursor.getCount()!=0)
				 returned =  cursor.getInt(0);
		}
		if(isFluxActive==false)
			db.close(); 
		return returned;
	}
	public int dbGetTableCount(String dbTable){
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;
		Cursor cursor = null;	
		int returned = 0;
		String s = "SELECT COUNT(id) FROM "+dbTable+";";
		cursor = db.rawQuery(s,null);
		
		
		if (cursor != null){
			 cursor.moveToFirst();
			 if(cursor.getCount()!=0)
				 returned = cursor.getInt(0);
		}
		if(isFluxActive==false)
			db.close(); 
		return returned;
	}
	public void dbExecuteQuery(String query){
		SQLiteDatabase db;
		if(isFluxActive==false)
			db = this.getWritableDatabase();
		else
			db = dbFlux;
		db.execSQL(query);
		if(isFluxActive==false)
			db.close(); 
	}
	public void beginInsertFlux(){
		isFluxActive=true;
		dbFlux = this.getWritableDatabase();
		dbFlux.execSQL("BEGIN;");
	}
	public void endInsertFlux(){
		isFluxActive=false;
		dbFlux.execSQL("END;");
		dbFlux.close();
	}
	
}
