//***************************************************************
//***************************************************************
//*  fatfilemd5.cpp -- Generates MD5 Hash of a File or String  **
//*  2012.01 fatlyz.                                           **
//***************************************************************
//***************************************************************

#include "_fatfilemd5.h"

////////////////////////
//   Implementation   //
////////////////////////


//MD5 Transformation Function
void _md5transform(_uint32 *_state, _uint32 * _block);

//_uint32 genti(int i);						//MD5 Constant Generator
_uint32 _fathtonl(_uint32 l_input);			//htonl()
_uint32 rShiftLeft(_uint32 iData, int n);	//Rotate Left
_uint32 F(_uint32 X, _uint32 Y, _uint32 Z);
_uint32 G(_uint32 X, _uint32 Y, _uint32 Z);
_uint32 H(_uint32 X, _uint32 Y, _uint32 Z);
_uint32 I(_uint32 X, _uint32 Y, _uint32 Z);
void FF(_uint32 * a, _uint32 b, _uint32 c, _uint32 d, _uint32 M,int s, _uint32 t);
void GG(_uint32 * a, _uint32 b, _uint32 c, _uint32 d, _uint32 M,int s, _uint32 t);
void HH(_uint32 * a, _uint32 b, _uint32 c, _uint32 d, _uint32 M,int s, _uint32 t);
void II(_uint32 * a, _uint32 b, _uint32 c, _uint32 d, _uint32 M,int s, _uint32 t);


///////
BOOL FileMD5(TCHAR * _filepath, unsigned long *_hash, void(* _callback)(unsigned int _percent, void *_param), void *_callbackParam)
{
	_uint32 M[16];				//block data
	_uint64 _msglen64;			//data length (bytes)
	_uint64 _msglen64_bits;		//data length in bits
	_uint64 _msglen64_filled;	//data length filled (bytes)
	_uint64 _bigblocks;			//big block(1MBytes per block)
	_uint32 _num_blocks;		//small block(64Bytes per block)
	_uint32 _state[4];			//MD5 state

	//Open File
	FILE* _md5file;
	_tfopen_s(&_md5file, _filepath, _T("rb"));
	if (_md5file == NULL) {return 0;}

	//char m0[]-----------Data Bytes
	unsigned char *m0 = new unsigned char[1048576];

	//Get file length
	_fseeki64(_md5file, 0, SEEK_END);
	_msglen64 = _ftelli64(_md5file);
	_fseeki64(_md5file, 0, SEEK_SET);	//Position back 2 zero

	_msglen64_bits = _msglen64 * 8;							//get length in bits
	_msglen64_filled = 56 - (_msglen64 % 64) + _msglen64;	//length after filled
	_bigblocks = (_msglen64 / 1048576) + 1;			//get number of big blocks
	
	//initialize the "link variable"
	_state[0] = 0x67452301; _state[1] = 0xefcdab89; _state[2] = 0x98badcfe; _state[3] = 0x10325476; 
    
	for(_uint64 k = 0; k < _bigblocks; k++)
	{
		if (_callback != NULL){
			_callback((unsigned int)(100 * ((k + 1) * 1.00 / _bigblocks)), _callbackParam);
		}

		//Last Big Block
		if(k == _bigblocks - 1){ 
			//read data
			memset(m0, 0, 1048576);
			fread(m0, (size_t)(_msglen64 % 1048576), 1, _md5file);

			//add 1
			m0[_msglen64 % 1048576] = 0x80;

			//set message length
			for (_uint32 i = 0; i < 8; i++){
				m0[(_msglen64_filled % 1048576) + i] = (unsigned char) (_msglen64_bits >> (8 * i));
			}
			_num_blocks = (_uint32)(((_msglen64_filled % 1048576) / 64) + 1);

		}else{
			//read data
			memset(m0, 0, 1048576);

			fread(m0, 1048576, 1, _md5file);
			_num_blocks = 16384; // Small blocks = (1048576 / 64)
		}

		//operations
		for (_uint32 i = 0; i < _num_blocks; i++)
		{
			for (_uint32 j = 0; j < 16; j++)
			{	//Fill blocks(512 Bytes)
				M[j] = 0;
				M[j] += m0[(64 * i) + (4 * j) + 3];
				M[j] = M[j] << 8;
				
				M[j] += m0[(64 * i) + (4 * j) + 2];
				M[j] = M[j] << 8;
			
				M[j] += m0[(64 * i) + (4 * j) + 1];
				M[j] = M[j] << 8;

				M[j] += m0[(64 * i) + (4 * j)];
			}
			_md5transform(_state, M);

		}//i for

	}//Big for

	_hash[0] = _fathtonl(_state[0]); _hash[1] = _fathtonl(_state[1]); _hash[2] = _fathtonl(_state[2]); _hash[3] = _fathtonl(_state[3]);

	//Close File
	fclose(_md5file);
	free(m0);

	return 1;

}

