
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		contact@fatlyz.com
//		(C) 2014 FatLYZ.COM
//
/////////////////////////////////////

#pragma once
#include "afxwin.h"


// CTabAboutDlg dialog

class CTabAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CTabAboutDlg)

public:
	CTabAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabAboutDlg();

// Dialog Data
	enum { IDD = IDD_TAB_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void OnOK();
	void OnCancel();
	BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedChkCmenu();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CButton _ctl_chk_cmenu;

	TCHAR _lpszCmdLine[512];

};
