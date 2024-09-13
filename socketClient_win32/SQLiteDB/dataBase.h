#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "sqlite3.h"

using namespace std;

class Database
{
public:
	Database(char* filename);
	~Database();
	
	bool open(char* filename);
	vector<vector<string> > query(string query);
	bool executeStmt(string rStmt);

	void close();
	
private:
	sqlite3 *database;
};
