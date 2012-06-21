
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

#include "_SHA1.h"

/*
 * Some helper macros for processing 32-bit values, while
 * being careful about 32-bit vs 64-bit system differences.
 */
#if SIZEOF_LONG > 4
	#define	TRUNCLONG(x)	((x) & IL_MAX_UINT32)
	#define	ROTATE(x,n)		(TRUNCLONG(((x) << (n))) | ((x) >> (32 - (n))))
#else
	#define	TRUNCLONG(x)	(x)
	#define	ROTATE(x,n)		(((x) << (n)) | ((x) >> (32 - (n))))
#endif

/*
 * Functions for each of the SHA rounds.
 */
#define	FROUND1(x,y,z)	(((x) & (y)) | (TRUNCLONG(~(x)) & (z)))
#define	FROUND2(x,y,z)	((x) ^ (y) ^ (z))
#define	FROUND3(x,y,z)	(((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define	FROUND4(x,y,z)	((x) ^ (y) ^ (z))

/*
 * Constants used in each of the SHA rounds.
 */
#define	KROUND1			0x5A827999
#define	KROUND2			0x6ED9EBA1
#define	KROUND3			0x8F1BBCDC
#define	KROUND4			0xCA62C1D6

void SHA1_Init(SHA1_DATA *sha)
{
	sha->inputLen = 0;
	sha->A = 0x67452301;
	sha->B = 0xEFCDAB89;
	sha->C = 0x98BADCFE;
	sha->D = 0x10325476;
	sha->E = 0xC3D2E1F0;
	sha->totalLen = 0;
}

/*
 * Process a single block of input using the hash algorithm.
 */
static void ProcessBlock(SHA1_DATA *sha, const unsigned char *block)
{
	UINT32 W[80];
	UINT32 a, b, c, d, e;
	UINT32 temp;
	int t;

	/* Unpack the block into 80 32-bit words */
	for(t = 0; t < 16; ++t)
	{
		W[t] = (((UINT32)(block[t * 4 + 0])) << 24) |
		       (((UINT32)(block[t * 4 + 1])) << 16) |
		       (((UINT32)(block[t * 4 + 2])) <<  8) |
		        ((UINT32)(block[t * 4 + 3]));
	}
	for(t = 16; t < 80; ++t)
	{
		temp = W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16];
		W[t] = ROTATE(temp, 1);
	}

	/* Load the SHA state into local variables */
	a = sha->A;
	b = sha->B;
	c = sha->C;
	d = sha->D;
	e = sha->E;

	/* Round 1 */
	for(t = 0; t < 20; ++t)
	{
		temp = TRUNCLONG(ROTATE(a, 5) + FROUND1(b, c, d) +
						 e + W[t] + KROUND1);
		e = d;
		d = c;
		c = ROTATE(b, 30);
		b = a;
		a = temp;
	}

	/* Round 2 */
	for(t = 0; t < 20; ++t)
	{
		temp = TRUNCLONG(ROTATE(a, 5) + FROUND2(b, c, d) +
						 e + W[t + 20] + KROUND2);
		e = d;
		d = c;
		c = ROTATE(b, 30);
		b = a;
		a = temp;
	}

	/* Round 3 */
	for(t = 0; t < 20; ++t)
	{
		temp = TRUNCLONG(ROTATE(a, 5) + FROUND3(b, c, d) +
						 e + W[t + 40] + KROUND3);
		e = d;
		d = c;
		c = ROTATE(b, 30);
		b = a;
		a = temp;
	}

	/* Round 4 */
	for(t = 0; t < 20; ++t)
	{
		temp = TRUNCLONG(ROTATE(a, 5) + FROUND4(b, c, d) +
						 e + W[t + 60] + KROUND4);
		e = d;
		d = c;
		c = ROTATE(b, 30);
		b = a;
		a = temp;
	}

	/* Combine the previous SHA state with the new state */
	sha->A = TRUNCLONG(sha->A + a);
	sha->B = TRUNCLONG(sha->B + b);
	sha->C = TRUNCLONG(sha->C + c);
	sha->D = TRUNCLONG(sha->D + d);
	sha->E = TRUNCLONG(sha->E + e);

	/* Clear the temporary state */
	memset(W, 0, sizeof(UINT32) * 80);
	a = b = c = d = e = temp = 0;
}

void SHA1_Data(SHA1_DATA *sha, const void *buffer, unsigned long len)
{
	unsigned long templen;

	/* Add to the total length of the input stream */
	sha->totalLen += (UINT64)len;

	/* Copy the blocks into the input buffer and process them */
	while(len > 0)
	{
		if(!(sha->inputLen) && len >= 64)
		{
			/* Short cut: no point copying the data twice */
			ProcessBlock(sha, (const unsigned char *)buffer);
			buffer = (const void *)(((const unsigned char *)buffer) + 64);
			len -= 64;
		}
		else
		{
			templen = len;
			if(templen > (64 - sha->inputLen))
			{
				templen = 64 - sha->inputLen;
			}
			memcpy(sha->input + sha->inputLen, buffer, templen);
			if((sha->inputLen += templen) >= 64)
			{
				ProcessBlock(sha, sha->input);
				sha->inputLen = 0;
			}
			buffer = (const void *)(((const unsigned char *)buffer) + templen);
			len -= templen;
		}
	}
}

/*
 * Write a 32-bit big-endian long value to a buffer.
 */
static void WriteLong(unsigned char *buf, UINT32 value)
{
	buf[0] = (unsigned char)(value >> 24);
	buf[1] = (unsigned char)(value >> 16);
	buf[2] = (unsigned char)(value >> 8);
	buf[3] = (unsigned char)value;
}

void SHA1_Finalize(SHA1_DATA *sha, unsigned char *hash)
{
	UINT64 totalBits;

	/* Compute the final hash if necessary */
	if(hash)
	{
		/* Pad the input data to a multiple of 512 bits */
		if(sha->inputLen >= 56)
		{
			/* Need two blocks worth of padding */
			sha->input[(sha->inputLen)++] = (unsigned char)0x80;
			while(sha->inputLen < 64)
			{
				sha->input[(sha->inputLen)++] = (unsigned char)0x00;
			}
			ProcessBlock(sha, sha->input);
			sha->inputLen = 0;
		}
		else
		{
			/* Need one block worth of padding */
			sha->input[(sha->inputLen)++] = (unsigned char)0x80;
		}
		while(sha->inputLen < 56)
		{
			sha->input[(sha->inputLen)++] = (unsigned char)0x00;
		}
		totalBits = (sha->totalLen << 3);
		WriteLong(sha->input + 56, (UINT32)(totalBits >> 32));
		WriteLong(sha->input + 60, (UINT32)totalBits);
		ProcessBlock(sha, sha->input);

		/* Write the final hash value to the supplied buffer */
		WriteLong(hash,      sha->A);
		WriteLong(hash + 4,  sha->B);
		WriteLong(hash + 8,  sha->C);
		WriteLong(hash + 12, sha->D);
		WriteLong(hash + 16, sha->E);
	}

	/* Fill the entire context structure with zeros to blank it */
	memset(sha, 0, sizeof(SHA1_DATA));
}