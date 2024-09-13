#include "sqliteAPI.h"


sqliteAPI::sqliteAPI(void)
{
	dbInitialise();
}
sqliteAPI::~sqliteAPI(void)
{
	dbClose();
}


void sqliteAPI::dbInitialise(){
	string s;
	db = new Database("Database.db");
	
	s = "CREATE TABLE products (id INTEGER PRIMARY KEY AUTOINCREMENT, produs varchar(255), categorie varchar(255), stoc INTEGER, pret INTEGER);";
	db->query(s);

	s = "CREATE TABLE buy (id INTEGER PRIMARY KEY AUTOINCREMENT, idProdus varchar(255), cantitate INTEGER);";
	db->query(s);
}

void sqliteAPI::dbInsertItem(string dbTable, string cmds){
	string dbColumnsName;
	string dbValues;
	{
		cmds.append(",");
		while(cmds.empty()==false){
			if((dbColumnsName!="")&&(dbValues!="")){
				dbColumnsName += ",";
				dbValues += ",";
			}
			dbColumnsName += cmds.substr(0,cmds.find("="));
			cmds.erase(0,cmds.find("=")+1);
			dbValues += cmds.substr(0,cmds.find(","));
			cmds.erase(0,cmds.find(",")+1);
		}
	}
	string s = "INSERT INTO "+dbTable+" (" +dbColumnsName+ ") VALUES(" +dbValues+ ");";
	db->query(s);
}
void sqliteAPI::dbUpdateItem(string dbTable, string id, string cmds){
	string s = "UPDATE "+dbTable+" SET "+cmds+" WHERE id = '"+id+"';";
	db->query(s);
}
void sqliteAPI::dbDeleteItem(string dbTable, string deleteItemID){
	string whereCmds="";
	if(deleteItemID!=""){
		whereCmds=" WHERE id = '"+deleteItemID+"'";
	}
	string s = "DELETE FROM "+dbTable+whereCmds+";";
	db->query(s);
}
vector<vector<string> > sqliteAPI::dbGetItem(string dbTable, string selCol, string cmds){
	string whereCMD = "";
	if(cmds!=""){
		replaceStr(cmds, ",", " AND ");
		whereCMD = " WHERE " + cmds;
	}

	string s = "SELECT "+selCol+" FROM "+dbTable+whereCMD+";";
	return db->query(s);
}
string sqliteAPI::dbLastRowId(string dbTable){
	string returned = "0";
	string s = "SELECT id FROM "+dbTable+" ORDER BY id DESC LIMIT 1";

	vector<vector<string> > result;
	result = db->query(s);

	if(result.empty()==true){
		return returned;
	}
	vector<vector<string> >::iterator it = result.begin();
	vector<string> row = *it;
	return row.at(0);
}
int sqliteAPI::dbGetTableCount(string dbTable){
	int returned;
	vector<vector<string> > result;

	string s = "SELECT COUNT(id) FROM "+dbTable+";";
	result = db->query(s);

	vector<vector<string> >::iterator it = result.begin();
	vector<string> row = *it;

	returned = atoi(row.at(0).c_str());

	return returned;
}
void sqliteAPI::dbExecuteQuery(string query){
	db->query(query);
}
void sqliteAPI::dbClose(){
	db->close();
	delete db;
}
void sqliteAPI::replaceStr(std::string& target, const std::string oldstr, const std::string newstr) {
	unsigned int x;
	while(x = target.find(oldstr), x != std::string::npos) {
		target.erase(x, oldstr.length());
		target.insert(x, newstr);
	}
}