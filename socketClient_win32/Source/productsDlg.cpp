// productsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "socketClient.h"
#include "socketClientDlg.h"
#include "productsDlg.h"
#include "afxdialogex.h"



// GridList include
#include "..\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\CGridListCtrlEx\ViewConfigSection.h"



// productsDlg dialog

IMPLEMENT_DYNAMIC(productsDlg, CDialogEx)

productsDlg::productsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(productsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON5);
}

productsDlg::~productsDlg()
{
}

void productsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrlProducts);//////////
	DDX_Control(pDX, IDC_LIST2, m_ListCtrlBuy);//////////
}


BEGIN_MESSAGE_MAP(productsDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON1, &productsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &productsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &productsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &productsDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &productsDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &productsDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDCANCEL, &productsDlg::OnBnClickedCancel)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()


// productsDlg message handlers


void productsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	DefWindowProc(WM_SYSCOMMAND, SC_MOVE+1,MAKELPARAM(point.x,point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void productsDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	if(rectMinWindowState.IsRectEmpty()==false){
		lpMMI->ptMinTrackSize.x = rectMinWindowState.Width()-60;
		lpMMI->ptMinTrackSize.y = rectMinWindowState.Height()-0;
	}
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void productsDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(IsZoomed()==0)
		ShowWindow(SW_SHOWMAXIMIZED);
	else
		ShowWindow(SW_SHOWDEFAULT);
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void productsDlg::OnPaint()
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

void productsDlg::OnExitSizeMove()
{
	// TODO: Add your message handler code here and/or call default
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	//pdlg->reSizeListCtrlColumn(m_ListCtrlProducts,GetDlgItem(IDC_LIST1),this,m_ListCtrlProducts.GetColumnCount());
	//pdlg->reSizeListCtrlColumn(m_ListCtrlBuy,GetDlgItem(IDC_LIST1),this,m_ListCtrlBuy.GetColumnCount());

	CDialogEx::OnExitSizeMove();
}

void productsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi


	if(nType!=1){ // 1 on minimize
		pdlg->moveControlsOnSize(GetDlgItem(IDC_LIST1),this,rectWindowStateACTUAL,rectWindowState,0,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_LIST1),this,rectWindowStateACTUAL,rectWindowState,3,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_LIST2),this,rectWindowStateACTUAL,rectWindowState,0,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_LIST2),this,rectWindowStateACTUAL,rectWindowState,2,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDCANCEL),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDCANCEL),this,rectWindowStateACTUAL,rectWindowState,2,0);

		pdlg->moveControlsOnSize(GetDlgItem(IDC_STATIC1),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_STATIC1),this,rectWindowStateACTUAL,rectWindowState,2,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_STATIC4),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_STATIC4),this,rectWindowStateACTUAL,rectWindowState,2,0);


		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON1),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON2),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON3),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON4),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON5),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON6),this,rectWindowStateACTUAL,rectWindowState,1,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON5),this,rectWindowStateACTUAL,rectWindowState,2,0);
		pdlg->moveControlsOnSize(GetDlgItem(IDC_BUTTON6),this,rectWindowStateACTUAL,rectWindowState,2,0);

		rectWindowState=rectWindowStateACTUAL;
	}
}

BOOL productsDlg::OnInitDialog()
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

