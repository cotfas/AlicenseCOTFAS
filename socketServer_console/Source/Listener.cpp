#include "Listener.h"
#include "Conex.h"

CListener::CListener(void)
{
	slash = "//";
}
CListener::~CListener(void)
{
}



void CListener::run(){
	while(isRunning()&&!isExited()){
		socketListenerToConex = new CConex(this,serverSockListener.acceptSocket());
		if(isRunning()){
			socketListenerToConex->start();
		}else{
			delete socketListenerToConex;
		}
	}

}



multimap<string,string> CListener::FunctieAutentificareDB(CConex* remote, string passwordT){
	objMutexFunctieAutentificareDB.lock();
	multimap<string,string> returned;
	string cmds = 
		"type='"+remote->typeIMEI+"',"+
		"user='"+remote->thisUserName+"',"+
		"password='"+passwordT+"',"+
		//"auth='"+remote->thisGroup+"',"+
		"confirmed='1'";
	if(dbHandler.dbGetItem("accounts","type,name,auth",cmds).empty()==false)
		returned=dbHandler.dbGetItem("accounts","name,auth",cmds);
	objMutexFunctieAutentificareDB.unlock();
	return returned;
}
bool CListener::FunctieInregistrareUserDB(string thisUserName, string thisGroup, string typeIMEI, string password, string name, string confirmed){
	objMutexFunctieInregistrareUserDB.lock();
	string cmds;

	if(dbHandler.dbGetItem("accounts","user","").empty()==true){	// if no user exist to db, create admin
		cmds = "name='"+name+"',type='"+typeIMEI+"',user='"+thisUserName+"',password='"+password+"',auth='Admin',confirmed='1'";
		dbHandler.dbInsertItem("accounts",cmds);
		return true;
	}

	cmds = "user='"+thisUserName+"'";
	if(dbHandler.dbGetItem("accounts","user",cmds).empty()==true){
		dbHandler.dbInsertItem(
			"accounts",
			"name='"+name+"',type='"+typeIMEI+"',user='"+thisUserName+"',password='"+password+"',auth='"+thisGroup+"',confirmed='"+confirmed+"'");
		objMutexFunctieInregistrareUserDB.unlock();
		return true;
	}else{
		objMutexFunctieInregistrareUserDB.unlock();
		return false;
	}
}
void CListener::FunctieSendAllUsersDB(CConex* remote){
	objMutexFunctieSendAllUsersDB.lock();
	string idCmds, s;
	multimap<string,string> returnedIDS, returnedValue;


	returnedIDS = dbHandler.dbGetItem("accounts","id","");
	if(returnedIDS.empty()==false){
		for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
			idCmds=(*it).first+"="+(*it).second;
			returnedValue = dbHandler.dbGetItem("accounts","name,type,user,auth,confirmed",idCmds);
			if(returnedValue.empty()==false){
				if(FunctieVerificareUserOnline(returnedValue.find("user")->second,returnedValue.find("auth")->second)==true){
					s="DB_GETALLUSERS"+slash+remote->thisUserName+slash+remote->thisGroup+slash+
						returnedValue.find("name")->second+slash+
						returnedValue.find("type")->second+slash+
						returnedValue.find("user")->second+slash+
						returnedValue.find("auth")->second+slash+
						returnedValue.find("confirmed")->second+slash+
						"Online";
				}else{
					s="DB_GETALLUSERS"+slash+remote->thisUserName+slash+remote->thisGroup+slash+
						returnedValue.find("name")->second+slash+
						returnedValue.find("type")->second+slash+
						returnedValue.find("user")->second+slash+
						returnedValue.find("auth")->second+slash+
						returnedValue.find("confirmed")->second+slash+
						"Offline";			
				}	
			}							
			remote->sendMessage(s);
		}
	}
	objMutexFunctieSendAllUsersDB.unlock();
}
void CListener::FunctieDeleteUserDB(CConex* remote){
	objMutexFunctieDeleteUserDB.lock();
	string idCmds = "user='"+remote->userName+"',auth='"+remote->group+"'";
	string id2Delete;

	multimap<string,string> returnedIDS, returnedValue;
	if(dbHandler.dbGetItem("accounts","id",idCmds).empty()==false){
		id2Delete = dbHandler.dbGetItem("accounts","id",idCmds).begin()->second;
		idCmds = "id='"+id2Delete+"'";
		dbHandler.dbDeleteItem("accounts",idCmds);

		FunctieSendMessage("DB_DELETEUSER","","Admin",remote->userName,remote->group,"NULL");

		// delete all the comands asigned to his account
		idCmds = "idUser='"+id2Delete+"'";
		dbHandler.dbDeleteItem("offlinesync",idCmds);
		returnedIDS = dbHandler.dbGetItem("comands","id",idCmds);
		if(returnedIDS.empty()==false){
			for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
				idCmds=(*it).first+"="+(*it).second;
				dbHandler.dbDeleteItem("comands",idCmds);
				FunctieSendMessage("DB_DELETECOMAND","","Admin",remote->userName,remote->group,(*it).second);
			}
		}
	}
	objMutexFunctieDeleteUserDB.unlock();
}
void CListener::FunctieUpdateUserDB(CConex* remote, bool isEditing){
	objMutexFunctieUpdateUserDB.lock();
	string id,idCmds,idEdit,isEdit;

	idCmds = "user='"+remote->userName+"'";
	if(isEditing==false){
		idEdit = "confirmed='"+remote->confirmed+"'";
		isEdit = "UPDATE";
	}else{
		if(dbHandler.dbGetItem("accounts","user",idCmds).empty()==true){
			idEdit = "name='"+remote->name+"',type='"+remote->typeIMEI+"',user='"+remote->userName+"',auth='"+remote->group+"',confirmed='"+remote->confirmed+"'";
		}else{
			idEdit = "name='"+remote->name+"',type='"+remote->typeIMEI+"',auth='"+remote->group+"',confirmed='"+remote->confirmed+"'";
		}
		idCmds = "user='"+remote->oldUserName+"'";
		isEdit = "EDIT";
	}
	multimap<string,string> returnedValue;
	if(dbHandler.dbGetItem("accounts","id",idCmds).empty()==false){
		id = dbHandler.dbGetItem("accounts","id",idCmds).begin()->second;
		dbHandler.dbUpdateItem("accounts",id,idEdit);

		idCmds = "id='"+id+"'";
		returnedValue = dbHandler.dbGetItem("accounts","name,type,user,auth,confirmed",idCmds);
		if(returnedValue.empty()==false){
			FunctieSendMessage("DB_GETALLUSERS","","Admin",remote->oldUserName,remote->thisGroup,
				(returnedValue.find("name")->second+slash+
				returnedValue.find("type")->second+slash+
				returnedValue.find("user")->second+slash+
				returnedValue.find("auth")->second+slash+
				returnedValue.find("confirmed")->second+slash+
				isEdit)
			);
		}
	}
	objMutexFunctieUpdateUserDB.unlock();
}
bool CListener::FunctieSendAllProductsDB(CConex* remote){
	objMutexFunctieSendAllProductsDB.lock();
	int lastRow = dbHandler.dbGetLastRowId("products");
	string idCmds, s;

	char lastRowConfirmed[10]={0};
	itoa(lastRow,lastRowConfirmed,10);

	multimap<string,string> returnedIDS, returnedValue;
	

	s="DB_UPDATINGALLPRODUCTS"+slash+remote->thisUserName+slash+remote->thisGroup+slash+"START";							
	remote->sendMessage(s);

	
	if(lastRow > atoi(remote->confirmed.c_str())){ // trimit doar cand este de trimis
		returnedIDS = dbHandler.dbGetItemBetween("products","id",remote->confirmed,lastRowConfirmed);
		if(returnedIDS.empty()==false){
			for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
				if(FunctieVerificareUserOnline(remote->thisUserName,remote->thisGroup)==false){
					remote->inchideConex(-7);
					objMutexFunctieSendAllProductsDB.unlock();
					return false;
				}
				idCmds=(*it).first+"="+(*it).second;
				returnedValue = dbHandler.dbGetItem("products","id,produs,categorie,stoc,pret",idCmds);
				if(returnedValue.empty()==false){
					s="DB_GETALLPRODUCTS"+slash+remote->thisUserName+slash+remote->thisGroup+slash+
						returnedValue.find("id")->second+slash+
						returnedValue.find("produs")->second+slash+
						returnedValue.find("categorie")->second+slash+
						returnedValue.find("stoc")->second+slash+
						returnedValue.find("pret")->second;									
					remote->sendMessage(s);
				}
			}
		}
	}

	// check offline sync
	string cmds = "user='"+remote->thisUserName+"',auth='"+remote->thisGroup+"'";
	string idFound,offlinesync;

	idFound = dbHandler.dbGetItem("accounts","id",cmds).begin()->second;
	cmds = "idUser='"+idFound+"'";
	returnedIDS = dbHandler.dbGetItem("offlinesync","idProdus",cmds);


	if(returnedIDS.empty()==false){
		// UPDATE ? DELETE NEED
		for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
			if(FunctieVerificareUserOnline(remote->thisUserName,remote->thisGroup)==false){
				remote->inchideConex(-7);
				objMutexFunctieSendAllProductsDB.unlock();
				return false;
			}
			idCmds="id="+(*it).second;
			returnedValue = dbHandler.dbGetItem("products","id,produs,categorie,stoc,pret",idCmds);
			if(returnedValue.empty()==false){
				// modify
				s="DB_UPDATEPRODUCT"+slash+remote->thisUserName+slash+remote->thisGroup+slash+
					returnedValue.find("produs")->second+slash+
					returnedValue.find("categorie")->second+slash+
					returnedValue.find("id")->second+slash+
					returnedValue.find("produs")->second+slash+
					returnedValue.find("categorie")->second+slash+
					returnedValue.find("stoc")->second+slash+
					returnedValue.find("pret")->second;									
				remote->sendMessage(s);
			}else{
				// delete /////
				s="DB_DELETEPRODUCT"+slash+remote->thisUserName+slash+remote->thisGroup+slash+
					(*it).second+slash+
					"NULL"+slash+
					"NULL"+slash+
					"NULL";		
				remote->sendMessage(s);
			}
			cmds = "idUser='"+idFound+"',idProdus='"+(*it).second+"'";
			dbHandler.dbDeleteItem("offlinesync",cmds);
		}
		
	}else{
		// NO UPDATE NEED
	}

	s="DB_UPDATINGALLPRODUCTS"+slash+remote->thisUserName+slash+remote->thisGroup+slash+"STOP";							
	remote->sendMessage(s);
	remote->inchideConex(-7);

	objMutexFunctieSendAllProductsDB.unlock();
	return true;
}
void CListener::FunctieAddNewProductsDB(CConex* remote){
	objMutexFunctieAddNewProductsDB.lock();
	string cmds, s;
	multimap<string,string> returnedValue;

	cmds = "produs='"+remote->produs+"'";
	if(dbHandler.dbGetItem("products","produs",cmds).empty()==true){ // verificare existenta produs
		cmds = "produs='"+remote->produs+"',categorie='"+remote->categorie+"',stoc='"+remote->stoc+"',pret='"+remote->pret+"'";
		dbHandler.dbInsertItem("products",cmds);

		returnedValue = dbHandler.dbGetItem("products","id,produs,categorie,stoc,pret",cmds);
		if(returnedValue.empty()==false){
			FunctieSendMessage("DB_GETALLPRODUCTS","","",remote->thisUserName,remote->thisGroup,
				(returnedValue.find("id")->second+slash+
				returnedValue.find("produs")->second+slash+
				returnedValue.find("categorie")->second+slash+
				returnedValue.find("stoc")->second+slash+
				returnedValue.find("pret")->second)
			);
		}
	}
	objMutexFunctieAddNewProductsDB.unlock();
}
void CListener::FunctieDeleteProductDB(CConex* remote){
	objMutexFunctieDeleteProductDB.lock();
	string cmds = "produs='"+remote->produs+"',categorie='"+remote->categorie+"'";
	string id,offlinesync;
	multimap<string,string> returnedIDS, returnedValue;


	if(dbHandler.dbGetItem("products","id",cmds).empty()==false){
		id = dbHandler.dbGetItem("products","id",cmds).begin()->second;
		cmds = "id='"+id+"'";
		dbHandler.dbDeleteItem("products",cmds);


		returnedIDS = dbHandler.dbGetItem("accounts","id","");
		string idReturned,idUser,idGroup;
		if(returnedIDS.empty()==false){ // users
			for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
				cmds=(*it).first+"="+(*it).second;
				returnedValue = dbHandler.dbGetItem("accounts","id,user,auth",cmds);
				if(returnedValue.empty()==false){
					idReturned = returnedValue.find("id")->second;
					idUser = returnedValue.find("user")->second;
					idGroup = returnedValue.find("auth")->second;
					if(FunctieVerificareUserOnline(idUser,idGroup)==true){
						// online
						FunctieSendMessage("DB_DELETEPRODUCT",idUser,idGroup,remote->thisUserName,remote->thisGroup,
							(id+slash+
							remote->produs+slash+
							remote->categorie+slash+
							"NULL")
						);
					}else{
						// offline
						offlinesync = "idUser='"+idReturned+"',idProdus='"+id+"'";
						if(dbHandler.dbGetItem("offlinesync","idUser,idProdus",offlinesync).empty()==true)
							dbHandler.dbInsertItem("offlinesync",offlinesync);
					}
				}
			}
		}
		// delete all the comands asigned to this product
		cmds = "idProdus='"+id+"'";
		returnedIDS = dbHandler.dbGetItem("comands","id",cmds);
		if(returnedIDS.empty()==false){
			for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
				cmds=(*it).first+"="+(*it).second;
				dbHandler.dbDeleteItem("comands",cmds);
				FunctieSendMessage("DB_DELETECOMAND","","Admin",remote->userName,remote->group,(*it).second);
			}
		}

	}
	objMutexFunctieDeleteProductDB.unlock();
}
void CListener::FunctieUpdateProductDB(CConex* remote){
	objMutexFunctieUpdateProductDB.lock();
	string idCmds, idProdus, cmds, offlineSync;
	idCmds = "produs='"+remote->oldProdus+"',categorie='"+remote->oldCategorie+"'";
	multimap<string,string> returnedIDS, returnedValue;


	if(dbHandler.dbGetItem("products","id",idCmds).empty()==false){
		idProdus = dbHandler.dbGetItem("products","id",idCmds).begin()->second;
		cmds = "produs='"+remote->produs+"',categorie='"+remote->categorie+"',stoc='"+remote->stoc+"',pret='"+remote->pret+"'";
		dbHandler.dbUpdateItem("products",idProdus,cmds);
	

		returnedIDS = dbHandler.dbGetItem("accounts","id","");
		string idReturned,idUser,idGroup;
		if(returnedIDS.empty()==false){
			for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
				idCmds=(*it).first+"="+(*it).second;
				returnedValue = dbHandler.dbGetItem("accounts","id,user,auth",idCmds);
				if(returnedValue.empty()==false){
					idReturned = returnedValue.find("id")->second;
					idUser = returnedValue.find("user")->second;
					idGroup = returnedValue.find("auth")->second;

					if(FunctieVerificareUserOnline(idUser,idGroup)==true){
						// online
						returnedValue = dbHandler.dbGetItem("products","id,produs,categorie,stoc,pret",cmds);
						if(returnedValue.empty()==false){
							FunctieSendMessage("DB_UPDATEPRODUCT",idUser,idGroup,remote->thisUserName,remote->thisGroup,
								(remote->oldProdus+slash+
								remote->oldCategorie+slash+
								returnedValue.find("id")->second+slash+
								returnedValue.find("produs")->second+slash+
								returnedValue.find("categorie")->second+slash+
								returnedValue.find("stoc")->second+slash+
								returnedValue.find("pret")->second)
							);
						}
					}else{
						// offline
						offlineSync = "idUser='"+idReturned+"',idProdus='"+idProdus+"'";
						if(dbHandler.dbGetItem("offlinesync","idUser,idProdus",offlineSync).empty()==true)
							dbHandler.dbInsertItem("offlinesync",offlineSync);
					}
				}
			}
		}
	}
	objMutexFunctieUpdateProductDB.unlock();
}
void CListener::FunctieNewBuyDB(CConex* remote){
	objMutexFunctieNewBuyDB.lock();
	string cmds,idCmds;
	multimap<string,string> returnedIDS, returnedValue,returnedValueUser,returnedValueProdus;

	cmds = "user='"+remote->thisUserName+"',auth='"+remote->thisGroup+"'";
	if(dbHandler.dbGetItem("accounts","id",cmds).empty()==false){
		idCmds=dbHandler.dbGetItem("accounts","id",cmds).begin()->second;
		cmds = "idUser='"+idCmds+"',idProdus='"+remote->produs+"',cantitate='"+remote->stoc+"',confirmed='0',addedtime='"+remote->connectedAT()+"'";
		dbHandler.dbInsertItem("comands",cmds);
		
		// send new buy to all
		returnedValue = dbHandler.dbGetItem("comands","id,idUser,idProdus,cantitate,confirmed,addedtime",cmds);
		if(returnedValue.empty()==false){
			cmds="id='"+returnedValue.find("idUser")->second+"'";
			returnedValueUser=dbHandler.dbGetItem("accounts","user,auth",cmds);
			if(returnedValueUser.empty()==false){
				cmds="id='"+returnedValue.find("idProdus")->second+"'";
				if(dbHandler.dbGetItem("products","produs,categorie,stoc,pret",cmds).empty()==false){
					returnedValueProdus = dbHandler.dbGetItem("products","produs,categorie,stoc,pret",cmds);
					if(returnedValueProdus.empty()==false){
						FunctieSendMessage("DB_GETALLCOMANDS","","Admin",remote->thisUserName,remote->thisGroup,
							(returnedValue.find("id")->second+slash+
							returnedValueUser.find("user")->second+slash+
							returnedValueUser.find("auth")->second+slash+
							returnedValueProdus.find("produs")->second+slash+
							returnedValueProdus.find("categorie")->second+slash+
							returnedValueProdus.find("stoc")->second+slash+
							returnedValueProdus.find("pret")->second+slash+
							returnedValue.find("cantitate")->second+slash+
							returnedValue.find("confirmed")->second+slash+								
							returnedValue.find("addedtime")->second)
						);
					}
				}
			}
		}
	}
	objMutexFunctieNewBuyDB.unlock();
}
void CListener::FunctieSendAllComandsDB(CConex* remote){
	objMutexFunctieSendAllComandsDB.lock();
	string cmds,idCmds, s;
	multimap<string,string> returnedIDS, returnedValue,returnedValueUser,returnedValueProdus;


	returnedIDS = dbHandler.dbGetItem("comands","id","");
	if(returnedIDS.empty()==false){
		for (multimap<string,string>::iterator it=returnedIDS.begin(); it != returnedIDS.end(); it++ ){
			idCmds=(*it).first+"="+(*it).second;
			returnedValue = dbHandler.dbGetItem("comands","id,idUser,idProdus,cantitate,confirmed,addedtime",idCmds);
			if(returnedValue.empty()==false){
				cmds="id='"+returnedValue.find("idUser")->second+"'";
				returnedValueUser=dbHandler.dbGetItem("accounts","user,auth",cmds);
				if(returnedValueUser.empty()==false){
					cmds="id='"+returnedValue.find("idProdus")->second+"'";
					if(dbHandler.dbGetItem("products","produs,categorie,stoc,pret",cmds).empty()==false){
						returnedValueProdus = dbHandler.dbGetItem("products","produs,categorie,stoc,pret",cmds);
						if(returnedValueProdus.empty()==false){
							s="DB_GETALLCOMANDS"+slash+remote->thisUserName+slash+remote->thisGroup+slash+
								returnedValue.find("id")->second+slash+
								returnedValueUser.find("user")->second+slash+
								returnedValueUser.find("auth")->second+slash+
								returnedValueProdus.find("produs")->second+slash+
								returnedValueProdus.find("categorie")->second+slash+
								returnedValueProdus.find("stoc")->second+slash+
								returnedValueProdus.find("pret")->second+slash+
								returnedValue.find("cantitate")->second+slash+
								returnedValue.find("confirmed")->second+slash+								
								returnedValue.find("addedtime")->second;
							remote->sendMessage(s);
						}
					}
				}
			}
		}
	}
	objMutexFunctieSendAllComandsDB.unlock();
}
void CListener::FunctieDeleteComandDB(CConex* remote){
	objMutexFunctieDeleteComandDB.lock();
	string idCmds = "id='"+remote->produs+"'";
	string id2Delete;

	if(dbHandler.dbGetItem("comands","id",idCmds).empty()==false){
		id2Delete = dbHandler.dbGetItem("comands","id",idCmds).begin()->second;
		idCmds = "id='"+id2Delete+"'";
		dbHandler.dbDeleteItem("comands",idCmds);

		FunctieSendMessage("DB_DELETECOMAND","","Admin",remote->userName,remote->group,id2Delete);
	}
	objMutexFunctieDeleteComandDB.unlock();
}
void CListener::FunctieUpdateComandDB(CConex* remote){
	objMutexFunctieUpdateComandDB.lock();
	string idCmds = "id='"+remote->produs+"'";
	string idFound,cmds;


	multimap<string,string> returned,returnedValue,returnedValueUser,returnedValueProdus;

	if(dbHandler.dbGetItem("comands","id",idCmds).empty()==false){
		idFound = dbHandler.dbGetItem("comands","id",idCmds).begin()->second;
		idCmds="confirmed='"+remote->confirmed+"'";
		dbHandler.dbUpdateItem("comands",idFound,idCmds);

		
		{// returning updated item
			idCmds="id='"+idFound+"'";
			returnedValue = dbHandler.dbGetItem("comands","id,idUser,idProdus,cantitate,confirmed,addedtime",idCmds);
			if(returnedValue.empty()==false){
				cmds="id='"+returnedValue.find("idUser")->second+"'";
				returnedValueUser=dbHandler.dbGetItem("accounts","user,auth",cmds);
				if(returnedValueUser.empty()==false){
					cmds="id='"+returnedValue.find("idProdus")->second+"'";
					if(dbHandler.dbGetItem("products","produs,categorie,stoc,pret",cmds).empty()==false){
						returnedValueProdus = dbHandler.dbGetItem("products","produs,categorie,stoc,pret",cmds);
						if(returnedValueProdus.empty()==false){
							FunctieSendMessage("DB_GETALLCOMANDS","","Admin",remote->userName,remote->group,(
								returnedValue.find("id")->second+slash+
								returnedValueUser.find("user")->second+slash+
								returnedValueUser.find("auth")->second+slash+
								returnedValueProdus.find("produs")->second+slash+
								returnedValueProdus.find("categorie")->second+slash+
								returnedValueProdus.find("stoc")->second+slash+
								returnedValueProdus.find("pret")->second+slash+
								returnedValue.find("cantitate")->second+slash+
								returnedValue.find("confirmed")->second+slash+
								returnedValue.find("addedtime")->second+slash+
								"UPDATE")	
							);
						}
					}
				}
			}	
		}
	}
	objMutexFunctieUpdateComandDB.unlock();
}











