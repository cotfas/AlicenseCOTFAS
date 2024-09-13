#include "Conex.h"


CConex::CConex(CListener *Listen,TCPSocket* sock){
	clientDisconnected=false;
	clientIsValid=false;
	backToListener=Listen;
	thisRemoteSocket=sock;
	slash=backToListener->slash;

}
CConex::~CConex(void){

}

void CConex::run(){    
	while(isRunning()&&!isExited()){
		char szHeader[5]={0};
		int nBytesRecv = thisRemoteSocket->getData(szHeader,sizeof(szHeader));
		int nBufferSize = atoi(szHeader);

		if(clientDisconnected==true) break;
		if(nBytesRecv<5){
			inchideConex(nBytesRecv);
			break;
		}
		if(nBufferSize<=0){
			inchideConex(-2);
			break;
		}
		
		char *szBuffer = (char*)malloc(sizeof(char)*nBufferSize);
		szBuffer[nBufferSize]='\0';
		nBytesRecv = thisRemoteSocket->getData(szBuffer,nBufferSize);


		cout << "recv: " << red << szBuffer << white << endl;
		protocol(szBuffer);
	}
	backToListener->FunctieEliberareRemoteSocket(this);
}

void CConex::inchideConex(int status){

	switch(status){
	case 0:
		cout << "<server> Clientul " + thisUserName + "/" + thisGroup + " sa deconectat!"<<endl;
		break;
	case -1:
		cout << "<server> " + thisUserName + "/" + thisGroup + " O eroare a aparut la socket!"<<endl;
		break;
	case -2:
		cout << "<server> PROTOCOL NEACCEPTAT!" << endl;
		break;
	case -3:
		cout << "<server> Client " + thisUserName + "/" + thisGroup + " neautentificat!" << endl;
		break;
	case -4:
		cout << "<server> Client " + thisUserName + "/" + thisGroup + " deconectat fortat!"<<endl;
		break;
	case -5:
		cout << "<server> Client " + userName + "/" + group + " nou inregistrat!" << endl;
		break;
	case -6:
		cout << "<server> Client " + thisUserName + "/" + thisGroup + " duplicat nu sa inregistrat!" << endl;
		break;

	default:
		break;
	}

	clientDisconnected=true;
	stop();
	thisRemoteSocket->setStatus(status);
}



