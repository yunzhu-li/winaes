
#include "_Keccak512.h"

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;

#define cKeccakR_SizeInBytes    (cKeccakR / 8)

#ifndef crypto_hash_BYTES
    #ifdef cKeccakFixedOutputLengthInBytes
        #define crypto_hash_BYTES cKeccakFixedOutputLengthInBytes
    #else
        #define crypto_hash_BYTES cKeccakR_SizeInBytes
    #endif
#endif
#if (crypto_hash_BYTES > cKeccakR_SizeInBytes)
    #error "Full squeezing not yet implemented"
#endif

#define IS_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */
#define IS_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */
#define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN /* WARNING: This implementation works on little-endian platform. Support for big-endinanness is implemented, but not tested. */

#if     (cKeccakB   == 1600)
    typedef QWORD  UINT64;
    typedef UINT64 tKeccakLane;
    #define cKeccakNumberOfRounds   24
#elif   (cKeccakB   == 800)
    typedef DWORD        UINT32;
    // WARNING: on 8-bit and 16-bit platforms, this should be replaced by:
    //typedef unsigned long       UINT32;
    typedef UINT32 tKeccakLane;
    #define cKeccakNumberOfRounds   22
#elif   (cKeccakB   == 400)
    typedef WORD      UINT16;
    typedef UINT16 tKeccakLane;
    #define cKeccakNumberOfRounds   20
#elif   (cKeccakB   == 200)
    typedef BYTE       UINT8;
    typedef UINT8 tKeccakLane;
    #define cKeccakNumberOfRounds   18
#else
    #error  "Unsupported Keccak-f width"
#endif

#define cKeccakLaneSizeInBits   (sizeof(tKeccakLane) * 8)

#define ROL(a, offset) (tKeccakLane)((((tKeccakLane)a) << ((offset) % cKeccakLaneSizeInBits)) ^ (((tKeccakLane)a) >> (cKeccakLaneSizeInBits-((offset) % cKeccakLaneSizeInBits))))

void KeccakF( tKeccakLane * state, const tKeccakLane *in, int laneCount );

int crypto_hash(BYTE *out, const BYTE *in, QWORD inlen )
{
    tKeccakLane		state[5 * 5];
	#if (crypto_hash_BYTES >= cKeccakR_SizeInBytes)
    #define temp out
	#else
    BYTE 	temp[cKeccakR_SizeInBytes];
	#endif

    memset( state, 0, sizeof(state) );

    for ( /* empty */; inlen >= cKeccakR_SizeInBytes; inlen -= cKeccakR_SizeInBytes, in += cKeccakR_SizeInBytes )
    {
        KeccakF( state, (const tKeccakLane*)in, cKeccakR_SizeInBytes / sizeof(tKeccakLane) );
    }

    /*    Last data and padding	*/
    memcpy( temp, in, (size_t)inlen );
    temp[inlen++] = 1;
    memset( temp+inlen, 0, cKeccakR_SizeInBytes - (size_t)inlen );
    temp[cKeccakR_SizeInBytes-1] |= 0x80;
    KeccakF( state, (const tKeccakLane*)temp, cKeccakR_SizeInBytes / sizeof(tKeccakLane) );

    #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN) || (cKeccakB == 200)

    memcpy( out, state, crypto_hash_BYTES );

	#else

    for ( i = 0; i < (crypto_hash_BYTES / sizeof(tKeccakLane)); ++i )
	{
		tSmallUInt		j;
	    tKeccakLane		t;

		t = state[i];
		for ( j = 0; j < sizeof(tKeccakLane); ++j )
		{
			*(out++) = (BYTE)t;
			t >>= 8;
		}
	}

	#endif
	#if (crypto_hash_BYTES >= cKeccakR_SizeInBytes)
    #undef temp
	#endif

    return ( 0 );
}

const tKeccakLane KeccakF_RoundConstants[cKeccakNumberOfRounds] = 
{
    (tKeccakLane)0x0000000000000001,
    (tKeccakLane)0x0000000000008082,
    (tKeccakLane)0x800000000000808a,
    (tKeccakLane)0x8000000080008000,
    (tKeccakLane)0x000000000000808b,
    (tKeccakLane)0x0000000080000001,
    (tKeccakLane)0x8000000080008081,
    (tKeccakLane)0x8000000000008009,
    (tKeccakLane)0x000000000000008a,
    (tKeccakLane)0x0000000000000088,
    (tKeccakLane)0x0000000080008009,
    (tKeccakLane)0x000000008000000a,
    (tKeccakLane)0x000000008000808b,
    (tKeccakLane)0x800000000000008b,
    (tKeccakLane)0x8000000000008089,
    (tKeccakLane)0x8000000000008003,
    (tKeccakLane)0x8000000000008002,
    (tKeccakLane)0x8000000000000080
	#if		(cKeccakB	>= 400)
  , (tKeccakLane)0x000000000000800a,
    (tKeccakLane)0x800000008000000a
	#if		(cKeccakB	>= 800)
  , (tKeccakLane)0x8000000080008081,
    (tKeccakLane)0x8000000000008080
	#if		(cKeccakB	== 1600)
  , (tKeccakLane)0x0000000080000001,
    (tKeccakLane)0x8000000080008008
	#endif
	#endif
	#endif
};

const WORD KeccakF_RotationConstants[25] = 
{
	 1,  3,  6, 10, 15, 21, 28, 36, 45, 55,  2, 14, 27, 41, 56,  8, 25, 43, 62, 18, 39, 61, 20, 44
};

const WORD KeccakF_PiLane[25] = 
{
    10,  7, 11, 17, 18,  3,  5, 16,  8, 21, 24,  4, 15, 23, 19, 13, 12,  2, 20, 14, 22,  9,  6,  1 
};

