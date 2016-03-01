
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
