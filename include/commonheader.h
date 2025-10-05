#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_
#include "type.h"
// common header

#ifndef PI
#define PI (3.14159265358979323846f) 
#endif

#include <tchar.h>
#include <windows.h>
#ifndef NOT_HASHMAP
#include <hash_map>
#endif


#define CDDIR _chdir

#ifndef snprintf
#define snprintf _snprintf
#endif

#pragma warning(disable:4996)
#pragma warning(disable:4786)
#pragma warning(disable:4800)

// please noted that here it is just _atoi64, 
// not to unsigned long long, it is long long
#define atoll _atoi64
#define strtoll _strtoi64


#ifndef NULL
#define NULL ((void*)0)
#endif

#include <cassert>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "hashhelper.h"

#include <string>
#include <list>
#include <vector>
#include <map>
using namespace std;

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef INVALID_8BIT_ID
#define INVALID_8BIT_ID   uint32_t(~0)
#endif
#ifndef INVALID_16BIT_ID
#define INVALID_16BIT_ID   uint16_t(~0)
#endif
#ifndef INVALID_32BIT_ID
#define INVALID_32BIT_ID   uint32_t(~0)
#endif
#ifndef INVALID_64BIT_ID
#define INVALID_64BIT_ID   uint64_t(~0)
#endif


#ifndef MY_INT_MAX
#define MY_INT_MAX ((uint32(~0)-2)/2)
#endif


inline string uint64tostr1(const uint64_t& llValue)
{
	char szData[64] = { 0 };
	sprintf(szData, "%lld", llValue);
	return string(szData);
}

#endif
