
/////////////////////////////////////
//
//		WinAES
//
//		http://fatlyz.com
//		fatlyz.pub@gmail.com
//		(C)2012 FatLYZ
//
/////////////////////////////////////

#include "_SHA512.h"


#define	ROTATE(x,n)		(((x) >> (n)) | ((x) << (64 - (n))))
#define	SHIFT(x,n)		((x) >> (n))

/*
 * Helper macros used by the SHA-512 computation.
 */
#define	CH(x,y,z)		(((x) & (y)) ^ ((~(x)) & (z)))
#define	MAJ(x,y,z)		(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define	SUM0(x)			(ROTATE((x), 28) ^ ROTATE((x), 34) ^ ROTATE((x), 39))
#define	SUM1(x)			(ROTATE((x), 14) ^ ROTATE((x), 18) ^ ROTATE((x), 41))
#define	RHO0(x)			(ROTATE((x), 1) ^ ROTATE((x), 8) ^ SHIFT((x), 7))
#define	RHO1(x)			(ROTATE((x), 19) ^ ROTATE((x), 61) ^ SHIFT((x), 6))

/*
 * Constants used in each of the SHA-512 rounds.
 */
const QWORD K[80] = {
(QWORD)(0x428a2f98d728ae22), (QWORD)(0x7137449123ef65cd),
(QWORD)(0xb5c0fbcfec4d3b2f), (QWORD)(0xe9b5dba58189dbbc),
(QWORD)(0x3956c25bf348b538), (QWORD)(0x59f111f1b605d019),
(QWORD)(0x923f82a4af194f9b), (QWORD)(0xab1c5ed5da6d8118),
(QWORD)(0xd807aa98a3030242), (QWORD)(0x12835b0145706fbe),
(QWORD)(0x243185be4ee4b28c), (QWORD)(0x550c7dc3d5ffb4e2),
(QWORD)(0x72be5d74f27b896f), (QWORD)(0x80deb1fe3b1696b1),
(QWORD)(0x9bdc06a725c71235), (QWORD)(0xc19bf174cf692694),
(QWORD)(0xe49b69c19ef14ad2), (QWORD)(0xefbe4786384f25e3),
(QWORD)(0x0fc19dc68b8cd5b5), (QWORD)(0x240ca1cc77ac9c65),
(QWORD)(0x2de92c6f592b0275), (QWORD)(0x4a7484aa6ea6e483),
(QWORD)(0x5cb0a9dcbd41fbd4), (QWORD)(0x76f988da831153b5),
(QWORD)(0x983e5152ee66dfab), (QWORD)(0xa831c66d2db43210),
(QWORD)(0xb00327c898fb213f), (QWORD)(0xbf597fc7beef0ee4),
(QWORD)(0xc6e00bf33da88fc2), (QWORD)(0xd5a79147930aa725),
(QWORD)(0x06ca6351e003826f), (QWORD)(0x142929670a0e6e70),
(QWORD)(0x27b70a8546d22ffc), (QWORD)(0x2e1b21385c26c926),
(QWORD)(0x4d2c6dfc5ac42aed), (QWORD)(0x53380d139d95b3df),
(QWORD)(0x650a73548baf63de), (QWORD)(0x766a0abb3c77b2a8),
(QWORD)(0x81c2c92e47edaee6), (QWORD)(0x92722c851482353b),
(QWORD)(0xa2bfe8a14cf10364), (QWORD)(0xa81a664bbc423001),
(QWORD)(0xc24b8b70d0f89791), (QWORD)(0xc76c51a30654be30),
(QWORD)(0xd192e819d6ef5218), (QWORD)(0xd69906245565a910),
(QWORD)(0xf40e35855771202a), (QWORD)(0x106aa07032bbd1b8),
(QWORD)(0x19a4c116b8d2d0c8), (QWORD)(0x1e376c085141ab53),
(QWORD)(0x2748774cdf8eeb99), (QWORD)(0x34b0bcb5e19b48a8),
(QWORD)(0x391c0cb3c5c95a63), (QWORD)(0x4ed8aa4ae3418acb),
(QWORD)(0x5b9cca4f7763e373), (QWORD)(0x682e6ff3d6b2b8a3),
(QWORD)(0x748f82ee5defb2fc), (QWORD)(0x78a5636f43172f60),
(QWORD)(0x84c87814a1f0ab72), (QWORD)(0x8cc702081a6439ec),
(QWORD)(0x90befffa23631e28), (QWORD)(0xa4506cebde82bde9),
(QWORD)(0xbef9a3f7b2c67915), (QWORD)(0xc67178f2e372532b),
(QWORD)(0xca273eceea26619c), (QWORD)(0xd186b8c721c0c207),
(QWORD)(0xeada7dd6cde0eb1e), (QWORD)(0xf57d4f7fee6ed178),
(QWORD)(0x06f067aa72176fba), (QWORD)(0x0a637dc5a2c898a6),
(QWORD)(0x113f9804bef90dae), (QWORD)(0x1b710b35131c471b),
(QWORD)(0x28db77f523047d84), (QWORD)(0x32caab7b40c72493),
(QWORD)(0x3c9ebe0a15c9bebc), (QWORD)(0x431d67c49c100d4c),
(QWORD)(0x4cc5d4becb3e42b6), (QWORD)(0x597f299cfc657e2a),
(QWORD)(0x5fcb6fab3ad6faec), (QWORD)(0x6c44198c4a475817)
};

