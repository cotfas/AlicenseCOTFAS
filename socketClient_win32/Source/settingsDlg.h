#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "..\res\resource.h"

// settingsDlg dialog

class settingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(settingsDlg)

public:
	settingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~settingsDlg();

// Dialog Data
	enum { IDD = IDD_SETTINGS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();






public:
	settingsDlg *m_newUserDlg;
	CComboBox m_ComboBoxStatus1;
	CComboBox m_ComboBoxStatus2;
	string whatFormIs;
	CString keyValue;
	bool isRegistred;
	
	CString ID_OLDUSERNAME;

	CString ID_NAME;
	CString ID_TYPEIMEI;
	CString ID_USERNAME;
	CString ID_PASSWORD;
	CString ID_AUTH;
	CString ID_CONFIRMED;

	CString ID_OLDPRODUS;
	CString ID_OLDCATEGORIE;

	CString ID_PRODUS;
	CString ID_CATEGORIE;
	CString ID_STOC;
	CString ID_PRET;

	CString ID_CANTITATE;

private:
	CRect rectWindowState;
	CRect rectWindowStateACTUAL;
	CRect rectMinWindowState;


public:
	CString genKeyP();
	bool verificareString(int nID, bool isNumbers);
	void setWhatFormIs(string whatForm);

	void settingsFormIntialise();
	void newUserFormIntialise();
	void addAccountFormIntialise(string editAccount);
	void addProductFormIntialise(string editProduct);

	void settingsFormOk();
	void newUserFormOk();
	void addAccountFormOk(string editAccount);
	void addProductFormOk(string editProduct);

};