///////
void StringMD5(char * _lpszStr, unsigned long *_hash)
{
	//char m0[]-----------Data Bytes
	unsigned char *m0 = new unsigned char[4096];

	_uint32 M[16];				//block data
	_uint64 _msglen64;			//data length (bytes)
	_uint64 _msglen64_bits;		//data length in bits
	_uint64 _msglen64_filled;	//data length filled (bytes)
	_uint32 _num_blocks;		//number of blocks(64Bytes per block)
	_uint32 _state[4];			//MD5 state

	//fill up
	memset(m0, 0, 4096);									//set zero
	_msglen64 = (_uint32)strlen(_lpszStr);					//get length
	_msglen64_bits = _msglen64 * 8;							//get length in bits
	_msglen64_filled = 56 - (_msglen64 % 64) + _msglen64;	//length after filled
	_num_blocks = (_uint32)(_msglen64_filled / 64) + 1;		//set number of blocks

	//set message length
	for (_uint32 i = 0; i < 8; i++){
		m0[_msglen64_filled + i] = (unsigned char) (_msglen64_bits >> (8 * i));
	}

	//input the message.
	for (_uint32 i = 0; i < _msglen64; i++){	m0[i] = _lpszStr[i];	}
	m0[_msglen64] = 0x80;

	//initialize the "link variable"
	_state[0] = 0x67452301; _state[1] = 0xefcdab89; _state[2] = 0x98badcfe; _state[3] = 0x10325476; 
    
	//operations
	for (_uint32 k = 0; k < _num_blocks; k++)
	{
		for (_uint32 j = 0; j < 16; j++)
		{	//Fill blocks(512 Bytes)
			M[j] = 0;
			M[j] += m0[(64 * k) + (4 * j) + 3];
			M[j] = M[j] << 8;
			
			M[j] += m0[(64 * k) + (4 * j) + 2];
			M[j] = M[j] << 8;
		
			M[j] += m0[(64 * k) + (4 * j) + 1];
			M[j] = M[j] << 8;

			M[j] += m0[(64 * k) + (4 * j)];
		}
		_md5transform(_state, M);

	}//Big for

	_hash[0] = _fathtonl(_state[0]); _hash[1] = _fathtonl(_state[1]); _hash[2] = _fathtonl(_state[2]); _hash[3] = _fathtonl(_state[3]);
	
	free(m0);
	return;
}


///////
void _md5transform(_uint32 *_state, _uint32 * _block)
{
	//definitions
	_uint32 M[16];
	_uint32 a = 0, b = 0, c = 0, d = 0;
	
	//copy block
	memcpy(M, _block, 64);

	a = _state[0]; b = _state[1]; c = _state[2]; d = _state[3];
	
	FF(&a,b,c,d,M[ 0], 7,0xD76AA478);         //Round 1.
	FF(&d,a,b,c,M[ 1],12,0xE8C7B756);
	FF(&c,d,a,b,M[ 2],17,0x242070DB);
	FF(&b,c,d,a,M[ 3],22,0xC1BDCEEE);

	FF(&a,b,c,d,M[ 4], 7,0xF57C0FAF);
	FF(&d,a,b,c,M[ 5],12,0x4787C62A);
	FF(&c,d,a,b,M[ 6],17,0xA8304613);
	FF(&b,c,d,a,M[ 7],22,0xFD469501);

	FF(&a,b,c,d,M[ 8], 7,0x698098D8);
	FF(&d,a,b,c,M[ 9],12,0x8B44F7AF);
	FF(&c,d,a,b,M[10],17,0xFFFF5BB1);
	FF(&b,c,d,a,M[11],22,0x895CD7BE);

	FF(&a,b,c,d,M[12], 7,0x6B901122);
	FF(&d,a,b,c,M[13],12,0xFD987193);
	FF(&c,d,a,b,M[14],17,0xA679438E);
	FF(&b,c,d,a,M[15],22,0x49B40821);


	GG(&a,b,c,d,M[ 1], 5,0xF61E2562);         //Round 2.
	GG(&d,a,b,c,M[ 6], 9,0xC040B340);
	GG(&c,d,a,b,M[11],14,0x265E5A51);
	GG(&b,c,d,a,M[ 0],20,0xE9B6C7AA);

	GG(&a,b,c,d,M[ 5], 5,0xD62F105D);
	GG(&d,a,b,c,M[10], 9,0x02441453);
	GG(&c,d,a,b,M[15],14,0xD8A1E681);
	GG(&b,c,d,a,M[ 4],20,0xE7D3FBC8);

	GG(&a,b,c,d,M[ 9], 5,0x21E1CDE6);
	GG(&d,a,b,c,M[14], 9,0xC33707D6);
	GG(&c,d,a,b,M[ 3],14,0xF4D50D87);
	GG(&b,c,d,a,M[ 8],20,0x455A14ED);

	GG(&a,b,c,d,M[13], 5,0xA9E3E905);
	GG(&d,a,b,c,M[ 2], 9,0xFCEFA3F8);
	GG(&c,d,a,b,M[ 7],14,0x676F02D9);
	GG(&b,c,d,a,M[12],20,0x8D2A4C8A);


	HH(&a,b,c,d,M[ 5], 4,0xFFFA3942);         //Round 3.
	HH(&d,a,b,c,M[ 8],11,0x8771F681);
	HH(&c,d,a,b,M[11],16,0x6D9D6122);
	HH(&b,c,d,a,M[14],23,0xFDE5380C);

	HH(&a,b,c,d,M[ 1], 4,0xA4BEEA44);
	HH(&d,a,b,c,M[ 4],11,0x4BDECFA9);
	HH(&c,d,a,b,M[ 7],16,0xF6BB4B60);
	HH(&b,c,d,a,M[10],23,0xBEBFBC70);

	HH(&a,b,c,d,M[13], 4,0x289B7EC6);
	HH(&d,a,b,c,M[ 0],11,0xEAA127FA);
	HH(&c,d,a,b,M[ 3],16,0xD4EF3085);
	HH(&b,c,d,a,M[ 6],23,0x04881D05);

	HH(&a,b,c,d,M[ 9], 4,0xD9D4D039);
	HH(&d,a,b,c,M[12],11,0xE6DB99E5);
	HH(&c,d,a,b,M[15],16,0x1FA27CF8);
	HH(&b,c,d,a,M[ 2],23,0xC4AC5665);


	II(&a,b,c,d,M[ 0], 6,0xF4292244);         //Round 4.
	II(&d,a,b,c,M[ 7],10,0x432AFF97);
	II(&c,d,a,b,M[14],15,0xAB9423A7);
	II(&b,c,d,a,M[ 5],21,0xFC93A039);

	II(&a,b,c,d,M[12], 6,0x655B59C3);
	II(&d,a,b,c,M[ 3],10,0x8F0CCC92);
	II(&c,d,a,b,M[10],15,0xFFEFF47D);
	II(&b,c,d,a,M[ 1],21,0x85845DD1);

	II(&a,b,c,d,M[ 8], 6,0x6FA87E4F);
	II(&d,a,b,c,M[15],10,0xFE2CE6E0);
	II(&c,d,a,b,M[ 6],15,0xA3014314);
	II(&b,c,d,a,M[13],21,0x4E0811A1);

	II(&a,b,c,d,M[ 4], 6,0xF7537E82);
	II(&d,a,b,c,M[11],10,0xBD3AF235);
	II(&c,d,a,b,M[ 2],15,0x2AD7D2BB);
	II(&b,c,d,a,M[ 9],21,0xEB86D391);

	_state[0] += a; _state[1] += b; _state[2] += c; _state[3] += d;

}
/*
_uint32 genti(int i){
	return (_uint32)(pow((double)2, (double)32) * abs(sin((double)i)));
}
*/
_uint32 _fathtonl(_uint32 l_input){
	return ((l_input & 0xff000000) >> 24) | ((l_input & 0x00ff0000) >> 8) |	((l_input & 0x0000ff00) << 8) | ((l_input & 0x000000ff) <<24);
}


