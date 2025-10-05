
/******************************************************************************
Copyright (C) Shanda Corporation. All rights reserved.


******************************************************************************/


#ifndef _RX_STRING_H_
#define _RX_STRING_H_
/**
* @file RXstring.h
* @author lw
* @brief string处理系列
*
**/
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#if defined(WIN32) && defined(WIN64)
#include <windows.h>
#endif
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <vector>
#include <stdarg.h>
#include "rxtype.h"
#include "rxfundation.h"
#ifdef WIN32 
#include "char_conversion.h"
#include "ConvertUTF.h"
#endif
/**
* @file RXstring.h
* @author lw
* @brief some function for string process
*
**/
#if (defined(WIN32) || defined(WIN64))

#ifdef UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#else
#ifdef UNICODE
typedef std::basic_string<TCHAR> tstring;
#else
typedef std::string tstring;
#endif
#endif
using namespace std;

#if (defined(WIN32) || defined(WIN64))
#include <tchar.h>
#define RXSYMBOL_RT         "\r\n"
#define RXFMT_I64           "%I64d"
#define WCHAR_SDFMT_I64      L"%I64d"
#define RXFMT_U64           "%I64u"
#define WCHAR_SDFMT_U64     L"%I64u"
#define RXFMT_X64           "%I64x"
#else
#include <wchar.h>
#include <wctype.h>
#define RXSYMBOL_RT         "\n"
#define RXFMT_I64           "%lld"
#define WCHAR_SDFMT_I64      L"%lld"
#define RXFMT_U64           "%llu"
#define WCHAR_SDFMT_U64     L"%llu"
#define RXFMT_64X           "%llx"
#endif


    /**
    * @defgroup groupstring 字符串处理
    * @brief 字符串处理不做边界检测
    * @ingroup  SGDP
    * @{
    */

    /** @see isalnum */
#define RXIsAlNum(c) (isalnum(((UCHAR)(c))))
    /** @see isalpha */
#define RXIsAlpha(c) (isalpha(((UCHAR)(c))))
    /** @see intrl */
#define RXIsCntrl(c) (iscntrl(((UCHAR)(c))))
    /** @see isdigit */
#define RXIsDigit(c) (isdigit(((UCHAR)(c))))
    /** @see isgraph */
#define RXISGraph(c) (isgraph(((UCHAR)(c))))
    /** @see islower*/
#define RXIsLower(c) (islower(((UCHAR)(c))))
    /** @see isascii */
#ifdef isascii
#define RXIsAscii(c) (isascii(((UCHAR)(c))))
#else
#define RXIsAscii(c) (((c) & ~0x7f)==0)
#endif
    /** @see isprint */
#define RXIsPrint(c) (isprint(((UCHAR)(c))))
    /** @see ispunct */
#define RXIsPunct(c) (ispunct(((UCHAR)(c))))
    /** @see isspace */
#define RXIsSpace(c) (isspace(((UCHAR)(c))))
    /** @see isupper */
#define RXIsUpper(c) (isupper(((UCHAR)(c))))
    /** @see isxdigit */
#define RXIsXdigit(c) (isxdigit(((UCHAR)(c))))
    /** @see tolower */
#define RXToLower(c) (tolower(((UCHAR)(c))))
    /** @see toupper */
#define RXToUpper(c) (toupper(((UCHAR)(c))))

    ///////////////////////////////////////////////////////////////////////////
#define RXSprintf sprintf
#define RXSwprintf swprintf

#if (defined(WIN32) || defined(WIN64))
#define RXSnprintf _snprintf
#else
#define RXSnprintf snprintf
#endif

///////////////////////////////////////////////////////////////////////
#define RXAtoi atoi

#if (defined(WIN32) || defined(WIN64))
#define RXAtoi64 _atoi64
#else
#define RXAtoi64 atoll
#endif

