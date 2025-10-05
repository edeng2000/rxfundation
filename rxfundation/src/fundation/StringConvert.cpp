#include "fundation/StringConvert.h"
#include "fundation/ConvertUTF.h"
#include <vector>
#include <windows.h>
using namespace llvm; //for ConvertUTF.h

namespace ui
{

std::basic_string<UTF16Char> StringConvert::UTF8ToUTF16(const UTF8Char* utf8, size_t length)
{
    if ((utf8 == nullptr) || (length == 0)) {
        return std::basic_string<UTF16Char>();
    }
    std::vector<UTF16Char> data;
    data.resize(8192);
    UTF16Char* output = &data[0];
    const UTF8* src_begin = reinterpret_cast<const UTF8*>(utf8);
    const UTF8* src_end = src_begin + length;
    UTF16* dst_begin = reinterpret_cast<UTF16*>(output);

    std::basic_string<UTF16Char> utf16;
    while (src_begin < src_end) {
        ConversionResult result = ConvertUTF8toUTF16(&src_begin,
                                                     src_end,
                                                     &dst_begin,
                                                     dst_begin + data.size(),
                                                     lenientConversion);
        utf16.append((const UTF16Char*)output, dst_begin - reinterpret_cast<UTF16*>(output));
        dst_begin = reinterpret_cast<UTF16*>(output);
        if (result == sourceIllegal || result == sourceExhausted)
        {
            utf16.clear();
            break;
        }
    }
    return utf16;
}

std::wstring StringConvert::UTF8ToWString(const std::string& utf8)
{
#if defined(WCHAR_T_IS_UTF16)
    return UTF8ToUTF16(utf8.c_str(), utf8.length());
#else
    return UTF8ToUTF32(utf8.c_str(), utf8.length());
#endif
}

std::string StringConvert::UTF16ToUTF8(const UTF16Char* utf16, size_t length)
{
    if ((utf16 == nullptr) || (length == 0)) {
        return std::string();
    }
    std::vector<UTF8Char> data;
    data.resize(8192);
    UTF8Char* output = &data[0];
    const UTF16* src_begin = reinterpret_cast<const UTF16*>(utf16);
    const UTF16* src_end = src_begin + length;
    UTF8* dst_begin = reinterpret_cast<UTF8*>(output);

    std::string utf8;
    while (src_begin < src_end) {
        ConversionResult result = ConvertUTF16toUTF8(&src_begin,
                                                     src_end,
                                                     &dst_begin,
                                                     dst_begin + data.size(),
                                                     lenientConversion);
        utf8.append(output, dst_begin - reinterpret_cast<UTF8*>(output));
        dst_begin = reinterpret_cast<UTF8*>(output);
        if (result == sourceIllegal || result == sourceExhausted) {
            utf8.clear();
            break;
        }
    }

    return utf8;
}

std::string StringConvert::WStringToUTF8(const std::wstring& wstr)
{
#if defined(WCHAR_T_IS_UTF16)
    return UTF16ToUTF8((const UTF16Char*)wstr.c_str(), wstr.length());
#else
    ASSERT(sizeof(UTF32Char) == sizeof(std::wstring::value_type));
    return UTF32ToUTF8((const UTF32Char*)wstr.c_str(), wstr.length());
#endif
}

std::basic_string<UTF32Char> StringConvert::UTF8ToUTF32(const UTF8Char* utf8, size_t length)
{
    std::vector<UTF32Char> data;
    data.resize(8192);
    UTF32Char* output = &data[0];
    const UTF8* src_begin = reinterpret_cast<const UTF8*>(utf8);
    const UTF8* src_end = src_begin + length;
    UTF32* dst_begin = reinterpret_cast<UTF32*>(output);

    std::basic_string<UTF32Char> utf32;
    while (src_begin < src_end)
    {
        ConversionResult result = ConvertUTF8toUTF32(&src_begin,
            src_end,
            &dst_begin,
            dst_begin + data.size(),
            lenientConversion);

        utf32.append(output, dst_begin - reinterpret_cast<UTF32*>(output));
        dst_begin = reinterpret_cast<UTF32*>(output);
        if (result == sourceIllegal || result == sourceExhausted)
        {
            utf32.clear();
            break;
        }
    }

    return utf32;
}

std::string StringConvert::UTF32ToUTF8(const UTF32Char* utf32, size_t length)
{
    std::vector<UTF8Char> data;
    data.resize(8192);
    UTF8Char* output = &data[0];
    const UTF32* src_begin = reinterpret_cast<const UTF32*>(utf32);
    const UTF32* src_end = src_begin + length;
    UTF8* dst_begin = reinterpret_cast<UTF8*>(output);

    std::string utf8;
    while (src_begin < src_end)
    {
        ConversionResult result = ConvertUTF32toUTF8(&src_begin,
            src_end,
            &dst_begin,
            dst_begin + data.size(),
            lenientConversion);

        utf8.append(output, dst_begin - reinterpret_cast<UTF8*>(output));
        dst_begin = reinterpret_cast<UTF8*>(output);
        if (result == sourceIllegal || result == sourceExhausted)
        {
            utf8.clear();
            break;
        }
    }

    return utf8;
}

std::basic_string<UTF32Char> StringConvert::UTF16ToUTF32(const UTF16Char* utf16, size_t length)
{
    if ((utf16 == nullptr) || (length == 0)) {
        return std::basic_string<UTF32Char>();
    }
    std::vector<UTF32Char> data;
    data.resize(8192);
    UTF32Char* output = &data[0];
    const UTF16* src_begin = reinterpret_cast<const UTF16*>(utf16);
    const UTF16* src_end = src_begin + length;
    UTF32* dst_begin = reinterpret_cast<UTF32*>(output);

    std::basic_string<UTF32Char> utf32;
    while (src_begin < src_end) {
        ConversionResult result = ConvertUTF16toUTF32(&src_begin,
                                                      src_end,
                                                      &dst_begin,
                                                      dst_begin + data.size(),
                                                      lenientConversion);
        utf32.append(output, dst_begin - reinterpret_cast<UTF32*>(output));
        dst_begin = reinterpret_cast<UTF32*>(output);
        if (result == sourceIllegal || result == sourceExhausted) {
            utf32.clear();
            break;
        }
    }
    return utf32;
}

std::basic_string<UTF32Char> StringConvert::WStringToUTF32(const std::wstring& wstr)
{
#if defined(WCHAR_T_IS_UTF16)
    return UTF16ToUTF32((const UTF16Char*)wstr.c_str(), wstr.length());
#else
    ASSERT(sizeof(UTF32Char) == sizeof(std::wstring::value_type));
    std::basic_string<UTF32Char> utf32;
    utf32.append((const UTF32Char*)wstr.c_str(), wstr.length());
    return utf32;
#endif
}

std::wstring StringConvert::UTF32ToWString(const UTF32Char* utf32, size_t length)
{
    if ((utf32 == nullptr) || (length == 0)) {
        return std::wstring();
    }
#if defined(WCHAR_T_IS_UTF16)
    std::vector<UTF16Char> data;
    data.resize(8192);
    UTF16Char* output = &data[0];
    const UTF32* src_begin = reinterpret_cast<const UTF32*>(utf32);
    const UTF32* src_end = src_begin + length;
    UTF16* dst_begin = reinterpret_cast<UTF16*>(output);
    std::wstring utf16;
    while (src_begin < src_end) {
        ConversionResult result = ConvertUTF32toUTF16(&src_begin,
                                                      src_end,
                                                      &dst_begin,
                                                      dst_begin + data.size(),
                                                      lenientConversion);
        utf16.append((const std::wstring::value_type*)output, dst_begin - reinterpret_cast<UTF16*>(output));
        dst_begin = reinterpret_cast<UTF16*>(output);
        if (result == sourceIllegal || result == sourceExhausted) {
            utf16.clear();
            break;
        }
    }
    return utf16;
#else
    ASSERT(sizeof(wchar_t) == sizeof(UTF32Char));
    std::wstring utf32String;
    utf32String.append(utf32, length);
    return utf32String;
#endif
}

std::wstring StringConvert::UTF32ToWString(const std::basic_string<UTF32Char>& utf32)
{
    return UTF32ToWString(utf32.c_str(), utf32.length());
}

std::string StringConvert::TToUTF8(const std::wstring& str)
{
    return StringConvert::WStringToUTF8(str);
}

const std::string& StringConvert::TToUTF8(const std::string& str)
{
    return str;
}

tstring StringConvert::UTF8ToT(const UTF8Char* utf8, size_t length)
{
#ifdef DUILIB_UNICODE
    return StringConvert::UTF8ToUTF16(utf8, length);
#else
    return std::string(utf8, length);
#endif
}

tstring StringConvert::UTF8ToT(const std::string& utf8)
{
#ifdef DUILIB_UNICODE
    return StringConvert::UTF8ToWString(utf8);
#else
    return utf8;
#endif
}

const std::wstring& StringConvert::TToWString(const std::wstring& str)
{
    return str;
}

std::wstring StringConvert::TToWString(const std::string& str)
{
    return StringConvert::UTF8ToWString(str);
}

#ifdef DUILIB_UNICODE
const tstring& StringConvert::WStringToT(const std::wstring& wstr)
{
    return wstr;
}
#else
DString StringConvert::WStringToT(const std::wstring& wstr)
{
    return StringConvert::WStringToUTF8(wstr);
}
#endif

std::basic_string<UTF32Char> StringConvert::UTF8ToUTF32(const std::string& utf8)
{
    return UTF8ToUTF32(utf8.c_str(), utf8.length());
}

std::string StringConvert::UTF32ToUTF8(const std::basic_string<UTF32Char>& utf32)
{
    return UTF32ToUTF8(utf32.c_str(), utf32.length());
}

#ifdef DUILIB_BUILD_FOR_WIN
std::wstring StringConvert::MBCSToUnicode(const std::string& input, int32_t code_page)
{
    return MBCSToUnicode2(input.c_str(), input.size(), code_page);
}

std::wstring StringConvert::MBCSToUnicode2(const char* input, size_t inputSize, int32_t code_page)
{
    std::wstring output;
    if ((inputSize == 0) || (input == nullptr)) {
        return output;
    }

    int length = ::MultiByteToWideChar(code_page, 0, input, static_cast<int>(inputSize), NULL, 0);
    if (length < 0) {
        length = 0;
    }
    output.resize(length);
    if (output.empty()) {
        return output;
    }
    ::MultiByteToWideChar(code_page, 0, input, static_cast<int>(inputSize), &output[0], static_cast<int>(output.size()));
    return output;
}

tstring StringConvert::MBCSToT(const std::string& input)
{
    tstring output;
#ifdef DUILIB_UNICODE
    output = MBCSToUnicode(input);
#else
    std::wstring temp = MBCSToUnicode(input);
    output = WStringToUTF8(temp);
#endif
    return output;
}

std::string StringConvert::UnicodeToMBCS(const std::wstring& input, int32_t code_page)
{
    std::string output;
    int length = ::WideCharToMultiByte(code_page, 0, input.c_str(), static_cast<int>(input.size()), NULL, 0, NULL, NULL);
    if (length < 0) {
        length = 0;
    }
    output.resize(length);
    if (output.empty()) {
        return output;
    }
    ::WideCharToMultiByte(code_page, 0, input.c_str(), static_cast<int>(input.size()),
                          &output[0], static_cast<int>(output.size()), NULL, NULL);
    return output;
}

std::string StringConvert::TToMBCS(const tstring& input)
{
    std::string output;
#ifdef DUILIB_UNICODE
    output = UnicodeToMBCS(input);
#else
    std::wstring temp = UTF8ToWString(input);
    output = UnicodeToMBCS(temp);
#endif
    return output;
}

#endif //DUILIB_BUILD_FOR_WIN

#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_UNICODE)
DString StringConvert::TToLocal(const DString& input)
{
    DString output;
    std::wstring temp = UTF8ToWString(input);
    output = UnicodeToMBCS(temp);
    return output;
}
#else
const tstring& StringConvert::TToLocal(const tstring& input)
{
    return input;
}
#endif

#if defined (DUILIB_BUILD_FOR_WIN) && !defined (DUILIB_UNICODE)
DString StringConvert::LocalToT(const DString& input)
{
    DStringW output = MBCSToUnicode(input);
    return WStringToUTF8(output);
}
#else
const tstring& StringConvert::LocalToT(const tstring& input)
{
    return input;
}
#endif

} //namespace ui
