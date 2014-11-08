
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		contact@fatlyz.com
//		(C) 2014 FatLYZ.COM
//
/////////////////////////////////////

// TabToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinAES.h"
#include "TabToolsDlg.h"

// CTabToolsDlg dialog

IMPLEMENT_DYNAMIC(CTabToolsDlg, CDialog)

CTabToolsDlg::CTabToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabToolsDlg::IDD, pParent)
{
}

CTabToolsDlg::~CTabToolsDlg()
{
}

void CTabToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, _EDT_LENGTH, _ctl_edit_length);
	DDX_Control(pDX, _EDT_GPASS, _ctl_edit_gpass);
	DDX_Control(pDX, _CHK_UPC, _ctl_chk_upc);
	DDX_Control(pDX, _CHK_LWC, _ctl_chk_lwc);
	DDX_Control(pDX, _CHK_NUM, _ctl_chk_num);
	DDX_Control(pDX, _CHK_SYM, _ctl_chk_sym);
	DDX_Control(pDX, _LST_FILES, _ctl_list_files);
	DDX_Control(pDX, _BTN_START_SE, _ctl_btn_start_se);
	DDX_Control(pDX, _BTN_ADD_SE, _ctl_btn_add_se);
	DDX_Control(pDX, _BTN_REMOVE_SE, _ctl_btn_rem_se);
	DDX_Control(pDX, _BTN_CLR_SE, _ctl_btn_clr_se);
	DDX_Control(pDX, _BTN_STOP_SE, _ctl_btn_stop_se);
	DDX_Control(pDX, _EDT_B64_IN, _ctl_edit_b64_input);
}

BEGIN_MESSAGE_MAP(CTabToolsDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(_BTN_GEN, &CTabToolsDlg::OnBnClickedBtnGen)
	ON_BN_CLICKED(_BTN_START_SE, &CTabToolsDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(_BTN_ADD_SE, &CTabToolsDlg::OnBnClickedBtnAddSe)
	ON_BN_CLICKED(_BTN_REMOVE_SE, &CTabToolsDlg::OnBnClickedBtnRemoveSe)
	ON_BN_CLICKED(_BTN_CLR_SE, &CTabToolsDlg::OnBnClickedBtnClrSe)
	ON_BN_CLICKED(_BTN_STOP_SE, &CTabToolsDlg::OnBnClickedBtnStopSe)
	ON_BN_CLICKED(_BTN_B64_ENC, &CTabToolsDlg::OnBnClickedBtnB64Enc)
	ON_BN_CLICKED(_BTN_B64_DEC, &CTabToolsDlg::OnBnClickedBtnB64Dec)
END_MESSAGE_MAP()

// CTabToolsDlg message handlers

void CTabToolsDlg::OnOK()
{
}
void CTabToolsDlg::OnCancel()
{
}

HBRUSH CTabToolsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd != &this->_ctl_edit_gpass
		&& pWnd != &this->_ctl_edit_length
		&& pWnd != &this->_ctl_edit_b64_input
		&& pWnd != &this->_ctl_list_files
		)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(GetPixel(this->GetDC()->m_hDC, 1, 1));
	}
	return hbr;
}

BOOL CTabToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Add Columns
	LPLVCOLUMN _tmpColumn = new LVCOLUMN;
	if (!this->_ctl_list_files.GetColumnWidth(0))
	{
		//Set ListBox Style
		_ctl_list_files.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
		//Set Mask
		_tmpColumn->mask = LVCF_SUBITEM | LVCF_TEXT;
		//Insert Columns
		_tmpColumn->iSubItem = 0;	//SubItemIndex
		_tmpColumn->pszText = _T("File Path");	//Caption
		this->_ctl_list_files.InsertColumn(0, _tmpColumn);	//Insert

		_tmpColumn->iSubItem = 1;
		_tmpColumn->pszText = _T("Status");
		this->_ctl_list_files.InsertColumn(2, _tmpColumn);

		//Set Width
		this->_ctl_list_files.SetColumnWidth(0,180);
		this->_ctl_list_files.SetColumnWidth(1,90);
	}
	free(_tmpColumn);

	//Secure Eraser
	this->_ctl_rad_etimes_1 = (CButton *)GetDlgItem(_RAD_ETIMES_1);
	this->_ctl_rad_etimes_7 = (CButton *)GetDlgItem(_RAD_ETIMES_7);
	this->_ctl_rad_etimes_35 = (CButton *)GetDlgItem(_RAD_ETIMES_35);
	this->_ctl_rad_etimes_1->SetCheck(TRUE);
	this->_EnableCtrls(1);
	
	//Pass Generator
	this->_ctl_edit_length.SetLimitText(4);
	this->_ctl_chk_upc.SetCheck(1);
	this->_ctl_chk_lwc.SetCheck(1);
	this->_ctl_chk_num.SetCheck(1);

	//Base64 Encoder
	this->_ctl_edit_b64_input.SetLimitText(1048576);
	this->_ctl_rad_b64_ansi = (CButton *)GetDlgItem(_RAD_B64_ANSI);
	this->_ctl_rad_b64_unicode = (CButton *)GetDlgItem(_RAD_B64_UNICODE);
	this->_ctl_rad_b64_ansi->SetCheck(TRUE);

	//Initiate Variables 
	this->_iFileIndex = 0;

	return TRUE;
}

