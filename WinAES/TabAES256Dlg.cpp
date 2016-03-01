
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

// TabARS256Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinAES.h"
#include "TabAES256Dlg.h"


// CTabAES256Dlg dialog

IMPLEMENT_DYNAMIC(CTabAES256Dlg, CDialog)

CTabAES256Dlg::CTabAES256Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabAES256Dlg::IDD, pParent)
{

}

CTabAES256Dlg::~CTabAES256Dlg()
{
}

void CTabAES256Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, _LST_FILES, _ctl_lst_files);
	DDX_Control(pDX, _BTN_START, _ctl_btn_start);
	DDX_Control(pDX, _BTN_STOP, _ctl_btn_stop);
	DDX_Control(pDX, _BTN_ADD, _ctl_btn_add);
	DDX_Control(pDX, _BTN_LOCATE, _ctl_btn_locate);
	DDX_Control(pDX, _BTN_REMOVE, _ctl_btn_remove);
	DDX_Control(pDX, _BTN_CLR, _ctl_btn_clear);
	DDX_Control(pDX, _CHK_DEC, _ctl_chk_decrypt);
	DDX_Control(pDX, _CHK_ERASE, _ctl_chk_erase);
	DDX_Control(pDX, _EDT_PASS, _ctl_edit_pass);
}


BEGIN_MESSAGE_MAP(CTabAES256Dlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(_BTN_START, &CTabAES256Dlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(_BTN_ADD, &CTabAES256Dlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(_BTN_CLR, &CTabAES256Dlg::OnBnClickedBtnClr)
	ON_BN_CLICKED(_BTN_LOCATE, &CTabAES256Dlg::OnBnClickedBtnLocate)
	ON_BN_CLICKED(_BTN_REMOVE, &CTabAES256Dlg::OnBnClickedBtnRemove)
	ON_BN_CLICKED(_BTN_STOP, &CTabAES256Dlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(_CHK_DEC, &CTabAES256Dlg::OnBnClickedChkDec)
END_MESSAGE_MAP()

// CTabAES256Dlg message handlers

HBRUSH CTabAES256Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd != &this->_ctl_edit_pass && pWnd != &this->_ctl_lst_files)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(GetPixel(this->GetDC()->m_hDC, 1, 1));
	}
	return hbr;
}

void CTabAES256Dlg::OnOK()
{
}
void CTabAES256Dlg::OnCancel()
{
}


BOOL CTabAES256Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Add Columns
	LPLVCOLUMN _tmpColumn = new LVCOLUMN;
	if (!this->_ctl_lst_files.GetColumnWidth(0))
	{
		//Set ListBox Style
		_ctl_lst_files.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_BORDERSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
		//Set Mask
		_tmpColumn->mask = LVCF_SUBITEM | LVCF_TEXT;
		//Insert Columns
		_tmpColumn->iSubItem = 0;	//SubItemIndex
		_tmpColumn->pszText = _T("File Path");	//Caption
		this->_ctl_lst_files.InsertColumn(0, _tmpColumn);	//Insert

		_tmpColumn->iSubItem = 1;
		_tmpColumn->pszText=_T("Size");
		this->_ctl_lst_files.InsertColumn(1, _tmpColumn);

		_tmpColumn->iSubItem = 2;
		_tmpColumn->pszText = _T("Status");
		this->_ctl_lst_files.InsertColumn(2, _tmpColumn);

		//Set Width
		this->_ctl_lst_files.SetColumnWidth(0,410);
		this->_ctl_lst_files.SetColumnWidth(1,110);
		this->_ctl_lst_files.SetColumnWidth(2,120);
	}

	//Set Default Radio(Check) Buttons
	this->_ctl_rad_aes = (CButton *)GetDlgItem(_RAD_AES);
	this->_ctl_rad_cam = (CButton *)GetDlgItem(_RAD_CAM);
	this->_ctl_rad_aes->SetCheck(TRUE);
	
	//Set Controls
	this->_ctl_edit_pass.SetLimitText(32);
	_EnableCtrls(1);

	//Initiate Variables 
	this->_iFileIndex = 0;

	if(_tcslen(theApp.m_lpCmdLine))
	{
		TCHAR _curFilePath[512];
		TCHAR _curFileLenStr[512];
		UINT64 _curFileLen;
		_curFilePath[0] = 0;

		//Copy Path
		_tcscpy_s(_curFilePath, 512, theApp.m_lpCmdLine + 1);

		//Remove "
		_curFilePath[_tcslen(_curFilePath) - 1] = 0;

		//Get Length
		if(this->_GetFileLen(_curFilePath, &_curFileLen)){
			_stprintf_s(_curFileLenStr, 128, _T("%lld KB"), (_curFileLen / 1024));
		}else{
			_stprintf_s(_curFileLenStr, 128, _T("Open Failed"));
		}
		//add to list
		this->_AddToList(0, _curFilePath, _curFileLenStr, _T("Ready"));
	}
	return TRUE;
}

