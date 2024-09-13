#pragma once
#include "stdafx.h"
#include "Listener.h"

class socketServer
{

enum StringValue
{
	evNotDefined,kick,kickall,msg,msgto,status,stop,start,help,clear,about,evEnd
};

public:
	socketServer(void);
	virtual ~socketServer(void);

public:
	void InitializeHelpStrings();// Intialization strings
	void HelpCommands();
	void StartStopServer(bool state);
	void mainCommandsConsole();


public:
	CListener myListener;
private:
	string command,operation,userName,group;
	map<string, StringValue> mapStrings;// Map to associate the strings with the enum values
};

