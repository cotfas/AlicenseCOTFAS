#include "stdafx.h"
#include "Conex.h"



CConex::CConex(CsocketClientDlg * pdlg){
	clientDisconnected=false;
	isClientConnected=false;
	remoteSockToConex=pdlg->clientSock;
	pdlgclass=pdlg;
	slash = pdlgclass->slash;

	isNewConexInit=false;

	


}
CConex::~CConex(void){
}


void CConex::run(){
	while(isRunning()&&!isExited()){
		if(isClientConnected==false){
			connectToServer();
		}else{
			char szHeader[5]={0};
			int nBytesRecv = remoteSockToConex.getData(szHeader,sizeof(szHeader));
			int nBufferSize = atoi(szHeader);

			if(clientDisconnected==true) break;
			if(nBytesRecv<=0){
				inchideConex(nBytesRecv);
				break;
			}
			if(nBufferSize<=0){
				inchideConex(-2);
				break;
			}
			char *szBuffer = (char*)malloc(sizeof(char)*nBufferSize);
			szBuffer[nBufferSize]='\0';
			nBytesRecv = remoteSockToConex.getData(szBuffer,nBufferSize);

			protocol(szBuffer);
		}
	}
	pdlgclass->FunctieEliberareRemoteSock(this);
}

void CConex::connectToServer(){
	if(remoteSockToConex.makeClientSocket(pdlgclass->convCStr2Str(pdlgclass->ID_IP),pdlgclass->convCStringToInt(pdlgclass->ID_PORT))==true){
		isClientConnected=true;
		clientDisconnected=false;
		if(isNewConexInit==false){
			pdlgclass->isConnected();
		}else{
			pdlgclass->sendGetAllProducts(this);
		}
	}else{
		clientDisconnected=true;
		isClientConnected=false;
		stop();
		pdlgclass->insertStringInEditList("[APP]Nu sa putut conecta la server!");	
	}
}
void CConex::inchideConex(int status){
	clientDisconnected=true;
	stop();
	remoteSockToConex.setStatus(status);
}
void CConex::protocol(string bufferTemp){
	status.assign(bufferTemp,0, bufferTemp.find(slash)); // gasire index status
	bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
	userName.assign(bufferTemp,0, bufferTemp.find(slash)); // gasire index userName
	bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
	group.assign(bufferTemp,0, bufferTemp.find(slash));	// gasire index group
	bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

	
	if(status=="SEND_MESSAGETOALL"){
		pdlgclass->insertStringInEditList("["+userName+"/"+group+"]: " + bufferTemp);
	}else if(status=="SEND_MESSAGETO"){
		toUserName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		toGroup.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		pdlgclass->insertStringInEditList("***["+userName+"/"+group+" >> "+toUserName+"/"+toGroup+"] : " + bufferTemp);
	}else if(status=="CLIENT_CONNECT"){ 
		name.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		typeIMEI.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		osVersion.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		remoteIPPORTConnected.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		clientTimeConnected.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		string items[] = {osVersion,name,userName, group,remoteIPPORTConnected,typeIMEI,clientTimeConnected};
		pdlgclass->insertItemInList(pdlgclass->m_ListCtrl,items,"main");

		if(bufferTemp=="CONNECTED1"){
			pdlgclass->ID_THISUSERNAME=userName.c_str();
			pdlgclass->ID_THISAUTH=group.c_str();
			pdlgclass->insertStringInEditList("[ClientStats]: Eu ["+userName+"/"+group+"] sunt conectat!");


			newConex = new CConex(pdlgclass);
			newConex->isNewConexInit=true;
			newConex->start();
			//pdlgclass->sendGetAllProducts();

			if(group=="Admin"){
				pdlgclass->GetDlgItem(IDC_BUTTON4)->ShowWindow(true);
				pdlgclass->GetDlgItem(IDC_BUTTON5)->ShowWindow(true);
				pdlgclass->GetDlgItem(IDC_BUTTON6)->ShowWindow(true);
				pdlgclass->GetDlgItem(IDC_BUTTON7)->ShowWindow(true);

			}

		}

		if(bufferTemp=="CONNECTED2"){
			pdlgclass->insertStringInEditList("[ClientStats]: Clientul ["+userName+"/"+group+"] sa conectat!");
		
		}
	}else if(status=="CLIENT_NOACCESS"){
		pdlgclass->insertStringInEditList("[SERVER]: Nu aveti drepuri de administrare!");
	}else if(status=="CLIENT_DISCONNECT"){
		pdlgclass->findItemListToDelete(pdlgclass->m_ListCtrl,userName,"USERNAME");
		pdlgclass->insertStringInEditList("[ClientStats]: Clientul ["+userName+"/"+group+"] sa deconectat!");
	}else if(status=="CLIENT_REGISTER"){
		if(bufferTemp=="DUPLICATE"){
		pdlgclass->m_settingsDlg.m_newUserDlg->GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"User existent in baza de date! Alegeti alt nume de utilizator!");
		}else if(bufferTemp=="OKEY"){
			pdlgclass->m_settingsDlg.m_newUserDlg->GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"User a fost inregistrat in baza de date, apasa OK pentru continuare!");
			pdlgclass->m_settingsDlg.m_newUserDlg->isRegistred=true;

		}
	}else if(status=="DB_GETALLUSERS"){
		name.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		typeIMEI.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		toUserName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		toGroup.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		confirmed.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		if(confirmed=="1")
			confirmed="Confirmat";
		else
			confirmed="Neconfirmat";

		if(pdlgclass->m_accountsDlg.isInitialise==true){
			if(bufferTemp=="DUPLICATE"){
				pdlgclass->m_accountsDlg.m_addAccountDlg.GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"User existent in baza de date! Alegeti alt nume de utilizator!");
			}else if(bufferTemp=="OKEY"){
				pdlgclass->m_accountsDlg.m_addAccountDlg.GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"User a fost inregistrat in baza de date, apasa OK pentru continuare!");
			}else{
					
				if(bufferTemp=="UPDATE"){
					pdlgclass->nItemPosition = pdlgclass->findItemListToDelete(pdlgclass->m_accountsDlg.m_ListCtrlAccounts,toUserName,"USERNAME");
				}		
				if(bufferTemp=="EDIT"){
					pdlgclass->nItemPosition = pdlgclass->findItemListToDelete(pdlgclass->m_accountsDlg.m_ListCtrlAccounts,userName,"USERNAME");
				}
				string items[] = {name,typeIMEI, toUserName,toGroup,confirmed,bufferTemp};
				pdlgclass->insertItemInList(pdlgclass->m_accountsDlg.m_ListCtrlAccounts,items,"accounts");
				pdlgclass->nItemPosition = -1;
			}
		}
	
	}else if(status=="DB_DELETEUSER"){
		if(pdlgclass->m_accountsDlg.isInitialise==true){
			pdlgclass->findItemListToDelete(pdlgclass->m_accountsDlg.m_ListCtrlAccounts,userName,"USERNAME");
		}


	}else if(status=="DB_GETALLPRODUCTS"){
		id.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		stoc.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		pret.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		
		string cmds = "id='"+id+"',produs='"+produs+"',categorie='"+categorie+"',stoc='"+stoc+"',pret='"+pret+"'";
		if(pdlgclass->sql.dbGetItem("products","id",cmds).empty()==true)
			pdlgclass->sql.dbInsertItem("products",cmds);

		if(pdlgclass->m_productsDlg.isInitialise==true){ // actualizare ultim produs adaugat
			string items[] = {produs,categorie,stoc,pret};
			pdlgclass->insertItemInList(pdlgclass->m_productsDlg.m_ListCtrlProducts,items,"products");

		}

	}else if(status=="DB_UPDATINGALLPRODUCTS"){
		CString temp;
		if(bufferTemp=="START"){
			pdlgclass->GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
			temp="Products\r\nUpdating\r\n...";
			pdlgclass->GetDlgItem(IDC_BUTTON3)->SetWindowTextW(temp);
			pdlgclass->sql.dbExecuteQuery("BEGIN;");
			QueryPerformanceCounter(&start1);
		}else if(bufferTemp=="STOP"){
			pdlgclass->GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
			temp="Products\r\nUpdated!";
			pdlgclass->GetDlgItem(IDC_BUTTON3)->SetWindowTextW(temp);	
			pdlgclass->sql.dbExecuteQuery("END;");
			
			QueryPerformanceCounter(&stop1);
			QueryPerformanceFrequency(&frequency);
			//cout << "\nTimp de executie: generarea si introducerea nr in DB: " << 1000*(stop.QuadPart-start.QuadPart)/((float)frequency.QuadPart);
			int value = 1000*(stop1.QuadPart-start1.QuadPart)/((float)frequency.QuadPart);
			char test[10]={0};
			itoa(value,test,10);
			string testTimp;
			testTimp = "<!> Time: "+(string)test;
			pdlgclass->insertStringInEditList(testTimp);
			
			//MessageBox(NULL,(CString)"",(CString) test,0);
		}
	}else if(status=="DB_UPDATEPRODUCT"){
		oldProdus.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		oldCategorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		id.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		stoc.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		pret.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());


		string cmds = "produs='"+produs+"',categorie='"+categorie+"',stoc='"+stoc+"',pret='"+pret+"'";
		pdlgclass->sql.dbUpdateItem("products",id,cmds);

	
		if(pdlgclass->m_productsDlg.isInitialise==true){ // actualizare ultim produs adaugat
		
			pdlgclass->nItemPosition  = pdlgclass->findItemListToDelete(pdlgclass->m_productsDlg.m_ListCtrlProducts,oldProdus,"PRODUS");
		
			string items[] = {produs,categorie,stoc,pret};
			pdlgclass->insertItemInList(pdlgclass->m_productsDlg.m_ListCtrlProducts,items,"products");
			pdlgclass->nItemPosition = -1;
		}

	}else if(status=="DB_DELETEPRODUCT"){
		id.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());

		pdlgclass->sql.dbDeleteItem("products",id);

		if(pdlgclass->m_productsDlg.isInitialise==true){
				pdlgclass->findItemListToDelete(pdlgclass->m_productsDlg.m_ListCtrlProducts,produs,"PRODUS");
		}



	}else if(status=="DB_NEWBUY"){
		if(bufferTemp=="OKEY"){
			if(pdlgclass->m_productsDlg.isInitialise==true){
				pdlgclass->m_productsDlg.functionOfflineDeleteFromBuy(true);
			}
		}
	}else if(status=="DB_GETALLCOMANDS"){ ///?
		id.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		toUserName.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		toGroup.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		produs.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		categorie.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		stoc.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		pret.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		cantitate.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		confirmed.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		addedtime.assign(bufferTemp,0, bufferTemp.find(slash));
		bufferTemp.erase(0,bufferTemp.find(slash)+slash.length());
		

		if(confirmed=="1")
			confirmed="Confirmat";
		else
			confirmed="Neconfirmat";

		if(pdlgclass->m_comandsDlg.isInitialise==true){
			if(bufferTemp=="UPDATE"){
				pdlgclass->nItemPosition = pdlgclass->findItemListToDelete(pdlgclass->m_comandsDlg.m_ListCtrlComands,id,"IDCOMAND");
			}

			string items[] = {id,toUserName,toGroup,produs,categorie,stoc,pret,cantitate,confirmed,addedtime};
			pdlgclass->insertItemInList(pdlgclass->m_comandsDlg.m_ListCtrlComands,items,"comands");
			pdlgclass->nItemPosition = -1;
		}
	
	}else if(status=="DB_DELETECOMAND"){
		if(pdlgclass->m_comandsDlg.isInitialise==true){
			pdlgclass->findItemListToDelete(pdlgclass->m_comandsDlg.m_ListCtrlComands,bufferTemp,"IDCOMAND");
		}


	}


	

	
}

void CConex::sendMessage(string Message){

	char szHeader[5]={0};
	itoa(Message.length(),szHeader,10);
	remoteSockToConex.sendData(szHeader,sizeof(szHeader));
	remoteSockToConex.sendData(Message.c_str(),Message.length());

}
