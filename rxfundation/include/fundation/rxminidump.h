#pragma once
#include <Windows.h>
#include <tchar.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

#ifdef UNICODE
#define TSprintf	wsprintf
#else
#define TSprintf	sprintf
#endif


class CRXMiniDump
{
private:
	CRXMiniDump();
	~CRXMiniDump();

public:
	// 程序崩溃时是否启动自动生成dump文件;
	// 只需要在main函数开始处调用该函数即可;
	static void EnableAutoDump(bool bEnable = true,const TCHAR* pszName=_T(""));

private:

	static LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException);

	static void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);
};