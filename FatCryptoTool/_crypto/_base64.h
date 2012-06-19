
#ifndef _BASE64_H_
#define _BASE64_H_


/*
** Translation Table as described in RFC1113
*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** Translation Table to decode (created by author)
*/
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
** _b64EncodeBlock
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
extern void _b64EncodeBlock( unsigned char in[3], unsigned char out[4], int len );


/*
** _b64DecodeBlock
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
extern void _b64DecodeBlock( unsigned char in[4], unsigned char out[3] );



#endif