#include "rxlogger.h"
#include "rxutil.h"
#include "rxtime.h"
#include "rxstring.h"
#include "type.h"
#include <fcntl.h>
#include <corecrt_io.h>
int g_nLogLevel = RX_LOG_LEVEL_INFO | RX_LOG_LEVEL_DEBUG |
RX_LOG_LEVEL_WARN | RX_LOG_LEVEL_ERROR | RX_LOG_LEVEL_CRIT | RX_LOG_LEVEL_FATAL | RX_LOG_LEVEL_NOTI;


RX_IMPLEMENT_SINGLETON(CRXLogger)
CRXLogger::CRXLogger()
{
	m_hConselOut = NULL;
	m_hFile = NULL;
}

CRXLogger::~CRXLogger()
{
}

void CRXLogger::OpenOutput()
{
	if (m_hConselOut == NULL)
	{
		int nCrt = 0;
		FILE* fp;
		AllocConsole();
		nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		fp = _fdopen(nCrt, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);
		CRXLogger::Instance()->SetConselOutput((FILE*)fp);
	}
	else
	{
		fclose((FILE*)m_hConselOut);
		_fcloseall();
		m_hConselOut = NULL;
	}
}

bool CRXLogger::StartThread()
{
	StopThread();
	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile = NULL;
	}
	CHAR szPath[MAX_PATH] = {0};
	CRXDateTime oNow = RXNow();
	sprintf(szPath, "%s/log", _RXTT2Local(GetModulePath()).c_str());
	CreateDirectoryA(szPath, NULL);
	sprintf(szPath, "%s/log/%s_%4d_%02d_%02d.log",
		_RXTT2Local(GetModulePath()).c_str(),
		m_strName.c_str(),
		oNow.GetYear(),
		oNow.GetMonth(),
		oNow.GetDay());
	if (false == _FileExit(szPath))
	{
		m_hFile = fopen(szPath, "ab+");
	}
	else
	{
		m_hFile = fopen(szPath, "ab+");
	}

	for (uint32 t = 0; t < DEFAULT_FREE_LOGGER; t++)
	{
		CHAR* pszLogger =(CHAR*) malloc( DEFAULT_LOGGER_LEN);
		memset(pszLogger,0,DEFAULT_LOGGER_LEN);
		m_vecFreeData.push_back(pszLogger);
	}
	return CRXThreadEx::StartThread();
}

CHAR* CRXLogger::_PopFree()
{
	CHAR* pszResult = NULL;
	m_oFreeLock.Lock();
	if (m_vecFreeData.size()>0)
	{
		vector<CHAR*>::iterator itr = m_vecFreeData.begin();
		pszResult = *itr;
		memset(pszResult, 0, sizeof(CHAR) * DEFAULT_LOGGER_LEN);
		m_vecFreeData.erase(itr);
	}
	m_oFreeLock.Unlock();
	return pszResult;
}

void CRXLogger::_RecycleFree(CHAR* pszLogger)
{
	m_oFreeLock.Lock();
	m_vecFreeData.push_back(pszLogger);
	m_oFreeLock.Unlock();
}

void CRXLogger::StopThread()
{	
	CRXThreadEx::StopThread();
	_DeleteAll();
}

void CRXLogger::Terminate()
{
	CRXThreadEx::Terminate();
	_DeleteAll();
}

CHAR* CRXLogger::_Pop()
{
	CHAR* szData = NULL;
	m_oWorkLock.Lock();
	if (m_vecWorkData.size()>0)
	{
		vector<CHAR*>::iterator itr = m_vecWorkData.begin();
		szData = *itr;
		m_vecWorkData.erase(itr);
	}
	m_oWorkLock.Unlock();
	return szData;
}

VOID CRXLogger::Run()
{
	SetStarted();
	while (IsRunning())
	{
		CHAR* szData = _Pop();
		if (szData)
		{
			fprintf(m_hFile, "%s", szData);
			if (m_hConselOut)
			{
				fprintf(m_hConselOut, "%s", szData);
				fflush(m_hConselOut);
			}
			_RecycleFree(szData);
			fflush(m_hFile);
			
		}
		else
			Sleep(10);
	}
}
void CRXLogger::_DeleteAll()
{

	for (UINT32 m =0; m < (UINT32)m_vecWorkData.size();m++)
	{
		fprintf(m_hFile, "%s", m_vecWorkData[m]);
		free(m_vecWorkData[m]);
	
	}
	if (m_hFile)
	{
		fclose(m_hFile);
		m_hFile = NULL;
	}
	m_vecWorkData.clear();

	for (UINT32 m = 0; m < (UINT32)m_vecFreeData.size(); m++)
	{		
		free(m_vecFreeData[m]);		
	}
	m_vecFreeData.clear();
	if (m_hConselOut)
	{
		fclose(m_hConselOut);
	}
}