_uint32 rShiftLeft(_uint32 lData,int n){
	/*
	_uint32 tmp=0,rSL=0;
	tmp=iData;
	tmp=tmp>>(32-(n));
	rSL=iData<<n;
	rSL=rSL+tmp;
	//printf("iDta :%lx ,n:%d\n",iData,n);
	//printf("rSL :%lx ,tmp:%lx\n",rSL,tmp);
	//rSL= (iData) << (n) | (iData) >> (32-(n));
	//printf("rSL1:%lx\n",rSL);	
	*/
	return (lData) << (n) | (lData) >> (32 - (n));
}


//4  nonlinearity operations.
	_uint32 F(_uint32 X,_uint32 Y,_uint32 Z){
		return (X & Y) | ( (~ X) & Z);	
	}

	_uint32 G(_uint32 X,_uint32 Y,_uint32 Z){
		return (X & Z)|(Y & ( ~Z ));
	}

	_uint32 H(_uint32 X,_uint32 Y,_uint32 Z){
		return X ^ Y ^ Z;	
	}
	_uint32 I(_uint32 X,_uint32 Y,_uint32 Z){
		return Y ^ (X | ( ~Z ));	
	}

	void FF(_uint32 * a,_uint32 b,_uint32 c,_uint32 d,_uint32 M,int s,_uint32 t){	
		*a = b + rShiftLeft((*a + F(b, c, d) + M + t), s);
		return;
	}

	void GG(_uint32 * a,_uint32 b,_uint32 c,_uint32 d,_uint32 M,int s,_uint32 t){	
		*a = b + rShiftLeft((*a + G(b, c, d) + M + t), s);	
		return;
	}

	void HH(_uint32 * a,_uint32 b,_uint32 c,_uint32 d,_uint32 M,int s,_uint32 t){	
		*a = b + rShiftLeft((*a + H(b, c, d) + M + t), s);	
		return;
	}

	void II(_uint32 * a,_uint32 b,_uint32 c,_uint32 d,_uint32 M,int s,_uint32 t){	
		*a = b + rShiftLeft((*a + I(b, c, d) + M + t), s);	
		return;
	}

	/*
	MD5 test suite:
	MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
	MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
	MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
	MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
	MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
	MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
	d174ab98d277d9f5a5611c2c9f419d9f
	MD5 ("123456789012345678901234567890123456789012345678901234567890123456
	78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
	*/
