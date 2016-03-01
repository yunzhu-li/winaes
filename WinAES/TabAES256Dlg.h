
//
//  Copyright (c) 2016 Yunzhu Li.
//  contact@yunzhu.li
//
//  You can redistribute it and/or modify it under the terms 
//  of the GNU General Public License version 3 as published
//  by the Free Software Foundation.
//
//  This file is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty 
//  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
//  the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public
//  License along with this file.
//  If not, see http://www.gnu.org/licenses/.
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "_crypto/_fat_aes_cbc.h"
#include "_crypto/_fat_camellia_cbc.h"
#include "_crypto/_fat_secure_erase.h"

// CTabAES256Dlg dialog

class CTabAES256Dlg : public CDialog
{
	DECLARE_DYNAMIC(CTabAES256Dlg)

public:
	CTabAES256Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabAES256Dlg();

// Dialog Data
	enum { IDD = IDD_TAB_AES256 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog();

public:


	CListCtrl _ctl_lst_files;
	CEdit _ctl_edit_pass;
	CButton _ctl_btn_start;
	CButton _ctl_btn_stop;
	CButton *_ctl_rad_aes;
	CButton *_ctl_rad_cam;

	CButton _ctl_btn_add;
	CButton _ctl_btn_locate;
	CButton _ctl_btn_remove;
	CButton _ctl_btn_clear;
	CButton _ctl_chk_decrypt;
	CButton _ctl_chk_erase;

	CButton _ctl_btn_generate;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnClr();
	afx_msg void OnBnClickedBtnLocate();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedChkDec();


	//UI
	TCHAR *_inFilePath[4096];
	int _iFileIndex;

	//Multi-thread
	HANDLE _opThds;

	void OnOK();
	void OnCancel();
	void _EnableCtrls(char _cEnable);
	BOOL _GetFileLen(TCHAR *_lpszFilePath, UINT64 *_len);
	void _AddToList(int _iIndex, TCHAR *_lpszName, TCHAR *_lpszSize, TCHAR *_lpszStatus);
	void _SetItemStatusInList(int _iIndex, TCHAR *_lpszStatus);

	//Crypto
	unsigned char _bCamellia;
	unsigned char _CryptoCipherKey[32];
	unsigned char _bDecrypt;
	unsigned char _bErasing;
	unsigned char _bErase;
	unsigned int _curEncItem;
	unsigned int _inFilePathAllCount;
	
	TCHAR *_inFilePath_All[4096];
};
