
/////////////////////////////////////
//                                 //
//		Fat Cryptography Tools     //
//                                 //
//		http://hevphoto.com        //
//		fatlyz_pub@gmail.com       //
//		(C)2012 FatLYZ             //
//                                 //
/////////////////////////////////////

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "_crypto/_fatfilemd5.h"
#include "_crypto/_fatfilehash.h"


// CTabHashDlg dialog

class CTabHashDlg : public CDialog
{
	DECLARE_DYNAMIC(CTabHashDlg)

public:
	CTabHashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabHashDlg();

// Dialog Data
	enum { IDD = IDD_TAB_HASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnOpenH();
	afx_msg void OnBnClickedBtnTermH();

	void OnOK();
	void OnCancel();
	void _EnableCtrls(char _cEnable);
	BOOL OnInitDialog();


	CEdit _ctl_edit_results_h;
	CProgressCtrl _ctl_prog_h;
	CStatic _ctl_st_status_h;
	CButton _ctl_btn_term_h;
	CButton _ctl_btn_open_h;
	CButton _ctl_chk_md5;
	CButton _ctl_chk_SHA1;
	CButton _ctl_chk_sha256;
	CButton _ctl_chk_sha512;
	CButton _ctl_chk_keccak;

	BOOL _bDrop;
	BOOL _bMD5;
	BOOL _bSHA1;
	BOOL _bSHA256;
	BOOL _bSHA512;
	BOOL _bKeccak512;

	TCHAR _inFilePath[512];
	HANDLE _opThds;


};
