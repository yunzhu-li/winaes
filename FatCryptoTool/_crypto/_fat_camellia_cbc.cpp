
/////////////////////////////////////
//                                 //
//		Fat Cryptography Tools     //
//                                 //
//		http://hevphoto.com        //
//		fatlyz_pub@gmail.com       //
//		(C)2012 FatLYZ             //
//                                 //
/////////////////////////////////////

//***************************************************************
//***************************************************************
//*  _fat_camellia_cbc.cpp--Camellia cipher on Files(Mode:CBC) **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************


#include "_fat_camellia_cbc.h"

#include "io.h"


void _Camellia_Encrypt_CBC(BYTE *_buf, UINT32 _iBlockCount, BYTE *_expanded_key, BYTE *_cbc_state)
{
	//CBC Mode
	for(UINT32 _curBlock = 0; _curBlock < _iBlockCount; _curBlock++)
	{
		//XOR with next block
		for (BYTE _ix = 0; _ix < 16; _ix++){
			_buf[16 * _curBlock + _ix] ^= _cbc_state[_ix];
		}
		//Encrypt block
		Camellia_encrypt(_expanded_key, 256, &_buf[16 * _curBlock], &_buf[16 * _curBlock]);

		//Set state = encrypted block
		for (BYTE _ix = 0; _ix < 16; _ix++){
			_cbc_state[_ix] = _buf[16 * _curBlock + _ix];
		}
	}//for

}

void _Camellia_Decrypt_CBC(BYTE *_buf, UINT32 _iBlockCount, BYTE *_expanded_key, BYTE *_cbc_state_prev, BYTE *_cbc_state_curr)
{

	//CBC Mode
	for(UINT32 _curBlock = 0; _curBlock < _iBlockCount; _curBlock++)
	{
		//Set state = encrypted block
		for (unsigned char _ix = 0; _ix < 16; _ix++){
			_cbc_state_prev[_ix] = _cbc_state_curr[_ix];
			_cbc_state_curr[_ix] = _buf[16 * _curBlock + _ix];
		}
		Camellia_decrypt(_expanded_key, 256, &_buf[16 * _curBlock], &_buf[16 * _curBlock]);
		//decrypted block XOR previous encrypted block
		for (unsigned char _ix = 0; _ix < 16; _ix++){
			_buf[16 * _curBlock + _ix] ^= _cbc_state_prev[_ix];
		}
	}

}

