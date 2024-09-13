#pragma once
#include "dataBase.h"


class sqliteAPI
{
public:
	sqliteAPI(void);
	virtual ~sqliteAPI(void);


public:
	void dbInitialise();



	virtual void dbInsertItem(string dbTable, string cmds);
	virtual void dbUpdateItem(string dbTable, string id, string cmds);
	virtual void dbDeleteItem(string dbTable, string deleteItemID);
	virtual vector<vector<string> > dbGetItem(string dbTable, string selCol, string cmds);
	virtual string dbLastRowId(string dbTable);
	virtual int dbGetTableCount(string dbTable);
	virtual void dbExecuteQuery(string query);

	virtual void dbClose();

private:
	virtual void replaceStr(std::string& target, const std::string oldstr, const std::string newstr);



private:
	Database *db;
};

