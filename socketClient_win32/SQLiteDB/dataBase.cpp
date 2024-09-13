#include "dataBase.h"

Database::Database(char* filename)
{
	database = NULL;
	open(filename);
}

Database::~Database()
{
	close();
}

bool Database::open(char* filename){
	if(sqlite3_open(filename, &database) == SQLITE_OK)
		return true;
		
	return false;   
}

vector<vector<string> > Database::query(string query){
	sqlite3_stmt *statement;
	vector<vector<string> > results;

	if(sqlite3_prepare_v2(database, query.c_str(), -1, &statement, 0) == SQLITE_OK){
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while(true){
			result = sqlite3_step(statement);
			
			if(result == SQLITE_ROW){
				vector<string> values;
				for(int col = 0; col < cols; col++){
					string s;
					char *ptr = (char*)sqlite3_column_text(statement, col);
					if(ptr) s = ptr;


					values.push_back(s);
				}
				results.push_back(values);
			}else{
				break;   
			}
		}
	   
		sqlite3_finalize(statement);
	}
	
	string error = sqlite3_errmsg(database);
	if(error != "not an error") cout << query << " " << error << endl;
	
	return results;  
}

bool Database::executeStmt(string rStmt)
{
	if( SQLITE_OK != sqlite3_get_table(database, rStmt.c_str(), NULL, 0, 0, NULL))
	{
		return true;
	}
	return true;
}

void Database::close(){
	sqlite3_close(database);   
}