VOID  CRXLogger::LogDebug(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}


VOID CRXLogger::LogErr(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_ERROR, format, args);
	va_end(args);
}

VOID  CRXLogger::LogNoti(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_NOTI, format, args);
	va_end(args);
}

VOID  CRXLogger::LogCrit(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_CRIT, format, args);
	va_end(args);
}


VOID  CRXLogger::LogWarn(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_WARN, format, args);
	va_end(args);
}
VOID  CRXLogger::LogFatal(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_FATAL, format, args);
	va_end(args);
}

VOID CRXLogger::TorrentLogCrit(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_CRIT, format, argptr);
}
VOID CRXLogger::TorrentLogDebug(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_DEBUG, format, argptr);
}

VOID CRXLogger::TorrentLogInfo(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_INFO, format, argptr);
}

VOID CRXLogger::TorrentLogNoti(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_NOTI, format, argptr);
}

VOID CRXLogger::TorrentLogWarn(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_WARN, format, argptr);
}

VOID CRXLogger::TorrentLogErr(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_ERROR, format, argptr);
}

VOID CRXLogger::FFMPEGLogFatal(const CHAR* format, va_list& argptr)
{
	_FFMPEGLog(RX_LOG_LEVEL_FATAL, format, argptr);
}
VOID CRXLogger::TorrentLogFatal(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_FATAL, format, argptr);
}
VOID CRXLogger::FFMPEGLogCrit(const CHAR* format, va_list& argptr)
{
	_FFMPEGLog(RX_LOG_LEVEL_CRIT, format, argptr);
}
VOID CRXLogger::FFMPEGLogDebug(const CHAR* format, va_list& argptr)
{
	_FFMPEGLog(RX_LOG_LEVEL_DEBUG, format, argptr);
}

VOID CRXLogger::FFMPEGLogInfo(const CHAR* format, va_list& argptr)
{
	_FFMPEGLog(RX_LOG_LEVEL_INFO, format, argptr);
}

VOID CRXLogger::FFMPEGLogNoti(const CHAR* format, va_list& argptr)
{
	_TorrentLog(RX_LOG_LEVEL_NOTI, format, argptr);
}

VOID CRXLogger::FFMPEGLogWarn(const CHAR* format, va_list& argptr)
{
	_FFMPEGLog(RX_LOG_LEVEL_WARN, format, argptr);
}

VOID CRXLogger::FFMPEGLogErr(const CHAR* format, va_list& argptr)
{
	_FFMPEGLog(RX_LOG_LEVEL_ERROR, format, argptr);
}





VOID  CRXLogger::LogInfo(const CHAR* format, ...)
{
	va_list args;
	va_start(args, format);
	_Log(RX_LOG_LEVEL_INFO, format, args);
	va_end(args);
}

void CRXLogger::Log(const char* pszMsg)
{
	CHAR* szMsg = (CHAR*)_PopFree();
	if (NULL == szMsg)
	{
		return;
	}
	memset(szMsg, 0, DEFAULT_LOGGER_LEN);
	CRXDateTime oNow = RXNow();
	snprintf(szMsg, DEFAULT_LOGGER_LEN - 1, ("[%d-%02d-%02d %02d:%02d:%02d]: %s \r\n"),
		oNow.GetYear(),
		oNow.GetMonth(),
		oNow.GetDay(),
		oNow.GetHour(),
		oNow.GetMinute(),
		oNow.GetSecond(),
		pszMsg);

	m_oWorkLock.Lock();
	m_vecWorkData.push_back(szMsg);
	m_oWorkLock.Unlock();
}

VOID CRXLogger::_TorrentLog(ELogLevel enLevel, const CHAR* format, va_list& argptr)
{
	if (!(g_nLogLevel & enLevel))
	{
		return;
	}
	CHAR* szMsg = (CHAR*)_PopFree();
	if (NULL == szMsg)
	{
		return;
	}
	INT32 n2 = vsnprintf(szMsg, DEFAULT_LOGGER_LEN-1, format, argptr);
	szMsg[n2] = ('\0');
	CRXDateTime oNow = RXNow();
	CHAR szPrefix[128] = { 0 };
	switch (enLevel)
	{
	case RX_LOG_LEVEL_CRIT:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [CRIT]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_DEBUG:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [DEBUG]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_NOTI:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [NOTI]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_ERROR:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [ERROR]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_WARN:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [WARN]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_FATAL:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FATAL]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_INFO:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [INFO]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	default:
		break;
	}
	snprintf(szMsg, DEFAULT_LOGGER_LEN - 1, ("%s %s \r\n"), szPrefix, szMsg);
	if (m_hConselOut)
	{
		fprintf(m_hConselOut, "%s", szMsg);
	}
	if (strlen(szMsg) > 0)
	{
		m_oWorkLock.Lock();
		std::string strOut = szMsg;
		memset(szMsg, 0,  DEFAULT_LOGGER_LEN);
		RXStrcpy(szMsg, strOut.c_str());
		m_vecWorkData.push_back(szMsg);
		m_oWorkLock.Unlock();
	}
}


