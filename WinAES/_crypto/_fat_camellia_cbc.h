
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
//*  _fat_camellia_cbc.h -- Camellia cipher on Files(Mode:CBC) **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************

// Header
#ifndef __FAT_CAMELLIA_CBC_H_
#define __FAT_CAMELLIA_CBC_H_

#include "_Camellia.h"

// Function: 
//	File_Camellia_CBC(...) -- Camellia Operations on a File.
//
// Parameters:
// TCHAR * _filepath_in     -- Full path of the file to be encrypted
// TCHAR * _filepath_out    -- Full path of the encrypted file to output.
// unsigned char *_cipherKey   -- AES encryption key
// void(* _callback)(unsigned int _percent, void *_param)
//                          --CallBack Function -- precents of current operation, with a custom parameter pointer (void *_callbackParam)
//

extern BOOL _File_Camellia_Encrypt_CBC(	LPTSTR			_filepath_in,
										LPTSTR			_filepath_out,
										unsigned char	*_cipherKey,
										void			(* _callback)(unsigned int _percent, void *_param),
										void			*_callbackParam
										);

extern BOOL _File_Camellia_Decrypt_CBC(	LPTSTR			_filepath_in,
										LPTSTR			_filepath_out,
										unsigned char	*_cipherKey,
										void			(* _callback)(unsigned int _percent, void *_param),
										void			*_callbackParam
										);

#endif
