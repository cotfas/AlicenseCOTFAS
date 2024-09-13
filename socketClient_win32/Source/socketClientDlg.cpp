
// socketClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "socketClient.h" // legatura inapoi
#include "socketClientDlg.h"
#include "afxdialogex.h"


// GridList include
#include "..\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\CGridListCtrlEx\ViewConfigSection.h"
#include "Conex.h" // legatura catre


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsocketClientDlg dialog




CsocketClientDlg::CsocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);//////////
	DDX_Control(pDX, IDC_LIST2, m_EditList);
	DDX_Control(pDX, IDC_EDIT1, m_EditChat);
}

BEGIN_MESSAGE_MAP(CsocketClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON1, &CsocketClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CsocketClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CsocketClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CsocketClientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CsocketClientDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CsocketClientDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CsocketClientDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDOK, &CsocketClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CsocketClientDlg::OnBnClickedCancel)

	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()

	ON_WM_EXITSIZEMOVE()

END_MESSAGE_MAP()


// CsocketClientDlg message handlers

BOOL CsocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon



	// TODO: Add extra initialization here
	applicationInitialise(); /////

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CsocketClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


	if(isMaximized==true){
		isMaximized=false;
		OnExitSizeMove();
	}
	if(IsZoomed()!=0){
		isMaximized=true;
		OnExitSizeMove();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CsocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}








// ---------------------------------------------------------------------------------------- //
void CsocketClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DefWindowProc(WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}
void CsocketClientDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(IsZoomed()==0)
		ShowWindow(SW_SHOWMAXIMIZED);
	else
		ShowWindow(SW_SHOWDEFAULT);

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
void CsocketClientDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	connecToServer();
}
void CsocketClientDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	m_settingsDlg.setWhatFormIs("settings");
	m_settingsDlg.DoModal(); //show settings dialog
}
void CsocketClientDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	m_productsDlg.DoModal();
}
void CsocketClientDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	m_accountsDlg.DoModal();
}
void CsocketClientDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	m_comandsDlg.DoModal();
}
void CsocketClientDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	FunctieKickUser(false);
}
void CsocketClientDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	FunctieKickUser(true);
}

void CsocketClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	sendChatMessage();
}
void CsocketClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	startXML(false);
	disconnectClient();
	isDisconnected=true;
	Sleep(200);
	CDialogEx::OnCancel();
}
void CsocketClientDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	
	
	if(nType!=1){ // 1 on minimize
		moveControlsOnSize(GetDlgItem(IDC_LIST1),this,rectWindowStateACTUAL,rectWindowState,0,0);
		moveControlsOnSize(GetDlgItem(IDC_LIST1),this,rectWindowStateACTUAL,rectWindowState,3,0);

		moveControlsOnSize(GetDlgItem(IDC_BUTTON1),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDC_BUTTON2),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDC_BUTTON3),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDC_BUTTON4),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDC_BUTTON5),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDC_BUTTON6),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDC_BUTTON7),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDCANCEL),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDOK),this,rectWindowStateACTUAL,rectWindowState,1,0);
		moveControlsOnSize(GetDlgItem(IDCANCEL),this,rectWindowStateACTUAL,rectWindowState,2,0);
		moveControlsOnSize(GetDlgItem(IDOK),this,rectWindowStateACTUAL,rectWindowState,2,0);
		moveControlsOnSize(GetDlgItem(IDC_EDIT1),this,rectWindowStateACTUAL,rectWindowState,0,0);
		moveControlsOnSize(GetDlgItem(IDC_LIST2),this,rectWindowStateACTUAL,rectWindowState,0,0);
		moveControlsOnSize(GetDlgItem(IDC_EDIT1),this,rectWindowStateACTUAL,rectWindowState,2,0);
		moveControlsOnSize(GetDlgItem(IDC_LIST2),this,rectWindowStateACTUAL,rectWindowState,2,0);

		rectWindowState=rectWindowStateACTUAL;
	}


}
void CsocketClientDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	// set the minimum tracking width // and the minimum tracking height of the window

	if(rectMinWindowState.IsRectEmpty()==false){
		lpMMI->ptMinTrackSize.x = rectMinWindowState.Width()-60;
		lpMMI->ptMinTrackSize.y = rectMinWindowState.Height()-30;
	}

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
void CsocketClientDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default

	reSizeListCtrlColumn(m_ListCtrl,GetDlgItem(IDC_LIST1),this,m_ListCtrl.GetColumnCount());
	CDialogEx::OnExitSizeMove();
}
// ---------------------------------------------------------------------------------------- //

























