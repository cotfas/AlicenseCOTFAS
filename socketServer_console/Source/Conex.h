#pragma once
#include "stdafx.h"
#include "Listener.h"
#include "ObjThread.h"



class TCPSocket;
class CConex : public ObjThread 
{
public:
	CConex(CListener *Listen,TCPSocket* sock);
	virtual ~CConex(void);
protected:
	virtual void run();

	

	
private:
	CListener *backToListener;
	TCPSocket *thisRemoteSocket;

public:
	bool clientDisconnected;
	bool clientIsValid;


	string slash;

	string status;
	string userName;
	string group;

	string thisName;
	string thisUserName;
	string thisGroup;
	string remoteIPPORTConnected;
	string clientTimeConnected;
		
	string oldUserName;


	string typeIMEI;
	string osVersion;
	string name;
	string confirmed;

	string oldProdus;
	string oldCategorie;
	string produs;
	string categorie;
	string stoc;
	string pret;






public:
	virtual int protocol(string bufferTemp);
	virtual void inchideConex(int status);
	virtual void sendMessage(string Message);

	string connectedAT();
};