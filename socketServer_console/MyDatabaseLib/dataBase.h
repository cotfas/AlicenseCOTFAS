#pragma once
#include "header.h"



class dataBase
{
public:
	dataBase(void);
	virtual ~dataBase(void);

private:
	Driver *driver;
	Connection *connection;
	bool isDBConnected;

	ObjMutex objMutexInsertItem;
	ObjMutex objMutexUpdateItem;
	ObjMutex objMutexDeleteItem;
	ObjMutex objMutexGetItem;
	ObjMutex objMutexGetItemBetween;
	ObjMutex objMutexGetTableCount;
	ObjMutex objMutexGetLastRowId;
	ObjMutex objMutexExecuteQuery;
	ObjMutex objMutexExecuteStmt;
	ObjMutex objMutexstrReplaceAll;

public:
	bool dbInitialiseConnection(map<string,string> dbVectorValues);
	bool dbCreateTables();

	bool dbInsertItem(string dbTable, string cmds);
	bool dbUpdateItem(string dbTable, string id, string cmds);
	bool dbDeleteItem(string dbTable, string deleteItemCmds);
	multimap<string,string> dbGetItem(string dbTable, string selCol, string cmds);
	multimap<string,string> dbGetItemBetween(string dbTable, string selCol, string startNumber, string endNumber);
	string dbGetTableCount(string dbTable);
	int dbGetLastRowId(string dbTable);
	bool dbExecuteQuery(string query);

	void dbClose();


private:
	PreparedStatement *executeStmt(string s, bool isReturning);
	void strReplaceAll(std::string& target, const std::string oldstr, const std::string newstr);
	void showSQLException(SQLException &e);
};

