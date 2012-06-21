
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

// TabHashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinAES.h"
#include "TabHashDlg.h"


// CTabHashDlg dialog

IMPLEMENT_DYNAMIC(CTabHashDlg, CDialog)

CTabHashDlg::CTabHashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabHashDlg::IDD, pParent)
{

}

CTabHashDlg::~CTabHashDlg()
{
}

void CTabHashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, _EDIT_RESULTS_H, _ctl_edit_results_h);
	DDX_Control(pDX, _PROG_H, _ctl_prog_h);
	DDX_Control(pDX, _ST_STATUS_H, _ctl_st_status_h);
	DDX_Control(pDX, _BTN_TERM_H, _ctl_btn_term_h);
	DDX_Control(pDX, _BTN_OPEN_H, _ctl_btn_open_h);
	DDX_Control(pDX, _CHK_MD5, _ctl_chk_md5);
	DDX_Control(pDX, _CHK_SHA1, _ctl_chk_SHA1);
	DDX_Control(pDX, _CHK_SHA256, _ctl_chk_sha256);
	DDX_Control(pDX, _CHK_SHA512, _ctl_chk_sha512);
	DDX_Control(pDX, _CHK_KECCAK, _ctl_chk_keccak);
}


BEGIN_MESSAGE_MAP(CTabHashDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(_BTN_OPEN_H, &CTabHashDlg::OnBnClickedBtnOpenH)
	ON_BN_CLICKED(_BTN_TERM_H, &CTabHashDlg::OnBnClickedBtnTermH)
END_MESSAGE_MAP()


// CTabHashDlg message handlers
void CTabHashDlg::OnOK()
{
}
void CTabHashDlg::OnCancel()
{
}

BOOL CTabHashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->_ctl_chk_md5.SetCheck(1);
	this->_ctl_chk_SHA1.SetCheck(1);
	this->_ctl_chk_sha256.SetCheck(0);
	this->_ctl_chk_sha512.SetCheck(0);
	this->_ctl_chk_keccak.SetCheck(0);
	this->_bDrop = 0;

	return 0;
}

HBRUSH CTabHashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd != &this->_ctl_edit_results_h)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(GetPixel(this->GetDC()->m_hDC, 1, 1));
	}
	return hbr;
}

void WINAPI _UpdateOpProcessH(unsigned int _percent, void *_param)
{
	CTabHashDlg *_TabHashDlg;
	_TabHashDlg = (CTabHashDlg *)_param;

	_TabHashDlg->_ctl_prog_h.SetPos(_percent);
}

DWORD WINAPI _MDThdFunc(LPVOID _lParam)
{
	CTabHashDlg *_TabHashDlg;
	_TabHashDlg = (CTabHashDlg *)_lParam;

	_TabHashDlg->_EnableCtrls(0);

	TCHAR *_outputText = new TCHAR[32768];
	TCHAR *_outputTextPart = new TCHAR[4096];
	_stprintf_s(_outputText, 32768, _T("FilePath: %s"), _TabHashDlg->_inFilePath);

	//MDs Calculation
	unsigned long _hashMD5[4];
	unsigned char _hashSHA1[20];
	unsigned char _hashSHA256[32];
	unsigned char _hashSHA512[64];
	unsigned char _hashKeccak512[64];

	//MD5
	if(_TabHashDlg->_bMD5)
	{
		_TabHashDlg->_ctl_st_status_h.SetWindowText(_T("Calculating MD5..."));
		if(FileMD5(_TabHashDlg->_inFilePath, _hashMD5, _UpdateOpProcessH, _lParam))
		{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nMD5:\r\n%08X%08X%08X%08X"), _hashMD5[0], _hashMD5[1], _hashMD5[2], _hashMD5[3]);
			_tcscat_s(_outputText, 32768, _outputTextPart);
		}else{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nMD5:\r\nOpen File Failed"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
		}
	}

	//SHA1
	if(_TabHashDlg->_bSHA1)
	{
		_TabHashDlg->_ctl_st_status_h.SetWindowText(_T("Calculating SHA-1..."));
		if(FileSHA1(_TabHashDlg->_inFilePath, _hashSHA1, _UpdateOpProcessH, _lParam))
		{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nSHA-1:\r\n"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
			for(char _ix = 0; _ix < 20; _ix++)
			{
				_stprintf_s(_outputTextPart, 4096, _T("%02X"), _hashSHA1[_ix]);
				_tcscat_s(_outputText, 32768, _outputTextPart);
			}
		}else{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nSHA-1:\r\nOpen File Failed"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
		}
	}

	//SHA256
	if(_TabHashDlg->_bSHA256)
	{
		_TabHashDlg->_ctl_st_status_h.SetWindowText(_T("Calculating SHA256..."));
		if(FileSHA256(_TabHashDlg->_inFilePath, _hashSHA256, _UpdateOpProcessH, _lParam))
		{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nSHA256:\r\n"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
			for(char _ix = 0; _ix < 32; _ix++)
			{
				_stprintf_s(_outputTextPart, 4096, _T("%02X"), _hashSHA256[_ix]);
				_tcscat_s(_outputText, 32768, _outputTextPart);
			}
		}else{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nSHA256:\r\nOpen File Failed"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
		}
	}

	//SHA512
	if(_TabHashDlg->_bSHA512)
	{
		_TabHashDlg->_ctl_st_status_h.SetWindowText(_T("Calculating SHA512..."));
		if(FileSHA512(_TabHashDlg->_inFilePath, _hashSHA512, _UpdateOpProcessH, _lParam))
		{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nSHA512:\r\n"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
			for(char _ix = 0; _ix < 64; _ix++)
			{
				_stprintf_s(_outputTextPart, 4096, _T("%02X"), _hashSHA512[_ix]);
				_tcscat_s(_outputText, 32768, _outputTextPart);
			}
		}else{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nSHA512:\r\nOpen File Failed"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
		}
	}

	//Keccak512
	if(_TabHashDlg->_bKeccak512)
	{
		_TabHashDlg->_ctl_st_status_h.SetWindowText(_T("Calculating Keccak..."));
		if(FileKeccak512(_TabHashDlg->_inFilePath, _hashKeccak512, _UpdateOpProcessH, _lParam))
		{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nKeccak:\r\n"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
			for(char _ix = 0; _ix < 64; _ix++)
			{
				_stprintf_s(_outputTextPart, 4096, _T("%02X"), _hashKeccak512[_ix]);
				_tcscat_s(_outputText, 32768, _outputTextPart);
			}
		}else{
			_stprintf_s(_outputTextPart, 4096, _T("\r\n\r\nKeccak:\r\nOpen File Failed"));
			_tcscat_s(_outputText, 32768, _outputTextPart);
		}
	}

	_TabHashDlg->_ctl_prog_h.SetPos(0);

	_TabHashDlg->_ctl_edit_results_h.SetWindowText(_outputText);
	_TabHashDlg->_ctl_st_status_h.SetWindowText(_T("Ready"));

	_TabHashDlg->_EnableCtrls(1);

	free(_outputText);
	free(_outputTextPart);

	return 0;
}

