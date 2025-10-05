
#include "fundation/rxminidump.h"
#include "rxfile.h"
#include "rxdir.h"

TCHAR  szName[64] = {0};
CRXMiniDump::CRXMiniDump()
{
}


CRXMiniDump::~CRXMiniDump()
{
}

void CRXMiniDump::EnableAutoDump(bool bEnable,const TCHAR* pszName)
{
	if (bEnable)
	{
		_RXTStrcpy(szName, pszName);
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER) ApplicationCrashHandler);
	}
}

LONG CRXMiniDump::ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	/*if (IsDebuggerPresent())
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}*/

	TCHAR szDumpDir[MAX_PATH] = { 0 };
	TCHAR szDumpFile[MAX_PATH] = { 0 };
	TCHAR szMsg[MAX_PATH] = { 0 };
	SYSTEMTIME	stTime = { 0 };
	// ����dump�ļ�·��;
	GetLocalTime(&stTime);
	_RXTsprintf(szDumpDir, _T("%s\\dump"), RX::RXGetModulePath());
	RX::RXCreateDirectory(szDumpDir, TRUE);
	TSprintf(szDumpFile, _T("%s\\%s_%04d_%02d_%02d_%02d_%02d_%02d.dmp"), 
		szDumpDir,
		szName,
		stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond);
	// ����dump�ļ�;
	CreateDumpFile(szDumpFile, pException);

	return EXCEPTION_EXECUTE_HANDLER;
}

void CRXMiniDump::CreateDumpFile(LPCWSTR strPath, EXCEPTION_POINTERS *pException)
{
	// ����Dump�ļ�;
	HANDLE hDumpFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Dump��Ϣ;
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// д��Dump�ļ�����;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}