#ifdef UNICODE
#define _RXT(x)               L##x
#define _RXTStrlen            RXWcslen
#define _RXTStrnlen           RXWcsnlen
#define _RXTStrcat            RXWcscat
#define _RXTStrcpy            RXWcscpy
#define _RXTStrncpy           RXWcsncpy
#define _RXTStrcmp            RXWcscmp
#define _RXTStrncmp           RXWcsncmp
#define _RXTStrchr            RXWcschr
#define _RXTStrrchr           RXWcsrchr
#define _RXTStrcasecmp        RXWcscasecmp
#define _RXTA2T(x)            RXA2W(x)
#define _RXTT2A(x)            RXW2A(x)
#define _RXTStrToLower(x)     RXStrToLowerW(x)
#define _RXTT2Local(x)        RXW2Local(x)
#define _RXTLocal2T(x)        RXLocal2W(x)
#define _RXTUTF82W(x)         RXUTF82W(x)
#define _RXTT2UTF8(x)         RXW2UTF8(x)
#define _RXTUTF82T(x)         RXUTF82W(x)
#define _RXTSource2T(x,y)     RXSource2W(x,y)
#define _RXTTtoi		      RXWtoi
#define _RXTsnprintf		  RXsnwprintf
#define _RXTvsprintf          vswprintf
#define _RXTvsnprintf          vswprintf
#define _RXTsprintf		      swprintf
#define _RXTStrchr            RXWcschr
#define _RXTStrrchr           RXWcsrchr
#define _RXTStrtok            RXWcstok
#define _RXTStrlwr            RXWcslwr
#define _RXTStrupr            RXWcsupr
#define _RXTStrcat             RXWcscat
#define _RXTStrncat             RXWcsncat
#define _RXTStrncasecmp   RXWcsncasecmp
#define _RXTBufferToHex       RXBufferToHexW
#define _RXTIntToHex          RXIntToHexW
#define _RXTStrTrimLeft       RXStrTrimLeftW
#define _RXTStrTrimRight      RXStrTrimRightW
#define _RXTStrTrim           RXStrTrimW
#define _RXTStrtod            RXWCStrtod
#define _RXTSplitStrings      RXSplitStringsW
#define _RXTAtou64            RXWtou64
#define _RXTAtou              RXWtou
#define _RXTItoa              RXItoa
#define _RXTItoa64            RXItoa64
#define _RXTUtoa              RXUtoa
#define _RXTUtoa64            RXUtoa64
#else
#define _RXT(x)               x
#define _RXTStrToLower(x)     RXStrToLowerW(x)
#define _RXTStrlen            RXStrlen
#define _RXTStrnlen           RXStrnlen
#define _RXTStrcat            RXStrcat
#define _RXTStrcpy            RXStrcpy
#define _RXTStrncpy           RXStrncpy
#define _RXTStrcmp            RXStrcmp
#define _RXTStrncmp           RXStrncmp
#define _RXTStrchr            RXStrchr
#define _RXTStrrchr           RXStrrchr
#define _RXTStrcasecmp        RXStrcasecmp
#define _RXTA2T(x)            (x)
#define _RXTT2A(x)            string(x)
#define _RXTT2Local(x)        (x)
#define _RXTLocal2T(x)        (x)
#define _RXTUTF82W(x)         (x)
#define _RXTT2UTF8(x)         (x)
#define _RXTUTF82T(x)         (x)
#define _RXTSource2T(x,y)     RXSource2Local(x,y).c_str()
#define _RXTTtoi		      RXAtoi
#ifdef __linux__
#define _RXTsnprintf		  snprintf
#else
#define _RXTsnprintf		  _snprintf
#endif
#define _RXTvsprintf          vsprintf
#define _RXTvsnprintf          vsnprintf
#define _RXTsprintf		      sprintf
#define _RXTStrchr            RXStrchr
#define _RXTStrrchr           RXStrrchr
#define _RXTStrtok            RXStrtok
#define _RXTStrlwr            RXStrlwr
#define _RXTStrupr            RXStrupr
#define _RXTStrcat             RXStrcat
#define _RXTStrncat             RXStrncat
#define _RXTStrncasecmp   RXStrncasecmp
#define _RXTBufferToHex       RXBufferToHexA
#define _RXTIntToHex          RXIntToHexA
#define _RXTStrTrimLeft       RXStrTrimLeftA
#define _RXTStrTrimRight      RXStrTrimRightA
#define _RXTStrTrim           RXStrTrimA
#define _RXTStrtod            RXStrtod
#define _RXTSplitStrings      RXSplitStringsA
#define _RXTAtou64            RXAtou64
#define _RXTAtou              RXAtou
#define _RXTItoa              RXItow
#define _RXTItoa64            RXItow64
#define _RXTUtoa              RXUtow
#define _RXTUtoa64            RXUtow64
#endif

    /**
    * @brief
    * 获取字符串的长度
    * @param string : 以0为结束符的字符串
    * @return : 字符串的长度
    */
    inline UINT32  RXStrlen( const CHAR* string )
    {
        return (UINT32)strlen(string);
    }

    /**
    * @brief
    * 获取Unicode字符串的长度
    * @param string : 以0为结束符的字符串
    * @return : Unicode字符串的长度
    */
    inline UINT32  RXWcslen( const WCHAR* string )
    {
        return (UINT32)wcslen(string);
    }

    /**
    * @brief
    * 获取字符串的长度
    * @param pszStr : 字符串
    * @param sizeInBytes :最大检测长度
    * @return 获取字符串的长度，如果字符串的长度超过sizeInBytes，返回sizeInBytes
    */
    inline UINT32  RXStrnlen(const CHAR* pszStr, UINT32 sizeInBytes )
    {
        return (UINT32)strnlen(pszStr, sizeInBytes);
    }

    /**
    * @brief
    * 获取Unicode字符串的长度
    * @param pszStr : Unicode字符串
    * @param sizeInBytes :最大检测长度
    * @return 获取Unicode字符串的长度，如果Unicode字符串的长度超过sizeInBytes，返回sizeInBytes
    */
    inline UINT32  RXWcsnlen(const WCHAR* pszStr, size_t sizeInBytes)
    {
        return (UINT32)wcsnlen(pszStr, sizeInBytes);
    }

    /**
    * @brief
    * 在目标字符串后添加一个字符串
    * @param strDestination : 以0为结束符的目标字符串
    * @param strSource : 以0为结束符的源字符串
    * @return 结果字符串，使用的是目标字符串空间，请预留足够的空间
    */
    inline CHAR*  RXStrcat(CHAR* strDestination, const CHAR* strSource )
    {
        return strcat(strDestination,strSource);
    }

    /**
    * @brief
    * 在目标字符串后添加一个字符串
    * @param strDestination : 以0为结束符的目标字符串
    * @param dstLen : 目标串内存的总长度,如果合并后的长度大于此长度,不会进行合并,返回空串,不修改目标串
    * @param strSource : 以0为结束符的源字符串
    * @return 结果字符串，使用的是目标字符串空间,如果合并后的长度大于此长度,不会进行合并,返回NULL,不修改目标串
    */
    inline CHAR *  RXStrSafeCat(CHAR* strDestination, UINT32 dstLen, const CHAR* strSource)
    {
        if (dstLen < strlen(strDestination) + strlen(strSource) + 1)
        {
            return NULL;
        }
        return strcat(strDestination,strSource);
    }

    /**
    * @brief
    * 在目标Unicode字符串后添加一个Unicode字符串
    * @param strDestination : 以0为结束符的目标Unicode字符串
    * @param strSource : 以0为结束符的源Unicode字符串
    * @return 结果Unicode字符串，使用的是目标Unicode字符串空间，请预留足够的空间
    */
    inline WCHAR* RXAPI  RXWcscat(WCHAR*strDestination,const WCHAR *strSource )
    {
        return wcscat(strDestination, strSource);
    }

    /**
    * @brief
    * 在目标字符串后添加一个字符串
    * @param strDestination : 以0为结束符的目标字符串
    * @param strSource : 源字符串
    * @param len : 源字符串的最大添加长度
    * @return 结果字符串，使用的是目标字符串空间，请预留足够的空间
    */
    inline CHAR* RXAPI RXStrncat(CHAR *strDestination,const CHAR *strSource, UINT32 len)
    {
        return strncat(strDestination,strSource, len);
    }

    /**
    * @brief
    * 在目标Unicode字符串后添加一个Unicode字符串
    * @param strDestination : 以0为结束符的目标Unicode字符串
    * @param strSource : 源Unicode字符串
    * @param len : 源Unicode字符串的最大添加长度
    * @return 结果Unicode字符串，使用的是目标Unicode字符串空间，请预留足够的空间
    */
    inline WCHAR* RXAPI  RXWcsncat(WCHAR *strDestination,const WCHAR *strSource, UINT32 len)
    {
        return wcsncat(strDestination,strSource, len);
    }

    /**
    * @brief
    * 拷贝字符串
    * @param strDestination : 以0为结束符的目标缓冲区，请保证有足够的空间
    * @param strSource : 以0为结束符的源字符串
    * @return 目标字符串
    */
    inline char* RXStrcpy(char*strDestination, const char*strSource )
    {
        return strcpy(strDestination, strSource);
    }

    /**
    * @brief
    * 安全拷贝字符串
    * @param strDestination : 需要拷贝字符串的目标缓冲区
    * @param numberOfElements : 目标字符串最大长度
    * @param strSource : 以0结尾的源字符串
    * @return 返回目标串,如果目标缓冲区的长度小于源串,返回NULL
    */
    inline char* RXAPI RXStrSafeCpy( char *strDestination, size_t numberOfElements, const char *strSource )
    {
        if (numberOfElements  < strlen(strSource) +1)
        {
            return NULL;
        }

        return strncpy(strDestination,strSource,numberOfElements);
    }

    /**
    * @brief
    * 安全拷贝字符串
    * @param strDestination : 需要拷贝字符串的目标缓冲区
    * @param numberOfElements : 目标字符串最大长度
    * @param strSource : 以0结尾的源字符串
    * @return 返回目标串,如果目标缓冲区的长度小于源串,返回NULL
    */
    template <class T>
    inline void RXStrSafeCpy(T& Destination, const char* Source) 
    {
        // Use cast to ensure that we only allow character arrays
        (static_cast<char[sizeof(Destination)]>(Destination));

        // Copy up to the size of the buffer
        RXStrSafeCpy(Destination, Source, sizeof(Destination));
    }


    /**
    * @brief
    * 拷贝Unicode字符串
    * @param strDestination : 以0结尾的目标缓冲区，请保证有足够的空间
    * @param strSource : 以0结尾的源字符串
    * @return 目标字符串
    */
    inline WCHAR*  RXWcscpy( WCHAR *strDestination, const WCHAR *strSource )
    {
        return wcscpy(strDestination, strSource);
    }

    /**
    * @brief
    * 安全拷贝Unicode字符串
    * @param strDestination : 需要拷贝Unicode字符串的目标缓冲区
    * @param numberOfElements : 目标字Unicode符串最大长度
    * @param strSource : 以0结尾的源Unicode字符串
    * @return 返回目标串,如果目标缓冲区的长度小于源串,返回NULL
    */
    WCHAR * RXWcsSafeCpy( WCHAR *strDestination, size_t numberOfElements, const WCHAR *strSource );

    /**
    * @brief
    * 拷贝字符串
    * @param strDestination : 以0结尾的目标字符串
    * @param strSource : 以0结尾的源字符串
    * @param len : 最大拷贝长度
    * @return 返回目标字符串
    */
    inline CHAR*  RXStrncpy( CHAR *strDestination, const CHAR *strSource, UINT32 len )
    {
        return strncpy(strDestination, strSource, len);
    }

    /**
    * @brief
    * 拷贝Unicode字符串
    * @param strDestination : 以0结尾的目标Unicode字符串
    * @param strSource : 以0结尾的源Unicode字符串
    * @param len : 最大拷贝长度
    * @return 返回目标Unicode字符串
    */
    inline WCHAR*  RXWcsncpy( WCHAR *strDestination, const WCHAR *strSource, UINT32 len )
    {
        return wcsncpy(strDestination, strSource, len);
    }

    /**
    * @brief
    * 比较字符串
    * @param string1 : 以0结束的字符串1
    * @param string2 : 以0结束的字符串2
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
    inline INT32  RXStrcmp( const CHAR *string1, const CHAR *string2 )
    {
        return strcmp(string1, string2);
    }

    inline string RXStrToLowerA(const string& str)
    {
        string strResult = str;
        for (int t=0;t<(int)str.size();t++)
        {
            strResult[t] = tolower(str[t]);
        }
        return strResult;
    }
	inline wstring RXStrToLowerW(const wstring& str)
	{
        wstring strResult = str;
		for (int t = 0; t < (int)str.size(); t++)
		{
			strResult[t] = tolower(str[t]);
		}
		return strResult;
	}
    /**
    * @brief
    * 比较Unicode字符串
    * @param string1 : 以0结束的Unicode字符串1
    * @param string2 : 以0结束的Unicode字符串2
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
    inline INT32  RXWcscmp( const WCHAR *string1, const WCHAR *string2 )
    {
        return wcscmp(string1, string2);
    }

    /**
    * @brief
    * 比较字符串
    * @param string1 : 以0结束的字符串1
    * @param string2 : 以0结束的字符串2
    * @param count : 最大比较长度
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
    inline INT32  RXStrncmp( const CHAR *string1, const CHAR *string2, UINT32 count )
    {
        return strncmp( string1, string2, count );
    }

    /**
    * @brief
    * 比较Unicode字符串
    * @param string1 : 以0结束的Unicode字符串1
    * @param string2 : 以0结束的Unicode字符串2
    * @param count : 最大比较长度
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
    inline INT32  RXWcsncmp( const WCHAR *string1, const WCHAR *string2, size_t count )
    {
        return wcsncmp( string1, string2, count );
    }

    /**
    * @brief
    * 在一个字符串中查找某个字符的位置
    * @param string : 以0结束的字符串
    * @param c : 需要被查找的字符字符
    * @return 指向查找的该字符的第一次出现的位置，或者没有找到返回NULL
    */
    inline CHAR*  RXStrchr( const CHAR *string, INT32 c )
    {
        return (CHAR*)strchr(string, c);
    }

    /**
    * @brief
    * 在一个Unicode字符串中查找某个字符的位置
    * @param string : 以0结束的Unicode字符串
    * @param c : 需要被查找的字符字符
    * @return 指向查找的该字符的第一次出现的位置，或者没有找到返回NULL
    */
    inline WCHAR*  RXWcschr( const WCHAR *string, WCHAR c )
    {
        return (WCHAR*)wcschr(string, c);
    }

    /**
    * @brief
    * 在一个字符串中逆向查找某个字符的位置
    * @param string : 以0结束的字符串
    * @param c : 需要被查找的字符字符
    * @return 指向查找的该字符的最后一次出现的位置，或者没有找到返回NULL
    */
    inline CHAR*  RXStrrchr( const CHAR *string, INT32 c )
    {
        return (CHAR*)strrchr(string ,c);
    }

    /**
    * @brief
    * 在一个Unicode字符串中逆向查找某个字符的位置
    * @param string : 以0结束的Unicode字符串
    * @param c : 需要被查找的字符字符
    * @return 指向查找的该字符的最后一次出现的位置，或者没有找到返回NULL
    */
    wchar_t*  RXWcsrchr( const wchar_t *string, wchar_t c );

    /**
    * @brief
    * 无视字符大小写，比较字符串
    * @param s1 : 以0结束的字符串1
    * @param s2 : 以0结束的字符串2
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
    inline INT32  RXStrcasecmp(const CHAR *s1, const CHAR *s2)
    {
#if (defined(WIN32) || defined(WIN64))
        return _stricmp(s1, s2);
#else
        return strcasecmp(s1, s2);
#endif
    }
	inline INT32  RXWcsncasecmp(const wchar_t* s1, const wchar_t* s2)
	{
#if (defined(WIN32) || defined(WIN64))
		return _wcsicmp(s1, s2);
#else
		return wcscasecmp(s1, s2);
#endif
	}
    /**
    * @brief
    * 无视字符大小写，比较字符串
    * @param s1 : 以0结束的字符串1
    * @param s2 : 以0结束的字符串2
    * @param count : 最大比较长度
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
    inline INT32  RXStrncasecmp(const CHAR *s1, const CHAR *s2, UINT32 count)
    {
#if (defined(WIN32) || defined(WIN64))
        return _strnicmp(s1, s2, count);
#else
        return strncasecmp(s1, s2, count);
#endif
    }

    /**
    * @brief
    * 无视字符大小写，比较Unicode字符串
    * @param s1 : 以0结束的Unicode字符串1
    * @param s2 : 以0结束的Unicode字符串2
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
      INT32  RXWcscasecmp(const wchar_t *s1, const wchar_t *s2);

    /**
    * @brief
    * 无视字符大小写，比较Unicode字符串
    * @param s1 : 以0结束的Unicode字符串1
    * @param s2 : 以0结束的Unicode字符串2
    * @param count : 最大比较长度
    * @return 比较结果
    * < 0 字符串1小于字符串2
    * 0 字符串1等于字符串2
    * > 0 字符串1大于字符串2
    */
     INT32  RXWcsncasecmp(const wchar_t *s1, const wchar_t *s2, UINT32 count);


    /**
    * @brief
    * 将某个字符串切分为多个字符串
    * @param pszStr : 第一次输入被切分的字符串，之后输入NULL
    * @param delim : 分割字符串，如“:”，“,”，“ ”等
    * @return 依次返回被切割的子字符串，如果没有新的子字符串，返回NULL
    */
    inline CHAR*  RXStrtok(CHAR *pszStr, const CHAR *delim)
    {
        return strtok(pszStr, delim);
    }

    /**
    * @brief
    * 将某个Unicode字符串切分为多个Unicode字符串
    * @param pszStr : 第一次输入被切分的Unicode字符串，之后输入NULL
    * @param delim : 分割Unicode字符串，如“:”，“,”，“ ”等
    * @return 依次返回被切割的子Unicode字符串，如果没有新的子Unicode字符串，返回NULL
    */
    inline wchar_t*  RXWcstok(wchar_t *pszStr, const wchar_t *delim);

    /**
    * @brief
    * 将字符串中的所有ANSI字符转化为小写
    * @param pszStr : 以0结尾的字符串
    * @return 被转化的字符串，使用原有字符串的空间，原有字符串将被破坏
    */
    CHAR* RXStrlwr(CHAR* pszStr);


    /**
    * @brief
    * 将字符串中的所有ANSI字符转化为大写
    * @param pszStr : 以0结尾的字符串
    * @return 被转化的字符串，使用原有字符串的空间，原有字符串将被破坏
    */
    CHAR* RXStrupr(CHAR* pszStr);

    /**
    * @brief
    * 将Unicode字符串中的所有字符转化为小写
    * @param pszStr : 以0结尾的Unicode字符串
    * @return 被转化的Unicode字符串，使用原有Unicode字符串的空间，原有Unicode字符串将被破坏
    */
    WCHAR*  RXWcslwr(WCHAR* pszStr);

    /**
    * @brief
    * 将Unicode字符串中的所有字符转化为大写
    * @param pszStr : 以0结尾的Unicode字符串
    * @return 被转化的Unicode字符串，使用原有Unicode字符串的空间，原有Unicode字符串将被破坏
    */
    WCHAR*   RXWcsupr(WCHAR* pszStr);

    /**
    * @brief
    * 将字符串转化为Unicode字符串
    * @param src : 以0结尾的字符串
    * @return 转化后的MultiBytes字符串
    */
	std::wstring RXAPI  RXA2W(const CHAR *src);


    /**
    * @brief
    * 将Unicode字符串转化为ANSI字符串
    * @param src : 以0结尾的Unicode字符串
    * @return 转化后的ANSI字符串
    */
	std::string RXAPI RXW2A(const WCHAR *src);
  
    /**
    * @brief
    * 将Unicode字符集转化为本地操作系统设置的字符集编码
    * @param src : 以0结尾的Unicode字符串
    * @return 转化后的字符串
    */
	std::string RXAPI RXW2Local(const WCHAR *src);

    /**
    * @brief
    * 将本地操作系统设置的字符集编码转换为Unicode字符集
    * @param src : 以0结尾的字符串
    * @return 转化后的字符串
    */
	std::wstring RXAPI RXLocal2W(const CHAR *src);

    /**
    * @brief
    * 将Unicode字符集转换为UTF8编码集
    * @param src : 以0结尾的Unicode字符串
    * @return 转化后的字符串
    */
    std::string  RXAPI RXW2UTF8(const WCHAR *src);

    /**
    * @brief
    * 将UTF8编码集转换为Unicode字符集
    * @param src : 以0结尾的字符串
    * @return 转化后的字符串
    */
    std::wstring  RXAPI RXUTF82W(const CHAR *src);

    /**
    * @brief
    * 将UTF8编码集转换为ASCII字符集
    * @param src : 以0结尾的字符串
    * @return 转化后的字符串
    */
    std::string  RXAPI RXUTF82A(const CHAR *src);

    /**
    * @brief
    * 将ASCII字符集转换为UTF8编码集
    * @param src : 以0结尾的字符串
    * @return 转化后的字符串
    */
    std::string  RXAPI RXA2UTF8(const CHAR *src);
  
    /**
    * @brief
    * 将本地操作系统设置的字符集编码转换为UTF8编码集
    * @param src : 以0结尾的字符串
    * @return 转化后的字符串
    */
    std::string RXAPI RXLocal2UTF8(const CHAR *src);

    /**
    * @brief
    * 将UTF8编码集转换为本地操作系统设置的字符集编码
    * @param src : 以0结尾的字符串
    * @return 转化后的字符串
    */
	std::string RXAPI  RXUTF82Local(const CHAR *src);


	/**
	* @brief
	* 将字符串转换为32位无符号数字
	* @param pStr : 被转换的字符串
	* @return 32位无符号数字
	*/
     UINT32 RXAPI  RXAtou(const CHAR* pStr);

	/**
	* @brief
	* 将字符串转换为64位无符号数字
	* @param pStr : 待转化的字符串
	* @return 64位无符号数字
	*/
	 UINT64  RXAPI  RXAtou64(const CHAR* pStr);

	/**
	* @brief
	* 将Unicode字符串转换为64位无符号数字
	* @param pStr : 待转化的字符串
	* @return 64位无符号数字
	*/
	 UINT64 RXAPI   RXWtou64(const WCHAR* pStr);

     UINT32  RXAPI  RXWtou(const WCHAR* pStr);

    /**
	* @brief
	* 将Unicode字符串转换为32位有符号数字
	* @param _Str : 待转化的字符串
	* @return 32位有符号数字
	*/
	inline INT32 RXAPI RXWtoi(const WCHAR *_Str)
	{
		return atoi(RXW2A(_Str).c_str());
	}
	inline void StrSplit(const char* szData, const char* szDelim,
		std::vector<string>& vecData)
	{
		if (NULL == szData || NULL == szDelim)
		{
			return;
		}

        std::string strTmp = szData;
		char* szStr = const_cast<char*>(strTmp.c_str());

		char* pszPos = strtok(szStr, szDelim);
		while (pszPos) {
			vecData.push_back(pszPos);
			pszPos = strtok(NULL, szDelim);
		}
	}
    /**
    * @brief
    * 将一个数字转化为字符串
    * @param pBuf : 转化后的字符存储空间
    * @param buflen : 字符存储空间的最大长度
    * @param dwNum : 将被转化的数字
    * @return 转化后字符串使用的空间长度
    */
    inline INT32 RXAPI  RXItoa(CHAR* pBuf, UINT32 buflen, INT32 dwNum)
    {
        return RXSnprintf(pBuf, buflen, "%d", dwNum);
    }

	/**
    * @brief
    * 将一个数字转化为Unicode字符串
    * @param pBuf : 转化后的字符存储空间
    * @param buflen : 字符存储空间的最大长度
    * @param dwNum : 将被转化的数字
    * @return 转化后字符串使用的空间长度
    */
    inline INT32 RXAPI  RXItow(WCHAR* pBuf, UINT32 buflen, INT32 dwNum)
    {
        return swprintf(pBuf, buflen, L"%d", dwNum);
    }


	/**
	* @brief
	* 将有64位符号整型转换为字符串
	* @param pBuf : 转化后的字符存储空间
	* @param buflen : 字符存储空间的最大长度
	* @param dqNum : 将被转化的数字
	* @return 转化后字符串使用的空间长度
	*/
	inline INT32 RXAPI RXItoa64(CHAR *pBuf, UINT32 buflen, INT64 dqNum)
	{
		return RXSnprintf(pBuf, buflen, RXFMT_I64 , dqNum);
	}

	/**
	* @brief
	* 将有64位符号整型转换为Unicode字符串
	* @param pBuf : 转化后的字符存储空间
	* @param buflen : 字符存储空间的最大长度
	* @param dqNum : 将被转化的数字
	* @return 转化后字符串使用的空间长度
	*/
    inline INT32 RXAPI  RXItow64(WCHAR *pBuf, UINT32 buflen, INT64 dqNum)
    {
        return swprintf(pBuf, buflen,WCHAR_SDFMT_I64 , dqNum);
    }

	/**
	* @brief
	* 将无符号整型转换为字符串
	* @param pBuf : 转化后的字符存储空间
	* @param buflen : 字符存储空间的最大长度
	* @param dwNum : 将被转化的数字
	* @return 转化后字符串使用的空间长度
	*/
	inline INT32 RXAPI  RXUtoa(CHAR *pBuf, UINT32 buflen, UINT32 dwNum)
	{
		return RXSnprintf(pBuf, buflen, "%u", dwNum);
	}

	/**
	* @brief
	* 将无符号整型转换为Unicode字符串
	* @param pBuf : 转化后的字符存储空间
	* @param buflen : 字符存储空间的最大长度
	* @param dwNum : 将被转化的数字
	* @return 转化后字符串使用的空间长度
	*/
    inline INT32 RXAPI  RXUtow(WCHAR *pBuf, UINT32 buflen, UINT32 dwNum)
    {
        return swprintf(pBuf, buflen, L"%u", dwNum);
    }


	/**
	* @brief
	* 将有64位无符号整型转换为字符串
	* @param pBuf : 转化后的字符存储空间
	* @param buflen : 字符存储空间的最大长度
	* @param dqNum : 将被转化的数字
	* @return 转化后字符串使用的空间长度
	*/
	inline INT32 RXAPI  RXUtoa64(CHAR* pBuf,UINT32 buflen, UINT64 dqNum)
	{
		return RXSnprintf(pBuf, buflen, RXFMT_U64, dqNum);
	}
   
	/**
	* @brief
	* 将有64位无符号整型转换为Unicode字符串
	* @param pBuf : 转化后的字符存储空间
	* @param buflen : 字符存储空间的最大长度
	* @param dqNum : 将被转化的数字
	* @return 转化后字符串使用的空间长度
	*/
    inline INT32 RXAPI RXUtow64(WCHAR* pBuf,UINT32 buflen, UINT64 dqNum)
    {
        return RXSwprintf(pBuf, buflen, WCHAR_SDFMT_U64, dqNum);
    }

    /**
    * @brief
    * 将字符串转化为浮点数
    * @param nptr : 将被转化的以0结尾的字符串
    * @param endptr : [输出参数]若不为NULL，则遇到无法转化的字符从这里返回其指针
    * @return 获取的浮点数
    */
    inline double RXAPI RXStrtod(const CHAR *nptr, CHAR **endptr)
    {
        return strtod(nptr, endptr);
    }

	/**
    * @brief
    * 将Unicode字符串转化为浮点数
    * @param nptr : 将被转化的以0结尾的字符串
    * @param endptr : [输出参数]若不为NULL，则遇到无法转化的字符从这里返回其指针
    * @return 获取的浮点数
    */
    inline double RXAPI RXWCStrtod(const WCHAR *nptr, WCHAR **endptr)
    {
        return wcstod(nptr, endptr);
    }
    
    /**
    * @brief
    * split a string将字符串分割为一个字符串的vector
    * @param sSource : 被分割的字符串
    * @param delim : 分隔符，如:“:”，“,”，“ ”等
    * @return a vector to store strings splited from sSource
    */
     std::vector<std::string>  RXAPI  RXSplitStringsA(const std::string &sSource, CHAR delim);
     std::vector<std::string>  RXSplitStrings(const std::string& sSource, const std::string delim);
	/**
    * @brief
    * split a wstring将Unicode字符串分割为一个Unicode字符串的vector
    * @param sSource : 被分割的Unicode字符串
    * @param delim : 分隔符，如:“:”，“,”，“ ”等
    * @return a vector to store wstrings splited from sSource
    */
	 std::vector<std::wstring>  RXAPI  RXSplitStringsW(const std::wstring &sSource, WCHAR delim);

    /**
    * @brief
    * 将字符串转化为大写
    * @param pszStr : 将被转化的字符串
    * @return 转化后的字符串
    */
     std::string   RXStrupr(std::string &pszStr);

    /**
    * @brief
    * 将字符串转化为小写
    * @param pszStr : 将被转化的字符串
    * @return 转化后的字符串
    */
     std::string   RXStrlwr(std::string &pszStr);

    /**
    * @brief
    * 将一个字符串置空
    * @param Destination : 需要置空的类型
    * @return VOID
    */
    template <class T>
    inline VOID RXAPI RXZeroString(T &Destination) throw()
    {
        (static_cast<CHAR[sizeof(Destination)]>(Destination));
        Destination[0] = '\0';
        Destination[sizeof(Destination)-1] = '\0';
    }


    /**
    * @brief
    * 将二进制Buffer转换为字符串,
    * @param pBuf    二进制Buffer指针
    * @param bufLen  二进制Buffer长度
    * @param pSplitter 输出的二进制数据间隔符
	* @param lineLen 源字符串分割多行的每行长度,该长度不包含分隔符的长度
    * @return 转换后的字符串
    */
     std::string RXAPI  RXBufferToHexA(const CHAR *pBuf, UINT32 bufLen, const CHAR *pSplitter = "", INT32 lineLen = -1);

	/**
    * @brief
    * 将二进制Buffer转换为Unicode字符串,
    * @param pBuf    二进制Buffer指针
    * @param bufLen  二进制Buffer长度
    * @param pSplitter 输出的二进制数据间隔符
	* @param lineLen 源字符串分割多行的每行长度,该长度不包含分隔符的长度
    * @return 转换后的Unicode字符串
    */
     std::wstring RXAPI   RXBufferToHexW(const WCHAR *pBuf, UINT32 bufLen, const WCHAR *pSplitter = L"", INT32 lineLen = -1);

    /**
    * @brief
    * 将32位有符号整型数字转换为二进制串,
    * @param dwNum : 32位有符号整型数字
    * @return 转换后的字符串
    */
     std::string  RXAPI  RXIntToHexA(INT32 dwNum);

	/**
    * @brief
    * 将32位有符号整型数字转换为二进制串,
    * @param dwNum : 32位有符号整型数字
    * @return 转换后的Unicode字符串
    */
     std::wstring  RXAPI  RXIntToHexW(INT32 dwNum);

    /**
    * @brief
    * 将str左边的ch去掉，原字符串将被修改
    * @param pszStr : 去掉左边的ch字符
    * @param pTrimStr : 被去掉的字符
    * @return 去掉字符pTrimStr后的字符串
    */
     std::string RXAPI  RXStrTrimLeftA(std::string &pszStr, const CHAR *pTrimStr = " \r\t\n");

	/**
    * @brief
    * 将str左边的ch去掉，原Unicode字符串将被修改
    * @param pszStr : 去掉左边的ch字符
    * @param pTrimStr : 被去掉的字符
    * @return 去掉字符pTrimStr后的Unicode字符串
    */
     std::wstring  RXAPI  RXStrTrimLeftW(std::wstring &pszStr, const WCHAR *pTrimStr = L" \r\t\n");

    /**
    * @brief
    * 将str右边的ch去掉，原字符串将被修改
    * @param pszStr : 去掉右边的ch字符
    * @param pTrimStr : 被去掉的字符
    * @return 去掉字符pTrimStr后的字符串
    */
     std::string RXAPI RXStrTrimRightA(std::string &pszStr, const CHAR *pTrimStr = " \r\t\n");

	/**
    * @brief
    * 将str右边的ch去掉，原Unicode字符串将被修改
    * @param pszStr : 去掉右边的ch字符
    * @param pTrimStr : 被去掉的字符
    * @return 去掉字符pTrimStr后的Unicode字符串
    */
	 std::wstring  RXAPI  RXStrTrimRightW(std::wstring &pszStr, const WCHAR *pTrimStr = L" \r\t\n");

    /**
    * @brief
    * 将str左右两边的pTrimStr去掉，原字符串将被修改
    * @param pszStr : 去掉左右两边的ch字符
    * @param pTrimStr : 被去掉的字符
    * @return 去掉字符pTrimStr后的字符串
    */
     std::string RXAPI   RXStrTrimA(std::string &pszStr, const CHAR* pTrimStr = " \r\t\n");
   
	/**
    * @brief
    * 将str左右两边的pTrimStr去掉，原Unicode字符串将被修改
    * @param pszStr : 去掉左右两边的ch字符
    * @param pTrimStr : 被去掉的字符
    * @return 去掉字符pTrimStr后的Unicode字符串
    */
	 std::wstring RXAPI   RXStrTrimW(std::wstring &pszStr, const WCHAR* pTrimStr = L" \r\t\n");

    /** @} */


    /**
    * @brief Unicode字符串格式化
    * @param buffer : 存储格式化后的buffer
    * @param n      : Unicode字符串长度
    * @param format : 待格式化的Unicode字符串
    * @return 正值:成功转换后字符串的长度 负值:格式化失败
    */
	 INT32 RXAPI  RXsnwprintf(WCHAR* buffer, size_t n, const WCHAR* format, ...);

    template <class T>
    inline void RXAPI RXSafeSprintf(T& Destination, const CHAR *format, ...)
    {
        (static_cast<char[sizeof(Destination)]>(Destination));

        va_list args;
        va_start(args,format);
        _vsnprintf(Destination, sizeof(Destination)-1, format, args);
        va_end(args);
        Destination[sizeof(Destination)-1] = '\0';
    }
    INT32 RXIconvCovert(CHAR* desc, CHAR* src, CHAR* input, size_t ilen, CHAR* output, size_t& olen);
	 char* RXAPI  gg_base64_decode(const char *buf);
	 char* RXAPI  gg_base64_encode(const char *buf);
	 char* RXAPI JabberBase64Encode( const char* buffer, int bufferLen );
	 char* RXAPI JabberUrlEncode(const char* str);
	 tstring  RXAPI RXGetShortPathName(const TCHAR* pszPath);

#endif


