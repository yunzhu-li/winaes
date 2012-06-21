
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

#ifndef _RIJNDAEL_H
#define _RIJNDAEL_H

#include "../stdafx.h"


// ** Thread-safe implementation

// ** Aes cipher
// ** 128bit block size
// ** 256bit key


extern	int Rijndael_set_key_encrypt(DWORD *rk,const BYTE *key,const int keybits);
extern	int Rijndael_set_key_decrypt(DWORD *rk,const BYTE *key,const int keybits);
extern	void Rijndael_encrypt(const DWORD *rk,const int nrounds,const BYTE *plaintext,BYTE *ciphertext);
extern	void Rijndael_decrypt(const DWORD *rk,const int nrounds,const BYTE *ciphertext,BYTE *plaintext);

#endif
