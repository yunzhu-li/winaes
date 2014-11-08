
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
//*  fatfilemd5.h -- Generates MD5 Hash of a File or String    **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************

// Header

#ifndef __FATFILEMD5_H_
#define __FATFILEMD5_H_

#include "../stdafx.h" 

typedef unsigned long int _uint32;
typedef unsigned __int64 _uint64;
/* Calculates MD5 of a String. */
//_hash--array--_uint32 _hash[4]
extern void StringMD5(char * _lpszStr, unsigned long *_hash);

/* Calculates MD5 of a File. */
//_callback--CallBack Function -- precents of current operation, with a custom parameter pointer (void *_callbackParam)
extern BOOL FileMD5(TCHAR * _filepath, unsigned long *_hash, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam);



#endif //__FATFILEMD5_H_

