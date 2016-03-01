
#ifndef _SHA256_H_
#define _SHA256_H_


#include "../stdafx.h"


// ** Thread-safe implementation

// ** Sha256 hashing
// ** 256bit hash

typedef struct {
	unsigned char	input[64];
	UINT32		inputLen;
	UINT32		A, B, C, D, E, F, G, H;
	UINT64		totalLen;
} SHA256_DATA;


extern void SHA256_Init(SHA256_DATA *sha);
extern void SHA256_Data(SHA256_DATA *sha, const void *buffer, unsigned long len);
extern void SHA256_Finalize(SHA256_DATA *sha, unsigned char *hash);


#endif

