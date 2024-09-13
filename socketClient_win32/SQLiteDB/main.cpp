#include <iostream>
#include "sqliteAPI.h"
using namespace std;


int main(){

	sqliteAPI sql;

	//sql.dbInsertItem("products","produs='HDD',categorie='PC',stoc='10',pret='100'");

	//sql.dbUpdateItem("products","1","produs='HDD1',categorie='PC2'");

	//sql.dbDeleteItem("products","1");

	//cout << sql.dbGetTableCount("products");


	vector<vector<string> > result;
	result = sql.dbGetItem("products","produs,categorie","");

	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		cout << "Values: ";
		for(int i=0; i<row.size(); i++)
		cout << row.at(i) << " ";

		cout << endl;
	}




//	dbInsertItem(string dbTable, string cmds);
//	dbUpdateItem(string dbTable, string id, string cmds);
//	dbDeleteItem(string dbTable, string deleteItemID);
//	dbGetItem(string dbTable, string selCol, string cmds);
//	dbGetTableCount(string dbTable);




	/*
	db->query("INSERT INTO products (produs, categorie, stoc, pret) VALUES('HDD', 'PC', 10, 100);");
	db->query("INSERT INTO products (produs, categorie, stoc, pret) VALUES('COMPUTER', 'PC', 10, 100);");
	vector<vector<string> > result;
	result = db->query("SELECT * FROM products WHERE produs = 'HDD';");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		cout << "Values: ";
		for(int i=0; i<row.size(); i++)
		cout << row.at(i) << " ";

		cout << endl;
	}

	// update
	result = db->query("UPDATE products SET produs = 'HDD2UPD' WHERE produs = 'HDD';");
	result = db->query("DELETE FROM products WHERE id = '1';");


	
	cout << endl;
	
	result = db->query("SELECT * FROM products;");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it)
	{
		vector<string> row = *it;
		cout << "Values: " << row.at(0) << " " << row.at(1) << " " << row.at(2) << " " << row.at(3) << endl;
	}

	cout << endl;

	*/




	



return 0;
}