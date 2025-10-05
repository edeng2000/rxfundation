#include "fundation/StringCharset.h"
#include "fundation/StringConvert.h"

namespace ui
{

uint32_t StringCharset::GetBOMSize(CharsetType charsetType)
{
    switch (charsetType)
    {
    case CharsetType::UTF8:
        return 3;
    case CharsetType::UTF16_BE:
    case CharsetType::UTF16_LE:
        return 2;
    default:
        break;
    }
    return 0;
}

CharsetType StringCharset::GetDataCharsetByBOM(const char* data, uint32_t length)
{
    CharsetType charsetType = CharsetType::UNKNOWN;
    if ((length < 2) || (data == nullptr)) {
        return charsetType;
    }
    if (length >= 4 && data[0] == '\x00' && data[1] == '\x00' && data[2] == '\xFE' && data[3] == '\xFF') {
        //"UTF-32BE";
        charsetType = CharsetType::UNKNOWN;
    }
    else if (length >= 4 && data[0] == '\xFF' && data[1] == '\xFE' && data[2] == '\x00' && data[3] == '\x00') {        
        if (length == 4) {
            charsetType = CharsetType::UTF16_LE;
        }
        else {
            //"UTF-32LE";
            charsetType = CharsetType::UNKNOWN;
        }
    }
    else if (length >= 3 && data[0] == '\xEF' && data[1] == '\xBB' && data[2] == '\xBF') {
        charsetType = CharsetType::UTF8;
    }
    else if (length >= 2 && data[0] == '\xFF' && data[1] == '\xFE') {
        charsetType = CharsetType::UTF16_LE;
    }
    else if (length >= 2 && data[0] == '\xFE' && data[1] == '\xFF') {
        charsetType = CharsetType::UTF16_BE;
    }
    return charsetType;
}

CharsetType StringCharset::GetDataCharset(const char* data, uint32_t length)
{
    CharsetType charsetType = CharsetType::UNKNOWN;
    if ((length < 1) || (data == nullptr)) {
        return charsetType;
    }
    if (IsValidateASCIIStream(data, length)) {
        charsetType = CharsetType::ANSI;
    }
    else if (IsValidateGBKStream(data, length)) {
        if (IsValidateUTF8Stream(data, length)) {
            charsetType = CharsetType::UTF8;
        }
        else {
            charsetType = CharsetType::ANSI;
        }
    }
    else if (IsValidateUTF8Stream(data, length)) {
        charsetType = CharsetType::UTF8;
    }
    else if (IsValidateUTF16LEStream(data, length)) {
        charsetType = CharsetType::UTF16_LE;
    }
    else if (IsValidateUTF16BEStream(data, length)) {
        charsetType = CharsetType::UTF16_BE;
    }
    return charsetType;
}

bool StringCharset::GetDataAsString(const char* data, uint32_t length, std::wstring& result)
{
    CharsetType inCharsetType = CharsetType::UNKNOWN;
    CharsetType outCharsetType = CharsetType::UNKNOWN;
    uint32_t bomSize = 0;
    return GetDataAsString(data, length, inCharsetType, result, outCharsetType, bomSize);
}

bool StringCharset::GetDataAsString(const char* data, uint32_t length, CharsetType inCharsetType, std::wstring& result)
{
    CharsetType outCharsetType = CharsetType::UNKNOWN;
    uint32_t bomSize = 0;
    return GetDataAsString(data, length, inCharsetType, result, outCharsetType, bomSize);
}

bool StringCharset::GetDataAsString(const char* data, uint32_t length,
                                    std::wstring& result, CharsetType& outCharsetType, uint32_t& bomSize)
{
    CharsetType inCharsetType = CharsetType::UNKNOWN;
    return GetDataAsString(data, length, inCharsetType, result, outCharsetType, bomSize);
}

bool StringCharset::GetDataAsString(const char* data, uint32_t length, CharsetType inCharsetType,
                                    std::wstring& result, CharsetType& outCharsetType, uint32_t& bomSize)
{
    result.clear();
    outCharsetType = CharsetType::UNKNOWN;
    bomSize = 0;
    if ((length < 1) || (data == nullptr)) {
        return false;
    }
    outCharsetType = GetDataCharsetByBOM(data, length);
    bomSize = GetBOMSize(outCharsetType);
    if (inCharsetType != CharsetType::UNKNOWN) {
        if (inCharsetType != outCharsetType) {            
            if (bomSize > 0) {
                return false;
            }
            else {
                CharsetType checkCharsetType = GetDataCharset(data, length);
                if (inCharsetType != checkCharsetType) {
                    return false;
                }
                outCharsetType = inCharsetType;
            }
        }
    }    

    if (outCharsetType == CharsetType::UNKNOWN) {
        outCharsetType = GetDataCharset(data, length);
        bomSize = 0;
    }
    const char* realData = data + bomSize;
    uint32_t realLen = length - bomSize;

    if (outCharsetType == CharsetType::ANSI) {
#ifdef DUILIB_BUILD_FOR_WIN
        result = StringConvert::MBCSToUnicode2(realData, realLen);
#else
        result = StringConvert::UTF8ToWString(std::string(realData, realLen));
#endif
    }
    else if (outCharsetType == CharsetType::UTF8) {
        result = StringConvert::UTF8ToWString(std::string(realData, realLen));
    }
    else if (outCharsetType == CharsetType::UTF16_LE) {
#if defined(WCHAR_T_IS_UTF16)
        result.assign((const wchar_t*)realData, realLen / sizeof(wchar_t));
#else
        result = StringConvert::UTF16ToUTF32((const UTF16Char*)realData, realLen / sizeof(UTF16Char));
#endif
    }
    else if (outCharsetType == CharsetType::UTF16_BE) {
        uint32_t dataSize = realLen / sizeof(uint16_t);
        result.reserve(dataSize + 1);
        const uint16_t* dataBE = (const uint16_t*)realData;
        for (uint32_t i = 0; i < dataSize; i++) {
            result.push_back((dataBE[i] >> 8) | (dataBE[i] << 8));
        }
    }
    else {
        return false;
    }
    return true;
}

bool StringCharset::IsValidateASCIIStream(const char* stream, uint32_t length)
{
    if ((length < 1) || (stream == nullptr)) {
        return false;
    }

    for (uint32_t i = 0; i < length; i++) {
        if (stream[i] >= 0 && stream[i] <= 127) {
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

bool StringCharset::IsValidateGBKStream(const char* stream, uint32_t length)
{
    if ((length < 1) || (stream == nullptr)) {
        return false;
    }
    unsigned char* s = (unsigned char*)stream;
    unsigned char* e = s + length;

    for (; s < e; s++) {
        if (*s < 0x80) {
            continue;
        }
        if (*s < 0x81 || 0xFE < *s) {
            break;
        }
        if (++s == e) {
            return false;
        }
        if (*s < 0x40 || 0xFE < *s) {
            break;
        }
    }
    return s == e;
}

bool StringCharset::IsValidateUTF8Stream(const char* stream, uint32_t length)
{
    if ((length < 1) || (stream == nullptr)) {
        return false;
    }
    for (uint32_t i = 0; i < length; i++) {
        if ((stream[i] & 0x80) == 0x00) {
            continue;
        }
        else if ((stream[i] & 0xE0) == 0xC0) {
            uint32_t numBytes = 2;
            for (uint32_t j = 1; j < numBytes; j++) {
                if ((i + j >= length) || ((stream[i + j] & 0xC0) != 0x80)) {
                    return false;
                }
            }
            i += numBytes - 1;
            continue;
        }
        else if ((stream[i] & 0xF0) == 0xE0) {
            uint32_t numBytes = 3;
            for (uint32_t j = 1; j < numBytes; ++j) {
                if ((i + j >= length) || ((stream[i + j] & 0xC0) != 0x80)) {
                    return false;
                }
            }
            i += numBytes - 1;
            continue;
        }
        else if ((stream[i] & 0xF8) == 0xF0) {
            uint32_t numBytes = 4;
            for (uint32_t j = 1; j < numBytes; ++j) {
                if ((i + j >= length) || ((stream[i + j] & 0xC0) != 0x80)) {
                    return false;
                }
            }
            i += numBytes - 1;
            continue;
        }
        else {
            return false;
        }
    }
    return true;
}

bool StringCharset::IsValidateUTF16LEStream(const char* stream, uint32_t length)
{
    if ((length < 1) || (stream == nullptr)) {
        return false;
    }
   
    if (length % 2 != 0) {
        return false;
    }

    for (uint32_t i = 0; i < length; i += 2) {
        if ((stream[i + 1] & 0xFC) == 0xD8) {
            if (((i + 3) < length) && ((stream[i + 3] & 0xFC) == 0xDC)) {
                i += 2;
                continue;
            }
            else {
                return false;
            }
        }
        else if ((stream[i + 1] & 0xFC) == 0xDC) {
            return false;
        }
    }
    return true;
}

bool StringCharset::IsValidateUTF16BEStream(const char* stream, uint32_t length)
{
    if ((length < 1) || (stream == nullptr)) {
        return false;
    }
    if (length % 2 != 0) {
        return false;
    }
    for (uint32_t i = 0; i < length; i += 2) {
        if ((stream[i] & 0xFC) == 0xD8) {
            if (((i + 2) < length) && ((stream[i + 2] & 0xFC) == 0xDC)) {
                i += 2;
                continue;
            }
            else {
                return false;
            }
        }
        else if ((stream[i] & 0xFC) == 0xDC) {
            return false;
        }
    }
    return true;
}

}//namespace ui