// ---------------------------------------------------------------------------------------- //
void  CsocketClientDlg::applicationInitialise(){
	// dialog window state
	this->GetWindowRect(&rectWindowState);
	this->ScreenToClient(&rectWindowState);
	rectMinWindowState=rectWindowState;
	
	/*string cmds;
	for(int i=0;i<1000;i++){
	 cmds = "produs='produs',categorie='categorie',stoc='stoc',pret='pret'";

	sql.dbInsertItem("products",cmds,"","");
	}*/
	


	isButtonConnectLocked=false;


	isDisconnected=true;
	isRegistering=false;
	slash="//";

	startXML(true);
	nItemPosition = -1;

	// add flag image
	m_ImageList.Create(16, 16, ILC_COLOR16 | ILC_MASK, 1, 0);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON4));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON6));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON8));

	// Give better margin to editors
//	m_ListCtrl.SetCellMargin(1.2);
//	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;
//	m_ListCtrl.SetDefaultRowTrait(pRowTrait);



	// insert columns
	string col[] = {"OS","NAME","USERNAME","ACCOUNT","IP[PORT]","TYPE/IMEI","CONNECTED TIME"};
	insertColumnInList(m_ListCtrl, GetDlgItem(IDC_LIST1),this, col, (sizeof(col)/sizeof(string)));
	


	// save state
	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("MainDLG"));
	pColumnProfile->AddProfile(_T("Default"));//pColumnProfile->AddProfile(_T("Special"));
	m_ListCtrl.SetupColumnConfig(pColumnProfile);	


	if(autoEnableListen==true){
		connecToServer();
	}
	reSizeListCtrlColumn(m_ListCtrl,GetDlgItem(IDC_LIST1),this,m_ListCtrl.GetColumnCount());

}
void CsocketClientDlg::moveControlsOnSize(CWnd* pCtl, CWnd* pForm, CRect& rectWindowStateACTUAL_, CRect& rectWindowState_, int move, int upDownValue){
	if (pCtl) { // control may not exist yet.
		CRect rectCtl; // Allocate CRect for control's position.
		pCtl->GetWindowRect(&rectCtl); // Get control's position.
		pForm->ScreenToClient(&rectCtl); // Convert from absolute screen coordinates to dialog-relative coordinates.

		pForm->GetWindowRect(&rectWindowStateACTUAL_);
		pForm->ScreenToClient(&rectWindowStateACTUAL_);


		int countX=0, countY=0;
		countX = rectWindowStateACTUAL_.right-rectWindowState_.right;

		if(upDownValue==0){
			countY = rectWindowStateACTUAL_.bottom-rectWindowState_.bottom;
		}else{
			countY = upDownValue;
		}

		// Resize control MoveWindow( x left , y top, width right horizontally, height bottom vertically, ....
		if(move==0){ // keep left -> extend in +- right
			pCtl->MoveWindow(rectCtl.left, rectCtl.top, rectCtl.Width()+countX, rectCtl.Height(), true);
		}else if(move==1){ // move left + move in right
			pCtl->MoveWindow(rectCtl.left+countX, rectCtl.top, rectCtl.Width(), rectCtl.Height(), true);
		}else if(move==2){ // move all control up / down
			pCtl->MoveWindow(rectCtl.left, rectCtl.top+countY, rectCtl.Width(), rectCtl.Height(), true);
		}else if(move==3){ // keep top -> extend +- down
			pCtl->MoveWindow(rectCtl.left, rectCtl.top, rectCtl.Width(), rectCtl.Height()+countY, true);
		}


	}
}
void CsocketClientDlg::insertStringInEditList(string StringName){
	CString cstring(StringName.c_str());
	int index = m_EditList.GetCount();
	m_EditList.AddEntry(cstring, RGB(255,255,255),index);
	//m_EditList.AddString(cstring);
	//int index = m_EditList.GetCount();
	m_EditList.SetCurSel(index);
}
void CsocketClientDlg::connecToServer(){
	if(isButtonConnectLocked==false){
		isButtonConnectLocked=true;
		if(clientSock.isSocketInit()==false){
			clientSock.initSocketLibrary();
			conex = new CConex(this);
			conex->start();
			GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON1)->SetWindowTextW((CString)"Connecting");
		}else{
			disconnectClient();
		}
	}
}
void CsocketClientDlg::isConnected(){
	isDisconnected=false;
	string s;
	if(isRegistering==false){
		s = "CLIENT_CONNECT"+slash+convCStr2Str(ID_USERNAME)+slash+convCStr2Str(ID_AUTH)+slash+get_cpuid()+slash+getWindowsVersion()+slash+convCStr2Str(ID_PASSWORD);	
		if(autoEnableChat==true)
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW((CString)"Connected");
	}else{
		s = "CLIENT_REGISTER"+slash+convCStr2Str(ID_USERNAME)+slash+"NULL"+slash+get_cpuid()+slash+getWindowsVersion()+slash+convCStr2Str(ID_NAME)+slash+convCStr2Str(ID_PASSWORD);
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW((CString)"Registering");
	}
	sendMsg(s);
	isButtonConnectLocked=false;
}
void CsocketClientDlg::disconnectClient(){
	if(isDisconnected==false){
		string s = "CLIENT_DISCONNECT"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+"NULL";
		sendMsg(s);
	}
}
void CsocketClientDlg::sendChatMessage(){
	CString s;
	m_EditChat.GetWindowTextW(s);

	if(s!=""){
		string sTemp;
		vector<int> selectedRows = getSelectedItemFromList(m_ListCtrl);
		if(selectedRows.empty()==false){
			for(int i=selectedRows.size()-1;i>=0;i--){
				CString user,account;
				user=m_ListCtrl.GetItemText(selectedRows.at(i)-1,SearchListCtrlColumn(m_ListCtrl,"USERNAME"));
				account=m_ListCtrl.GetItemText(selectedRows.at(i)-1,SearchListCtrlColumn(m_ListCtrl,"ACCOUNT"));
				sTemp = "SEND_MESSAGETO"+slash+convCStr2Str(user)+slash+convCStr2Str(account)+slash+convCStr2Str(s);
				sendMsg(sTemp);
			}
		}else{
			sTemp = "SEND_MESSAGETOALL"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+convCStr2Str(s);
			sendMsg(sTemp);
		}
	}
	m_EditChat.SetWindowTextW((LPCTSTR)"");
	m_EditChat.SetFocus();
}
void CsocketClientDlg::sendMsg(string message){
	if(isDisconnected==false){
		conex->sendMessage(message);
	}else{
		insertStringInEditList("[APP]Inainte de orice operatiune, va rugam sa va conectati!");
	}
	
}
void CsocketClientDlg::sendAddAccount(){
	string s = "DB_ADD"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_NAME)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_TYPEIMEI)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_USERNAME)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_AUTH)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_CONFIRMED)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_PASSWORD);
	sendMsg(s);
}
void CsocketClientDlg::sendEditAccount(){
	string s = "DB_EDITUSER"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_NAME)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_TYPEIMEI)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_USERNAME)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_AUTH)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_CONFIRMED)+slash+
		convCStr2Str(m_accountsDlg.m_addAccountDlg.ID_OLDUSERNAME);
	sendMsg(s);
}
void CsocketClientDlg::sendGetAllUsers(){
	string s = "DB_GETALLUSERS"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+"NULL";
	sendMsg(s);
}
void CsocketClientDlg::sendDeleteUserDB(CString userNameT, CString authTypeT){
	string s = "DB_DELETEUSER"+slash+convCStr2Str(userNameT)+slash+convCStr2Str(authTypeT)+slash+"NULL";
	sendMsg(s);
}
void CsocketClientDlg::sendConfirmedUserDB(CString userNameT, CString authTypeT, CString confirmedT){
	string s = "DB_UPDATEUSER"+slash+convCStr2Str(userNameT)+slash+convCStr2Str(authTypeT)+slash+convCStr2Str(confirmedT);
	sendMsg(s);
}
void CsocketClientDlg::sendGetAllProducts(CConex * remote){
	string s = "DB_GETALLPRODUCTS"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+sql.dbLastRowId("products");
	//sendMsg(s);
	remote->sendMessage(s);

}
void CsocketClientDlg::sendAddNewProduct(){
	string s = "DB_ADDNEWPRODUCT"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_PRODUS)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_CATEGORIE)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_STOC)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_PRET);
	sendMsg(s);
}
void CsocketClientDlg::sendDeleteProductDB(CString produsT, CString categorieT){
	string s = "DB_DELETEPRODUCT"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		convCStr2Str(produsT)+slash+
		convCStr2Str(categorieT);
	sendMsg(s);
}
void CsocketClientDlg::sendUpdateProduct(){
	string s = "DB_UPDATEPRODUCT"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_OLDPRODUS)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_OLDCATEGORIE)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_PRODUS)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_CATEGORIE)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_STOC)+slash+
		convCStr2Str(m_productsDlg.m_addProductDlg.ID_PRET);
	sendMsg(s);
}
void CsocketClientDlg::sendKickUser(CString userNameT, CString authTypeT){
	string s = "CLIENT_KICK"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		convCStr2Str(userNameT)+slash+
		convCStr2Str(authTypeT)+slash;
	sendMsg(s);
}
void CsocketClientDlg::sendNewBuy(string newBuy){
	string s = "DB_NEWBUY"+slash+
		convCStr2Str(ID_THISUSERNAME)+slash+
		convCStr2Str(ID_THISAUTH)+slash+
		newBuy;
	sendMsg(s);

}
void CsocketClientDlg::sendGetAllComands(){
	string s = "DB_GETALLCOMANDS"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+"NULL";
	sendMsg(s);
}
void CsocketClientDlg::sendDeleteComandDB(string idComandT){
	string s = "DB_DELETECOMAND"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+idComandT;
	sendMsg(s);
}
void CsocketClientDlg::sendUpdateComand(string idComandT, string confirmedT){
	string s = "DB_UPDATECOMAND"+slash+convCStr2Str(ID_THISUSERNAME)+slash+convCStr2Str(ID_THISAUTH)+slash+idComandT+slash+confirmedT;
	sendMsg(s);
}

