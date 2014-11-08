
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		contact@fatlyz.com
//		(C) 2014 FatLYZ.COM
//
/////////////////////////////////////


//***************************************************************
//***************************************************************
//*  fatfilehash.cpp -- Generates Hash of a File               **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************


#include "_fatfilehash.h"





BOOL FileSHA1(TCHAR *_filepath, unsigned char *_hash, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	UINT64 _i64FileLen;
	UINT64 _i64BigBlocks;
	SHA1_DATA _SHAData;

	//Open File
	FILE* _inFile;
	_tfopen_s(&_inFile, _filepath, _T("rb"));
	if (_inFile == NULL) {return 0;}

	//Allocate Memory
	unsigned char *_buf = new unsigned char[1048576];

	//Get file length
	_fseeki64(_inFile, 0, SEEK_END);
	_i64FileLen = _ftelli64(_inFile);
	_fseeki64(_inFile, 0, SEEK_SET);						//Position back 2 zero

	_i64BigBlocks = (_i64FileLen / 1048576) + 1;			//get number of big blocks

	//SHA Initialization
	SHA1_Init(&_SHAData);
	
	for(UINT64 _i64CurBlock = 0; _i64CurBlock < _i64BigBlocks; _i64CurBlock++)
	{
		if (_callback){
			_callback((unsigned int)(100 * ((_i64CurBlock + 1) * 1.00 / _i64BigBlocks)), _callbackParam);
		}

		//Last Big Block
		if(_i64CurBlock == _i64BigBlocks - 1){ 
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(_i64FileLen % 1048576), 1, _inFile);
			//_SHAData.inputLen = 0;//(UINT32)(_i64FileLen % 1048576);
			SHA1_Data(&_SHAData, _buf, (UINT32)(_i64FileLen % 1048576));
			SHA1_Finalize(&_SHAData, _hash);

		}else{
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(1048576), 1, _inFile);
			//Set Len
			//_SHAData.inputLen = 0;
			SHA1_Data(&_SHAData, _buf, 1048576);

		}//if
	}//for
	fclose(_inFile);
	free(_buf);
	return 1;
}


BOOL FileSHA256(TCHAR *_filepath, unsigned char *_hash, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	UINT64 _i64FileLen;
	UINT64 _i64BigBlocks;
	SHA256_DATA _SHAData;

	//Open File
	FILE* _inFile;
	_tfopen_s(&_inFile, _filepath, _T("rb"));
	if (_inFile == NULL) {return 0;}

	//Allocate Memory
	unsigned char *_buf = new unsigned char[1048576];

	//Get file length
	_fseeki64(_inFile, 0, SEEK_END);
	_i64FileLen = _ftelli64(_inFile);
	_fseeki64(_inFile, 0, SEEK_SET);						//Position back 2 zero

	_i64BigBlocks = (_i64FileLen / 1048576) + 1;			//get number of big blocks

	//SHA Initialization
	SHA256_Init(&_SHAData);
	
	for(UINT64 _i64CurBlock = 0; _i64CurBlock < _i64BigBlocks; _i64CurBlock++)
	{
		if (_callback){
			_callback((unsigned int)(100 * ((_i64CurBlock + 1) * 1.00 / _i64BigBlocks)), _callbackParam);
		}

		//Last Big Block
		if(_i64CurBlock == _i64BigBlocks - 1){ 
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(_i64FileLen % 1048576), 1, _inFile);
			//_SHAData.inputLen = 0;//(UINT32)(_i64FileLen % 1048576);
			SHA256_Data(&_SHAData, _buf, (UINT32)(_i64FileLen % 1048576));
			SHA256_Finalize(&_SHAData, _hash);

		}else{
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(1048576), 1, _inFile);
			//Set Len
			//_SHAData.inputLen = 0;
			SHA256_Data(&_SHAData, _buf, 1048576);

		}//if
	}//for
	fclose(_inFile);
	free(_buf);
	return 1;
}

BOOL FileSHA512(TCHAR *_filepath, unsigned char *_hash, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	UINT64 _i64FileLen;
	UINT64 _i64BigBlocks;
	SHA512_DATA _SHAData;

	//Open File
	FILE* _inFile;
	_tfopen_s(&_inFile, _filepath, _T("rb"));
	if (_inFile == NULL) {return 0;}

	//Allocate Memory
	unsigned char *_buf = new unsigned char[1048576];

	//Get file length
	_fseeki64(_inFile, 0, SEEK_END);
	_i64FileLen = _ftelli64(_inFile);
	_fseeki64(_inFile, 0, SEEK_SET);						//Position back 2 zero

	_i64BigBlocks = (_i64FileLen / 1048576) + 1;			//get number of big blocks

	//SHA Initialization
	SHA512_Init(&_SHAData);
	
	for(UINT64 _i64CurBlock = 0; _i64CurBlock < _i64BigBlocks; _i64CurBlock++)
	{
		if (_callback){
			_callback((unsigned int)(100 * ((_i64CurBlock + 1) * 1.00 / _i64BigBlocks)), _callbackParam);
		}

		//Last Big Block
		if(_i64CurBlock == _i64BigBlocks - 1){ 
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(_i64FileLen % 1048576), 1, _inFile);
			//_SHAData.inputLen = 0;//(UINT32)(_i64FileLen % 1048576);
			SHA512_Data(&_SHAData, _buf, (UINT32)(_i64FileLen % 1048576));
			SHA512_Finalize(&_SHAData, _hash);

		}else{
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(1048576), 1, _inFile);
			//Set Len
			//_SHAData.inputLen = 0;
			SHA512_Data(&_SHAData, _buf, 1048576);

		}//if
	}//for
	fclose(_inFile);
	free(_buf);
	return 1;
}



BOOL FileKeccak512(TCHAR *_filepath, unsigned char *_hash, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	UINT64 _i64FileLen;
	UINT64 _i64BigBlocks;
	KECCAK512_DATA _KeccakData;

	//Open File
	FILE* _inFile;
	_tfopen_s(&_inFile, _filepath, _T("rb"));
	if (_inFile == NULL) {return 0;}

	//Get file length
	_fseeki64(_inFile, 0, SEEK_END);
	_i64FileLen = _ftelli64(_inFile);
	_fseeki64(_inFile, 0, SEEK_SET);						//Position back 2 zero

	_i64BigBlocks = (_i64FileLen / 1048576) + 1;			//get number of big blocks

	//Allocate Memory
	unsigned char *_buf = new unsigned char[1048576];

	//SHA Initialization
	Keccak512_Init(&_KeccakData);
	
	for(UINT64 _i64CurBlock = 0; _i64CurBlock < _i64BigBlocks; _i64CurBlock++)
	{
		if (_callback){
			_callback((unsigned int)(100 * ((_i64CurBlock + 1) * 1.00 / _i64BigBlocks)), _callbackParam);
		}

		//Last Big Block
		if(_i64CurBlock == _i64BigBlocks - 1){ 
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(_i64FileLen % 1048576), 1, _inFile);
			//_SHAData.inputLen = 0;//(UINT32)(_i64FileLen % 1048576);
			Keccak512_Data(&_KeccakData, _buf, (UINT32)(_i64FileLen % 1048576));
			Keccak512_Finalize(&_KeccakData, _hash);

		}else{
			//read data
			memset(_buf, 0, 1048576);
			fread(_buf, (size_t)(1048576), 1, _inFile);
			//Set Len
			//_SHAData.inputLen = 0;
			Keccak512_Data(&_KeccakData, _buf, 1048576);

		}//if
	}//for
	fclose(_inFile);
	free(_buf);
	return 1;
}








