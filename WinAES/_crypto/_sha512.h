
#ifndef _SHA512_H_
#define _SHA512_H_

#include "../stdafx.h"

// ** Thread-safe implementation

// ** Sha2 hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

typedef struct {
	DWORD	inputLen;
	QWORD	A,B,C,D,E,F,G,H;
	QWORD	totalLen;
	BYTE	input[128];
} SHA512_DATA;


extern	void SHA512_Init(SHA512_DATA *sha);
extern	void SHA512_Data(SHA512_DATA *sha,const void *buffer,DWORD len);
extern	void SHA512_Finalize(SHA512_DATA *sha,BYTE *hash);

#endif