void WINAPI _UpdateOpProcessSe(unsigned int _percent, void *_param)
{
	//input CDialog pointer
	CTabToolsDlg *_TabToolsDlg;
	_TabToolsDlg = (CTabToolsDlg *)_param;

	TCHAR *_curStatus = new TCHAR[128];

	_stprintf_s(_curStatus, 128, _T("Erasing %d MB.."), _percent);

	_TabToolsDlg->_SetItemStatusInList(_TabToolsDlg->_curErasingItem, _curStatus);

	free(_curStatus);

}

DWORD WINAPI _SEraseThdFunc(LPVOID _lParam)
{
	//input CDialog pointer
	CTabToolsDlg *_TabToolsDlg;
	_TabToolsDlg = (CTabToolsDlg *)_lParam;
	_TabToolsDlg->_EnableCtrls(0);

	for(UINT32 _i = 0; _i < _TabToolsDlg->_inFilePathAllCount; _i++)
	{
		_TabToolsDlg->_curErasingItem = _i;
		if(File_Secure_Erase(_TabToolsDlg->_inFilePath_All[_i], _TabToolsDlg->_iEraseTimes, &_UpdateOpProcessSe, _lParam)){
			_TabToolsDlg->_SetItemStatusInList(_i, _T("Erased"));
		}else{
			_TabToolsDlg->_SetItemStatusInList(_i, _T("Failed"));
		}
	}

	_TabToolsDlg->_EnableCtrls(1);
	return 0;
}

void CTabToolsDlg::OnBnClickedBtnStart()
{
	//UI
	this->_ctl_list_files.SetFocus();

	//Import List
	UINT32 _itemCount = this->_ctl_list_files.GetItemCount();
	this->_inFilePathAllCount = _itemCount;

	if(!_itemCount){
		this->MessageBox(_T("No files added to the list."), _T("Prompt"), MB_ICONEXCLAMATION);
		return;
	}

	if(this->MessageBox(_T("Selected files will be permanently LOST!"), _T("WARNING"), MB_ICONEXCLAMATION | MB_YESNO) != IDYES){
		return;
	}

	for(UINT32 _i = 0; _i < _itemCount; _i++){
		_inFilePath_All[_i] = new TCHAR[512];
		this->_ctl_list_files.GetItemText(_i, 0, _inFilePath_All[_i], 512);
	}
	
	//Get RadioButtons
	if(this->_ctl_rad_etimes_1->GetCheck()){
		this->_iEraseTimes = 1;
	}else
	if(this->_ctl_rad_etimes_7->GetCheck()){
		this->_iEraseTimes = 7;
	}else
	if(this->_ctl_rad_etimes_35->GetCheck()){
		this->_iEraseTimes = 35;
	}

	//Create Thread
	this->_opThds = CreateThread(NULL, NULL, &_SEraseThdFunc, this, 0, NULL);
	
}

void CTabToolsDlg::OnBnClickedBtnAddSe()
{
	UINT _curStrLen;
	UCHAR _cOneFile = 1;

	TCHAR *_lpszInPath = new TCHAR[262144];
	TCHAR *_pathDir = new TCHAR[512];
	TCHAR *_curFilePath = new TCHAR[512];
	TCHAR *_curFileName = new TCHAR[128];

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

					//add to list
					this->_AddToList(this->_iFileIndex++, _curFilePath, _T("Ready"));
				}
				_lastFNPos = _curFnPos + 1;
			}//Zero
			_curFnPos++;
		}//while

		if (_cOneFile){
			//allocate mem & copy full path
			this->_inFilePath[0] = new TCHAR[512];
			_tcscpy_s(_inFilePath[0], 512, _pathDir);
			this->_AddToList(this->_iFileIndex++, _inFilePath[0], _T("Ready"));
		}
	}
	free(_lpszInPath);
	free(_pathDir);
	free(_curFilePath);
	free(_curFileName);

}

