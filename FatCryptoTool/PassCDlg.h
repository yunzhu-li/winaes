#pragma once
#include "afxwin.h"


// CPassCDlg dialog

class CPassCDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassCDlg)

public:
	CPassCDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassCDlg();

// Dialog Data
	enum { IDD = IDD_PASSC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit _ctl_edit_passc;
	BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	TCHAR _passC[64];

};
