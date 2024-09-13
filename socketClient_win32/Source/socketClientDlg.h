
// socketClientDlg.h : header file
//

#pragma once

#include "settingsDlg.h"
#include "accountsDlg.h"
#include "productsDlg.h"
#include "comandsDlg.h"
#include "..\res\resource.h"


// GridList include
#include "..\CGridListCtrlEx\CGridListCtrlGroups.h"
#include "afxwin.h"

#include "..\MultilineListCtrl\MultiLineListBox.h"




class CConex;
class TCPSocket;

// CsocketClientDlg dialog
class CsocketClientDlg : public CDialogEx
{
// Construction
public:
	CsocketClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_socketClient_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnExitSizeMove();














public:
	void applicationInitialise();
	
	// sending
	void sendChatMessage();
	void sendAddAccount();
	void sendEditAccount();
	void sendGetAllUsers();
	void sendDeleteUserDB(CString userNameT, CString authTypeT);
	void sendConfirmedUserDB(CString userNameT, CString authTypeT, CString confirmedT);

	void sendGetAllProducts(CConex * remote);
	void sendAddNewProduct();
	void sendDeleteProductDB(CString produsT, CString categorieT);
	void sendUpdateProduct();

	void sendKickUser(CString userNameT, CString authTypeT);
	void sendNewBuy(string newBuy);
	void sendGetAllComands();
	void sendDeleteComandDB(string idComandT);
	void sendUpdateComand(string idComandT, string confirmedT);



	virtual void FunctieEliberareRemoteSock(CConex * remote);
	void FunctieKickUser(bool banUser);
	void connecToServer();
	void isConnected();
	void disconnectClient();
	void sendMsg(string message);



	string getWindowsVersion();
	string get_cpuid(void);
	void startXML(bool read);

	// Converting
	string convCStr2Str(CString strForConv);
	int convCStringToInt(CString strForConv);
	wstring convStr2ws(const std::string& s);
	string convLong2Str(long value);

	// Functions List Controls Edit Controls
	void insertStringInEditList(string StringName);
	void insertColumnInList(CListCtrl& oListCtrl, CWnd *pWnd, CWnd *pForm, string itemColumn[], int size);
	void insertItemInList(CGridListCtrlGroups& oListCtrl, string itemNames[], string wichIcon);
	void deleteItemList(CListCtrl& oListCtrl, int itemToDelete);
	vector<int> getSelectedItemFromList(CListCtrl& oListCtrl);
	int findItemListToDelete(CListCtrl& oListCtrl, string itemFound, string wichColumToFind);
	int SearchListCtrlRow(CListCtrl& oListCtrl, LPCTSTR psSearchText, string nColumn, bool bCaseSensitive, int nStartingRow);
	int SearchListCtrlColumn(CListCtrl& oListCtrl, string psColumn);
	
	void reSizeListCtrlColumn(CListCtrl& oListCtrl, CWnd* pWnd, CWnd* pForm, int columnCount);
	void moveControlsOnSize(CWnd* pCtl, CWnd* pForm, CRect& rectWindowStateACTUAL_, CRect& rectWindowState_, int move, int upDownValue);

public:
	CGridListCtrlGroups m_ListCtrl;
	CImageList m_ImageList;
	CMultiLineListBox m_EditList;
	CEdit m_EditChat;

	settingsDlg m_settingsDlg;
	accountsDlg m_accountsDlg;
	productsDlg m_productsDlg;
	comandsDlg m_comandsDlg;

	sqliteAPI sql;
	xmlParser xml;
	CConex *conex;
	TCPSocket clientSock; // stocare generala


	bool autoEnableListen;
	bool autoEnableChat;
	bool isDisconnected;
	bool isRegistering;

	bool isMaximized;
	bool autoMaximized;

	bool isButtonConnectLocked;

	int nItemPosition;
	

	string slash;
	CString ID_IP;
	CString ID_PORT;
	CString ID_NAME;
	CString ID_USERNAME;
	CString ID_PASSWORD;
	CString ID_AUTH;

	CString ID_THISUSERNAME;
	CString ID_THISAUTH;

private:
	CRect rectWindowStateTMP;
	CRect rectWindowState;
	CRect rectWindowStateACTUAL;
	CRect rectMinWindowState;


	ObjMutex objMutexRemoveSocket;
	ObjMutex objMutexInsertItemInList;
};
