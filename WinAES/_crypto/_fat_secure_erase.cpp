
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

#include "_fat_secure_erase.h"

BOOL File_Secure_Erase(TCHAR *_filePath, unsigned char _times, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	UINT64 _i64Blocks;			//Blocks
	UINT64 _i64Filelen;			//File Length
	int _eDataVal;


	//Open files
	_fcloseall();
	FILE *_file_in;
	_tfopen_s(&_file_in, _filePath, _T("r+b"));
	if (_file_in == NULL) {return 0;}
	
	//Get file length
	_fseeki64(_file_in, 0, SEEK_END);
	_i64Filelen = _ftelli64(_file_in);
	_fseeki64(_file_in, 0, SEEK_SET);

	
	//Check if file is empty 
	if(_i64Filelen == 0){
		//Delete file
		fclose(_file_in);
		if(!_wremove(_filePath)){
			return 1;
		}else{
			return 0;
		}
	}

	unsigned char *_blockbuf;	//Block (1MBytes)
	_blockbuf = (unsigned char *)malloc(1048576);

	//Erase File
	_eDataVal = 0;

	_i64Blocks = (((_i64Filelen) / 1048576) + 1);
	for(unsigned char _ix = 0; _ix < _times; _ix++)
	{
		_fseeki64(_file_in, 0, SEEK_SET);
		// Set Buffer
		_eDataVal = (!_eDataVal) * 0xff;
		for(UINT64 _curBlock = 0; _curBlock < _i64Blocks; _curBlock++)
		{
			// Operation Process
			if (_callback != NULL){
				_callback((UINT32)_curBlock, _callbackParam);
			}

			memset(_blockbuf, _eDataVal, 1048576);
			// Write data
			if (_curBlock == (_i64Blocks - 1)){
				fwrite(_blockbuf, (UINT32)(_i64Filelen % 1048576), 1, _file_in);
			}else{
				fwrite(_blockbuf, 1048576, 1, _file_in);
			}
		}
	}
	fclose(_file_in);
	free(_blockbuf);

	if(!_wremove(_filePath)){
		return 1;
	}else{
		return 0;
	}

}

