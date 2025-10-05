#ifndef UI_UTILS_STRING_CONVERT_H_
#define UI_UTILS_STRING_CONVERT_H_

#include "type.h"
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif
namespace ui
{
class StringConvert
{
public:
    static std::basic_string<UTF16Char> UTF8ToUTF16(const UTF8Char* utf8, size_t length);

    static std::wstring UTF8ToWString(const std::string& utf8);

    static std::string UTF16ToUTF8(const UTF16Char* utf16, size_t length);

    static std::string WStringToUTF8(const std::wstring& wstr);

    static std::string TToUTF8(const std::wstring& str);
    static const std::string& TToUTF8(const std::string& str);

    static tstring UTF8ToT(const std::string& utf8);
    static tstring UTF8ToT(const UTF8Char* utf8, size_t length);

    static const std::wstring& TToWString(const std::wstring& str);
    static std::wstring TToWString(const std::string& str);

#ifdef DUILIB_UNICODE
    static const tstring& WStringToT(const std::wstring& wstr);
#else
    static DString WStringToT(const std::wstring& wstr);
#endif
    static std::basic_string<UTF32Char> UTF8ToUTF32(const UTF8Char* utf8, size_t length);
    static std::basic_string<UTF32Char> UTF8ToUTF32(const std::string& utf8);

    static std::string UTF32ToUTF8(const UTF32Char* utf32, size_t length);
    static std::string UTF32ToUTF8(const std::basic_string<UTF32Char>& utf32);
    static std::basic_string<UTF32Char> UTF16ToUTF32(const UTF16Char* utf16, size_t length);
    static std::basic_string<UTF32Char> WStringToUTF32(const std::wstring& wstr);
    static std::wstring UTF32ToWString(const UTF32Char* utf32, size_t length);
    static std::wstring UTF32ToWString(const std::basic_string<UTF32Char>& utf32);

#ifdef DUILIB_BUILD_FOR_WIN
    static std::wstring MBCSToUnicode(const std::string& input, int32_t code_page = CP_ACP);
    static std::wstring MBCSToUnicode2(const char* input, size_t inputSize, int32_t code_page = CP_ACP);
    static std::string UnicodeToMBCS(const std::wstring& input, int32_t code_page = CP_ACP);
    static tstring MBCSToT(const std::string& input);
    static std::string TToMBCS(const tstring& input);
#endif

#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_UNICODE)
    static DString TToLocal(const DString& input);
#else
    static const tstring& TToLocal(const tstring& input);
#endif

#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_UNICODE)
    static DString LocalToT(const DString& input);
#else
    static const tstring& LocalToT(const tstring& input);
#endif
};

} //namespace ui

#endif // UI_UTILS_STRING_CONVERT_H_