void WINAPI _UpdateOpProcess(unsigned int _percent, void *_param)
{
	//input CDialog pointer
	CTabAES256Dlg *_TabAES256Dlg;
	_TabAES256Dlg = (CTabAES256Dlg *)_param;

	TCHAR *_curStatus = new TCHAR[128];
	if(_TabAES256Dlg->_bErasing)
	{
		_stprintf_s(_curStatus, 128, _T("Erasing %d%%.."), _percent);
	}else{
		if(!_TabAES256Dlg->_bDecrypt){
			_stprintf_s(_curStatus, 128, _T("Encrypting %d%%.."), _percent);
		}else{
			_stprintf_s(_curStatus, 128, _T("Decrypting %d%%.."), _percent);
		}
	}
	//Set status
	_TabAES256Dlg->_SetItemStatusInList(_TabAES256Dlg->_curEncItem, _curStatus);

	free(_curStatus);
}

DWORD WINAPI _CryptoThdFunc(LPVOID _lParam)
{
	TCHAR *_outFilePath = new TCHAR[512];

	//input CDialog pointer
	CTabAES256Dlg *_TabAES256Dlg;
	_TabAES256Dlg = (CTabAES256Dlg *)_lParam;

	//UI
	_TabAES256Dlg->_EnableCtrls(0);

	for(UINT32 _i = 0; _i < _TabAES256Dlg->_inFilePathAllCount; _i++)
	{
		//Set Index Flag
		_TabAES256Dlg->_curEncItem = _i;
		_tcscpy_s(_outFilePath, 512, _TabAES256Dlg->_inFilePath_All[_i]);
		
		//
		if(_TabAES256Dlg->_bCamellia)
		{
			// ********* Encrypt/Decrypt(Camellia) *********
			//Set Flag
			_TabAES256Dlg->_bErasing = 0;
			if(_TabAES256Dlg->_bDecrypt)
			{
				//Decrypt Mode
				if(_tcslen(_outFilePath) >= 8){
					if(!_tcscmp(&_outFilePath[_tcslen(_outFilePath) - 7], _T(".cam256")))
					{	//cut extension ".cam256"
						_outFilePath[_tcslen(_outFilePath) - 7] = 0;
					}else{
						_tcscat_s(_outFilePath, 512, _T(".decrypted"));
					}
				}
				if(_File_Camellia_Decrypt_CBC(_TabAES256Dlg->_inFilePath_All[_i], _outFilePath, _TabAES256Dlg->_CryptoCipherKey, &_UpdateOpProcess, _lParam)){
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Decrypted"));
				}else{
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Decryption Failed"));
					//No need to erase
					_TabAES256Dlg->_bErase = 0;
				}

			}else{
				//Encrypt Mode
				_tcscat_s(_outFilePath, 512, _T(".cam256"));
				if(_File_Camellia_Encrypt_CBC(_TabAES256Dlg->_inFilePath_All[_i], _outFilePath, _TabAES256Dlg->_CryptoCipherKey, &_UpdateOpProcess, _lParam)){
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Encrypted"));
				}else{
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Encryption Failed"));
					//No need to erase
					_TabAES256Dlg->_bErase = 0;
				}
			}//if
		}else{
			// ********* Encrypt/Decrypt(AES) *********
			//Set Flag
			_TabAES256Dlg->_bErasing = 0;
			if(_TabAES256Dlg->_bDecrypt)
			{
				//Decrypt Mode
				if(_tcslen(_outFilePath) >= 8){
					if(!_tcscmp(&_outFilePath[_tcslen(_outFilePath) - 7], _T(".aes256")))
					{	//cut extension ".aes256"
						_outFilePath[_tcslen(_outFilePath) - 7] = 0;
					}else{
						_tcscat_s(_outFilePath, 512, _T(".decrypted"));
					}
				}
				if(_File_AES_Decrypt_CBC(_TabAES256Dlg->_inFilePath_All[_i], _outFilePath, _TabAES256Dlg->_CryptoCipherKey, &_UpdateOpProcess, _lParam)){
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Decrypted"));
				}else{
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Decryption Failed"));
					//No need to erase
					_TabAES256Dlg->_bErase = 0;
				}

			}else{
				//Encrypt Mode
				_tcscat_s(_outFilePath, 512, _T(".aes256"));
				if(_File_AES_Encrypt_CBC(_TabAES256Dlg->_inFilePath_All[_i], _outFilePath, _TabAES256Dlg->_CryptoCipherKey, &_UpdateOpProcess, _lParam)){
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Encrypted"));
				}else{
					_TabAES256Dlg->_SetItemStatusInList(_i, _T("Encryption Failed"));
					//No need to erase
					_TabAES256Dlg->_bErase = 0;
				}
			}//if
		}//if(Camellia)

		//Erase Original File
		if(_TabAES256Dlg->_bErase)
		{
			//Set Flag
			_TabAES256Dlg->_bErasing = 1;
			if(File_Secure_Erase(_TabAES256Dlg->_inFilePath_All[_i], 1, &_UpdateOpProcess, _lParam)){
				_TabAES256Dlg->_SetItemStatusInList(_i, _T("Done and erased"));
			}else{
				_TabAES256Dlg->_SetItemStatusInList(_i, _T("Erasing failed"));
			}
		}
	}//for

	_TabAES256Dlg->_EnableCtrls(1);

	free(_outFilePath);

	return 0;
}