void productsDlg::initialiseDialog(){
	// dialog window state
	this->GetWindowRect(&rectWindowState);
	this->ScreenToClient(&rectWindowState);
	rectMinWindowState=rectWindowState;

	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	isInitialise=true;

	string col[] = {"PRODUS","CATEGORIE","STOC","PRET"};
	pdlg->insertColumnInList(m_ListCtrlProducts, GetDlgItem(IDC_LIST1),this, col, (sizeof(col)/sizeof(string)));
	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("ProductsDLG"));
	pColumnProfile->AddProfile(_T("Default"));//pColumnProfile->AddProfile(_T("Special"));
	m_ListCtrlProducts.SetupColumnConfig(pColumnProfile);

	string col2[] = {"PRODUS","CATEGORIE","STOC","PRET","CANTITATE"};
	pdlg->insertColumnInList(m_ListCtrlBuy, GetDlgItem(IDC_LIST2),this, col2, (sizeof(col2)/sizeof(string)));
	CViewConfigSectionWinApp* pColumnProfileBuy = new CViewConfigSectionWinApp(_T("BuyDLG"));
	pColumnProfileBuy->AddProfile(_T("Default"));//pColumnProfile->AddProfile(_T("Special"));
	m_ListCtrlBuy.SetupColumnConfig(pColumnProfileBuy);



	vector<vector<string> > result;
	result = pdlg->sql.dbGetItem("products","produs,categorie,stoc,pret","");
	for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it){
		vector<string> row = *it;
		string items[] = {row.at(0),row.at(1),row.at(2),row.at(3)};
		pdlg->insertItemInList(pdlg->m_productsDlg.m_ListCtrlProducts,items,"products");
	}

	functionUpdateBuyList();

	if(pdlg->isDisconnected==false){
		if(pdlg->ID_THISAUTH=="Admin"){
			GetDlgItem(IDC_BUTTON2)->ShowWindow(true);
			GetDlgItem(IDC_BUTTON3)->ShowWindow(true);
			GetDlgItem(IDC_BUTTON4)->ShowWindow(true);
		
		}
	}else{
	}



}
void productsDlg::functionAddToCart(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlProducts);
	if(selectedRows.empty()==false){
		m_addProductDlg.setWhatFormIs("buyProduct");

		for(int i=selectedRows.size()-1;i>=0;i--){
			m_addProductDlg.ID_PRODUS=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"PRODUS"));
			m_addProductDlg.ID_CATEGORIE=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"CATEGORIE"));
			m_addProductDlg.ID_STOC=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"STOC"));
			m_addProductDlg.ID_PRET=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"PRET"));
		}
		m_addProductDlg.DoModal();
	}
}
void productsDlg::functionAddNewProduct(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	m_addProductDlg.setWhatFormIs("addProduct");
	m_addProductDlg.DoModal();
}
void productsDlg::functionDeleteProduct(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi
	
	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlProducts);

	if(selectedRows.empty()==false){
		for(int i=selectedRows.size()-1;i>=0;i--){
			CString produs,categorie;
			produs=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"PRODUS"));
			categorie=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"CATEGORIE"));
			pdlg->sendDeleteProductDB(produs,categorie);
		}
	}
}
void productsDlg::functionModifyProduct(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlProducts);
	if(selectedRows.empty()==false){
		m_addProductDlg.setWhatFormIs("editProduct");

		for(int i=selectedRows.size()-1;i>=0;i--){
			m_addProductDlg.ID_PRODUS=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"PRODUS"));
			m_addProductDlg.ID_CATEGORIE=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"CATEGORIE"));
			m_addProductDlg.ID_STOC=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"STOC"));
			m_addProductDlg.ID_PRET=m_ListCtrlProducts.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlProducts,"PRET"));
		}
		m_addProductDlg.DoModal();
	}
}
void productsDlg::functionSendCartToServer(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	vector<vector<string> > result;

	string resultBuy;

	if(m_ListCtrlBuy.GetItemCount()!=0){
		if(pdlg->sql.dbGetItem("buy","idProdus,cantitate","").empty()==false){
			result=pdlg->sql.dbGetItem("buy","idProdus,cantitate","");
			for(vector<vector<string> >::iterator it = result.begin(); it < result.end(); ++it){
				vector<string> row = *it;
				resultBuy+=row.at(0)+pdlg->slash+row.at(1);
		
				resultBuy+=pdlg->slash;
			}
		
		}
		pdlg->sendNewBuy(resultBuy);
	}
	//functionOfflineDeleteFromBuy(true); // delete all items
}


void productsDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	functionAddToCart();
}
void productsDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	functionAddNewProduct();
}
void productsDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	functionDeleteProduct();
}
void productsDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	functionModifyProduct();
}
void productsDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	functionOfflineDeleteFromBuy(false);
}
void productsDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	functionSendCartToServer();
}
void productsDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	isInitialise=false;
	CDialogEx::OnCancel();
}

void productsDlg::functionOfflineAddToBuy(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	string cmds;
	string idProdus;

	cmds="produs='"+pdlg->convCStr2Str(m_addProductDlg.ID_PRODUS)+"',categorie='"+pdlg->convCStr2Str(m_addProductDlg.ID_CATEGORIE)+"'";	
	if(pdlg->sql.dbGetItem("products","id",cmds).empty()==false){
		idProdus = pdlg->sql.dbGetItem("products","id",cmds).begin()->at(0);
		cmds="idProdus='"+idProdus+"'";
		if(pdlg->sql.dbGetItem("buy","id",cmds).empty()==true){
			cmds="idProdus='"+idProdus+"',cantitate='"+pdlg->convCStr2Str(m_addProductDlg.ID_CANTITATE)+"'";
			pdlg->sql.dbInsertItem("buy",cmds);
		}
	}

	functionUpdateBuyList();
}