BOOL _File_Camellia_Encrypt_CBC(LPTSTR _filepath_in, LPTSTR _filepath_out, BYTE *_cipherKey, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{

	
	UINT32 _readBytes;
	UINT64 _curFilePos;
	UINT64 _i64FileLen;
	
	BYTE *_buf;		//buffer
	_buf = (BYTE *)malloc(1048592); // = 1048576 + 16

	unsigned char *_expanded_key;		//Expanded key
	_expanded_key = (unsigned char *)malloc(1024);
	BYTE _cbc_state[16];			//IV

	//Open files
	_fcloseall();
	FILE *_file_in;
	_tfopen_s(&_file_in, _filepath_in, _T("rb"));
	if (_file_in == NULL) {return 0;}
	FILE* _file_out;
	_tfopen_s(&_file_out, _filepath_out, _T("wb"));
	if (_file_out == NULL) {return 0;}

	//Get file length
	_fseeki64(_file_in, 0, SEEK_END);
	_i64FileLen = _ftelli64(_file_in);
	_fseeki64(_file_in, 0, SEEK_SET);

	//Generate Random IV & Write to Output File
	srand(GetTickCount());
	for (unsigned char _ix = 0; _ix < 16; _ix++){
		_cbc_state[_ix] = (rand() & 0x00ff);
	}
	fwrite(_cbc_state, 16, 1, _file_out);

	//Key expansion
	Camellia_set_key(_expanded_key, 256, _cipherKey);

	//Reset pos
	_curFilePos = 0;

	for(;;)
	{
		//Clear buffer
		memset(_buf, 0, 1048576);
		
		//Read file
		_readBytes = (UINT32)fread(_buf, 1, 1048576, _file_in);

		//Pos += read _readBytes
		_curFilePos += _readBytes;

		// Operation Process
		if (_callback != NULL){
			_callback((unsigned int)(100 * ((_curFilePos + 1) * 1.00 / _i64FileLen)), _callbackParam);
		}

		if(_readBytes == 1048576)
		{
			//CBC Encrypt Blocks
			_Camellia_Encrypt_CBC(_buf, (1048576 / 16), _expanded_key, _cbc_state);

			//Write file
			fwrite(_buf, 1048576, 1, _file_out);

		}else
		{
			//Pad
			_buf[_readBytes] = 0x01;
			_readBytes += 1;
			_readBytes += (16 - ((_readBytes) % 16));

			//CBC Encrypt Blocks
			_Camellia_Encrypt_CBC(_buf, (_readBytes / 16), _expanded_key, _cbc_state);

			//Write file
			fwrite(_buf, _readBytes, 1, _file_out);

			//Exit for
			break;

		}//if
	}//for

	//Close files
	fclose(_file_in);
	fclose(_file_out);

	free(_expanded_key);
	free(_buf);

	return 1;

}


BOOL _File_Camellia_Decrypt_CBC(LPTSTR _filepath_in, LPTSTR _filepath_out, BYTE *_cipherKey, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	UINT32 _readBytes;
	UINT64 _curFilePos;
	UINT64 _i64FileLen;
	BYTE _bLastBlock;
	_bLastBlock = 0;
	
	BYTE *_buf;		//buffer
	_buf = (BYTE *)malloc(1048576);

	unsigned char *_expanded_key;		//Expanded key
	_expanded_key = (unsigned char *)malloc(1024);


	//** Block for XOR operations in CBC Mode **
	BYTE _cbc_iv[16];				//IV
	BYTE _cbc_state_prev[16];		//Previous encrypted block
	BYTE _cbc_state_curr[16];		//Current encrypted block
	//** CBC Mode End **

	//Open files
	_fcloseall();
	FILE *_file_in;
	_tfopen_s(&_file_in, _filepath_in, _T("rb"));
	if (_file_in == NULL) {return 0;}
	FILE* _file_out;
	_tfopen_s(&_file_out, _filepath_out, _T("wb"));
	if (_file_out == NULL) {return 0;}

	//Get file length
	_fseeki64(_file_in, 0, SEEK_END);
	_i64FileLen = _ftelli64(_file_in);
	_fseeki64(_file_in, 0, SEEK_SET);

	//Read IV form File.
	fread(_cbc_iv, 16, 1, _file_in);
	_i64FileLen -= 16;

	//Key expansion
	Camellia_set_key(_expanded_key, 256, _cipherKey);

	//Reset pos
	_curFilePos = 0;

	for(;;)
	{
		//Clear buffer
		memset(_buf, 0, 1048576);
		
		//Read file
		_readBytes = (UINT32)fread(_buf, 1, 1048576, _file_in);

		//Pos += read _readBytes
		_curFilePos += _readBytes;
		if (_i64FileLen == _curFilePos){_bLastBlock = 1;}

		// Operation Process
		if (_callback != NULL){
			_callback((unsigned int)(100 * ((_curFilePos + 1) * 1.00 / _i64FileLen)), _callbackParam);
		}

		//CBC Decrypt Blocks
		if (_curFilePos == _readBytes)
		{	
			//Set iv
			for (unsigned char _ix = 0; _ix < 16; _ix++){
				_cbc_state_curr[_ix] = _cbc_iv[_ix];
			}
		}
		_Camellia_Decrypt_CBC(_buf, (_readBytes / 16), _expanded_key, _cbc_state_prev, _cbc_state_curr);

		if(_bLastBlock)
		{
			//UnPad
			//Remove 0x01 0x00 0x00...
			for(;;)
			{
				if((_readBytes))
				{
					if(_buf[_readBytes - 1] == 0x01){
						//remove 0x01
						_readBytes--;
						break;
					}
					//remove 0x00
					_readBytes--;
				}else{
					//remove last byte of last block
					_chsize_s(_file_out->_file, _i64FileLen - 16 - 1);
					_readBytes = 0;
					break;
				}
			}
			//write last block
			fwrite(_buf, _readBytes, 1, _file_out);
			break;
		}

		//Write file
		fwrite(_buf, _readBytes, 1, _file_out);

	}//for

	//Close files
	fclose(_file_in);
	fclose(_file_out);

	free(_expanded_key);
	free(_buf);

	return 1;


}




