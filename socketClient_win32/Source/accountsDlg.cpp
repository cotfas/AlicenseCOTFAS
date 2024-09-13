// accountsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "socketClient.h"
#include "socketClientDlg.h"
#include "accountsDlg.h"
#include "afxdialogex.h"


// GridList include
#include "..\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\CGridListCtrlEx\ViewConfigSection.h"



// accountsDlg dialog

IMPLEMENT_DYNAMIC(accountsDlg, CDialogEx)

accountsDlg::accountsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(accountsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON3);
}

accountsDlg::~accountsDlg()
{
}

void accountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlAccounts);//////////
}


BEGIN_MESSAGE_MAP(accountsDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON1, &accountsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &accountsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &accountsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &accountsDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDCANCEL, &accountsDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_EXITSIZEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// accountsDlg message handlers


void accountsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DefWindowProc(WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

void accountsDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	if(rectMinWindowState.IsRectEmpty()==false){
		lpMMI->ptMinTrackSize.x = rectMinWindowState.Width()-60;
		lpMMI->ptMinTrackSize.y = rectMinWindowState.Height()-100;
	}
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void accountsDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(IsZoomed()==0)
		ShowWindow(SW_SHOWMAXIMIZED);
	else
		ShowWindow(SW_SHOWDEFAULT);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}
void accountsDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	//pdlg->reSizeListCtrlColumn(m_ListCtrlAccounts,GetDlgItem(IDC_LIST1),this,m_ListCtrlAccounts.GetColumnCount());
	CDialogEx::OnExitSizeMove();
}
void accountsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	if(isMaximized==true){
		isMaximized=false;
		OnExitSizeMove();
	}
	if(IsZoomed()!=0){
		isMaximized=true;
		OnExitSizeMove();
	}
}

BOOL accountsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// initialise
	initialiseDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void accountsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	if(nType!=1){ // 1 on minimize
		pdlg->moveControlsOnSize(GetDlgItem(IDC_LIST1),this,rectWindowStateACTUAL,rectWindowState,0,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_LIST1),this,rectWindowStateACTUAL,rectWindowState,3,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDCANCEL),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDCANCEL),this,rectWindowStateACTUAL,rectWindowState,2,0);

		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON1),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON2),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON3),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON4),this,rectWindowStateACTUAL,rectWindowState,1,0);	
		rectWindowState=rectWindowStateACTUAL;
	}

}


void accountsDlg::initialiseDialog(){
	// dialog window state
	this->GetWindowRect(&rectWindowState);
	this->ScreenToClient(&rectWindowState);
	rectMinWindowState=rectWindowState;


	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	isInitialise=true;

	string col[] = {"NAME","TYPE/IMEI","USERNAME","ACCOUNT","CONFIRMED","STATUS"};
	pdlg->insertColumnInList(m_ListCtrlAccounts, GetDlgItem(IDC_LIST1),this, col, (sizeof(col)/sizeof(string)));

	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("AccountsDLG"));
	pColumnProfile->AddProfile(_T("Default"));//pColumnProfile->AddProfile(_T("Special"));
	m_ListCtrlAccounts.SetupColumnConfig(pColumnProfile);	


	if(pdlg->isDisconnected==false){
		pdlg->sendGetAllUsers();
	}else{
		OnBnClickedCancel();
	}
}

void accountsDlg::functionAddAccount(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	m_addAccountDlg.setWhatFormIs("addAccount");
	m_addAccountDlg.DoModal();
}
void accountsDlg::functionDeleteAccount(){
		CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	
	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlAccounts);

	if(selectedRows.empty()==false){
		for(int i=selectedRows.size()-1;i>=0;i--){
			CString user,account;
			user=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"USERNAME"));
			account=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"ACCOUNT"));
			pdlg->sendDeleteUserDB(user,account);
		}
	}
}
void accountsDlg::functionConfirmAccount(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	
	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlAccounts);

	if(selectedRows.empty()==false){
		for(int i=selectedRows.size()-1;i>=0;i--){
			CString user,account,confirmed;
			user=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"USERNAME"));
			account=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"ACCOUNT"));
			confirmed=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"CONFIRMED"));

			if(confirmed=="Confirmat")
				confirmed="0";
			else
				confirmed="1";
			pdlg->sendConfirmedUserDB(user,account,confirmed);
		}
	}
}
void accountsDlg::functionEditAccount(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	
	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlAccounts);
	if(selectedRows.empty()==false){
		m_addAccountDlg.setWhatFormIs("editAccount");
		for(int i=selectedRows.size()-1;i>=0;i--){
			m_addAccountDlg.ID_NAME=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"NAME"));
			m_addAccountDlg.ID_TYPEIMEI=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"TYPE/IMEI"));
			m_addAccountDlg.ID_USERNAME=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"USERNAME"));
			m_addAccountDlg.ID_AUTH=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"ACCOUNT"));
			m_addAccountDlg.ID_CONFIRMED=m_ListCtrlAccounts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlAccounts,"CONFIRMED"));
		}
		m_addAccountDlg.DoModal();
	}
}
void accountsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	functionAddAccount();
}
void accountsDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	functionDeleteAccount();
}
void accountsDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	functionConfirmAccount();
}
void accountsDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here	
	functionEditAccount();
}
void accountsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	isInitialise=false;
	CDialogEx::OnCancel();
}



