package com.android.socketclient;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;


public class dbSettings extends SQLiteOpenHelper {
   // All Static variables
   private static final int DATABASE_VERSION = 1;   // Database Version
   private static final String DATABASE_NAME = "settings.db";   // Database Name
   private static final String TABLE = "tableName";   // Table name
   // Table Columns names
   private static final String KEY_ID = "id";
   private static final String KEY_NAME = "name";
   private static final String KEY_VALUE = "val";
   
   public dbSettings(Context context) {
       super(context, DATABASE_NAME, null, DATABASE_VERSION);
   }

   // Creating Tables
   @Override
   public void onCreate(SQLiteDatabase db) {
       String s = "CREATE TABLE " + TABLE + "("
               + KEY_ID + " INTEGER PRIMARY KEY," + KEY_NAME + " TEXT,"
               + KEY_VALUE + " TEXT" + ")";
       db.execSQL(s);
   }
   // Upgrading database
   @Override
   public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
       // Drop older table if existed
       db.execSQL("DROP TABLE IF EXISTS " + TABLE);
       // Create tables again
       onCreate(db);
   }


   
   
   // add value
   public void insert(String name, String value){
	   SQLiteDatabase db = this.getWritableDatabase();
	    
	   ContentValues values = new ContentValues();
	   values.put(KEY_NAME, name); // Name
	   values.put(KEY_VALUE, value); // Value
	  
	   db.insert(TABLE, null, values);	   // Inserting Rows
	   db.close(); // Closing database connection
   }

   // update value
   public void update(String name, String value){
	   SQLiteDatabase db = this.getWritableDatabase();
	   
	   ContentValues values = new ContentValues();
	   values.put(KEY_NAME, name);
	   values.put(KEY_VALUE, value);
	   
	   db.update(TABLE, values, KEY_NAME + " = ?", new String[] { String.valueOf(name) });  // updating row
	   db.close(); // Closing database connection
   }
   
   // get value
   public String getValue(String name){
	    SQLiteDatabase db = this.getReadableDatabase();
	    String returned = "NULL";
	    Cursor cursor = null;
	    cursor = db.query(TABLE, new String[] { KEY_ID,
	            KEY_NAME, KEY_VALUE }, KEY_NAME + "=?",
	            new String[] { String.valueOf(name) }, null, null, null, null);


	    if (cursor != null){
	    	cursor.moveToFirst();
	    	if( cursor.getCount()!=0)
	    		returned = cursor.getString(2);// 0=id 1=nume 2=val
	    }
	    db.close(); // Closing database connection
	    return returned; 
   }
   
   
   
   
   
   
   
}