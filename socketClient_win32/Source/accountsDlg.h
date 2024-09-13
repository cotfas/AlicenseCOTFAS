#pragma once

#include "..\res\resource.h"
#include "..\CGridListCtrlEx\CGridListCtrlGroups.h"
#include "settingsDlg.h"

// accountsDlg dialog

class accountsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(accountsDlg)

public:
	accountsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~accountsDlg();

// Dialog Data
	enum { IDD = IDD_ACCOUNTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	HICON m_hIcon;
private:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExitSizeMove();
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCancel();





public:
	CGridListCtrlGroups m_ListCtrlAccounts;
	settingsDlg m_addAccountDlg;
	bool isInitialise;
	bool isMaximized;

private:
	CRect rectWindowState;
	CRect rectWindowStateACTUAL;
	CRect rectMinWindowState;

public:
	void initialiseDialog();
	void functionAddAccount();
	void functionDeleteAccount();
	void functionConfirmAccount();
	void functionEditAccount();

};
