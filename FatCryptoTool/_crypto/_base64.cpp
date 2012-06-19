
#include "_base64.h"


/*
                Example encoding:

                The stream 'ABCD' is 32 bits long.  It is mapped as
                follows:

                ABCD

                 A (65)     B (66)     C (67)     D (68)   (None) (None)
                01000001   01000010   01000011   01000100

                16 (Q)  20 (U)  9 (J)   3 (D)    17 (R) 0 (A)  NA (=) NA (=)
                010000  010100  001001  000011   010001 000000 000000 000000


                QUJDRA==
*/

/*
** _b64EncodeBlock
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void _b64EncodeBlock( unsigned char in[3], unsigned char out[4], int len )
{
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

/*
** _b64DecodeBlock
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
void _b64DecodeBlock( unsigned char in[4], unsigned char out[3] )
{   
    out[0] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[1] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}


