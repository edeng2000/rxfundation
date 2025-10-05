#ifndef _rx_type_h_
#define _rx_type_h_
#include <string>
#include <string_view>
#if defined(WIN32) && defined(WIN64)
#include <xstring>
#endif
#include <string.h>
// unsigned char
typedef unsigned char      uint8;

// unsigned short
typedef unsigned short     uint16;

// unsigned int
typedef unsigned int       uint32;

typedef int                int32;

// unsigned long
typedef unsigned long      dword;

// unsigned long long


// long long
typedef long long          int64;
typedef unsigned long long uint64;

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

#define DUILIB_BUILD_FOR_WIN
#define DUILIB_UNICODE

//���wchar_t�Ķ��壺Linuxƽ̨�£�wchar_t��4���ֽڣ�Windowsƽ̨�£�wchar_t��2���ֽ�
#if defined(DUILIB_BUILD_FOR_WIN)
	//Windowsƽ̨
#define WCHAR_T_IS_UTF16
#elif defined(DUILIB_BUILD_FOR_LINUX) && defined(__GNUC__) && \
      defined(__WCHAR_MAX__) && \
      (__WCHAR_MAX__ == 0x7fffffff || __WCHAR_MAX__ == 0xffffffff)
#define WCHAR_T_IS_UTF32
#elif defined(DUILIB_BUILD_FOR_LINUX) && defined(__GNUC__) && \
      defined(__WCHAR_MAX__) && \
      (__WCHAR_MAX__ == 0x7fff || __WCHAR_MAX__ == 0xffff)
	// On Posix, we'll detect short wchar_t, but projects aren't guaranteed to
	// compile in this mode (in particular, Chrome doesn't). This is intended for
	// other projects using base who manage their own dependencies and make sure
	// short wchar works for them.
#define WCHAR_T_IS_UTF16
#else
#if defined (_WIN32) || defined (_WIN64)
#define WCHAR_T_IS_UTF16
#else
#error Please add support for your compiler
#endif
#endif

typedef char UTF8Char;
#if defined(WCHAR_T_IS_UTF16)
typedef wchar_t UTF16Char;
typedef int32_t UTF32Char;
#else
typedef int16_t UTF16Char;
typedef wchar_t UTF32Char;
#endif

typedef std::basic_string<UTF8Char> UTF8String;
typedef UTF8String U8String;

typedef std::basic_string_view<UTF8Char> UTF8StringView;
typedef UTF8StringView U8StringView;

typedef std::basic_string<UTF16Char> UTF16String;
typedef UTF16String U16String;

typedef std::basic_string_view<UTF16Char> UTF16StringView;
typedef UTF16StringView U16StringView;

typedef std::basic_string<UTF32Char> UTF32String;
typedef UTF32String U32String;

typedef std::basic_string_view<UTF32Char> UTF32StringView;
typedef UTF32StringView U32StringView;

#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#endif