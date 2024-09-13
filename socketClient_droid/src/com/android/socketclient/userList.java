package com.android.socketclient;

import java.util.ArrayList;

public class userList {

	private ArrayList<ArrayList<String>> myList = new ArrayList<ArrayList<String>>();
	private int name=0, user=1, auth=2, contor=0;

	public userList(){
		// add 2nd dimension
		for ( int i = 0; i < 3; i++ ){
			myList.add(new ArrayList<String>());
		}
	}

	public void addItem(String nameT, String userT, String authT){
        myList.get(name).add(new String(nameT));
        myList.get(user).add(new String(userT));
        myList.get(auth).add(new String(authT));
		contor++;
	}
	public void removeItem(int position){
		if(position!=-1){
			myList.get(name).remove(position);
			myList.get(user).remove(position);
			myList.get(auth).remove(position);
			contor--;
		}
	}
	public void removeAllItems(){
		myList.get(name).clear();
		myList.get(user).clear();
		myList.get(auth).clear();
		contor=0;
	}
	public String getItem(String who, int position){
		if(who.equals("name"))
			return myList.get(name).get(position);
		else if(who.equals("user"))
			return myList.get(user).get(position);
		else if(who.equals("auth"))
			return myList.get(auth).get(position);
		return null;
	}
	public int getSize(){
		return contor;
	}

	public int findItemInList(String who, String searchValue){
		String currentValue=null;
		for(int i=0;i<contor;i++){
			if(who.equals("name")){
				currentValue = myList.get(name).get(i);
			}else if(who.equals("user")){
				currentValue = myList.get(user).get(i);
			}else if(who.equals("auth")){
				currentValue = myList.get(auth).get(i);
			}
			
			if(currentValue.equals(searchValue)==true){
				return i;
			}
		}
		return -1;
	}
}