void CListener::FunctieAddClientToVector(CConex* conexVector){
	objMutexVector.lock();
	vectorConex.push_back(conexVector);
	objMutexVector.unlock();
}
void CListener::FunctieEliberareRemoteSocket(CConex* remote){
	objMutexRemove.lock();
	if(remote->clientIsValid==true){
		FunctieSendConnectedDisconnectedClients(remote,false);
		vector< CConex* >::iterator it;
		for(int i=0; i < (int) vectorConex.size(); i++){
			if(remote==vectorConex.at(i)){
				it = vectorConex.begin();
				vectorConex.erase(it+i);
			}
		}
	}
	delete remote;
	objMutexRemove.unlock();
}
void CListener::FunctieSendMessage(string Status, string thisUserName, string thisGroup, string toUserName, string toGroup, string Message){
	objMutexSendMsg.lock();
	if(!Message.empty()){
		string tempMessage=Status+slash+toUserName+slash+toGroup+slash+Message;
		for(int i=0; i < (int) vectorConex.size(); i++){
			if(thisUserName=="" && thisGroup==""){	
				vectorConex.at(i)->sendMessage(tempMessage);
			}else if(thisUserName=="" && thisGroup==vectorConex.at(i)->thisGroup){
				vectorConex.at(i)->sendMessage(tempMessage);
			}else if(thisUserName==vectorConex.at(i)->thisUserName && thisGroup==vectorConex.at(i)->thisGroup){
				vectorConex.at(i)->sendMessage(tempMessage);
			}
			
		}
		tempMessage.clear();
	}else{
		cout << "<server> Introduceti un mesaj de transmis!"<<endl;
	}
	objMutexSendMsg.unlock();
}
void CListener::FunctieSendConnectedDisconnectedClients(CConex * remote, bool state){
	objMutexSendClient.lock();
	if(state==true){
		for(int i=0; i < (int) vectorConex.size(); i++){ // client curent
			for(int j=0; j < (int) vectorConex.size(); j++){ // orice client
				if((remote->thisUserName==vectorConex.at(i)->thisUserName)&&(remote->thisGroup==vectorConex.at(i)->thisGroup)){ //client curent
					if(i!=j){
						FunctieSendMessage("CLIENT_CONNECT",remote->thisUserName,remote->thisGroup,vectorConex.at(j)->thisUserName,vectorConex.at(j)->thisGroup,
							(vectorConex.at(j)->thisName+slash+
							vectorConex.at(j)->typeIMEI+slash+
							vectorConex.at(j)->osVersion+slash+
							vectorConex.at(j)->remoteIPPORTConnected+slash+
							vectorConex.at(j)->clientTimeConnected+slash+
							"CONNECTED0")); // transfer toata lista connectata catre mine inafara de EU
					

			FunctieSendMessage("CLIENT_CONNECT",vectorConex.at(j)->thisUserName,vectorConex.at(j)->thisGroup,remote->thisUserName,remote->thisGroup,
			(remote->thisName+slash+remote->typeIMEI+slash+remote->osVersion+slash+remote->remoteIPPORTConnected+slash+remote->clientTimeConnected+slash+"CONNECTED2"));  // trimit numele meu la toata lumea fara mine

					}else if(i==j){
						FunctieSendMessage("CLIENT_CONNECT",remote->thisUserName,remote->thisGroup,vectorConex.at(j)->thisUserName,vectorConex.at(j)->thisGroup,
							(vectorConex.at(j)->thisName+slash+
							vectorConex.at(j)->typeIMEI+slash+
							vectorConex.at(j)->osVersion+slash+
							vectorConex.at(j)->remoteIPPORTConnected+slash+
							vectorConex.at(j)->clientTimeConnected+slash+
							"CONNECTED1")); // transfer doar catre mine ca eu m-am conectat
					
					}
				}
			}
		}
	}else{
		FunctieSendMessage("CLIENT_DISCONNECT","","",remote->thisUserName,remote->thisGroup,"DISCONNECTED2");
	}
	objMutexSendClient.unlock();
}
void CListener::FunctieKickUser(string Nick, string group){
	objMutexKickUser.lock();
	for(int i = (int) vectorConex.size()-1; i >= 0 ; i--){
		if(Nick=="" && group==""){
			vectorConex.at(i)->inchideConex(-4);
		}else if(Nick==vectorConex.at(i)->thisUserName && group==vectorConex.at(i)->thisGroup){
			vectorConex.at(i)->inchideConex(-4);
		}
	}
	objMutexKickUser.unlock();
}
int CListener::DetectClientDuplicatRENAME(string Nick, string group){
	objMutexDetectClientDuplicatRENAME.lock();
	int count=0;
	for(int i=0; i < (int) vectorConex.size(); i++){
		if(Nick==vectorConex.at(i)->thisUserName && group==vectorConex.at(i)->thisGroup ){
			count++;
		}
	}
	objMutexDetectClientDuplicatRENAME.unlock();
	return count;
}
bool CListener::DetectClientDuplicat(string userName){
	objMutexDetectClientDuplicat.lock();
	for(int i=0; i < (int) vectorConex.size(); i++){
		if(userName==vectorConex.at(i)->thisUserName ){
			objMutexDetectClientDuplicat.unlock();
			return true;
		}
	}
	objMutexDetectClientDuplicat.unlock();
	return false;
}
bool CListener::FunctieVerificareUserOnline(string userNameT, string groupT){
	objMutexFunctieVerificareUserOnline.lock();
	for(int i=0; i < (int) vectorConex.size(); i++){
		if((userNameT==vectorConex.at(i)->thisUserName)&&(groupT==vectorConex.at(i)->thisGroup)){
			objMutexFunctieVerificareUserOnline.unlock();
			return true;
		}
	}
	objMutexFunctieVerificareUserOnline.unlock();
	return false;
}
void CListener::AfisareClienti(){
	if(vectorConex.empty()){
		cout << "--------------------------------------------"<<endl;
		cout << "\tNu exista clienti conectati"<<endl;
		cout << "--------------------------------------------"<<endl;
	}else{
		cout << "--------------------------------------------"<<endl;
		cout << "\tClienti conectati sunt"<<endl;
		cout << "--------------------------------------------"<<endl;
		cout << "Conectat la\t\tUtilizatori\tGrupuri\t\tIP:PORT"<<endl;

		for(int i=0; i < (int) vectorConex.size(); i++){
			cout << "(" +vectorConex.at(i)->clientTimeConnected+ ")\t" 
				<< vectorConex.at(i)->thisUserName << "\t\t" << vectorConex.at(i)->thisGroup << "\t\t" << vectorConex.at(i)->remoteIPPORTConnected << endl;
		}
	}
}