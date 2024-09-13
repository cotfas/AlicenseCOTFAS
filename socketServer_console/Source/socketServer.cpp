#include "socketServer.h"


socketServer::socketServer(void)
{
}
socketServer::~socketServer(void)
{
}


void socketServer::mainCommandsConsole(){

	while(1){
		cout << endl;
		command.clear();
		operation.clear();

		getline(cin,command);

		if(!command.empty()){
			operation.assign(command,0,command.find(' '));
			command.erase(0,command.find(' ')+1);


			switch (mapStrings[operation]){
		
			case kick:
				{
				userName.assign(command,0,command.find(' '));
				command.erase(0,command.find(' ')+1);
				group.assign(command,0,command.find(' '));
				command.erase(0,command.find(' ')+1);
				myListener.FunctieKickUser(userName,group);
				break;
				}
			case kickall:
				{
				myListener.FunctieKickUser("","");
				break;
				}
			case msgto:
				{
				userName.assign(command,0,command.find(' '));
				command.erase(0,command.find(' ')+1);
				group.assign(command,0,command.find(' '));
				if(command.find(' ')!=-1)
					command.erase(0,command.find(' ')+1);
				else
					command.clear();

				myListener.FunctieSendMessage("SEND_MESSAGETO",userName,group,"SERVER","SERVER",command);
				break;
				}
			case msg:
				{
				myListener.FunctieSendMessage("SEND_MESSAGETOALL","","","SERVER","SERVER",command);
				break;
				}
			case status:
				{
				myListener.AfisareClienti();
				break;
				}

			case stop:
				{
				StartStopServer(false);
				break;
				}
			case start:
				{
				StartStopServer(true);
				break;
				}		
			case help:
				{
				HelpCommands();
				break;
				}
			case clear:
				{
				system("cls");
				break;
				}	
			case about:
				{
				cout << "\nAplicatie proiectata pentru gestiunea produselor online, autor Cotfas Vlad."<<endl;
				break;
				}
			case evEnd:
				{
				exit( 0 );
				break;
				}
				
				
				default:
					break;
			}
		}
	}


}

void socketServer::InitializeHelpStrings(){
	mapStrings["kick"] = kick;
	mapStrings["kickall"] = kickall;
	mapStrings["msg"] = msg;
	mapStrings["msgto"] = msgto;
	mapStrings["status"] = status;
	mapStrings["exit"] = evEnd;
	mapStrings["stop"] = stop;
	mapStrings["start"] = start;
	mapStrings["help"] = help;
	mapStrings["clear"] = clear;
	mapStrings["about"] = about;
}

void socketServer::HelpCommands(){
	cout << "---------------------------------SERVER------------------------------"<<endl;
	cout << "############################Help commands############################"<<endl;
	cout << "\n\tkick <user> <group>          // deconectare utilizator"<<endl;
	cout << "\tkickall                      // deconectare toti utilizatori"<<endl;
	cout << "\tmsg mesaj                    // mesaj catre toti utilizatori conectati"<<endl;
	cout << "\tmsgto <user> <group> <mesaj> // mesaj un utilizator specificat"<<endl;
	cout << "\tstatus                       // afisare utilizatori conectati pe server"<<endl;
	cout << "\tstop                         // inchidere port de ascultare clienti"<<endl;
	cout << "\tstart                        // deschide port pentru ascultare clienti"<<endl;
	cout << "\thelp                         // afisare meniu help"<<endl;
	cout << "\texit                         // inchidere aplicatie"<<endl;
	cout << "\tclear                        // stergere ecran"<<endl;
	cout << "\tabout                        // afisare informatii"<<endl;
	cout << "---------------------------------------------------------------------"<<endl;
}


void socketServer::StartStopServer(bool state){
	if(state==true){
		if(myListener.serverSockListener.isSocketInit()==false){
			myListener.serverSockListener.initSocketLibrary();
			if ( false == myListener.serverSockListener.makeserverSockListener(myListener.port) ){
				myListener.serverSockListener.closeSocketLibrary();
				cout << "Eroare la creare server socket..." << endl;
			}else{
				myListener.start();
				cout << "Serverul a fost pornit cu portul: " << myListener.port << endl;
			}
		}else{
			cout << "Serverul este pornit deja..."<< endl;
		}
	}else if(state==false){
		if(myListener.serverSockListener.isSocketInit()==true){
			myListener.FunctieKickUser("","");
			myListener.serverSockListener.setStatus(-1);
			myListener.stop(ObjThread::INFINIT_WAIT);
			myListener.serverSockListener.closeSocketLibrary();
			cout << "Serverul a fost oprit..." << endl;
		}else{
			cout << "Serverul este oprit deja..."<< endl;
		}
	}
}