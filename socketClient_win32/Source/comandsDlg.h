#pragma once


#include "..\res\resource.h"
#include "..\CGridListCtrlEx\CGridListCtrlGroups.h"



// comandsDlg dialog

class comandsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(comandsDlg)

public:
	comandsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~comandsDlg();

// Dialog Data
	enum { IDD = IDD_COMANDS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	HICON m_hIcon;




private:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnExitSizeMove();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();





public:
	CGridListCtrlGroups m_ListCtrlComands;
	bool isInitialise;
	bool isMaximized;

private:
	CRect rectWindowState;
	CRect rectWindowStateACTUAL;
	CRect rectMinWindowState;

public:
	void initialiseDialog();
	void functionConfirmCommand();
	void functionDeleteCommand();

};