void CsocketClientDlg::FunctieKickUser(bool banUser){
	vector<int> selectedRows = getSelectedItemFromList(m_ListCtrl);

	if(selectedRows.empty()==false){
		for(int i=selectedRows.size()-1;i>=0;i--){
			CString userNameT,accountT;
			userNameT=m_ListCtrl.GetItemText(selectedRows.at(i)-1,SearchListCtrlColumn(m_ListCtrl,"USERNAME"));
			accountT=m_ListCtrl.GetItemText(selectedRows.at(i)-1,SearchListCtrlColumn(m_ListCtrl,"ACCOUNT"));
			sendKickUser(userNameT,accountT);
			if(banUser==true){
				sendConfirmedUserDB(userNameT,accountT,(CString)"0");
			}
		}
	}else{
		insertStringInEditList("[APP]Selectati cel putin un camp din lista." );
	}
}
void CsocketClientDlg::FunctieEliberareRemoteSock(CConex * remote){
	objMutexRemoveSocket.lock();
	if(remote==conex){
		if((isRegistering==false)&&(isDisconnected==false))
			insertStringInEditList("[APP]Ai fost deconectat de la server!");
		if(m_accountsDlg.isInitialise==true) // close accounts window
			m_accountsDlg.PostMessageW(WM_CLOSE,0,0);
		if(m_comandsDlg.isInitialise==true) // close comands window
			m_comandsDlg.PostMessageW(WM_CLOSE,0,0);
		if(m_productsDlg.isInitialise==true){ // products
			m_productsDlg.GetDlgItem(IDC_BUTTON2)->ShowWindow(false);
			m_productsDlg.GetDlgItem(IDC_BUTTON3)->ShowWindow(false);
			m_productsDlg.GetDlgItem(IDC_BUTTON4)->ShowWindow(false);
		}


		isRegistering=false;
		isDisconnected=true;
		clientSock.closeSocketLibrary();
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW((CString)"Connect");
		GetDlgItem(IDC_BUTTON3)->SetWindowTextW((CString)"Products");	
		GetDlgItem(IDOK)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(false);
		GetDlgItem(IDC_BUTTON5)->ShowWindow(false);
		GetDlgItem(IDC_BUTTON6)->ShowWindow(false);
		GetDlgItem(IDC_BUTTON7)->ShowWindow(false);

		m_ListCtrl.DeleteAllItems(); // stergere lista
		isButtonConnectLocked=false;
	}
	delete[] remote;
	objMutexRemoveSocket.unlock();
}
wstring CsocketClientDlg::convStr2ws(const std::string& s){
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
string CsocketClientDlg::convCStr2Str(CString strForConv){
	CT2CA pszConvertedAnsiString (strForConv);// Convert a TCHAR string to a LPCSTR
	std::string strStd (pszConvertedAnsiString);// construct a std::string using the LPCSTR input
	return (strStd);// construct a std::string using the LPCSTR input
}
int CsocketClientDlg::convCStringToInt(CString strForConv){
    CStringA ansiString(strForConv);
	return atoi(ansiString);
}
string CsocketClientDlg::convLong2Str(long value){
    std::stringstream ss;
    ss << value; 
    std::string result=ss.str();  
	return result;
}
void CsocketClientDlg::startXML(bool read){//, CString ipT, CString portT, CString userNameT, CString passwordT, CString authTypeT, CString autoEnableListenT, CString autoEnableChatT){
	const char* xmlPath = "configClient.xml";

	if(read==true){ // read values
		if(xml.loadFile(xmlPath)==false){ // first start
			this->GetWindowRect(&rectWindowStateTMP);

			map<string,string> Values;
			Values["ID_IP"]="127.0.0.1";
			Values["ID_PORT"]="3000";
			Values["ID_USERNAME"]="admin";
			Values["ID_PASSWORD"]="admin";
			//Values["ID_AUTH"]="Admin";
			Values["autoEnableListen"]="true";
			Values["autoEnableChat"]="true";

			Values["mainRectBottom"]=convLong2Str(rectWindowStateTMP.bottom);
			Values["mainRectTop"]=convLong2Str(rectWindowStateTMP.top);
			Values["mainRectLeft"]=convLong2Str(rectWindowStateTMP.left);
			Values["mainRectRight"]=convLong2Str(rectWindowStateTMP.right);
			Values["mainMaximized"]="false";

			xml.writeToXML(xmlPath,Values);
			xml.loadFile(xmlPath);
		}


		rectWindowStateTMP.bottom=atol(xml.getXMLValues()["mainRectBottom"].c_str()); // atol for conv string to long
		rectWindowStateTMP.top=atol(xml.getXMLValues()["mainRectTop"].c_str());
		rectWindowStateTMP.left=atol(xml.getXMLValues()["mainRectLeft"].c_str());
		rectWindowStateTMP.right=atol(xml.getXMLValues()["mainRectRight"].c_str());
		
		if(xml.getXMLValues()["mainMaximized"]=="false"){
			this->MoveWindow(rectWindowStateTMP, true);
		}else{
			ShowWindow(SW_SHOWMAXIMIZED);
		}


		ID_IP=xml.getXMLValues()["ID_IP"].c_str();
		ID_PORT=xml.getXMLValues()["ID_PORT"].c_str();
		ID_USERNAME=xml.getXMLValues()["ID_USERNAME"].c_str();
		ID_PASSWORD=xml.getXMLValues()["ID_PASSWORD"].c_str();
		//ID_AUTH=xml.getXMLValues()["ID_AUTH"].c_str();
		if(xml.getXMLValues()["autoEnableListen"]=="true"){
			autoEnableListen=true;
		}else{
			autoEnableListen=false;
		}
		if(xml.getXMLValues()["autoEnableChat"]=="true"){
			autoEnableChat=true;
		}else{
			autoEnableChat=false;
		}
	




	}else{ // write values
		map<string,string> Values;
		Values["ID_IP"]=convCStr2Str(ID_IP);
		Values["ID_PORT"]=convCStr2Str(ID_PORT);
		Values["ID_USERNAME"]=convCStr2Str(ID_USERNAME);
		Values["ID_PASSWORD"]=convCStr2Str(ID_PASSWORD);
		//Values["ID_AUTH"]=convCStr2Str(ID_AUTH);
		if(autoEnableListen==true){
			Values["autoEnableListen"]="true";
		}else{
			Values["autoEnableListen"]="false";
		}
		if(autoEnableChat==true){
			Values["autoEnableChat"]="true";
		}else{
			Values["autoEnableChat"]="false";	
		}

		if(IsZoomed()==0){
			Values["mainMaximized"]="false";
			this->GetWindowRect(&rectWindowStateTMP);
		}else{
			Values["mainMaximized"]="true";	
		}

		Values["mainRectBottom"]=convLong2Str(rectWindowStateTMP.bottom);
		Values["mainRectTop"]=convLong2Str(rectWindowStateTMP.top);
		Values["mainRectLeft"]=convLong2Str(rectWindowStateTMP.left);
		Values["mainRectRight"]=convLong2Str(rectWindowStateTMP.right);

		xml.writeToXML(xmlPath,Values);	
	
	}
}
string CsocketClientDlg::getWindowsVersion(){
	HKEY hKey;
	wchar_t mydata[2048];
	DWORD dataLength = sizeof(mydata);
	DWORD dwType = REG_SZ;
	LONG regOpenCriss = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);
	if (regOpenCriss == ERROR_SUCCESS) {
		RegQueryValueEx(hKey,TEXT("ProductName"), 0, &dwType, (BYTE*)mydata, &dataLength);
		string value = convCStr2Str(mydata);
		return value;
	}
	return 0;
}
string CsocketClientDlg::get_cpuid(void){
	UINT64 value;
	DWORD dwStandard = 0; 
	DWORD dwFeature = 0; 
     
	_asm { 
		mov eax, 1 
		cpuid 
		mov dwStandard, eax 
		mov dwFeature, edx 
   }
   value = ((UINT64)(dwFeature) << 32) | ((UINT64)(dwStandard));

   std::ostringstream os;
   os << value;
   return os.str();
}
void CsocketClientDlg::insertColumnInList(CListCtrl& oListCtrl, CWnd *pWnd, CWnd *pForm, string itemColumn[], int size){
	CRect rect;
	//CWnd *pWnd = GetDlgItem(nID);
	pWnd->GetWindowRect(&rect);//size: rect.Width(), rect.Height()
	pForm->ScreenToClient(&rect);

	for(int i=0; i < size; i++){
		oListCtrl.InsertColumn(i+1, (CString)itemColumn[i].c_str(), LVCFMT_LEFT, (rect.Width()/size)-5, i);
	
	}
}
void CsocketClientDlg::insertItemInList(CGridListCtrlGroups& oListCtrl,string itemNames[], string wichIcon){
	oListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL); // set list of icons
	int nItem;
	
	if(nItemPosition==-1)
		nItem=oListCtrl.GetItemCount();
	else
		nItem=nItemPosition;


	// insert item in list
	oListCtrl.InsertItem(nItem, (CString)itemNames[0].c_str());
	for(int i=1; i < oListCtrl.GetColumnCount(); i++){
			oListCtrl.SetItemText(nItem, i, (CString)itemNames[i].c_str());
	}

	// set selection
	if(nItemPosition!=-1){
		oListCtrl.SetItemState(nItemPosition, LVIS_SELECTED, LVIS_SELECTED);
		nItemPosition = -1;
	}

	// set item icon
	if(wichIcon=="accounts"){
		oListCtrl.SetCellImage(nItem,0,2); // accounts icon
	}else if(wichIcon=="main"){
		if(itemNames[0]=="Android"){
			oListCtrl.SetCellImage(nItem,0,0); // android icon
		}else{
			oListCtrl.SetCellImage(nItem,0,1); // windows icon
		}
	}else if(wichIcon=="products"){
		oListCtrl.SetCellImage(nItem,0,3); // products icon
	}else if(wichIcon=="comands"){
		oListCtrl.SetCellImage(nItem,0,4); // products icon
	}
}
void CsocketClientDlg::deleteItemList(CListCtrl& oListCtrl, int itemToDelete){
	if(itemToDelete==-1){
		vector<int> selectedRows = getSelectedItemFromList(oListCtrl);
		if(selectedRows.empty()==false){
			for(int i=selectedRows.size()-1;i>=0;i--){
				oListCtrl.DeleteItem(selectedRows.at(i)-1);
			}
		}else{
			insertStringInEditList("[APP]Selectati cel putin un camp din lista." );
		}
	}else{
		oListCtrl.DeleteItem(itemToDelete);
	}
}
vector<int> CsocketClientDlg::getSelectedItemFromList(CListCtrl& oListCtrl){
	vector<int> allValues;
	POSITION nItem = oListCtrl.GetFirstSelectedItemPosition();
	int index = 0;
	if(nItem!=0){
		while(nItem!=0){
			allValues.push_back((int)nItem);
			oListCtrl.GetNextSelectedItem(nItem);
		}

	}else if(nItem==0){
		return allValues;
	}
	return allValues;
}
int CsocketClientDlg::findItemListToDelete(CListCtrl& oListCtrl, string itemFound, string wichColumToFind){
	std::wstring stemp = convStr2ws(itemFound);
	LPCWSTR result = stemp.c_str();

	int indexRow = SearchListCtrlRow(oListCtrl,result,wichColumToFind,true,0);
	oListCtrl.DeleteItem(indexRow);
	return indexRow;
}
int CsocketClientDlg::SearchListCtrlRow(CListCtrl& oListCtrl, LPCTSTR psSearchText, string psColumn, bool bCaseSensitive, int nStartingRow){
	int nRow = -1;
	int nColumn=SearchListCtrlColumn(oListCtrl,psColumn);

	int nTotalRows = oListCtrl.GetItemCount();
	int nTotalColumns = oListCtrl.GetHeaderCtrl()->GetItemCount();

	

	if (nColumn<nTotalColumns && nStartingRow<nTotalRows && psSearchText!=NULL && *psSearchText!=0){
		for (int i=nStartingRow; i<nTotalRows && nRow<0; ++i){    
			CString sColumnText = oListCtrl.GetItemText(i, nColumn);
			if ((bCaseSensitive && sColumnText.Compare(psSearchText)==0) || (!bCaseSensitive && sColumnText.CompareNoCase(psSearchText)==0)){
				nRow = i;
			}
		}
	}
	return nRow;
}
int CsocketClientDlg::SearchListCtrlColumn(CListCtrl& oListCtrl, string psColumn){
	int nColumn=-1;
	int nTotalColumns = oListCtrl.GetHeaderCtrl()->GetItemCount();

	for(int i=0; i<nTotalColumns; i++){
		const INT SIZE = 0x100;
		TCHAR szText[SIZE];
		LVCOLUMN lvColInfo;
		lvColInfo.mask = LVCF_TEXT;
		lvColInfo.pszText = szText;
		lvColInfo.cchTextMax = SIZE;
		oListCtrl.GetColumn(i, &lvColInfo);
		if (!_tcscmp(szText, convStr2ws (psColumn).c_str() )){ // compare
			nColumn=i;
		}
	}
	return nColumn;
}
void CsocketClientDlg::reSizeListCtrlColumn(CListCtrl& oListCtrl, CWnd* pWnd, CWnd* pForm, int columnCount){
	CRect rectCtl; // Allocate CRect for control's position.
	pWnd->GetWindowRect(&rectCtl); // Get control's position.
	pForm->ScreenToClient(&rectCtl); // Convert from absolute screen coordinates to dialog-relative coordinates.
	

	for(int i=0; i<columnCount;i++){
		oListCtrl.SetColumnWidth(i,rectCtl.Width()/columnCount-5);
	}
}
// ---------------------------------------------------------------------------------------- //



