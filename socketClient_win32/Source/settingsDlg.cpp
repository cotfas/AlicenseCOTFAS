// settingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "socketClient.h"
#include "socketClientDlg.h"
#include "settingsDlg.h"
#include "afxdialogex.h"


// settingsDlg dialog

IMPLEMENT_DYNAMIC(settingsDlg, CDialogEx)

settingsDlg::settingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(settingsDlg::IDD, pParent)

{

}

settingsDlg::~settingsDlg()
{

}

void settingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxStatus1);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBoxStatus2);
}


BEGIN_MESSAGE_MAP(settingsDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDOK, &settingsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &settingsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &settingsDlg::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// settingsDlg message handlers
void settingsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DefWindowProc(WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}
void settingsDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	/*	
	if(rectMinWindowState.IsRectEmpty()==false){
		lpMMI->ptMinTrackSize.x = rectMinWindowState.Width()-60;
		lpMMI->ptMinTrackSize.y = rectMinWindowState.Height()-0;
	}
	*/
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
void settingsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	if(nType!=1){ // 1 on minimize

		rectWindowState=rectWindowStateACTUAL;
	}
}
BOOL settingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// dialog window state
	this->GetWindowRect(&rectWindowState);
	this->ScreenToClient(&rectWindowState);
	rectMinWindowState=rectWindowState;


	// initialise
	if(whatFormIs=="settings")
		settingsFormIntialise();
	else if(whatFormIs=="newUser")
		newUserFormIntialise();
	else if(whatFormIs=="addAccount")
		addAccountFormIntialise(whatFormIs);
	else if(whatFormIs=="editAccount")
		addAccountFormIntialise(whatFormIs);
	else if(whatFormIs=="addProduct")
		addProductFormIntialise(whatFormIs);
	else if(whatFormIs=="editProduct")
		addProductFormIntialise(whatFormIs);
	else if(whatFormIs=="buyProduct")
		addProductFormIntialise(whatFormIs);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void settingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(whatFormIs=="settings")
		settingsFormOk();
	else if(whatFormIs=="newUser")
		newUserFormOk();
	else if(whatFormIs=="addAccount")
		addAccountFormOk(whatFormIs);
	else if(whatFormIs=="editAccount")
		addAccountFormOk(whatFormIs);
	else if(whatFormIs=="addProduct")
		addProductFormOk(whatFormIs);
	else if(whatFormIs=="editProduct")
		addProductFormOk(whatFormIs);
	else if(whatFormIs=="buyProduct")
		addProductFormOk(whatFormIs);

}
void settingsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	m_newUserDlg = new settingsDlg();
	m_newUserDlg->setWhatFormIs("newUser");
	m_newUserDlg->DoModal();
}
void settingsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}








