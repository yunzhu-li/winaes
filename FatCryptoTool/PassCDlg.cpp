// PassCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FatCryptoTool.h"
#include "PassCDlg.h"


// CPassCDlg dialog

IMPLEMENT_DYNAMIC(CPassCDlg, CDialog)

CPassCDlg::CPassCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassCDlg::IDD, pParent)
{

}

CPassCDlg::~CPassCDlg()
{
}

void CPassCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, _EDIT_PASSC, _ctl_edit_passc);
}


BEGIN_MESSAGE_MAP(CPassCDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPassCDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPassCDlg message handlers
BOOL CPassCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->_passC[0] = 0;
	this->_ctl_edit_passc.SetLimitText(32);
	return 0;
}

void CPassCDlg::OnBnClickedOk()
{
	if(this->_ctl_edit_passc.GetWindowTextLength())
	{
		this->_ctl_edit_passc.GetWindowText(this->_passC, 64);
		OnOK();
	}else{
		this->MessageBox(_T("Type password"), _T("Prompt"), MB_ICONEXCLAMATION);
		this->_ctl_edit_passc.SetFocus();
	}

}