void productsDlg::functionOfflineDeleteFromBuy(bool allProducts){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	string cmds;
	string idProd;
	string idBuy;

	__int64 totalValue = 0;

	if(allProducts==false){
		vector<int> selectedRows = pdlg->getSelectedItemFromList(m_ListCtrlBuy);

		if(selectedRows.empty()==false){
			for(int i=selectedRows.size()-1;i>=0;i--){
				CString produs,categorie,pret,cantitate;
				produs=m_ListCtrlBuy.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlBuy,"PRODUS"));
				categorie=m_ListCtrlBuy.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlBuy,"CATEGORIE"));
		
				pret=m_ListCtrlBuy.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlBuy,"PRET"));
				cantitate=m_ListCtrlBuy.GetItemText(selectedRows.at(i)-1,pdlg->SearchListCtrlColumn(m_ListCtrlBuy,"CANTITATE"));
				
				cmds="produs='"+pdlg->convCStr2Str(produs)+"'";
				if(pdlg->sql.dbGetItem("products","id",cmds).empty()==false){
					idProd=pdlg->sql.dbGetItem("products","id",cmds).begin()->at(0);
					cmds="idProdus='"+idProd+"'";
					if(pdlg->sql.dbGetItem("buy","id",cmds).empty()==false){
						idBuy=pdlg->sql.dbGetItem("buy","id",cmds).begin()->at(0);
						pdlg->sql.dbDeleteItem("buy",idBuy);
						pdlg->findItemListToDelete(m_ListCtrlBuy,pdlg->convCStr2Str(produs),"PRODUS");


						{ // total
						CString strTemp;
						totalValue = _atoi64(pdlg->convCStr2Str(pret).c_str())*_atoi64(pdlg->convCStr2Str(cantitate).c_str());
						GetDlgItem(IDC_STATIC4)->GetWindowTextW(strTemp);
						totalValue=_atoi64(pdlg->convCStr2Str(strTemp).c_str())-totalValue;
						strTemp.Format((CString)"%d", totalValue);
						GetDlgItem(IDC_STATIC4)->SetWindowTextW(strTemp);
						}

					}

				}

			}
		}
	}else{


		pdlg->sql.dbDeleteItem("buy","");
		m_ListCtrlBuy.DeleteAllItems();
		GetDlgItem(IDC_STATIC4)->SetWindowTextW((CString)"0");
	}
}
void productsDlg::functionUpdateBuyList(){
	CsocketClientDlg *pdlg = (CsocketClientDlg *)theApp.m_pMainWnd; // legatura inapoi

	m_ListCtrlBuy.DeleteAllItems();
	vector<vector<string> > resultIDS;
	vector<string> resultProducts;

	string cmds;
	string idProd,idCantitate;

	__int64 totalValue = 0;

	
	if(pdlg->sql.dbGetItem("buy","idProdus,cantitate","").empty()==false){
		resultIDS = pdlg->sql.dbGetItem("buy","idProdus,cantitate","");
		for(vector<vector<string> >::iterator it = resultIDS.begin(); it < resultIDS.end(); ++it){
			vector<string> row = *it;
			idProd=row.at(0);
			idCantitate=row.at(1);

			cmds = "id='"+idProd+"'";
			if(pdlg->sql.dbGetItem("products","produs,categorie,stoc,pret",cmds).empty()==false){
				resultProducts = pdlg->sql.dbGetItem("products","produs,categorie,stoc,pret",cmds).at(0);
				string items[] = {resultProducts.at(0),resultProducts.at(1),resultProducts.at(2),resultProducts.at(3),idCantitate};
				pdlg->insertItemInList(pdlg->m_productsDlg.m_ListCtrlBuy,items,"products");
	
				{ //total
				CString strTemp;
				totalValue += _atoi64(resultProducts.at(3).c_str())*_atoi64(idCantitate.c_str());
				strTemp.Format((CString)"%d", totalValue);
				GetDlgItem(IDC_STATIC4)->SetWindowTextW(strTemp);
				}
			}
		}
	}
}
