
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

#ifndef _KECCAK512_H_
#define _KECCAK512_H_

#include "../stdafx.h"

// ** Thread-safe implementation

// ** Keccak hashing
// ** 512bit hash

typedef unsigned __int64	QWORD;

#define cKeccakB    1600
#define cKeccakR    512

typedef struct {
	BYTE	state[cKeccakB / 8];
	int		bitsInQueue;
} KECCAK512_DATA;


extern	void Keccak512_Init(KECCAK512_DATA *keccak);
extern	void Keccak512_Data(KECCAK512_DATA *keccak,const BYTE *buffer,DWORD len);
extern	void Keccak512_Finalize(KECCAK512_DATA *keccak,BYTE *hash);

#endif
