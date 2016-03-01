
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
