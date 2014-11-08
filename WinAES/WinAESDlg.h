
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		contact@fatlyz.com
//		(C) 2014 FatLYZ.COM
//
/////////////////////////////////////

// WinAESDlg.h : header file
//

#pragma once
#include "afxcmn.h"


#include "TabAES256Dlg.h"
#include "TabHashDlg.h"
#include "TabToolsDlg.h"
#include "TabAboutDlg.h"



// CWinAESDlg dialog
class CWinAESDlg : public CDialog
{
// Construction
public:
	CWinAESDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WinAES_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CTabCtrl _ctl_tab_main;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);

	CDialog *_m_lpTabDlg[16];
	
	unsigned char _m_lastTabIndex;

};
