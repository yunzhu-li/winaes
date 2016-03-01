
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

// WinAESDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinAES.h"
#include "WinAESDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CWinAESDlg::CWinAESDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinAESDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinAESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, _TAB_MAIN, _ctl_tab_main);
}

BEGIN_MESSAGE_MAP(CWinAESDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CWinAESDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, _TAB_MAIN, &CWinAESDlg::OnTcnSelchangeTabMain)
	ON_BN_CLICKED(IDCANCEL, &CWinAESDlg::OnBnClickedCancel)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CWinAESDlg message handlers

BOOL CWinAESDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Insert Tabs
	TCITEM *_lpTabItem = new TCITEM;
	_lpTabItem->mask = TCIF_TEXT;
	_lpTabItem->pszText = _T("AES256/Camellia256");
	this->_ctl_tab_main.InsertItem(0, _lpTabItem);
	_lpTabItem->pszText = _T("Hash(MD5/SHA-1/2/3..)");
	this->_ctl_tab_main.InsertItem(1, _lpTabItem);
	_lpTabItem->pszText = _T("Security Tools");
	this->_ctl_tab_main.InsertItem(2, _lpTabItem);
	_lpTabItem->pszText = _T("About..");
	this->_ctl_tab_main.InsertItem(3, _lpTabItem);

	//Initialize
	_m_lastTabIndex = 0;

	//Create windows
	_m_lpTabDlg[0] = new CTabAES256Dlg;
	_m_lpTabDlg[0]->Create(IDD_TAB_AES256, this);
	_m_lpTabDlg[1] = new CTabHashDlg;
	_m_lpTabDlg[1]->Create(IDD_TAB_HASH, this);
	_m_lpTabDlg[2] = new CTabToolsDlg;
	_m_lpTabDlg[2]->Create(IDD_TAB_TOOLS, this);
	_m_lpTabDlg[3] = new CTabAboutDlg;
	_m_lpTabDlg[3]->Create(IDD_TAB_ABOUT, this);

	this->OnTcnSelchangeTabMain(0, 0);
	this->DragAcceptFiles();

	free(_lpTabItem);

	return TRUE;  // return TRUE  unless you set the focus to a control

}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinAESDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinAESDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWinAESDlg::OnBnClickedOk()
{
}

void CWinAESDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	//476x240
	int _curTab;
	_curTab = this->_ctl_tab_main.GetCurSel();

	//Get Position
	CRect *_lpTabDlgRect = new CRect;
	this->GetClientRect(_lpTabDlgRect);
	_lpTabDlgRect->left += 20;
	_lpTabDlgRect->right -= 20;
	_lpTabDlgRect->top += 40;
	_lpTabDlgRect->bottom -= 45;

	//Move and Show
	this->_m_lpTabDlg[_curTab]->MoveWindow(_lpTabDlgRect);
	this->_m_lpTabDlg[_curTab]->ShowWindow(SW_SHOW);
	this->_m_lpTabDlg[_curTab]->SetFocus();
	//Hide Previous Tab
	if(pResult){this->_m_lpTabDlg[_m_lastTabIndex]->ShowWindow(SW_HIDE);}

	_m_lastTabIndex = _curTab;

	free(_lpTabDlgRect);

	if(pResult) {*pResult = 0;}
}

void CWinAESDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CWinAESDlg::OnDropFiles(HDROP hDropInfo)
{
	CDialog::OnDropFiles(hDropInfo);

	//Get Count
	UINT32 _DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR *_curFilePath = new TCHAR[512];
	UINT64 _curFileLen;
	TCHAR *_curFileLenStr = new TCHAR[128];

	if(this->_m_lastTabIndex == 0)
	{
		for(UINT32 _i = 0; _i < _DropCount; _i++)
		{
			DragQueryFile(hDropInfo, _i, _curFilePath, 512);
			//Get File Length
			if(((CTabAES256Dlg *)(this->_m_lpTabDlg[0]))->_GetFileLen(_curFilePath, &_curFileLen)){
				_stprintf_s(_curFileLenStr, 128, _T("%d KB"), (_curFileLen / 1024));
			}else{
				_stprintf_s(_curFileLenStr, 128, _T("Open Failed"));
			}
			((CTabAES256Dlg *)(this->_m_lpTabDlg[0]))->_AddToList(_i, _curFilePath, _curFileLenStr, _T("Ready"));
		}
	}else
	if(this->_m_lastTabIndex == 1)
	{
		DragQueryFile(hDropInfo, 0, _curFilePath, 512);
		_tcscpy_s(((CTabHashDlg *)(this->_m_lpTabDlg[1]))->_inFilePath, 512, _curFilePath);
		((CTabHashDlg *)(this->_m_lpTabDlg[1]))->_bDrop = 1;
		((CTabHashDlg *)(this->_m_lpTabDlg[1]))->OnBnClickedBtnOpenH();
	}else
	if(this->_m_lastTabIndex == 2)
	{
		for(UINT32 _i = 0; _i < _DropCount; _i++)
		{
			DragQueryFile(hDropInfo, _i, _curFilePath, 512);
			((CTabToolsDlg *)(this->_m_lpTabDlg[2]))->_AddToList(_i, _curFilePath, _T("Ready"));
		}
	}else
	if(this->_m_lastTabIndex == 3)
	{
		this->MessageBox(_T("Thank you for using my program! Please visit https://yunzhu.li"), _T("Hi~"), MB_ICONINFORMATION);
	}

	free(_curFileLenStr);
	free(_curFilePath);
}
