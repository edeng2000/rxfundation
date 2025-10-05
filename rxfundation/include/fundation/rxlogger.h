#ifndef _RX_LOGGER_INCLUDE___
#define _RX_LOGGER_INCLUDE___

#include "rxtype.h"
#if defined(WIN32) && defined(WIN64)
#include <windows.h>
#endif
#include <vector>
#include "rxstring.h"
#include "rxsingleton.h"
#include "rxutil.h"
#include "rxthread.h"
#include <cstdio>
#include <memory>
using namespace std;

enum ELogLevel
{
	RX_LOG_LEVEL_INFO = 1,
	RX_LOG_LEVEL_DEBUG = 2,
	RX_LOG_LEVEL_NOTI = 4,
	RX_LOG_LEVEL_WARN = 8,
	RX_LOG_LEVEL_ERROR = 16,
	RX_LOG_LEVEL_CRIT = 32,
	RX_LOG_LEVEL_FATAL = 64,
};

#define DEFAULT_LOGGER_LEN     10 * 1024
#define DEFAULT_FREE_LOGGER    100


class IRXLogger
{
public:	
	virtual VOID  Log(const char* pszMsg) = 0;
private:

};


class CRXLogger : public CRXThreadEx ,public IRXLogger
{
	RX_DECLARE_SINGLETON(CRXLogger)
public:
	CRXLogger();
	~CRXLogger();
	virtual bool StartThread();
	virtual void StopThread();
	virtual VOID Run();

	void    Terminate();
	VOID  LogDebug(const CHAR* format, ...);
	VOID  LogNoti(const CHAR* format, ...);
	VOID  LogCrit(const CHAR* format, ...);
	VOID  LogErr(const CHAR* format, ...);
	VOID  LogWarn(const CHAR* format, ...);
	VOID  LogFatal(const CHAR* format, ...);
	VOID  LogInfo(const CHAR* format, ...);
	void  SetConselOutput(FILE* hFile) { m_hConselOut = hFile; }
	void  OpenOutput();
	virtual VOID  Log(const char* pszMsg);

	VOID  TorrentLogDebug(const CHAR* format, va_list& argptr);
	VOID  TorrentLogNoti(const CHAR* format, va_list& argptr);
	VOID  TorrentLogCrit(const CHAR* format, va_list& argptr);
	VOID  TorrentLogErr(const CHAR* format, va_list& argptr);
	VOID  TorrentLogWarn(const CHAR* format, va_list& argptr);
	VOID  TorrentLogFatal(const CHAR* format, va_list& argptr);
	VOID  TorrentLogInfo(const CHAR* format, va_list& argptr);

	VOID  FFMPEGLogDebug(const CHAR* format, va_list& argptr);
	VOID  FFMPEGLogNoti(const CHAR* format, va_list& argptr);
	VOID  FFMPEGLogCrit(const CHAR* format, va_list& argptr);
	VOID  FFMPEGLogErr(const CHAR* format, va_list& argptr);
	VOID  FFMPEGLogWarn(const CHAR* format, va_list& argptr);
	VOID  FFMPEGLogFatal(const CHAR* format, va_list& argptr);
	VOID  FFMPEGLogInfo(const CHAR* format, va_list& argptr);
	void  SetName(const char* pszName) { m_strName = pszName; }
private:
	CRXLock		m_oWorkLock;
	CRXLock		m_oFreeLock;
	vector<CHAR*>  m_vecWorkData;
	vector<CHAR*>  m_vecFreeData;
	FILE* m_hFile;
	std::string  m_strName;
	FILE* m_hConselOut;
private:
	void   _DeleteAll();
	CHAR*   _Pop();
	VOID   _Log(ELogLevel enLevel, const CHAR* format, va_list& argptr);
	VOID   _TorrentLog(ELogLevel enLevel, const CHAR* format, va_list& argptr);
	VOID   _FFMPEGLog(ELogLevel enLevel, const CHAR* format, va_list& argptr);
	CHAR* _PopFree();
	void   _RecycleFree(CHAR* pszLogger);
	bool   _FileExit(const CHAR* pszFile);
};


/*#define LOG_DEBUG		CRXLogger::Instance()->LogDebug*/


#define LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	CRXLogger::Instance()->Log(buf);\
}

#define LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	CRXLogger::Instance()->Log(buf);\
}

#define LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	CRXLogger::Instance()->Log(buf);\
}

#define LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	CRXLogger::Instance()->Log(buf);\
}

#define LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	CRXLogger::Instance()->Log(buf);\
}

#define LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [TORRENT] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [TORRENT] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [TORRENT] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [TORRENT] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1, "[%s:%s:%d] [TORRENT] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [TORRENT] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}

#define TORRENT_LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    snprintf(buf, 4*1024 -1,"[%s:%s:%d] [TORRENT] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(CRXLogger::Instance()) \
		CRXLogger::Instance()->Log(buf);\
}


#define FFMPEG_LOG_DEBUG  	CRXLogger::Instance()->FFMPEGLogDebug
#define FFMPEG_LOG_CRIT  	CRXLogger::Instance()->FFMPEGLogCrit
#define FFMPEG_LOG_INFO  	CRXLogger::Instance()->FFMPEGLogInfo
#define FFMPEG_LOG_NOTI  	CRXLogger::Instance()->FFMPEGLogNoti
#define FFMPEG_LOG_ERR  	CRXLogger::Instance()->FFMPEGLogErr
#define FFMPEG_LOG_WARN  	CRXLogger::Instance()->FFMPEGLogWarn
#define FFMPEG_LOG_FATAL  	CRXLogger::Instance()->FFMPEGLogFatal
#endif // 