int CConex::protocol(string bufferTemp){
	status.assign(bufferTemp,0, bufferTemp.find(slash)); // gasire index status
	bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
	userName.assign(bufferTemp,0, bufferTemp.find(slash)); // gasire index userName
	bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
	group.assign(bufferTemp,0, bufferTemp.find(slash));	// gasire index group
	bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
	
	if(clientIsValid==false){
		if(!((status=="CLIENT_CONNECT")||(status=="CLIENT_REGISTER")||(status=="DB_GETALLPRODUCTS"))){
			inchideConex(-3);
			return 0;
		}
	}
	
	if(clientIsValid==true){
		if(thisGroup!="Admin"){
			if(	(status=="CLIENT_KICK")||
				(status=="DB_ADD")||
				(status=="DB_GETALLUSERS")||
				(status=="DB_DELETEUSER")||
				(status=="DB_UPDATEUSER")||
				(status=="DB_ADDNEWPRODUCT")||
				(status=="DB_DELETEPRODUCT")||
				(status=="DB_UPDATEPRODUCT")||
				(status=="DB_GETALLCOMANDS")||
				(status=="DB_DELETECOMAND")||
				(status=="DB_UPDATECOMAND")
		
				){
					string s = "CLIENT_NOACCESS";
					sendMessage(s);
					return 0;
			}
		}
	}

	if(status=="SEND_MESSAGETOALL"){
		cout << "(" +connectedAT()+ ") [" << userName << "/" << group << "]: " << bufferTemp << endl;
		backToListener->FunctieSendMessage(status,"","",userName,group,bufferTemp);

	}else if(status=="SEND_MESSAGETO"){
		cout << "(" +connectedAT()+ ") [" << thisUserName << "/" << thisGroup << " > " <<  userName << "/" <<  group << "]: " << bufferTemp << endl;
		backToListener->FunctieSendMessage(status,userName,group,thisUserName,thisGroup,(userName+slash+group+slash+bufferTemp));
		backToListener->FunctieSendMessage(status,thisUserName,thisGroup,thisUserName,thisGroup,(userName+slash+group+slash+bufferTemp));

	}else if(status=="CLIENT_DISCONNECT"){
		inchideConex(0);

	}else if(status=="CLIENT_KICK"){
		userName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		group.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		
		backToListener->FunctieKickUser(userName,group);

	}else if(status=="CLIENT_CONNECT"){
		typeIMEI.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		osVersion.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		thisUserName = userName;
		//thisGroup = group;//


		//thisGroup = "Admin";
		//thisName = "test";//

		
		multimap<string,string> returned;
		returned=backToListener->FunctieAutentificareDB(this,bufferTemp);
		
		if(returned.empty()==false){
			thisName = returned.find("name")->second;
			thisGroup = returned.find("auth")->second;
		}

		if((thisName.empty()==true)||(backToListener->DetectClientDuplicat(thisUserName)==true)){
			inchideConex(-3);
			return 0;
		}
		
		backToListener->FunctieAddClientToVector(this);
		clientIsValid=true;


		// IP+PORT
		char stringPORT[6]={0};
		itoa(backToListener->port,stringPORT,10);
		remoteIPPORTConnected=thisRemoteSocket->getHostIP()+":"+stringPORT;
		

		/*
		// duplicate user
		int i = 0;
		char buff[3]={0}; // max 999
		int size = thisUserName.size();
		while(backToListener->DetectClientDuplicatRENAME(thisUserName,thisGroup)>1){
			itoa(i++,buff,10);
			thisUserName.erase(size,thisUserName.size());
			thisUserName.insert(size,buff);
		}
		
		*/
		

		clientTimeConnected=connectedAT();
		cout << "<server> Client conectat: ";
		cout << thisUserName << "/" << thisGroup << " cu ip[port]: " << remoteIPPORTConnected <<  endl;
		backToListener->FunctieSendConnectedDisconnectedClients(this,true);


	}else if(status=="CLIENT_REGISTER"){
		typeIMEI.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		osVersion.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		name.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		string s;
		if( (backToListener->DetectClientDuplicat(userName)==true)||
			(backToListener->FunctieInregistrareUserDB(userName,"User",typeIMEI,bufferTemp,name,"0")==false) ){
				s = "CLIENT_REGISTER"+slash+userName+slash+group+slash+"DUPLICATE";
				sendMessage(s);
				inchideConex(-6);
		}else{
			s = "CLIENT_REGISTER"+slash+userName+slash+group+slash+"OKEY";
			sendMessage(s);
			inchideConex(-5);
		}

	}else if(status=="DB_ADD"){
		name.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		typeIMEI.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		userName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		group.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		confirmed.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		string s;
		if( (backToListener->DetectClientDuplicat(userName)==true)||
			(backToListener->FunctieInregistrareUserDB(userName,group,typeIMEI,bufferTemp,name,confirmed)==false) ){
				s = "DB_GETALLUSERS"+slash+thisUserName+slash+thisGroup+slash+name+slash+typeIMEI+slash+userName+slash+group+slash+confirmed+slash+"DUPLICATE";
				sendMessage(s);
		}else{
			backToListener->FunctieSendMessage("DB_GETALLUSERS","","",thisUserName,thisGroup,
			(name+slash+typeIMEI+slash+userName+slash+group+slash+confirmed+slash+"ADD")
			);
			s = "DB_GETALLUSERS"+slash+thisUserName+slash+thisGroup+slash+"NULL"+slash+"NULL"+slash+"NULL"+slash+"NULL"+slash+"NULL"+slash+"OKEY";
			sendMessage(s);

		}
	}else if(status=="DB_GETALLUSERS"){
		backToListener->FunctieSendAllUsersDB(this);

	}else if(status=="DB_DELETEUSER"){
		backToListener->FunctieDeleteUserDB(this);
		
	}else if(status=="DB_UPDATEUSER"){
		confirmed = bufferTemp;
		backToListener->FunctieUpdateUserDB(this,false);

	}else if(status=="DB_EDITUSER"){
		name.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		typeIMEI.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		userName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		group.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		confirmed.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		oldUserName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());	

		backToListener->FunctieUpdateUserDB(this,true);

	}else if(status=="DB_GETALLPRODUCTS"){
		thisUserName = userName;
		thisGroup = group;
		confirmed = bufferTemp;
		backToListener->FunctieSendAllProductsDB(this);

	}else if(status=="DB_ADDNEWPRODUCT"){
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		stoc.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		pret.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		backToListener->FunctieAddNewProductsDB(this);
		
	}else if(status=="DB_DELETEPRODUCT"){
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

	
		backToListener->FunctieDeleteProductDB(this);
	}else if(status=="DB_UPDATEPRODUCT"){
		oldProdus.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		oldCategorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		stoc.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		pret.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		backToListener->FunctieUpdateProductDB(this);
	}else if(status=="DB_NEWBUY"){

		while(bufferTemp.empty()==false){
			produs.assign(bufferTemp,0, bufferTemp.find(slash));
			bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
			stoc.assign(bufferTemp,0, bufferTemp.find(slash));
			bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

			backToListener->FunctieNewBuyDB(this);
		}
		string s;
		s = "DB_NEWBUY"+slash+thisUserName+slash+thisGroup+slash+"OKEY";//sending ok
		sendMessage(s);
	}else if(status=="DB_GETALLCOMANDS"){
		backToListener->FunctieSendAllComandsDB(this);
		
	}else if(status=="DB_DELETECOMAND"){
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		backToListener->FunctieDeleteComandDB(this);

	}else if(status=="DB_UPDATECOMAND"){
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		confirmed.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		backToListener->FunctieUpdateComandDB(this);

	}

	return 0;
}

void CConex::sendMessage(string Message){
	backToListener->objMutexConexSendMsg.lock();
	char szHeader[5]={0};
	itoa(Message.length(),szHeader,10);
	thisRemoteSocket->sendData(szHeader,sizeof(szHeader));
	thisRemoteSocket->sendData(Message.c_str(),Message.length());
	backToListener->objMutexConexSendMsg.unlock();

	//cout << "send: " << green << Message << white << endl; /////////////
}
string CConex::connectedAT(){
	struct tm *time_now;
	time_t secs_now;
	char str[50] ;
	time(&secs_now);
	time_now = localtime(&secs_now);
	strftime(str, 49, "%d/%m/%Y %H:%M:%S",time_now); // format YYYY-MM-DD H:M:S
	return str;
}