// includes
#include "stdafx.h"
#include "OneInstance.h"
#include "socketServer.h"


// variables
CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{9DA0BEED-7248-450a-B27C-C0409BDC377D}"));// The one and only CLimitSingleInstance object.
xmlParser xml;
socketServer menuSocket;

	

void main(){
	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
		exit( 0 );

	SetConsoleTitle(TEXT("SERVER 2.0"));
	
	cout << "--------------------------------SERVER 2.0------------------------------"<<endl;

	if(startXML()==true) {	// parse xml
		while (menuSocket.myListener.dbHandler.dbInitialiseConnection( xml.getXMLValues() )==false){ // database
			cout << "Eroare la conectare baza de date! Verificati setarile de conectare MySQL!" << endl;
			cin.get();
		}
		cout << "Conectare la baza de date cu succes!"<<endl;
		menuSocket.myListener.dbHandler.dbCreateTables();

		if(xml.getXMLValues()["autoListening"]!="true"){
			cout << "----------------------Press any key to start listening------------------"<<endl;
			cin.get();
		}

		/*
		cout << "\nSe adauga!";
		char buff[6]={0};
		int count=0;
		string cmds,value,prod = "prod";
		for(int i=0;i<10000;i++){
			itoa(count++,buff,10);
			value=buff;
			cmds = "produs='"+prod+value+"',categorie='categorie',stoc='stoc',pret='pret'";
			menuSocket.myListener.dbHandler.dbInsertItem("products",cmds);
		}
		cout << "\nAdaugat!";
		*/

	

		// start server
		menuSocket.myListener.port=atoi(xml.getXMLValues()["serverPORT"].c_str());
		menuSocket.StartStopServer(true);//Start listen server
		menuSocket.InitializeHelpStrings();
		menuSocket.mainCommandsConsole();


		
	

	}
}

bool startXML(){
	const char* path = "configServer.xml";
	if(xml.loadFile(path)==false){
		map<string,string> xmlValues;
		string input;
		cout << "Introduceti ip bazei de date: ";
		cin >> input;
		xmlValues.insert ( pair<string,string>( "dbIP", input) );
		cout << "Introduceti portul bazei de date: ";
		cin >> input;
		xmlValues.insert ( pair<string,string>( "dbPORT", input) );
		cout << "Introduceti userul bazei de date: ";
		cin >> input;
		xmlValues.insert ( pair<string,string>( "dbUSER", input) );
		cout << "Introduceti parola bazei de date: ";
		cin >> input;
		xmlValues.insert ( pair<string,string>( "dbPASS", input) );
		cout << "Introduceti numele bazei de date: ";
		cin >> input;
		xmlValues.insert ( pair<string,string>( "dbNAME", input) );
		cout << "Introduceti portul de comunicare al serverului: ";
		cin >> input;
		xmlValues.insert ( pair<string,string>( "serverPORT", input) );

		xml.setXMLValues(xmlValues);
		xml.writeToXML(path);
	}
	cout << "CONFIG FILE PARSED OKEY!!!"<<endl;

	return true;
}

