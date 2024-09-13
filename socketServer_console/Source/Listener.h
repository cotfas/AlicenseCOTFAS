#pragma once
#include "stdafx.h"
#include "ObjThread.h"
#include "TCPSocket.h"
#include "ObjMutex.h"

class CConex;
class TCPSocket;
class CListener : public ObjThread
{	
public:
	CListener(void);
	virtual ~CListener(void);

protected:
	virtual void run();



public:
	vector< CConex* > vectorConex;
	CConex* socketListenerToConex;
	TCPSocket serverSockListener;
	dataBase dbHandler;
	string slash;
	int port;

private:
	ObjMutex objMutexRemove;
	ObjMutex objMutexSendMsg;
	ObjMutex objMutexKickUser;
	ObjMutex objMutexSendClient;
	ObjMutex objMutexConexSendMsg;

	ObjMutex objMutexFunctieAutentificareDB;
	ObjMutex objMutexFunctieInregistrareUserDB;
	ObjMutex objMutexFunctieSendAllUsersDB;
	ObjMutex objMutexFunctieDeleteUserDB;
	ObjMutex objMutexFunctieUpdateUserDB;
	ObjMutex objMutexFunctieSendAllProductsDB;
	ObjMutex objMutexFunctieAddNewProductsDB;
	ObjMutex objMutexFunctieDeleteProductDB;
	ObjMutex objMutexFunctieUpdateProductDB;
	ObjMutex objMutexFunctieNewBuyDB;
	ObjMutex objMutexFunctieSendAllComandsDB;
	ObjMutex objMutexFunctieDeleteComandDB;
	ObjMutex objMutexFunctieUpdateComandDB;

	ObjMutex objMutexFunctieVerificareUserOnline;
	ObjMutex objMutexDetectClientDuplicatRENAME;
	ObjMutex objMutexDetectClientDuplicat;

	ObjMutex objMutexVector;


public:
	virtual multimap<string,string> FunctieAutentificareDB(CConex* remote, string passwordT);
	virtual bool FunctieInregistrareUserDB(string thisUserName, string thisGroup, string typeIMEI, string password, string name, string confirmed);
	virtual void FunctieSendAllUsersDB(CConex* remote);
	virtual void FunctieDeleteUserDB(CConex* remote);
	virtual void FunctieUpdateUserDB(CConex* remote, bool isEditing);
	virtual bool FunctieSendAllProductsDB(CConex* remote);
	virtual void FunctieAddNewProductsDB(CConex* remote);
	virtual void FunctieDeleteProductDB(CConex* remote);
	virtual void FunctieUpdateProductDB(CConex* remote);
	virtual void FunctieNewBuyDB(CConex* remote);
	virtual void FunctieSendAllComandsDB(CConex* remote);
	virtual void FunctieDeleteComandDB(CConex* remote);
	virtual void FunctieUpdateComandDB(CConex* remote);






	virtual void FunctieAddClientToVector(CConex* conexVector);
	virtual void FunctieEliberareRemoteSocket(CConex * remote);
	virtual void FunctieSendMessage(string Status, string userName, string group, string toUserName, string toGroup, string Message);
	virtual void FunctieSendConnectedDisconnectedClients(CConex * remote, bool state);
	virtual void FunctieKickUser(string Nick, string group);
	virtual bool FunctieVerificareUserOnline(string userNameT, string groupT);
	int DetectClientDuplicatRENAME(string Nick, string group);
	bool DetectClientDuplicat(string userName);
	void AfisareClienti();



};