void SHA512_Init(SHA512_DATA *sha)
{
	sha->inputLen = 0;
	sha->A = (QWORD)(0x6a09e667f3bcc908);
	sha->B = (QWORD)(0xbb67ae8584caa73b);
	sha->C = (QWORD)(0x3c6ef372fe94f82b);
	sha->D = (QWORD)(0xa54ff53a5f1d36f1);
	sha->E = (QWORD)(0x510e527fade682d1);
	sha->F = (QWORD)(0x9b05688c2b3e6c1f);
	sha->G = (QWORD)(0x1f83d9abfb41bd6b);
	sha->H = (QWORD)(0x5be0cd19137e2179);
	sha->totalLen = 0;
}

void ProcessBlock(SHA512_DATA *sha, const BYTE *block)
{
	QWORD W[80];
	QWORD a, b, c, d, e, f, g, h;
	QWORD temp, temp2;
	int t;

	/* Unpack the block into 80 64-bit words */
	for(t = 0; t < 16; ++t)
	{
		W[t] = (((QWORD)(block[t * 8 + 0])) << 56) |
		       (((QWORD)(block[t * 8 + 1])) << 48) |
		       (((QWORD)(block[t * 8 + 2])) << 40) |
		       (((QWORD)(block[t * 8 + 3])) << 32) |
		       (((QWORD)(block[t * 8 + 4])) << 24) |
		       (((QWORD)(block[t * 8 + 5])) << 16) |
		       (((QWORD)(block[t * 8 + 6])) <<  8) |
		        ((QWORD)(block[t * 8 + 7]));
	}
	for(t = 16; t < 80; ++t)
	{
		W[t] = RHO1(W[t - 2]) + W[t - 7] +
			   RHO0(W[t - 15]) + W[t - 16];
	}

	/* Load the SHA-512 state into local variables */
	a = sha->A;
	b = sha->B;
	c = sha->C;
	d = sha->D;
	e = sha->E;
	f = sha->F;
	g = sha->G;
	h = sha->H;

	/* Perform 80 rounds of hash computations */
	for(t = 0; t < 80; ++t)
	{
		temp = h + SUM1(e) + CH(e, f, g) + K[t] + W[t];
		temp2 = SUM0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + temp;
		d = c;
		c = b;
		b = a;
		a = temp + temp2;
	}

	/* Combine the previous SHA-512 state with the new state */
	sha->A += a;
	sha->B += b;
	sha->C += c;
	sha->D += d;
	sha->E += e;
	sha->F += f;
	sha->G += g;
	sha->H += h;
}

void SHA512_Data(SHA512_DATA *sha, const void *buffer, DWORD len)
{
	DWORD templen;

	/* Add to the total length of the input stream */
	sha->totalLen += (QWORD)len;

	/* Copy the blocks into the input buffer and process them */
	while(len > 0)
	{
		if(!(sha->inputLen) && len >= 128)
		{
			/* Short cut: no point copying the data twice */
			ProcessBlock(sha, (const BYTE *)buffer);
			buffer = (const void *)(((const BYTE *)buffer) + 128);
			len -= 128;
		}
		else
		{
			templen = len;
			if(templen > (128 - sha->inputLen))
			{
				templen = 128 - sha->inputLen;
			}
			memcpy(sha->input + sha->inputLen, buffer, templen);
			if((sha->inputLen += templen) >= 128)
			{
				ProcessBlock(sha, sha->input);
				sha->inputLen = 0;
			}
			buffer = (const void *)(((const BYTE *)buffer) + templen);
			len -= templen;
		}
	}
}

void WriteLong(BYTE *buf, QWORD value)
{
	buf[0] = (BYTE)(value >> 56);
	buf[1] = (BYTE)(value >> 48);
	buf[2] = (BYTE)(value >> 40);
	buf[3] = (BYTE)(value >> 32);
	buf[4] = (BYTE)(value >> 24);
	buf[5] = (BYTE)(value >> 16);
	buf[6] = (BYTE)(value >> 8);
	buf[7] = (BYTE)value;
}

void SHA512_Finalize(SHA512_DATA *sha,BYTE *hash)
{
	QWORD totalBits;

	/* Compute the final hash if necessary */
	if(hash)
	{
		/* Pad the input data to a multiple of 1024 bits */
		if(sha->inputLen >= (128 - 16))
		{
			/* Need two blocks worth of padding */
			sha->input[(sha->inputLen)++] = (BYTE)0x80;
			while(sha->inputLen < 128)
			{
				sha->input[(sha->inputLen)++] = (BYTE)0x00;
			}
			ProcessBlock(sha, sha->input);
			sha->inputLen = 0;
		}
		else
		{
			/* Need one block worth of padding */
			sha->input[(sha->inputLen)++] = (BYTE)0x80;
		}
		while(sha->inputLen < (128 - 16))
		{
			sha->input[(sha->inputLen)++] = (BYTE)0x00;
		}
		totalBits = (sha->totalLen << 3);
		memset(sha->input + (128 - 16), 0, 8);
		WriteLong(sha->input + (128 - 8), totalBits);
		ProcessBlock(sha, sha->input);

		/* Write the final hash value to the supplied buffer */
		WriteLong(hash,      sha->A);
		WriteLong(hash + 8,  sha->B);
		WriteLong(hash + 16, sha->C);
		WriteLong(hash + 24, sha->D);
		WriteLong(hash + 32, sha->E);
		WriteLong(hash + 40, sha->F);
		WriteLong(hash + 48, sha->G);
		WriteLong(hash + 56, sha->H);
	}
}
