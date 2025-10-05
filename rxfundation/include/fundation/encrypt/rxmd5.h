#ifndef ___RX_FUNDATION_MD5_H___
#define ___RX_FUNDATION_MD5_H___

/* Parameters of MD5. */
#define s11 7
#define s12 12
#define s13 17
#define s14 22

#define s21 5
#define s22 9
#define s23 14
#define s24 20

#define s31 4
#define s32 11
#define s33 16
#define s34 23

#define s41 6
#define s42 10
#define s43 15
#define s44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
  (a) += F ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
  (a) += G ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
  (a) += H ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
  (a) += I ((b), (c), (d)) + (x) + ac; \
  (a) = ROTATELEFT ((a), (s)); \
  (a) += (b); \
}

#include <string>
#include <cstring>

using std::string;

/* Define of btye.*/
typedef unsigned char byte;
/* Define of byte. */
typedef unsigned int bit32;
#include <string>  
#include <fstream>  

/* Type define */
typedef unsigned char byte_t;
typedef unsigned long ulong;

using std::string;
using std::ifstream;

class CRXMD5 {
public:
    /* Construct a MD5 object with a string. */
    CRXMD5(const string& message);

    /* Generate md5 digest. */
    const unsigned char* getDigest();

    /* Convert digest to string value */
    string toStr();

private:
    /* Initialization the md5 object, processing another message block,
     * and updating the context.*/
    void init(const unsigned char* input, size_t len);

    /* MD5 basic transformation. Transforms state based on block. */
    void transform(const unsigned char block[64]);

    /* Encodes input (usigned long) into output (byte). */
    void encode(const bit32* input, unsigned char* output, size_t length);

    /* Decodes input (byte) into output (usigned long). */
    void decode(const unsigned char* input, bit32* output, size_t length);

private:
    /* Flag for mark whether calculate finished. */
    bool finished;

    /* state (ABCD). */
    bit32 state[4];

    /* number of bits, low-order word first. */
    bit32 count[2];

    /* input buffer. */
    unsigned char buffer[64];

    /* message digest. */
    unsigned char digest[16];

    /* padding for calculate. */
    static const unsigned char PADDING[64];

    /* Hex numbers. */
    static const char HEX_NUMBERS[16];
};

class CRXMD5Ex {
public:
    CRXMD5Ex();
    CRXMD5Ex(const void* input, size_t length);
    CRXMD5Ex(const string& str);
    CRXMD5Ex(ifstream& in);
	void update(const void* input, size_t length);
	void update(const string& str);
	void update(ifstream& in);
	const byte_t* digest();
	string toString();
	void reset();
private:
	void update(const byte_t* input, size_t length);
	void final();
	void transform(const byte_t block[64]);
	void encode(const ulong* input, byte_t* output, size_t length);
	void decode(const byte_t* input, ulong* output, size_t length);
	string bytesToHexString(const byte_t* input, size_t length);

	/* class uncopyable */
    CRXMD5Ex(const CRXMD5Ex&);
    CRXMD5Ex& operator=(const CRXMD5Ex&);
private:
	ulong _state[4];    /* state (ABCD) */
	ulong _count[2];    /* number of bits, modulo 2^64 (low-order word first) */
	byte_t _buffer[64];   /* input buffer */
	byte_t _digest[16];   /* message digest */
	bool _finished;     /* calculate finished ? */

	static const byte_t PADDING[64];  /* padding for calculate */
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024;
};


string RXCalculateMD5File(const char* pszFilePath);

#endif //CODEMD5_MD5_H

