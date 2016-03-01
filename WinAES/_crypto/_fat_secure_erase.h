
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
//*  _fat_secure_erase.h -- Secure File Eraser(n times)        **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************

#ifndef _FAT_SECURE_ERASE_H_
#define _FAT_SECURE_ERASE_H_

#include "../stdafx.h"

extern BOOL File_Secure_Erase(TCHAR			*_filePath,
							  unsigned char	 _times,
							  void			(* _callback)(unsigned int _percent, void *_param),
							  void			*_callbackParam
							  );

#endif
