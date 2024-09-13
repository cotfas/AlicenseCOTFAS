#pragma once
#include <string>
#include "..\TCPSocket\ObjThread.h"

#include "socketClientDlg.h" // legatura inapoi


#include <windows.h>


class TCPSocket;
class CConex : public ObjThread
{
public:
	CConex(CsocketClientDlg * pdlg);
	virtual ~CConex(void);
protected:
	virtual void run();




public:
	CsocketClientDlg * pdlgclass; // legatura inapoi
	TCPSocket remoteSockToConex; // mut aici
	CConex* newConex;
	bool isNewConexInit;
	bool clientDisconnected;
	bool isClientConnected;
	SYSTEMTIME connectedAt;
	LARGE_INTEGER start1,stop1,frequency;

public:
	string slash;
	string userName;
	string group;
	string status;
	string toUserName;
	string toGroup;
	string remoteIPPORTConnected;
	string clientTimeConnected;
	string typeIMEI;
	string osVersion;
	string name;
	string confirmed;
	string cantitate;
	string id;
	string oldProdus;
	string oldCategorie;
	string produs;
	string categorie;
	string stoc;
	string pret;
	string addedtime;



public:
	virtual void connectToServer();
	virtual void protocol(string bufferTemp);
	virtual void inchideConex(int status);
	virtual void sendMessage(string Message);



};