void CTabToolsDlg::OnBnClickedBtnRemoveSe()
{
	int _curSel;
	int _selCount;
	int *_selectedItem;
	POSITION _curSelPOS;

	_selCount = this->_ctl_list_files.GetSelectedCount();
	if(_selCount)
	{
		_selectedItem = new int[_selCount + 1];
		memset(_selectedItem, 0, (_selCount + 1) * sizeof(int));
		//Get before-first pos
		_curSelPOS = this->_ctl_list_files.GetFirstSelectedItemPosition();
		for(int _ix = 0; _ix < _selCount; _ix++)
		{
			//Get next pos
			_curSel = this->_ctl_list_files.GetNextSelectedItem(_curSelPOS);
			_selectedItem[_ix] = _curSel;
		}

		for(int _ix = _selCount - 1; _ix >= 0; _ix--)
		{
			this->_iFileIndex--;
			this->_inFilePath[_selectedItem[_ix]] = NULL;
			this->_ctl_list_files.DeleteItem(_selectedItem[_ix]);
		}
		free(_selectedItem);
	}

}

void CTabToolsDlg::OnBnClickedBtnClrSe()
{
	this->_iFileIndex = 0;
	this->_ctl_list_files.DeleteAllItems();	
}

void CTabToolsDlg::OnBnClickedBtnStopSe()
{
	TerminateThread(this->_opThds, 0);
	_fcloseall();
	this->_SetItemStatusInList(this->_curErasingItem, _T("Canceled"));
	this->_EnableCtrls(1);
}

//***** Base64 Encoder *****

void CTabToolsDlg::OnBnClickedBtnB64Enc()
{
	//UI
	this->_ctl_edit_b64_input.SetFocus();

	//Flags
	this->_bANSI = this->_ctl_rad_b64_ansi->GetCheck();

	UINT32 _msgLen;
	UINT32 _msgLen_Bytes;
	UINT32 _msgLen_Out;
	UINT32 _blocks_3;
	UINT32 _lenMod3;

	//Get length in unicode
	_msgLen = this->_ctl_edit_b64_input.GetWindowTextLength();
	if(!_msgLen){return;}

	//Allocate Memory
	wchar_t *_msg_in_unicode = new wchar_t[_msgLen + 2];

	//Get input
	this->_ctl_edit_b64_input.GetWindowText(_msg_in_unicode, _msgLen + 1);
	_msg_in_unicode[_msgLen] = 0;

	if(this->_bANSI){
		//Get length in ansi
		_msgLen_Bytes = WideCharToMultiByte(CP_ACP, NULL, _msg_in_unicode, _msgLen, NULL, NULL, NULL, NULL);
	} else {
		_msgLen_Bytes = _msgLen * 2 + 2;
	}

	//Allocate Memory
	unsigned char *_msg_in_byte = new unsigned char[_msgLen_Bytes + 2];
	_msg_in_byte[0] = 0;

	if(this->_bANSI){
		//Convert to ansi
		WideCharToMultiByte(CP_ACP, NULL, _msg_in_unicode, _msgLen, (char *)_msg_in_byte, _msgLen_Bytes, NULL, NULL);
	} else {
		//Copy
		memcpy(_msg_in_byte, _msg_in_unicode, _msgLen_Bytes);
	}

	//add 0
	_msg_in_byte[_msgLen_Bytes] = 0;
	
	//Calculate number of blocks
	_blocks_3 = (_msgLen_Bytes / 3);
	_lenMod3 = (_msgLen_Bytes % 3);

	//Get Length for output
	_msgLen_Out = (16 + (_blocks_3 * 4));

	//Encode
	char *_msg_out_ansi = new char[_msgLen_Out + 2];
	_msg_out_ansi[0] = 0;
	unsigned char *_msg_out_part_ansi = new unsigned char[8];
	
	for(UINT32 _i = 0; _i < _blocks_3 + 1; _i++)
	{
		if(_i == _blocks_3){
			if(!_lenMod3){break;}
			_b64EncodeBlock(&_msg_in_byte[3 * _i], _msg_out_part_ansi, _lenMod3);
		}else{
			_b64EncodeBlock(&_msg_in_byte[3 * _i], _msg_out_part_ansi, 3);
		}
		//add 0
		_msg_out_part_ansi[4] = 0;

		//strcat
		strcat_s(_msg_out_ansi, _msgLen_Out + 1, (char *)_msg_out_part_ansi);
	}

	//Initialize out string
	wchar_t *_msg_out_unicode = new wchar_t[_msgLen_Out + 2];
	_msg_out_unicode[0] = 0;

	//convert to unicode
	MultiByteToWideChar(CP_ACP, NULL, _msg_out_ansi, (int)strlen(_msg_out_ansi) + 1, _msg_out_unicode, _msgLen_Out);

	//Output
	this->_ctl_edit_b64_input.SetWindowText(_msg_out_unicode);

	//free memory	
	free(_msg_in_unicode);
	free(_msg_in_byte);
	free(_msg_out_ansi);
	free(_msg_out_part_ansi);
	free(_msg_out_unicode);
}

