
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
//*  fatfilehash.h -- Generates Hash of a File                 **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************


#ifndef __FATFILEHASH_H_
#define __FATFILEHASH_H_

#include "../stdafx.h"
#include "_SHA1.h"
#include "_SHA256.h"
#include "_SHA512.h"
#include "_Keccak512.h"


extern BOOL FileSHA1(	TCHAR			*_filepath,
						unsigned char	*_hash,
						void			(* _callback)(unsigned int _percent, void *_param),
						void			*_callbackParam
						);

extern BOOL FileSHA256(	TCHAR			*_filepath,
						unsigned char	*_hash,
						void			(* _callback)(unsigned int _percent, void *_param),
						void			*_callbackParam
						);


extern BOOL FileSHA512(	TCHAR			*_filepath,
						unsigned char	*_hash,
						void			(* _callback)(unsigned int _percent, void *_param),
						void			*_callbackParam
						);

extern BOOL FileKeccak512(	TCHAR			*_filepath,
							unsigned char	*_hash,
							void			(* _callback)(unsigned int _percent, void *_param),
							void			*_callbackParam
							);


#endif





