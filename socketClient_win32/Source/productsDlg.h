#pragma once


#include "..\res\resource.h"
#include "..\CGridListCtrlEx\CGridListCtrlGroups.h"
#include "settingsDlg.h"

// productsDlg dialog

class productsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(productsDlg)

public:
	productsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~productsDlg();

// Dialog Data
	enum { IDD = IDD_PRODUCTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	HICON m_hIcon;
private:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnExitSizeMove();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedCancel();





public:
	CGridListCtrlGroups m_ListCtrlProducts;
	CGridListCtrlGroups m_ListCtrlBuy;
	settingsDlg m_addProductDlg;
	bool isInitialise;
	bool isMaximized;

private:
	CRect rectWindowState;
	CRect rectWindowStateACTUAL;
	CRect rectMinWindowState;

public:
	void initialiseDialog();

	void functionOfflineAddToBuy();
	void functionOfflineDeleteFromBuy(bool allProducts);
	void functionUpdateBuyList();


	void functionAddToCart();
	void functionAddNewProduct();
	void functionDeleteProduct();
	void functionModifyProduct();
	void functionSendCartToServer();

};
