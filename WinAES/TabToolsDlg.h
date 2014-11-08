
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
#include "afxcmn.h"

#include "_crypto/_base64.h"
#include "_crypto/_fat_secure_erase.h"

// CTabToolsDlg dialog

class CTabToolsDlg : public CDialog
{
	DECLARE_DYNAMIC(CTabToolsDlg)

public:
	CTabToolsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabToolsDlg();

// Dialog Data
	enum { IDD = IDD_TAB_TOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void OnOK();
	void OnCancel();
	BOOL OnInitDialog();
	void _EnableCtrls(char _cEnable);
	void _SetItemStatusInList(int _iIndex, TCHAR *_lpszStatus);
	void _AddToList(int _iIndex, TCHAR *_lpszName, TCHAR *_lpszStatus);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnAddSe();
	afx_msg void OnBnClickedBtnGen();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnClrSe();
	afx_msg void OnBnClickedBtnRemoveSe();
	afx_msg void OnBnClickedBtnStopSe();
	afx_msg void OnBnClickedBtnB64Enc();
	afx_msg void OnBnClickedBtnB64Dec();

	//Base64 Encoder
	CEdit _ctl_edit_b64_input;
	CButton *_ctl_rad_b64_ansi;
	CButton *_ctl_rad_b64_unicode;
	char _bANSI;

	//Pass Generator
	CEdit _ctl_edit_length;
	CEdit _ctl_edit_gpass;
	CButton _ctl_chk_upc;
	CButton _ctl_chk_lwc;
	CButton _ctl_chk_num;
	CButton _ctl_chk_sym;

	//Secure Eraser
	CButton _ctl_btn_add_se;
	CButton _ctl_btn_rem_se;
	CButton _ctl_btn_clr_se;
	CButton _ctl_btn_stop_se;
	CButton _ctl_btn_start_se;
	CButton *_ctl_rad_etimes_1;
	CButton *_ctl_rad_etimes_7;
	CButton *_ctl_rad_etimes_35;
	CListCtrl _ctl_list_files;
	TCHAR *_inFilePath[4096];
	TCHAR *_inFilePath_All[4096];
	int _iFileIndex;
	int _iEraseTimes;
	unsigned int _curErasingItem;
	unsigned int _inFilePathAllCount;

	//Multi-thread
	HANDLE _opThds;


};



