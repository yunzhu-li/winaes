
/////////////////////////////////////
//                                 //
//		Fat Cryptography Tools     //
//                                 //
//		http://hevphoto.com        //
//		fatlyz_pub@gmail.com       //
//		(C)2012 FatLYZ             //
//                                 //
/////////////////////////////////////

#ifndef _SHA1_H_
#define _SHA1_H_

#include "../stdafx.h"

// ** Thread-safe implementation

// ** Sha1 hashing
// ** 160bit hash

typedef unsigned __int64	QWORD;

typedef struct {
	unsigned char	input[64];
	UINT32		inputLen;
	UINT32		A, B, C, D, E;
	UINT64		totalLen;
}SHA1_DATA;


extern	void SHA1_Init(SHA1_DATA *sha);
extern	void SHA1_Data(SHA1_DATA *sha, const void *buffer, unsigned long len);
extern	void SHA1_Finalize(SHA1_DATA *sha, unsigned char *hash);

#endif