void CTabToolsDlg::OnBnClickedBtnB64Dec()
{
	//UI
	this->_ctl_edit_b64_input.SetFocus();

	//Flags
	this->_bANSI = this->_ctl_rad_b64_ansi->GetCheck();

	UINT32 _msgLen;
	UINT32 _msgLen_Out;
	UINT32 _blocks_4;

	//Get length in unicode
	_msgLen = this->_ctl_edit_b64_input.GetWindowTextLength();
	if(!_msgLen){return;}

	//Allocate Memory
	unsigned char *_msg_out_part_ansi = new unsigned char[8];
	wchar_t *_msg_in_unicode = new wchar_t[_msgLen + 2];
	unsigned char *_msg_in_byte = new unsigned char[_msgLen + 2];

	//Get input
	this->_ctl_edit_b64_input.GetWindowText(_msg_in_unicode, _msgLen + 1);

	//Convert to ansi
	unsigned char _tmpChr;
	for(UINT32 _i = 0; _i < _msgLen; _i++)
	{
		_tmpChr = _msg_in_unicode[_i] & 0x00ff;
		_tmpChr = (unsigned char) ((_tmpChr < 43 || _tmpChr > 122) ? 0 : cd64[ _tmpChr - 43 ]);
		if(_tmpChr){
			_tmpChr = (unsigned char) ((_tmpChr == '$') ? 0 : _tmpChr - 61);
		}
		if(_tmpChr){
			_msg_in_byte[_i] = _tmpChr - 1;
		}else{
			_msg_in_byte[_i] = 0;
		}
	}

	//add 0
	_msg_in_byte[_msgLen] = 0;
	
	//Calculate number of blocks
	_blocks_4 = (_msgLen / 4);

	//Get Length for output
	_msgLen_Out = (_blocks_4 * 4 + 16);

	//Initialize out string
	char *_msg_out_ansi = new char[_msgLen_Out + 2];
	wchar_t *_msg_out_unicode = new wchar_t[_msgLen_Out + 2];
	_msg_out_ansi[0] = 0;
	_msg_out_unicode[0] = 0;

	//Decode
	for(UINT32 _i = 0; _i < _blocks_4; _i++)
	{
		_b64DecodeBlock(&_msg_in_byte[4 * _i], _msg_out_part_ansi);

		//strcat
		//strcat_s(_msg_out_ansi, _msgLen_Out + 1, (char *)_msg_out_part_ansi);
		memcpy(_msg_out_ansi + 3 * _i, _msg_out_part_ansi, 3);
		_msg_out_ansi[3 * _i + 3] = 0;
	}

	if(this->_bANSI){
		//convert to unicode
		MultiByteToWideChar(CP_ACP, NULL, _msg_out_ansi, (int)strlen(_msg_out_ansi) + 1, _msg_out_unicode, _msgLen_Out);
	} else {
		memcpy(_msg_out_unicode, _msg_out_ansi, _msgLen_Out);
		_msg_out_unicode[_msgLen_Out] = 0;
	}
	
	//Output
	this->_ctl_edit_b64_input.SetWindowText(_msg_out_unicode);

	//free memory	
	free(_msg_in_unicode);
	free(_msg_in_byte);
	free(_msg_out_ansi);
	free(_msg_out_part_ansi);
	free(_msg_out_unicode);

}