const WORD KeccakF_Mod5[10] = 
{
    0, 1, 2, 3, 4, 0, 1, 2, 3, 4
};

void KeccakF( tKeccakLane * state, const tKeccakLane *in, int laneCount )
{
	WORD x, y;
    tKeccakLane temp;
    tKeccakLane BC[5];

	#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN) || (cKeccakB == 200)
    while ( --laneCount >= 0 )
	{
        state[laneCount] ^= in[laneCount];
	}
	#else
	temp = 0; /* please compiler */
    while ( --laneCount >= 0 )
	{
		for ( x = 0; x < sizeof(tKeccakLane); ++x )
		{
			temp <<= 8;
			temp |= ((char*)&in[laneCount])[x];
		}
        state[laneCount] = temp;
	}
	#endif

	#define	round	laneCount
    for( round = 0; round < cKeccakNumberOfRounds; ++round )
    {
		// Theta
		for ( x = 0; x < 5; ++x )
		{
			BC[x] = state[x] ^ state[5 + x] ^ state[10 + x] ^ state[15 + x] ^ state[20 + x];
		}
		for ( x = 0; x < 5; ++x )
		{
			temp = BC[KeccakF_Mod5[x+4]] ^ ROL(BC[KeccakF_Mod5[x+1]], 1);
			for ( y = 0; y < 25; y += 5 )
			{
				state[y + x] ^= temp;
			}
		}

        // Rho Pi
		temp = state[1];
		for ( x = 0; x < 24; ++x )
		{
			BC[0] = state[KeccakF_PiLane[x]];
			state[KeccakF_PiLane[x]] = ROL( temp, KeccakF_RotationConstants[x] );
			temp = BC[0];
		}

		//	Chi
		for ( y = 0; y < 25; y += 5 )
		{
			BC[0] = state[y + 0];
			BC[1] = state[y + 1];
			BC[2] = state[y + 2];
			BC[3] = state[y + 3];
			BC[4] = state[y + 4];
			for ( x = 0; x < 5; ++x )
			{
				state[y + x] = BC[x] ^((~BC[KeccakF_Mod5[x+1]]) & BC[KeccakF_Mod5[x+2]]);
			}
		}

		//	Iota
		state[0] ^= KeccakF_RoundConstants[round];
    }
	#undef	round

}

void Keccak512_Init(KECCAK512_DATA *keccak)
{
	keccak->bitsInQueue = 0;
	memset( keccak->state, 0, sizeof(keccak->state) );
}

void Keccak512_Data(KECCAK512_DATA *keccak, const BYTE *buffer, DWORD len)
{
	if ( (keccak->bitsInQueue < 0) || ((keccak->bitsInQueue % 8) != 0) )
	{
		/*	 Final() already called or bits already in queue not modulo 8.	*/
		return;
	}

	/*	If already data in queue, continue queuing first */
	for ( /* empty */; (len >= 8) && (keccak->bitsInQueue != 0); len -= 8 )
	{
		keccak->state[keccak->bitsInQueue / 8] ^= *(buffer++);
		if ( (keccak->bitsInQueue += 8) == cKeccakR )
		{
			KeccakF( (tKeccakLane *)keccak->state, 0, 0 );
			keccak->bitsInQueue = 0;
		}
	}

	/*	Absorb complete blocks */
	for ( /* */; len >= cKeccakR; len -= cKeccakR, buffer += cKeccakR_SizeInBytes )
	{
		KeccakF( (tKeccakLane *)keccak->state, (const tKeccakLane *)buffer, cKeccakR_SizeInBytes / sizeof(tKeccakLane) );
	}

	/*	Queue remaining data bytes */
	for ( /* empty */; len >=8; len -= 8, keccak->bitsInQueue += 8 )
	{
		keccak->state[keccak->bitsInQueue / 8] ^= *(buffer++);
	}
	/*	Queue eventual remaining data bits plus add first padding bit */
	if ( len != 0 )
	{
		keccak->state[keccak->bitsInQueue / 8] ^= (*buffer >> (8 - len));
		keccak->bitsInQueue += (int)len;
	}
}

void Keccak512_Finalize(KECCAK512_DATA *keccak, BYTE *hash)
{
	WORD	i;
	int		hashbytelen=64;

	if ( keccak->bitsInQueue < 0 )
	{
		/*	 Final() already called.	*/
		return;
	}

    // Padding
    if (keccak->bitsInQueue + 1 == cKeccakR_SizeInBytes*8) {
        keccak->state[cKeccakR_SizeInBytes-1] ^= 0x80;
		KeccakF( (tKeccakLane *)keccak->state, 0, 0 );
    }
    else {
        keccak->state[keccak->bitsInQueue/8] ^= 1 << (keccak->bitsInQueue % 8);
    }
    keccak->state[cKeccakR_SizeInBytes-1] ^= 0x80;
    KeccakF( (tKeccakLane *)keccak->state, 0, 0 );

    // Output
	for ( /* empty */; hashbytelen != 0; hash += i, hashbytelen -= i )
	{
		i = (hashbytelen < cKeccakR_SizeInBytes) ? hashbytelen : cKeccakR_SizeInBytes;

		#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN) || (cKeccakB == 200)

	    memcpy( hash, keccak->state, i );

		#else

	    for ( offset = 0; offset < i; offset += sizeof(tKeccakLane) )
		{
			tSmallUInt		j;

			for ( j = 0; j < sizeof(tKeccakLane); ++j )
			{
				hash[offset + j] = keccak->state[offset + (sizeof(tKeccakLane) - 1) - j];
			}
		}

		#endif

		if ( i != hashbytelen )
		{
			KeccakF( (tKeccakLane *)keccak->state, 0, 0 );
		}
	}

	keccak->bitsInQueue = -1;	/* flag final state */
}
