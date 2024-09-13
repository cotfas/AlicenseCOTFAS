// Source\comandsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "socketClient.h"
#include "socketClientDlg.h"
#include "comandsDlg.h"
#include "afxdialogex.h"





// GridList include
#include "..\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\CGridListCtrlEx\ViewConfigSection.h"





// comandsDlg dialog

IMPLEMENT_DYNAMIC(comandsDlg, CDialogEx)

comandsDlg::comandsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(comandsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON7);
}

comandsDlg::~comandsDlg()
{
}

void comandsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlComands);//////////
}


BEGIN_MESSAGE_MAP(comandsDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDCANCEL, &comandsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &comandsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &comandsDlg::OnBnClickedButton2)
	ON_WM_PAINT()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()


// comandsDlg message handlers


void comandsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DefWindowProc(WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

void comandsDlg::OnPaint()
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

void comandsDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	pdlg->reSizeListCtrlColumn(m_ListCtrlComands,GetDlgItem(IDC_LIST1),this,m_ListCtrlComands.GetColumnCount());

	CDialogEx::OnExitSizeMove();
}

void comandsDlg::OnSize(UINT nType, int cx, int cy)
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
	
		rectWindowState=rectWindowStateACTUAL;
	}

}


void comandsDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(IsZoomed()==0)
		ShowWindow(SW_SHOWMAXIMIZED);
	else
		ShowWindow(SW_SHOWDEFAULT);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void comandsDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	if(rectMinWindowState.IsRectEmpty()==false){
		lpMMI->ptMinTrackSize.x = rectMinWindowState.Width()-60;
		lpMMI->ptMinTrackSize.y = rectMinWindowState.Height()-100;
	}
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


BOOL comandsDlg::OnInitDialog()
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
void comandsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	isInitialise=false;
	CDialogEx::OnCancel();

}

void comandsDlg::initialiseDialog(){
	// dialog window state
	this->GetWindowRect(&rectWindowState);
	this->ScreenToClient(&rectWindowState);
	rectMinWindowState=rectWindowState;


	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	isInitialise=true;

	string col[] = {"NRCOMANDA","USERNAME","ACCOUNT","PRODUS","CATEGORIE","STOC","PRET","CANTITATE","CONFIRMAT","ADDED TIME"};
	pdlg->insertColumnInList(m_ListCtrlComands, GetDlgItem(IDC_LIST1),this, col, (sizeof(col)/sizeof(string)));

	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("ComandsDLG"));
	pColumnProfile->AddProfile(_T("Default"));//pColumnProfile->AddProfile(_T("Special"));
	m_ListCtrlComands.SetupColumnConfig(pColumnProfile);	

	if(pdlg->isDisconnected==false){
		pdlg->sendGetAllComands();
	}else{
		OnBnClickedCancel();
	}
}
void comandsDlg::functionConfirmCommand(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	
	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlComands);

	if(selectedRows.empty()==false){
		for(int i=selectedRows.size()-1;i>=0;i--){
			CString idcomand,confirmed;
			idcomand=m_ListCtrlComands.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlComands,"NRCOMANDA"));

			confirmed=m_ListCtrlComands.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlComands,"CONFIRMAT"));

			if(confirmed=="Confirmat")
				confirmed="0";
			else
				confirmed="1";


			pdlg->sendUpdateComand(pdlg->convCStr2Str(idcomand),pdlg->convCStr2Str(confirmed));
		}
	}
}
void comandsDlg::functionDeleteCommand(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	
	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlComands);

	if(selectedRows.empty()==false){
		for(int i=selectedRows.size()-1;i>=0;i--){
			CString idcomand;
			idcomand=m_ListCtrlComands.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlComands,"NRCOMANDA"));
			pdlg->sendDeleteComandDB(pdlg->convCStr2Str(idcomand));
		}
	}
}
void comandsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	functionConfirmCommand();
}
void comandsDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	functionDeleteCommand();
}