// ---------------------------------------------------------------------------------------- //
bool settingsDlg::verificareString(int nID, bool isNumbers){
	CString stringTest;
	GetDlgItem(nID)->GetWindowTextW(stringTest);
	if(stringTest.IsEmpty()==false){
	if(isNumbers==true){
		for(int i = 0; i < stringTest.GetLength(); i++){
			if(!(stringTest.GetAt(i)>=48 && stringTest.GetAt(i)<=57)){
				return false;
			}
		}	
	}else{
		
			for(int i = 0; i < stringTest.GetLength(); i++){
				if(!((stringTest.GetAt(i)>=65 && stringTest.GetAt(i)<=90)
					||(stringTest.GetAt(i)>=97 && stringTest.GetAt(i)<=122)
					||(stringTest.GetAt(i)>=48 && stringTest.GetAt(i)<=57)
					||(stringTest.GetAt(i)==' ')
					)
					){
					return false;
				}
			}
		}
		return true;
	}else{
		return false;
	}
	return false;
}
void settingsDlg::settingsFormIntialise(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"Settings");
	GetDlgItem(IDC_STATIC2)->SetWindowTextW(L"Server IP");
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(L"Server PORT");
	GetDlgItem(IDC_STATIC4)->SetWindowTextW(L"User NAME");
	GetDlgItem(IDC_STATIC5)->SetWindowTextW(L"User PWD");
	GetDlgItem(IDC_STATIC6)->SetWindowTextW(L"Auth TYPE");
	GetDlgItem(IDC_STATIC7)->SetWindowTextW(L"Language");



	CButton* pBtn1 = (CButton*) GetDlgItem(IDC_CHECK1);
	CButton* pBtn2 = (CButton*) GetDlgItem(IDC_CHECK2);
	if( pdlg->autoEnableListen == true ){
		pBtn1->SetCheck(1);// check it
	}else{
		pBtn1->SetCheck(0);// check it
	}
	if( pdlg->autoEnableChat == true ){
		pBtn2->SetCheck(1);// check it
	}else{
		pBtn2->SetCheck(0);// check it
	}

	m_ComboBoxStatus1.AddString((CString)"Admin");
	m_ComboBoxStatus1.AddString((CString)"User");
	m_ComboBoxStatus1.SelectString(0,(CString)pdlg->ID_THISAUTH);



	GetDlgItem(IDC_EDIT1)->SetWindowTextW(pdlg->ID_IP);
	GetDlgItem(IDC_EDIT2)->SetWindowTextW(pdlg->ID_PORT);
	GetDlgItem(IDC_EDIT3)->SetWindowTextW(pdlg->ID_USERNAME);
	GetDlgItem(IDC_EDIT4)->SetWindowTextW(pdlg->ID_PASSWORD);
	GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"");



	//MessageBox((CString)whatFormIs.c_str(),TEXT(""));

}
void settingsDlg::newUserFormIntialise(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"New User");
	GetDlgItem(IDC_STATIC2)->SetWindowTextW(L"Name");
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(L"User NAME");
	GetDlgItem(IDC_STATIC4)->SetWindowTextW(L"User PWD");
	GetDlgItem(IDC_STATIC5)->SetWindowTextW(L"Auth Type");
	GetDlgItem(IDC_STATIC6)->SetWindowTextW(L"Bot test");


	m_ComboBoxStatus1.AddString((CString)"Admin");
	m_ComboBoxStatus1.AddString((CString)"User");
	m_ComboBoxStatus1.SelectString(0,(CString)"User");

	GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"");

	pdlg->moveControlsOnSize(GetDlgItem(IDC_EDIT4),this,rectWindowStateACTUAL,rectWindowState,2,-26);
	pdlg->moveControlsOnSize(GetDlgItem(IDC_COMBO1),this,rectWindowStateACTUAL,rectWindowState,2,-26);

	GetDlgItem(IDC_BUTTON1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK2)->ShowWindow(false);
	GetDlgItem(IDC_EDIT3)->ShowWindow(false);
	GetDlgItem(IDC_STATIC7)->ShowWindow(false);
	GetDlgItem(IDC_EDIT5)->ShowWindow(true);
	GetDlgItem(IDC_EDIT6)->ShowWindow(true);

	m_ComboBoxStatus2.ShowWindow(false);

	isRegistred=false;
	keyValue=genKeyP(); // generate unique key
	GetDlgItem(IDC_EDIT5)->SetWindowTextW(keyValue);

}
void settingsDlg::addAccountFormIntialise(string editAccount){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	

	m_ComboBoxStatus1.AddString((CString)"Admin");
	m_ComboBoxStatus1.AddString((CString)"User");
	m_ComboBoxStatus2.AddString((CString)"Confirmat");
	m_ComboBoxStatus2.AddString((CString)"Neconfirmat");


	if(editAccount=="addAccount"){
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"Add account");
		m_ComboBoxStatus1.SelectString(0,(CString)"User");
		m_ComboBoxStatus2.SelectString(0,(CString)"Neconfirmat");


	}else if(editAccount=="editAccount"){
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"Edit account");
		ID_OLDUSERNAME = ID_USERNAME;
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(ID_NAME);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(ID_TYPEIMEI);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(ID_USERNAME);
		GetDlgItem(IDC_EDIT4)->SetWindowTextW(ID_PASSWORD);
		m_ComboBoxStatus1.SelectString(0,(CString)ID_AUTH);
		m_ComboBoxStatus2.SelectString(0,(CString)ID_CONFIRMED);
		GetDlgItem(IDC_EDIT4)->EnableWindow(false);
	}

	GetDlgItem(IDC_STATIC2)->SetWindowTextW(L"Name");
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(L"Type/Imei");
	GetDlgItem(IDC_STATIC4)->SetWindowTextW(L"User NAME");
	GetDlgItem(IDC_STATIC5)->SetWindowTextW(L"User PWD");
	GetDlgItem(IDC_STATIC6)->SetWindowTextW(L"Auth Type");
	GetDlgItem(IDC_STATIC7)->SetWindowTextW(L"Confirmed");



	GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"");

	GetDlgItem(IDC_BUTTON1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK2)->ShowWindow(false);

	GetDlgItem(IDC_COMBO1)->EnableWindow(true);

}
void settingsDlg::addProductFormIntialise(string editProduct){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	if(editProduct=="editProduct"){
		ID_OLDPRODUS=ID_PRODUS;
		ID_OLDCATEGORIE=ID_CATEGORIE;
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(ID_PRODUS);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(ID_CATEGORIE);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(ID_STOC);
		GetDlgItem(IDC_EDIT8)->SetWindowTextW(ID_PRET);

		GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"Edit Product");
		GetDlgItem(IDC_STATIC6)->ShowWindow(false);
	}else if(editProduct=="addProduct"){
		GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"New Product");
		GetDlgItem(IDC_STATIC6)->ShowWindow(false);
	}else if(editProduct=="buyProduct"){
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(ID_PRODUS);
		GetDlgItem(IDC_EDIT2)->SetWindowTextW(ID_CATEGORIE);
		GetDlgItem(IDC_EDIT3)->SetWindowTextW(ID_STOC);
		GetDlgItem(IDC_EDIT8)->SetWindowTextW(ID_PRET);

		GetDlgItem(IDC_EDIT1)->EnableWindow(false);
		GetDlgItem(IDC_EDIT2)->EnableWindow(false);
		GetDlgItem(IDC_EDIT3)->EnableWindow(false);
		GetDlgItem(IDC_EDIT8)->EnableWindow(false);

		GetDlgItem(IDC_STATIC6)->SetWindowTextW(L"Cantitate");

		GetDlgItem(IDC_STATIC1)->SetWindowTextW(L"Buy Product");
		GetDlgItem(IDC_EDIT9)->ShowWindow(true);
	}
	GetDlgItem(IDC_STATIC2)->SetWindowTextW(L"Produs");
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(L"Categorie");
	GetDlgItem(IDC_STATIC4)->SetWindowTextW(L"Stoc");
	GetDlgItem(IDC_STATIC5)->SetWindowTextW(L"Pret");

	GetDlgItem(IDC_EDIT7)->SetWindowTextW((CString)"");


	GetDlgItem(IDC_BUTTON1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK1)->ShowWindow(false);
	GetDlgItem(IDC_CHECK2)->ShowWindow(false);

	GetDlgItem(IDC_STATIC7)->ShowWindow(false);
	GetDlgItem(IDC_EDIT4)->ShowWindow(false);

	GetDlgItem(IDC_EDIT8)->ShowWindow(true);

	m_ComboBoxStatus1.ShowWindow(false);
	m_ComboBoxStatus2.ShowWindow(false);