void CTabAES256Dlg::OnBnClickedBtnStart()
{
	TCHAR *_lpszKey;
	_lpszKey = new TCHAR[64];

	UINT32 _itemCount = this->_ctl_lst_files.GetItemCount();
	this->_inFilePathAllCount = _itemCount;

	//Import List
	if(!_itemCount){
		this->MessageBox(_T("No files added to the list."), _T("Prompt"), MB_ICONEXCLAMATION);
		this->_ctl_lst_files.SetFocus();
		return;
	}

	for(UINT32 _i = 0; _i < _itemCount; _i++){
		_inFilePath_All[_i] = new TCHAR[512];
		this->_ctl_lst_files.GetItemText(_i, 0, _inFilePath_All[_i], 512);
	}

	//Import key
	this->_ctl_edit_pass.GetWindowText(_lpszKey, 64);
	if (!_tcslen(_lpszKey)){
		this->MessageBox(_T("Type Encryption Password."), _T("Prompt"), MB_ICONEXCLAMATION);
		this->_ctl_edit_pass.SetFocus();
		return;
	}
	
	//Get CheckBoxes
	this->_bDecrypt = this->_ctl_chk_decrypt.GetCheck();
	this->_bErase = this->_ctl_chk_erase.GetCheck();
	this->_bCamellia = this->_ctl_rad_cam->GetCheck();
	
	//Password confirmation
	if(!this->_bDecrypt)
	{
		TCHAR *_msg = new TCHAR[256];
		_stprintf_s(_msg, 256, _T("Encrypt files with password: %s ?"), _lpszKey);
		if(this->MessageBox(_msg, _T("Warning"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		{
			free(_msg);
		}else
		{
			this->_ctl_edit_pass.SetFocus();
			free(_msg);
			return;
		}
	}

	//Import key
	for(UINT32 _i = 0; _i < _tcslen(_lpszKey); _i++){
		this->_CryptoCipherKey[_i] = (_lpszKey[_i] & 0x00ff);
	}

	//Fill with 0
	for(UINT32 _i = (UINT32)_tcslen(_lpszKey); _i < 32; _i++){
		this->_CryptoCipherKey[_i] = 0x00;
	}

	//Create Thread
	this->_opThds = CreateThread(NULL, NULL, &_CryptoThdFunc, this, 0, NULL);

	free(_lpszKey);

}

void CTabAES256Dlg::OnBnClickedBtnAdd()
{

	UINT _curStrLen;
	UINT64 _curFileLen;
	
	UCHAR _cOneFile = 1;

	TCHAR *_lpszInPath = new TCHAR[262144];
	TCHAR *_pathDir = new TCHAR[512];
	TCHAR *_curFilePath = new TCHAR[512];
	TCHAR *_curFileName = new TCHAR[128];
	TCHAR *_curFileLenStr = new TCHAR[128];


	//declare _lp_in_fn and initialize
	LPOPENFILENAME _lp_in_fn;
	_lp_in_fn = new OPENFILENAME;
	memset(_lp_in_fn, 0, sizeof(OPENFILENAME));

	_lp_in_fn->lStructSize = sizeof(OPENFILENAME);
	_lp_in_fn->hwndOwner = this->m_hWnd;
	_lp_in_fn->lpstrFilter = _T("All Files(*.*)\0*.*\0");
	
	_lp_in_fn->lpstrFile = _lpszInPath;
	_lp_in_fn->lpstrFile[0] = _T('\0');
	_lp_in_fn->nMaxFile = 262144;

	_lp_in_fn->lpstrFileTitle = NULL;
	_lp_in_fn->nMaxFileTitle = 0;

	_lp_in_fn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if(GetOpenFileName(_lp_in_fn)){

		//Save Base Path
		_tcscpy_s(_pathDir, 512, _lpszInPath);

		UINT _lastFNPos = 0;
		UINT _curFnPos = 0;
		
		while(1)
		{
			if(!_lpszInPath[_curFnPos]){
				if(_curFnPos == _lastFNPos){break;}
				if(_lastFNPos){
					//more than 1 file
					_cOneFile = 0;

					//copy filename
					_tcscpy_s(_curFileName, 128, &_lpszInPath[_lastFNPos]);
					
					//copy base path
					_tcscpy_s(_curFilePath, 512, _pathDir);
					
					//append "\"
					_curStrLen = (UINT)_tcslen(_curFilePath);
					if(_curFilePath[_curStrLen - 1] != 0x5c){
						_curFilePath[_curStrLen + 1] = 0x00;
						_curFilePath[_curStrLen] = 0x5c;
					}

					//append filename
					_tcscat_s(_curFilePath, 512, _curFileName);

					//allocate mem & copy full path
					this->_inFilePath[this->_iFileIndex] = new TCHAR[512];
					_tcscpy_s(_inFilePath[this->_iFileIndex], 512, _curFilePath);

					if(this->_GetFileLen(_curFilePath, &_curFileLen)){
						_stprintf_s(_curFileLenStr, 128, _T("%lld KB"), (_curFileLen / 1024));
					}else{
						_stprintf_s(_curFileLenStr, 128, _T("Open Failed"));
					}

					//add to list
					this->_AddToList(this->_iFileIndex++, _curFilePath, _curFileLenStr, _T("Ready"));
				}
				_lastFNPos = _curFnPos + 1;
			}//Zero
			_curFnPos++;
		}//while

		if (_cOneFile){
			//allocate mem & copy full path
			this->_inFilePath[0] = new TCHAR[512];
			_tcscpy_s(_inFilePath[0], 512, _pathDir);

			if(this->_GetFileLen(_inFilePath[0], &_curFileLen)){
				_stprintf_s(_curFileLenStr, 128, _T("%lld KB"), (_curFileLen / 1024));
			}else{
				_stprintf_s(_curFileLenStr, 128, _T("Open Failed"));
			}
			this->_AddToList(this->_iFileIndex++, _inFilePath[0], _curFileLenStr, _T("Ready"));
		}
	}

	free(_lp_in_fn);
	free(_lpszInPath);
	free(_pathDir);
	free(_curFilePath);
	free(_curFileName);
	free(_curFileLenStr);

}

void CTabAES256Dlg::OnBnClickedBtnClr()
{
	this->_iFileIndex = 0;
	this->_ctl_lst_files.DeleteAllItems();
}

void CTabAES256Dlg::OnBnClickedBtnLocate()
{
	int _curSel;
	TCHAR _execArg[512];

	if(this->_ctl_lst_files.GetSelectedCount())
	{
			_curSel = this->_ctl_lst_files.GetSelectionMark();
			_stprintf_s(_execArg, 512, _T("/select,\""));
			_tcscat_s(_execArg, 512, this->_ctl_lst_files.GetItemText(_curSel, 0).GetBuffer());
			_tcscat_s(_execArg, 512, _T("\""));
			::ShellExecute(NULL, _T("open"), _T("explorer.exe"), _execArg, NULL, SW_SHOWNORMAL);
	}
}

void CTabAES256Dlg::OnBnClickedBtnRemove()
{
	int _curSel;
	int _selCount;
	int *_selectedItem;
	POSITION _curSelPOS;

	_selCount = this->_ctl_lst_files.GetSelectedCount();
	if(_selCount)
	{
		_selectedItem = new int[_selCount + 1];
		memset(_selectedItem, 0, (_selCount + 1) * sizeof(int));
		//Get before-first pos
		_curSelPOS = this->_ctl_lst_files.GetFirstSelectedItemPosition();
		for(int _ix = 0; _ix < _selCount; _ix++)
		{
			//Get next pos
			_curSel = this->_ctl_lst_files.GetNextSelectedItem(_curSelPOS);
			_selectedItem[_ix] = _curSel;
		}

		for(int _ix = _selCount - 1; _ix >= 0; _ix--)
		{
			this->_iFileIndex--;
			this->_inFilePath[_selectedItem[_ix]] = NULL;
			this->_ctl_lst_files.DeleteItem(_selectedItem[_ix]);
		}
		free(_selectedItem);
	}

}

void CTabAES256Dlg::OnBnClickedBtnStop()
{
	TerminateThread(this->_opThds, 0);
	_fcloseall();
	this->_SetItemStatusInList(this->_curEncItem, _T("Canceled"));
	this->_EnableCtrls(1);
}

void CTabAES256Dlg::OnBnClickedChkDec()
{
	if(this->_ctl_chk_decrypt.GetCheck()){
		this->_ctl_chk_erase.SetCheck(0);
		this->_ctl_btn_start.SetWindowText(_T("Decrypt"));
	}else{
		this->_ctl_btn_start.SetWindowText(_T("Encrypt"));
	}
	this->_EnableCtrls(1);
}

void CTabAES256Dlg::_EnableCtrls(char _cEnable)
{
	this->_ctl_btn_add.EnableWindow(_cEnable);
	this->_ctl_btn_clear.EnableWindow(_cEnable);
	this->_ctl_btn_locate.EnableWindow(_cEnable);
	this->_ctl_btn_remove.EnableWindow(_cEnable);
	this->_ctl_btn_start.EnableWindow(_cEnable);
	this->_ctl_btn_stop.EnableWindow(!_cEnable);
	if(this->_ctl_chk_decrypt.GetCheck()){
		this->_ctl_chk_erase.EnableWindow(0);
	}else{
		this->_ctl_chk_erase.EnableWindow(_cEnable);
	}
	this->_ctl_chk_decrypt.EnableWindow(_cEnable);
	this->_ctl_edit_pass.EnableWindow(_cEnable);
	this->_ctl_rad_aes->EnableWindow(_cEnable);
	this->_ctl_rad_cam->EnableWindow(_cEnable);

}

BOOL CTabAES256Dlg::_GetFileLen(TCHAR *_lpszFilePath, UINT64 *_len)
{
	//get file size
	FILE *_curFile = new FILE;

	if(!_tfopen_s(&_curFile, _lpszFilePath, _T("rb"))){
		_fseeki64(_curFile, 0, SEEK_END);
		*_len = _ftelli64(_curFile);
		fclose(_curFile);
		return 1;
	}else{
		return 0;
	}
	free(_curFile);
}

void CTabAES256Dlg::_AddToList(int _iIndex, TCHAR *_lpszName, TCHAR *_lpszSize, TCHAR *_lpszStatus)
{
	LVITEM _curItem = {0};
	
	//Set mask
	_curItem.mask = LVIF_TEXT;// | LVS_EX_BORDERSELECT;
	_curItem.cColumns = 3;//Number of Columns
	
	//Add
	_curItem.iItem = _iIndex;
	_curItem.iSubItem = 0;
	_curItem.pszText = _lpszName;
	this->_ctl_lst_files.InsertItem(&_curItem);

	_curItem.iSubItem = 1;
	_curItem.pszText = _lpszSize;
	this->_ctl_lst_files.SetItem(&_curItem);
	
	_curItem.iSubItem = 2;
	_curItem.pszText = _lpszStatus;
	this->_ctl_lst_files.SetItem(&_curItem);

}

void CTabAES256Dlg::_SetItemStatusInList(int _iIndex, TCHAR *_lpszStatus)
{
	LVITEM _curItem = {0};
	
	//Set mask
	_curItem.mask = LVIF_TEXT;
	_curItem.cColumns = 3;//Number of Columns

	_curItem.iItem = _iIndex;
	_curItem.iSubItem = 2;
	_curItem.pszText = _lpszStatus;
	this->_ctl_lst_files.SetItem(&_curItem);
	this->_ctl_lst_files.EnsureVisible(_iIndex, FALSE);
}