VOID CRXLogger::_FFMPEGLog(ELogLevel enLevel, const CHAR* format, va_list& argptr)
{
	if (!(g_nLogLevel & enLevel))
	{
		return;
	}
	CHAR* szMsg = (CHAR*)_PopFree();
	if (NULL == szMsg)
	{
		return;
	}
	CHAR* szOut = (CHAR*)_PopFree();
	if (NULL == szOut)
	{
		return;
	}

	INT32 n2 = vsnprintf(szMsg, DEFAULT_LOGGER_LEN-1, format, argptr);
	szMsg[n2] = ('\0');
	CRXDateTime oNow = RXNow();
	CHAR szPrefix[128] = { 0 };
	switch (enLevel)
	{
	case RX_LOG_LEVEL_CRIT:
	{
		sprintf(szPrefix,("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][CRIT]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_DEBUG:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][DEBUG]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_NOTI:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][NOTI]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_ERROR:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][ERROR]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_WARN:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][WARN]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_FATAL:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][FATAL]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_INFO:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [FFMPEG][INFO]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	default:
		break;
	}
	snprintf(szOut, DEFAULT_LOGGER_LEN-1,("%s %s\n"), szPrefix, szMsg);
	if (m_hConselOut)
	{
		fprintf(m_hConselOut, "%s", szOut);
	}
	if (strlen(szOut) > 0)
	{
		m_oWorkLock.Lock();
		std::string strOut = szOut;
		memset(szOut, 0, sizeof(CHAR) * DEFAULT_LOGGER_LEN);
		RXStrcpy((CHAR*)szOut, strOut.c_str());
		m_vecWorkData.push_back((CHAR*)szOut);
		m_oWorkLock.Unlock();
	}
	_RecycleFree((CHAR*)szMsg);
}

VOID CRXLogger::_Log(ELogLevel enLevel, const CHAR* format, va_list& argptr)
{
	if (!(g_nLogLevel & enLevel))
	{
		return;
	}
	CHAR* szMsg = _PopFree();
	if (NULL == szMsg)
	{
		return;
	}

	INT32 n2 = vsnprintf(szMsg, DEFAULT_LOGGER_LEN-1, format, argptr);
	szMsg[n2] = ('\0');
	CRXDateTime oNow = RXNow();
	CHAR szPrefix[128] = {0};
	switch (enLevel)
	{
	case RX_LOG_LEVEL_CRIT:
	{
		sprintf(szPrefix, ("[%d-%02d-%02d %02d:%02d:%02d] [CRIT]: "),
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_DEBUG:
	{
		sprintf(szPrefix, "[%d-%02d-%02d %02d:%02d:%02d] [DEBUG]: ",
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_NOTI:
	{
		sprintf(szPrefix, "[%d-%02d-%02d %02d:%02d:%02d] [NOTI]: ",
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_ERROR:
	{
		sprintf(szPrefix, "[%d-%02d-%02d %02d:%02d:%02d] [ERROR]: ",
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_WARN:
	{
		sprintf(szPrefix, "[%d-%02d-%02d %02d:%02d:%02d] [WARN]: ",
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_FATAL:
	{
		sprintf(szPrefix, "[%d-%02d-%02d %02d:%02d:%02d] [FATAL]: ",
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	case RX_LOG_LEVEL_INFO:
	{
		sprintf(szPrefix, "[%d-%02d-%02d %02d:%02d:%02d] [INFO]: ",
			oNow.GetYear(),
			oNow.GetMonth(),
			oNow.GetDay(),
			oNow.GetHour(),
			oNow.GetMinute(),
			oNow.GetSecond());
	}
	break;
	default:
		break;
	}
	CHAR* szOut = _PopFree();
	if (NULL == szOut)
	{
		return;
	}
	snprintf(szOut, DEFAULT_LOGGER_LEN-1, "%s %s \r\n", szPrefix, szMsg);
	if (m_hConselOut)
	{
		fprintf(m_hConselOut, "%s", szOut);
	}
	if (strlen(szOut) > 0)
	{
		m_oWorkLock.Lock();
		m_vecWorkData.push_back(szOut);
		m_oWorkLock.Unlock();
	}
	else
	{
		_RecycleFree(szOut);
	}
	_RecycleFree(szMsg);
}

bool CRXLogger::_FileExit(const CHAR* pszFile)
{
	FILE* hFile = fopen(pszFile, "rb");
	if (hFile)
	{
		fclose(hFile);
		return true;
	}
	return false;
}