//	pdlg->moveControlsOnSize(this,this,rectWindowStateACTUAL,rectWindowState,3,-30); //



}


void settingsDlg::settingsFormOk(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	CButton* pBtn1 = (CButton*) GetDlgItem(IDC_CHECK1);
	CButton* pBtn2 = (CButton*) GetDlgItem(IDC_CHECK2);
	bool semafor=false;
	CString stringTest;
	CString idcEdit1Test;

	GetDlgItem(IDC_EDIT1)->GetWindowTextW(idcEdit1Test);
	if(idcEdit1Test.IsEmpty()==true){
		stringTest+="*Introduceti un ip.\r\n";
		semafor=true;
	}
	if(verificareString(IDC_EDIT2,true)==false){
		stringTest+="*Introduceti un port format doar din numere.\r\n";
		semafor=true;
	}
	if(verificareString(IDC_EDIT3,false)==false){
		stringTest+="*Introduceti un nume de utilizator format doar din numere/litere.\r\n";
		semafor=true;
	}
	if(verificareString(IDC_EDIT4,false)==false){
		stringTest+="*Introduceti o parola formata doar din numere/litere.\r\n";
		semafor=true;
	}


  	if(semafor==false){
		if( pBtn1->GetCheck()==1 ){
			pdlg->autoEnableListen=true;
		}else{
			pdlg->autoEnableListen=false;
		}
		if( pBtn2->GetCheck()==1 ){
			pdlg->autoEnableChat=true;
			if(pdlg->isDisconnected==false)
				pdlg->GetDlgItem(IDOK)->EnableWindow(true);
		}else{
			pdlg->autoEnableChat=false;
			pdlg->GetDlgItem(IDOK)->EnableWindow(false);
		}

		GetDlgItem(IDC_EDIT1)->GetWindowTextW(pdlg->ID_IP);
		GetDlgItem(IDC_EDIT2)->GetWindowTextW(pdlg->ID_PORT);
		GetDlgItem(IDC_EDIT3)->GetWindowTextW(pdlg->ID_USERNAME);
		GetDlgItem(IDC_EDIT4)->GetWindowTextW(pdlg->ID_PASSWORD);
		//m_ComboBoxStatus1.GetLBText(m_ComboBoxStatus1.GetCurSel(), pdlg->ID_AUTH);
	
		pdlg->startXML(false);

		OnOK();
	}else{
		GetDlgItem(IDC_EDIT7)->SetWindowTextW(stringTest);
	
	}
}
void settingsDlg::newUserFormOk(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	if(isRegistred==false){

		bool semafor=false;
		CString strTest;
		CString keyTest;

		GetDlgItem(IDC_EDIT6)->GetWindowTextW(keyTest);

		if(verificareString(IDC_EDIT1,false)==false){
			strTest+="*Introduceti un nume/prenume format doar din numere/litere.\r\n";
			semafor=true;
		}
		if(verificareString(IDC_EDIT2,false)==false){
			strTest+="*Introduceti un nume de utilizator format doar din numere/litere.\r\n";
			semafor=true;
		}
		if(verificareString(IDC_EDIT4,false)==false){
			strTest+="*Introduceti o parola formata doar din numere/litere.\r\n";
			semafor=true;
		}
		if(keyTest!=keyValue){
			strTest+="*Codul introdus este gresit.\r\n";
			semafor=true;
		}


		if(semafor==false){
			GetDlgItem(IDC_EDIT1)->GetWindowTextW(pdlg->ID_NAME);
			GetDlgItem(IDC_EDIT2)->GetWindowTextW(pdlg->ID_USERNAME);
			GetDlgItem(IDC_EDIT4)->GetWindowTextW(pdlg->ID_PASSWORD);
			GetDlgItem(IDC_COMBO1)->GetWindowTextW(pdlg->ID_AUTH);

			pdlg->isRegistering=true;
			pdlg->connecToServer();
		}else{
			keyValue=genKeyP(); // generate unique key
			GetDlgItem(IDC_EDIT5)->SetWindowTextW(keyValue);
			GetDlgItem(IDC_EDIT7)->SetWindowTextW(strTest);
		}
	}

	if(isRegistred==true){
		CDialogEx::OnOK();
		pdlg->m_settingsDlg.OnBnClickedCancel();
	}
}
void settingsDlg::addAccountFormOk(string editAccount){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	bool semafor=false;

	CString stringTest;




	if(verificareString(IDC_EDIT1,false)==false){
		stringTest+="*Introduceti un nume/prenume format doar din numere/litere.\r\n";
		semafor=true;
	}
	if(verificareString(IDC_EDIT2,true)==false){
		stringTest+="*Introduceti un IMEI format doar din numere.\r\n";
		semafor=true;
	}	
	if(verificareString(IDC_EDIT3,false)==false){
		stringTest+="*Introduceti un nume de utilizator format doar din numere/litere.\r\n";
		semafor=true;
	}
	if(editAccount=="addAccount"){
		if(verificareString(IDC_EDIT4,false)==false){
			stringTest+="*Introduceti o parola formata doar din numere/litere.\r\n";
			semafor=true;
		}
	}


  	if(semafor==false){
		GetDlgItem(IDC_EDIT1)->GetWindowTextW(ID_NAME);
		GetDlgItem(IDC_EDIT2)->GetWindowTextW(ID_TYPEIMEI);
		GetDlgItem(IDC_EDIT3)->GetWindowTextW(ID_USERNAME);
		GetDlgItem(IDC_EDIT4)->GetWindowTextW(ID_PASSWORD);
		m_ComboBoxStatus1.GetLBText(m_ComboBoxStatus1.GetCurSel(), ID_AUTH); // from getCurSel to ID_AUTH....
		m_ComboBoxStatus2.GetLBText(m_ComboBoxStatus2.GetCurSel(), ID_CONFIRMED);
		if(ID_CONFIRMED=="Confirmat")
			ID_CONFIRMED="1";
		else
			ID_CONFIRMED="0";

		if(editAccount=="addAccount"){
			pdlg->sendAddAccount();
		}else if(editAccount=="editAccount"){
			pdlg->sendEditAccount();
			OnOK();
		}
	}else{
		GetDlgItem(IDC_EDIT7)->SetWindowTextW(stringTest);
	
	}
}
void settingsDlg::addProductFormOk(string editProduct){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	bool semafor=false;

	CString stringTest;



	if(verificareString(IDC_EDIT1,false)==false){
		stringTest+="*Introduceti un produs format doar din numere/litere.\r\n";
		semafor=true;
	}
	if(verificareString(IDC_EDIT2,false)==false){
		stringTest+="*Introduceti o categorie formata doar din numere/litere.\r\n";
		semafor=true;
	}	
	if(verificareString(IDC_EDIT3,true)==false){
		stringTest+="*Introduceti stocul format din numere.\r\n";
		semafor=true;
	}
	if(verificareString(IDC_EDIT8,true)==false){
		stringTest+="*Introduceti pretul format din numere.\r\n";
		semafor=true;
	}	
	if(editProduct=="buyProduct"){
		if(verificareString(IDC_EDIT9,true)==false){
			stringTest+="*Introduceti cantitatea formata din numere.\r\n";
			semafor=true;
		}
	}

  	if(semafor==false){

		GetDlgItem(IDC_EDIT1)->GetWindowTextW(ID_PRODUS);
		GetDlgItem(IDC_EDIT2)->GetWindowTextW(ID_CATEGORIE);
		GetDlgItem(IDC_EDIT3)->GetWindowTextW(ID_STOC);
		GetDlgItem(IDC_EDIT8)->GetWindowTextW(ID_PRET);


		if(editProduct=="addProduct"){
			pdlg->sendAddNewProduct();
		}else if(editProduct=="editProduct"){
			pdlg->sendUpdateProduct();
			OnOK();
		}else if(editProduct=="buyProduct"){


			GetDlgItem(IDC_EDIT9)->GetWindowTextW(ID_CANTITATE);
			pdlg->m_productsDlg.functionOfflineAddToBuy();
			OnOK();
		}


	}else{
		GetDlgItem(IDC_EDIT7)->SetWindowTextW(stringTest);
	
	}

}
CString settingsDlg::genKeyP(){
	__int64 number;
	srand ( time(NULL) );
	number = rand()+rand();

	// converting
	char buff[10]={0};
	_i64toa(number,buff,10);
	CString test;
	test=buff;
	return test;
}
void settingsDlg::setWhatFormIs(string whatForm){
	whatFormIs=whatForm;
}

