#include "dataBase.h"


dataBase::dataBase(void)
{	

}
dataBase::~dataBase(void)
{
	dbClose();
}
bool dataBase::dbInitialiseConnection(map<string,string> dbVectorValues){
	try {
		driver = get_driver_instance();
		connection = driver->connect("tcp://"+dbVectorValues["dbIP"]+":"+dbVectorValues["dbPORT"], dbVectorValues["dbUSER"], dbVectorValues["dbPASS"]);
		connection->setSchema(dbVectorValues["dbNAME"]); /* Connect to the MySQL test database */
		isDBConnected = true;
		return true;
	}catch (SQLException &e) {
		showSQLException(e);
		isDBConnected = false;
		return false;
	}
}
bool dataBase::dbCreateTables(){
	if(isDBConnected==false){
		return false;
	}
	try {	
		string s;
		s = "CREATE TABLE IF NOT EXISTS `accounts` (" 
			"`id` int(255) NOT NULL AUTO_INCREMENT," 
			"`name` varchar(255) CHARACTER SET ascii NOT NULL," 
			"`type` varchar(255) CHARACTER SET ascii NOT NULL," 
			"`user` varchar(255) CHARACTER SET ascii NOT NULL," 
			"`password` varchar(255) CHARACTER SET ascii NOT NULL," 
			"`auth` varchar(255) CHARACTER SET ascii NOT NULL,"
			"`confirmed` int(1) NOT NULL,"
			"PRIMARY KEY (`id`)" 
			") ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1";
		executeStmt(s,false);

		s = "CREATE TABLE IF NOT EXISTS `products` ("
			"`id` int(255) NOT NULL AUTO_INCREMENT,"
			"`produs` varchar(255) CHARACTER SET ascii NOT NULL,"
			"`categorie` varchar(255) CHARACTER SET ascii NOT NULL,"
			"`stoc` int(255) NOT NULL,"
			"`pret` varchar(255) CHARACTER SET ascii NOT NULL,"
			"PRIMARY KEY (`id`)"
			") ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1";
		executeStmt(s,false);

		s = "CREATE TABLE IF NOT EXISTS `comands` ("
			"`id` int(255) NOT NULL AUTO_INCREMENT,"
			"`idUser` int(255) NOT NULL,"
			"`idProdus` int(255) NOT NULL,"
			"`cantitate` int(255) NOT NULL,"
			"`confirmed` int(1) NOT NULL,"
			"`addedtime` varchar(255) CHARACTER SET ascii NOT NULL,"
			"PRIMARY KEY (`id`)"
			") ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1";
		executeStmt(s,false);

		s = "CREATE TABLE IF NOT EXISTS `offlinesync` ("
			"`id` int(255) NOT NULL AUTO_INCREMENT,"
			"`idUser` int(255) NOT NULL,"
			"`idProdus` int(255) NOT NULL,"
			"PRIMARY KEY (`id`)"
			") ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1";
		executeStmt(s,false);
		return true;
	}catch (SQLException &e) {
		showSQLException(e);
		return false;
	}
}
bool dataBase::dbInsertItem(string dbTable, string cmds){
	objMutexInsertItem.lock();
	if(isDBConnected==false){
		objMutexInsertItem.unlock();
		return false;
	}
	try {
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
		string s = "INSERT INTO " +dbTable+ "(" +dbColumnsName+ ") VALUES (" +dbValues+ ")";
		executeStmt(s,false);
		objMutexInsertItem.unlock();
		return true;
	}catch (SQLException &e) {
		showSQLException(e);
		objMutexInsertItem.unlock();
		return false;
	}
}
bool dataBase::dbUpdateItem(string dbTable, string id, string cmds){
	objMutexUpdateItem.lock();
	if(isDBConnected==false){
		objMutexUpdateItem.unlock();
		return false;
	}
	try {
		string s = "UPDATE `" +dbTable+ "` SET " +cmds+ " WHERE `id` = '" +id+ "'";
		executeStmt(s,false);
		objMutexUpdateItem.unlock();
		return true;
	}catch (SQLException &e) {
		showSQLException(e);
		objMutexUpdateItem.unlock();
		return false;
	}
}
bool dataBase::dbDeleteItem(string dbTable, string deleteItemCmds){
	objMutexDeleteItem.lock();
	if(isDBConnected==false){
		objMutexDeleteItem.unlock();
		return false;
	}
	try {
		string whereCMD = "";
		if(deleteItemCmds!=""){
			strReplaceAll(deleteItemCmds, ",", " AND ");
			whereCMD = " WHERE " + deleteItemCmds;
		}
		string s = "DELETE FROM `" +dbTable+ "`"+whereCMD;
		executeStmt(s,false);
		objMutexDeleteItem.unlock();
		return true;
	}catch (SQLException &e) {
		showSQLException(e);
		objMutexDeleteItem.unlock();
		return false;
	}
}
multimap<string,string> dataBase::dbGetItem(string dbTable, string selCol, string cmds){
	objMutexGetItem.lock();
	multimap<string,string> returned;
	try {
		string whereCMD = "";
		if(cmds!=""){
			strReplaceAll(cmds, ",", " AND ");
			whereCMD = " WHERE " + cmds;
		}
		string s = "SELECT " +selCol+ " FROM `" +dbTable+ "`"+whereCMD;
		PreparedStatement *pstmt = executeStmt(s,true);
		ResultSet *res = pstmt->executeQuery();

		// getting the data
		while (res->next()) {
			string tempVal;
			string tempCol = selCol;
			tempCol.append(",");
			while(tempCol.empty()==false){
				tempVal = tempCol.substr(0,tempCol.find(","));
				tempCol.erase(0,tempCol.find(",")+1);

				returned.insert ( pair<string,string>(tempVal, res->getString( tempVal )));
				//cout << tempVal +" = " << res->getString(tempVal) << endl; ////////////////
			}
		}
	
		delete res,pstmt;
	}catch (SQLException &e) {
		showSQLException(e);
	}
	objMutexGetItem.unlock();
	return returned;
}
multimap<string,string> dataBase::dbGetItemBetween(string dbTable, string selCol, string startNumber, string endNumber){
	objMutexGetItemBetween.lock();
	multimap<string,string> returned;
	try {
		string whereCMD = " WHERE "+selCol+" BETWEEN '"+startNumber+"' AND '"+endNumber+"'";
		string s = "SELECT " +selCol+ " FROM `" +dbTable+ "`"+whereCMD;

		PreparedStatement *pstmt = executeStmt(s,true);
		ResultSet *res = pstmt->executeQuery();

		// getting the data
		while (res->next()) {
			string tempVal;
			string tempCol = selCol;
			tempCol.append(",");
			while(tempCol.empty()==false){
				tempVal = tempCol.substr(0,tempCol.find(","));
				tempCol.erase(0,tempCol.find(",")+1);

				returned.insert ( pair<string,string>(tempVal, res->getString( tempVal )));
				//cout << tempVal +" = " << res->getString(tempVal) << endl; ////////////////
			}
		}
	
		delete res,pstmt;
	}catch (SQLException &e) {
		showSQLException(e);
	}
	objMutexGetItemBetween.unlock();
	return returned;
}
string dataBase::dbGetTableCount(string dbTable){
	objMutexGetTableCount.lock();
	string returned = "-1";
	try {
		string s = "SELECT COUNT('id') AS row_count FROM " + dbTable;
		PreparedStatement *pstmt = executeStmt(s,true);
		ResultSet *res = pstmt->executeQuery();

		// getting the data
		while (res->next()) {
			returned = res->getString(1);
		}
		delete res,pstmt;
	}catch (SQLException &e) {
		showSQLException(e);
	}
	objMutexGetTableCount.unlock();
	return returned;
}
int dataBase::dbGetLastRowId(string dbTable){
	objMutexGetLastRowId.lock();
	int returned = 0;
	try {
		string s = "SELECT id FROM "+dbTable+" ORDER BY id DESC LIMIT 1";
		PreparedStatement *pstmt = executeStmt(s,true);
		ResultSet *res = pstmt->executeQuery();

		// getting the data
		while (res->next()) {
			returned = res->getInt(1);
		}
		delete res,pstmt;
	}catch (SQLException &e) {
		showSQLException(e);
	}
	objMutexGetLastRowId.unlock();
	return returned;
}
bool dataBase::dbExecuteQuery(string query){
	objMutexExecuteQuery.lock();
	if(isDBConnected==false){
		objMutexExecuteQuery.unlock();
		return false;
	}
	try {
		executeStmt(query,false);
		objMutexExecuteQuery.unlock();
		return true;
	}catch (SQLException &e) {
		showSQLException(e);
		objMutexExecuteQuery.unlock();
		return false;
	}
}
PreparedStatement *dataBase::executeStmt(string s, bool isReturning){
	objMutexExecuteStmt.lock();
	if(isReturning==false){
		Statement *stmt = connection->createStatement();
		stmt->execute(s);
		delete stmt;
		objMutexExecuteStmt.unlock();
	}else{
		objMutexExecuteStmt.unlock();
		return connection->prepareStatement(s);
	}
	return NULL;
}
void dataBase::dbClose(){
	if(isDBConnected==true)
		connection->close();
	delete &driver,&connection;
}
void dataBase::strReplaceAll(std::string& target, const std::string oldstr, const std::string newstr) {
	objMutexstrReplaceAll.lock();
	unsigned int x;
	while(x = target.find(oldstr), x != std::string::npos) {
		target.erase(x, oldstr.length());
		target.insert(x, newstr);
	}
	objMutexstrReplaceAll.unlock();
}
void dataBase::showSQLException(SQLException &e){
	cout << "# ERR: SQLException in " << __FILE__;
	cout << "(" << __FUNCTION__ << ") on line "     << __LINE__ << endl;
	cout << "# ERR: " << e.what();
	cout << " (MySQL error code: " << e.getErrorCode();
	cout << ", SQLState: " << e.getSQLState() <<     " )" << endl;
}
