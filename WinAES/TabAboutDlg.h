
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