//***** Password Generator *****
void CTabToolsDlg::OnBnClickedBtnGen()
{
	TCHAR *_gLenStr = new TCHAR[10];
	TCHAR *_gGenPass;

	char _genFlags;
	int _gLen;

	this->_ctl_edit_gpass.SetFocus();

	//Get Flags
	_genFlags = 0;
	if(this->_ctl_chk_upc.GetCheck()){_genFlags |= 0x01;}
	if(this->_ctl_chk_lwc.GetCheck()){_genFlags |= 0x02;}
	if(this->_ctl_chk_num.GetCheck()){_genFlags |= 0x04;}
	if(this->_ctl_chk_sym.GetCheck()){_genFlags |= 0x08;}
	if(!_genFlags) {return;}

	//Get Length
	this->_ctl_edit_length.GetWindowText(_gLenStr, 8);
	_gLen = _ttoi(_gLenStr);
	if(!_gLen) {this->_ctl_edit_length.SetFocus(); return;}

	//Allocate Memory
	_gGenPass = new TCHAR[_gLen + 1];
	
	//Set String Ending
	_gGenPass[_gLen] = 0;

	//Set Random Seed
	srand(GetTickCount());
	while((--_gLen) + 1)
	{
		_gGenPass[_gLen] = (rand() / 128);
		if(_gGenPass[_gLen] >= 65 && _gGenPass[_gLen] <= 90){ //Upper Case
			if(!(_genFlags & 0x01)){_gLen++; continue;}
		}else
		if(_gGenPass[_gLen] >= 97 && _gGenPass[_gLen] <= 122){ //Lower Case
			if(!((_genFlags & 0x02) >> 1)){_gLen++; continue;}
		}else
		if(_gGenPass[_gLen] >= 48 && _gGenPass[_gLen] <= 57){ //Numbers
			if(!((_genFlags & 0x04) >> 2)){_gLen++; continue;}
		}else
		if(_gGenPass[_gLen] >= 33 && _gGenPass[_gLen] <= 38 || _gGenPass[_gLen] == 63){ //Symbols
			if(!((_genFlags & 0x08) >> 3)){_gLen++; continue;}
		}else{
			_gLen++;
		}
	}
	this->_ctl_edit_gpass.SetWindowText(_gGenPass);
	free(_gGenPass);
	free(_gLenStr);
}

//***** Custom Funcs *****
void CTabToolsDlg::_EnableCtrls(char _cEnable)
{
	_ctl_rad_etimes_1->EnableWindow(_cEnable);
	_ctl_rad_etimes_7->EnableWindow(_cEnable);
	_ctl_rad_etimes_35->EnableWindow(_cEnable);
	_ctl_btn_add_se.EnableWindow(_cEnable);
	_ctl_btn_rem_se.EnableWindow(_cEnable);
	_ctl_btn_clr_se.EnableWindow(_cEnable);
	_ctl_btn_stop_se.EnableWindow(!_cEnable);
	_ctl_btn_start_se.EnableWindow(_cEnable);
}

void CTabToolsDlg::_AddToList(int _iIndex, TCHAR *_lpszName, TCHAR *_lpszStatus)
{
	LVITEM _curItem = {0};
	
	//Set mask
	_curItem.mask = LVIF_TEXT;// | LVS_EX_BORDERSELECT;
	_curItem.cColumns = 2;//Number of Columns
	
	//Add
	_curItem.iItem = _iIndex;
	_curItem.iSubItem = 0;
	_curItem.pszText = _lpszName;
	this->_ctl_list_files.InsertItem(&_curItem);
	
	_curItem.iSubItem = 1;
	_curItem.pszText = _lpszStatus;
	this->_ctl_list_files.SetItem(&_curItem);
}

void CTabToolsDlg::_SetItemStatusInList(int _iIndex, TCHAR *_lpszStatus)
{
	LVITEM _curItem = {0};
	
	//Set mask
	_curItem.mask = LVIF_TEXT;// | LVS_EX_BORDERSELECT;
	_curItem.cColumns = 2;//Number of Columns

	_curItem.iItem = _iIndex;
	_curItem.iSubItem = 1;
	_curItem.pszText = _lpszStatus;
	this->_ctl_list_files.SetItem(&_curItem);
	this->_ctl_list_files.EnsureVisible(_iIndex, FALSE);
}