void CTabHashDlg::OnBnClickedBtnOpenH()
{
	TCHAR *_lpszInPath = new TCHAR[512];
	//UI
	this->_ctl_edit_results_h.SetFocus();

	this->_bMD5 = this->_ctl_chk_md5.GetCheck();
	this->_bSHA1 = this->_ctl_chk_SHA1.GetCheck();
	this->_bSHA256 = this->_ctl_chk_sha256.GetCheck();
	this->_bSHA512 = this->_ctl_chk_sha512.GetCheck();
	this->_bKeccak512 = this->_ctl_chk_keccak.GetCheck();

	if(!(this->_bMD5 || this->_bSHA1 || this->_bSHA256 || this->_bSHA512 || this->_bKeccak512)){
		this->MessageBox(_T("No hash function selected."), _T("Prompt"), MB_ICONEXCLAMATION);
		return;
	}

	//If Drop File
	if(this->_bDrop)
	{
		this->_opThds = CreateThread(NULL, NULL, &_MDThdFunc, this, 0, NULL);
		this->_bDrop = 0;
		return;
	}

	//declare _lp_in_fn and initialize
	LPOPENFILENAME _lp_in_fn;
	_lp_in_fn = new OPENFILENAME;
	memset(_lp_in_fn, 0, sizeof(OPENFILENAME));

	_lp_in_fn->lStructSize = sizeof(OPENFILENAME);
	_lp_in_fn->hwndOwner = this->m_hWnd;
	_lp_in_fn->lpstrFilter = _T("All Files(*.*)\0*.*\0");
	
	_lp_in_fn->lpstrFile = _lpszInPath;
	_lp_in_fn->lpstrFile[0] = _T('\0');
	_lp_in_fn->nMaxFile = 512;

	_lp_in_fn->lpstrFileTitle = NULL;
	_lp_in_fn->nMaxFileTitle = 0;

	_lp_in_fn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	
	if(GetOpenFileName(_lp_in_fn))
	{
		//Save Path
		_tcscpy_s(this->_inFilePath, 512, _lpszInPath);
		this->_opThds = CreateThread(NULL, NULL, &_MDThdFunc, this, 0, NULL);
	}
	free(_lpszInPath);
}

void CTabHashDlg::OnBnClickedBtnTermH()
{
	TerminateThread(this->_opThds, 0);
	_fcloseall();

	this->_ctl_st_status_h.SetWindowText(_T("Canceled"));
	this->_ctl_prog_h.SetPos(0);

	this->_EnableCtrls(1);
}

void CTabHashDlg::_EnableCtrls(char _cEnable)
{
	this->_ctl_chk_md5.EnableWindow(_cEnable);
	this->_ctl_chk_SHA1.EnableWindow(_cEnable);
	this->_ctl_chk_sha256.EnableWindow(_cEnable);
	this->_ctl_chk_sha512.EnableWindow(_cEnable);
	this->_ctl_chk_keccak.EnableWindow(_cEnable);
	this->_ctl_btn_open_h.EnableWindow(_cEnable);
	this->_ctl_btn_term_h.EnableWindow(!_cEnable);
}



