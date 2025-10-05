#ifndef _RX_FUNDATION_UTIL_INCLUDE_
#define _RX_FUNDATION_UTIL_INCLUDE_
#if defined (WIN32) || defined (WIN64)
#include "windows.h"
#include "tchar.h"
#endif
#include <string>
#include "rxstring.h"
#ifndef rxuint64
#define rxuint64  unsigned long long
#endif
tstring  GetTextByValue(const uint64_t& llValue);
std::wstring GetTimeByValue(const uint64_t& llValue);
tstring& replace_all(tstring& str, const tstring& old_value, const tstring& new_value);
std::string replace_allA(std::string& str, const std::string & old_value, const std::string& new_value);
bool FileExist(std::string strFile);
rxuint64 FileSize(std::string strFile);


std::string GetDataTimeString(long long llTime);
const TCHAR* GetModulePath(void);
rxuint64  RXTimeMilliSec();

std::wstring Utf82Unicode(const std::string& utf8string);
std::string WideByte2Acsi(const std::wstring& wstrcode);
std::string UTF_82ASCII(const std::string& strUtf8Code);
std::wstring Acsi2WideByte(const std::string& strascii);
std::string Unicode2Utf8(const std::wstring& widestring);
std::string ASCII2UTF_8(const std::string& strAsciiCode);
#if defined (WIN32) || defined (WIN64)
int GetIconIndex(LPCTSTR ext);
#endif
tstring GetFileExt(const tstring& strFilePath);
void RXSetAutoRunator(const TCHAR* pszName, const TCHAR* pszPath);
void RXCancelAutoRunator(const TCHAR* pszName, const TCHAR* pszPath);

class CRXLock
{
public:
	CRXLock();
	~CRXLock();
	void   Lock();
	void   Unlock();
private:
#if defined (WIN32) || defined (WIN64)
	CRITICAL_SECTION m_hLock;
#endif
};


class CRXAutoLock
{
public:
	CRXAutoLock(CRXLock& oLock);
	~CRXAutoLock();

private:
	CRXLock& m_oLock;
};

class CVideoDataBuff
{
public:
	CVideoDataBuff();
	~CVideoDataBuff();
	void  PushData(const char* pszData,
		const uint32_t& dwWidth,
		const uint32_t& dwHeight,
		const uint32_t& dwImageSize);
	void  Alloc(const uint32_t& dwImageSize);
	uint32_t GetWidth() { return m_dwWidth; }
	uint32_t GetHeight() { return m_dwHeight; }
	uint32_t GetImageSize() { return m_dwImageSize; }
	char* GetData() { return m_pszData; }
	bool  IsReady() { return m_pszData != NULL && m_dwImageSize > 0; }
	uint64_t GetTimestamp() { return m_llTime; }
	void     SetTime(const uint64_t& llTime) { m_llTime = llTime; }
	uint64_t GetLocalTimestamp() { return m_llLocalTime; }
	void     SetLocalTime(const uint64_t& llTime) { m_llLocalTime = llTime; }
	void     SetKeyFrame(const bool& bIsKeyFrame) { m_bIsKeyFrame = bIsKeyFrame; }
	bool     IsKeyFrame() { return m_bIsKeyFrame; }

	void     SetFrameNumber(const uint32_t& dwNumber) { m_dwFrameNumber = dwNumber; }
	uint32_t GetFrameNumber() { return m_dwFrameNumber; }
private:
	char* m_pszData;
	uint32_t m_dwWidth;
	uint32_t m_dwHeight;
	uint32_t m_dwImageSize;
	uint64_t m_llTime;
	bool     m_bIsKeyFrame;
	uint64_t m_llLocalTime;
	uint32_t m_dwFrameNumber;
};

std::wstring RXGetTextBySize(const uint64_t& llValue);
std::wstring RXGetTimeByValue(const uint64_t& llValue);
void  OpenUrl(const TCHAR* pszUrl,const bool& bIsIE=  false);
#endif
