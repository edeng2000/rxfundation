#ifndef ___RX_TYPE_H____
#define ___RX_TYPE_H____
/**
* @file sdtype.h
* @author wangkui
* @brief SGDP�������ͼ���
*
**/
#include "rxconfig.h"
#include <string>
#include <wchar.h>
#ifdef _M_X64
#ifndef WIN64
#define WIN64
#endif
#endif
#ifndef _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#endif

#if defined(__linux__)
#define LINUX32
#if defined(__x86_64__)
#undef  LINUX32
#define LINUX64
#endif
#endif //__linux__


#if (defined(WIN32) || defined(WIN64))
#pragma warning(disable:4996) // suppress VS 2005 deprecated function warnings
#pragma warning(disable:4786) // for string

#include <Windows.h>
#include <BaseTsd.h>
#include <tchar.h>


#define SDAPI __stdcall      

typedef HANDLE              SDHANDLE;
#define SDINVALID_HANDLE    NULL
#define SDCloseHandle(x)    CloseHandle(x)
typedef unsigned __int64 QWORD;

typedef  unsigned char      BYTE;
typedef  unsigned char      uint8_t;

#ifndef FALSE
#define FALSE			    false
#endif

#ifndef TRUE
#define TRUE                true
#endif

typedef wchar_t             WCHAR;

typedef char                CHAR;

typedef unsigned char       UCHAR;

typedef signed char         INT8;

typedef unsigned char       UINT8;

typedef signed short        INT16;
typedef signed short        int16;
typedef unsigned short      UINT16;
typedef unsigned short      uint16;
typedef signed int          INT32;
typedef signed int          int32;

typedef  INT32              BOOL;

typedef long				LONG;
typedef unsigned long       ULONG;		

typedef signed long long    INT64;

typedef signed long long    int64;

typedef unsigned long long  UINT64;
typedef unsigned long long  uint64;

typedef float               FLOAT;

#ifndef VOID
typedef void                VOID;
#endif

#ifdef UNICODE
typedef WCHAR               TCHAR;
#else
typedef CHAR                TCHAR;
#endif
#endif //endif __linux__


#define SD_OK     0
#define SD_ERR    0xFFFFFFFF

#define SDINFINITE  0xFFFFFFFF
#ifndef NULL
typedef 0                   NULL 
#endif

typedef double              DOUBLE;


#if defined(__APPLE__) && defined(__MACH__)

typedef  unsigned char      BYTE;
typedef  unsigned char      uint8_t;

#ifndef FALSE
#define FALSE			    false
#endif

#ifndef TRUE
#define TRUE                true
#endif

typedef wchar_t             WCHAR;

typedef char                CHAR;

typedef unsigned char       UCHAR;

typedef signed char         INT8;

typedef unsigned char       UINT8;

typedef signed short        INT16;
typedef signed short        int16;
typedef unsigned short      UINT16;
typedef unsigned short      uint16;
typedef signed int          INT32;
typedef signed int          int32;
typedef unsigned int          UINT32;
typedef unsigned int          uint32;
typedef  INT32              BOOL;
#ifndef SDHANDLE
typedef unsigned int         SDHANDLE;
#endif
typedef long				LONG;
typedef unsigned long       ULONG;

typedef signed long long    INT64;

typedef signed long long    int64;

typedef unsigned long long  UINT64;
typedef unsigned long long  uint64;

typedef float               FLOAT;

#ifndef VOID
typedef void                VOID;
#endif

#ifdef UNICODE
typedef WCHAR               TCHAR;
#else
typedef CHAR                TCHAR;
#endif

#endif

#endif





