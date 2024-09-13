package com.android.socketclient;

import java.util.ArrayList;

public class commandsList {

	private ArrayList<ArrayList<String>> myList = new ArrayList<ArrayList<String>>();
	private int prod=0, cat=1, stoc=2, pret=3, cant=4, contor=0;

	public commandsList(){
		// add 2nd dimension
		for ( int i = 0; i < 5; i++ ){
			myList.add(new ArrayList<String>());
		}
	}

	public void addItem(String prodT, String catT, String stocT, String pretT, String cantT){
        myList.get(prod).add(new String(prodT));
        myList.get(cat).add(new String(catT));
        myList.get(stoc).add(new String(stocT));
        myList.get(pret).add(new String(pretT));
        myList.get(cant).add(new String(cantT));
		contor++;
	}
	public void removeItem(int position){
		if(position!=-1){
			myList.get(prod).remove(position);
			myList.get(cat).remove(position);
			myList.get(stoc).remove(position);
			myList.get(pret).remove(position);
			myList.get(cant).remove(position);
			contor--;
		}
	}
	public void removeAllItems(){
		myList.get(prod).clear();
		myList.get(cat).clear();
		myList.get(stoc).clear();
		myList.get(pret).clear();
		myList.get(cant).clear();
		contor=0;
	}
	public String getItem(String who, int position){
		if(who.equals("prod"))
			return myList.get(prod).get(position);
		else if(who.equals("cat"))
			return myList.get(cat).get(position);
		else if(who.equals("stoc"))
			return myList.get(stoc).get(position);
		else if(who.equals("pret"))
			return myList.get(pret).get(position);
		else if(who.equals("cant"))
			return myList.get(cant).get(position);
		return null;
	}
	public int getSize(){
		return contor;
	}

	public int findItemInList(String who, String searchValue){
		String currentValue=null;
		for(int i=0;i<contor;i++){
			if(who.equals("prod")){
				currentValue = myList.get(prod).get(i);
			}else if(who.equals("cat")){
				currentValue = myList.get(cat).get(i);
			}else if(who.equals("stoc")){
				currentValue = myList.get(stoc).get(i);
			}else if(who.equals("pret")){
				currentValue = myList.get(pret).get(i);
			}else if(who.equals("cant")){
				currentValue = myList.get(cant).get(i);
			}
			
			if(currentValue.equals(searchValue)==true){
				return i;
			}
		}
		return -1;
	}
}
