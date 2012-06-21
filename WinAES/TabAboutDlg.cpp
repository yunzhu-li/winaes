
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

// TabAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinAES.h"
#include "TabAboutDlg.h"


// CTabAboutDlg dialog

IMPLEMENT_DYNAMIC(CTabAboutDlg, CDialog)

CTabAboutDlg::CTabAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabAboutDlg::IDD, pParent)
{

}

CTabAboutDlg::~CTabAboutDlg()
{
}

void CTabAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, _CHK_CMENU, _ctl_chk_cmenu);
}


BEGIN_MESSAGE_MAP(CTabAboutDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(_CHK_CMENU, &CTabAboutDlg::OnBnClickedChkCmenu)
END_MESSAGE_MAP()


// CTabAboutDlg message handlers
void CTabAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CTabAboutDlg::OnOK()
{
}

void CTabAboutDlg::OnCancel()
{
}

HBRUSH CTabAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	hbr = CreateSolidBrush(GetPixel(this->GetDC()->m_hDC, 1, 1));
	return hbr;
}

BOOL CTabAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HKEY _hKey;
	BYTE _dataBytes[1024] = {0};
	DWORD _dataLen = 1024;
	DWORD _dataType = REG_SZ;
	TCHAR _exePath[512] = {0};
	TCHAR _dataWide[512] = {0};

	_lpszCmdLine[0] = 0;
	GetModuleFileName(NULL, _exePath, 512);
	_stprintf_s(this->_lpszCmdLine, 512, _T("%c%s%c %c%%1%c"), 34, _exePath, 34, 34, 34);

	RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("*\\shell\\Open with WinAES\\command"), NULL, KEY_READ, &_hKey);
	if(_hKey){
		RegQueryValueEx(_hKey, NULL, NULL, &_dataType, _dataBytes, &_dataLen);

		//Copy to wide char
		for(UINT32 _i = 0; _i < 512; _i++){
			_dataWide[_i] = _dataWide[_i] | _dataBytes[2 * _i];
			_dataWide[_i] = _dataWide[_i] | (UINT16)(_dataBytes[2 * _i + 1] << 8);
		}

		if(!_tcscmp(this->_lpszCmdLine, _dataWide)){
			this->_ctl_chk_cmenu.SetCheck(1);
		}
		RegCloseKey(_hKey);
	}

	return 0;
}

void CTabAboutDlg::OnBnClickedChkCmenu()
{
	HKEY _hKey;
	BYTE _cmdLine_Bytes[1024];

	//Copy to bytes
	for(UINT32 _i = 0; _i < 512; _i++){
		_cmdLine_Bytes[2 * _i] = this->_lpszCmdLine[_i] & 0x00ff;
		_cmdLine_Bytes[2 * _i + 1] = this->_lpszCmdLine[_i] & 0xff00;
	}

	if(this->_ctl_chk_cmenu.GetCheck())
	{
		if(RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("*\\shell\\Open with WinAES\\command"), NULL, NULL, NULL, KEY_ALL_ACCESS, NULL,  &_hKey, NULL) == ERROR_SUCCESS)
		{
			if(RegSetValueEx(_hKey, NULL, NULL, REG_SZ, _cmdLine_Bytes, 1024) == ERROR_SUCCESS){
				RegCloseKey(_hKey);

				this->_ctl_chk_cmenu.SetCheck(1);
				this->MessageBox(_T("Successfully added to the windows context menu."), _T("Prompt"), MB_ICONINFORMATION);
				return;
			}
		}

	}else{
		if(RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("*\\shell"), NULL, NULL, NULL, KEY_ALL_ACCESS, NULL,  &_hKey, NULL) == ERROR_SUCCESS)
		{
			RegDeleteKey(_hKey, _T("Open with WinAES\\command"));
			if (RegDeleteKey(_hKey, _T("Open with WinAES")) == ERROR_SUCCESS){
				RegCloseKey(_hKey);
				this->_ctl_chk_cmenu.SetCheck(0);
				this->MessageBox(_T("Successfully deleted items from the windows context menu."), _T("Prompt"), MB_ICONINFORMATION);
				return;
			}
		}

	}
	//Failed
	RegCloseKey(_hKey);
	this->MessageBox(_T("Cannot access the windows registry, this operation need Administrator privilege."), _T("Prompt"), MB_ICONEXCLAMATION);
	this->_ctl_chk_cmenu.SetCheck(!this->_ctl_chk_cmenu.GetCheck());
	return;

}


