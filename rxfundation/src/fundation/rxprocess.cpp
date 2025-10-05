#include "fundation/rxprocess.h"
#include <windows.h>
#include <WinUser.h>
#include <TlHelp32.h>
#include "rxstring.h"
#include "mkmutex.h"
#include "rxtime.h"
#include <WinSock2.h>
#include <timeapi.h>
#include <libloaderapi.h>
#include <ShlObj.h>
#include <ShlObj_core.h>
#include <Shlwapi.h>
#include <shtypes.h>
#include "rxfile.h"
#include "rxdir.h"
CRXProcess::CRXProcess()
{
	memset(&m_saAttr, 0, sizeof(SECURITY_ATTRIBUTES));
	m_hReadFromChild = NULL;
	m_hWriteToParent = NULL;
	m_hWriteToChild = NULL;
	m_hReadFromParent = NULL;

	memset(&m_siStartupInfo, 0, sizeof(STARTUPINFO));
	memset(&m_piProcessInfo, 0, sizeof(PROCESS_INFORMATION));
	m_bPipeFull = FALSE;
	m_nPipeFullCount = 0;
	m_nPipeBufferSize = 8192; // default
}

CRXProcess::~CRXProcess()
{
	_DestroyCmdPipe();

}

VOID CRXProcess::Start(const TCHAR* pszCMDLine)
{
	Stop();
	_CreateCmdPipe();
	int nReturnCode = 0;

	memset(&m_piProcessInfo, 0, sizeof(m_piProcessInfo));
	memset(&m_siStartupInfo, 0, sizeof(m_siStartupInfo));

	m_siStartupInfo.cb = sizeof(m_siStartupInfo);
	m_siStartupInfo.hStdError = m_hWriteToParent;
	m_siStartupInfo.hStdOutput = m_hWriteToParent;
	m_siStartupInfo.hStdInput = m_hReadFromParent;
	m_siStartupInfo.wShowWindow = SW_HIDE;
	m_siStartupInfo.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

	BOOL bStarted;
	bStarted = ::CreateProcess(NULL,                                // command is part of input string
		(LPWSTR)pszCMDLine,                                 // (writeable) command string
		NULL,                                // process security
		NULL,                                // thread security
		TRUE,                                // inherit handles flag
		CREATE_NO_WINDOW | DETACHED_PROCESS | CREATE_SUSPENDED, // flags
		NULL,                                // inherit environment
		NULL,                                // inherit directory
		&m_siStartupInfo,                    // STARTUPINFO
		&m_piProcessInfo);                   // PROCESS_INFORMATION

	if (bStarted)
	{
		::CloseHandle(m_hWriteToParent);
		::CloseHandle(m_hReadFromParent);
		ResumeThread(m_piProcessInfo.hThread);
	}
}

VOID CRXProcess::Stop()
{
	_DestroyCmdPipe();
}

void CRXProcess::_DestroyCmdPipe(void)
{
	if (m_hReadFromChild == NULL)
	{
		return;
	}
	m_hReadFromChild = NULL;
	m_hWriteToParent = NULL;
	m_hWriteToChild = NULL;
	m_hReadFromParent = NULL;
}

void CRXProcess::_CreateCmdPipe(void)
{
	m_saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_saAttr.lpSecurityDescriptor = NULL;
	m_saAttr.bInheritHandle = TRUE;

	HANDLE hReadFromChild;
	if (!::CreatePipe(&hReadFromChild, &m_hWriteToParent, &m_saAttr, m_nPipeBufferSize))
	{
		// pipe failed
		return;
	}

	if (!::DuplicateHandle(::GetCurrentProcess(),
		hReadFromChild,
		::GetCurrentProcess(),
		&m_hReadFromChild,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
	{
		// duplicate failed
		::CloseHandle(hReadFromChild);
		::CloseHandle(m_hWriteToParent);
		return;
	}
	::CloseHandle(hReadFromChild);

	HANDLE hWriteToChild;
	if (!::CreatePipe(&m_hReadFromParent, &hWriteToChild, &m_saAttr, 0))
	{
		return;
	}

	if (!::DuplicateHandle(::GetCurrentProcess(),
		hWriteToChild,
		::GetCurrentProcess(),
		&m_hWriteToChild,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS))
	{
		::CloseHandle(m_hReadFromParent);
		::CloseHandle(hWriteToChild);
		return;
	}
	::CloseHandle(hWriteToChild);
}
CProcessInfoMap  g_mapProcessInfo;
void GetAllProcess()
{
	ClearAllProccess();	
	PROCESSENTRY32 pe;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &pe)) {
		do {
			CProcessInfoMapItr itr = g_mapProcessInfo.find(tstring(pe.szExeFile));
			if (itr != g_mapProcessInfo.end())
			{
				SProcessInfo oInfo;
				oInfo.dwProcessID = pe.th32ProcessID;
				ProcessIdToSessionId(pe.th32ProcessID, &oInfo.dwSessionID);
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
				if (hProcess != NULL) {
					oInfo.hProcess = hProcess;
					CloseHandle(hProcess);
				}
				itr->second.push_back(oInfo);
			}
			else
			{
				CProcessInfoVec vec;
				SProcessInfo oInfo;
				oInfo.dwProcessID = pe.th32ProcessID;
				ProcessIdToSessionId(pe.th32ProcessID, &oInfo.dwSessionID);
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
				if (hProcess != NULL) {
					oInfo.hProcess = hProcess;
					CloseHandle(hProcess);
				}
				vec.push_back(oInfo);
				g_mapProcessInfo.insert(make_pair(tstring(pe.szExeFile), vec));
			}

		} while (Process32Next(snapshot, &pe));
	}
	CloseHandle(snapshot);
	
}

void ClearAllProccess()
{	
	g_mapProcessInfo.clear();
	
}

CProcessInfoVec* FindProcess(const TCHAR* pszName)
{
	GetAllProcess();	
	CProcessInfoVec* poResult = NULL;
	CProcessInfoMapItr itr = g_mapProcessInfo.find(tstring(pszName));
	if (itr != g_mapProcessInfo.end())
	{		
		return &(itr->second);
	}
	
	return poResult;
}

bool   IsProcessExit(const TCHAR* pszName)
{
	CProcessInfoVec* poResult = FindProcess(pszName);
	if (poResult)
	{
		return true;
	}
	return false;
}

BOOL   KillProcessByName(const TCHAR* pszName)
{
	PROCESSENTRY32 pe;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &pe)) {
		do {
			if (_RXTStrcasecmp(pszName, pe.szExeFile) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
				if (hProcess != NULL) {
					CloseHandle(snapshot);
					TerminateProcess(hProcess, 0);
					return TRUE;
				}
			}
		} while (Process32Next(snapshot, &pe));
	}
	CloseHandle(snapshot);
	return FALSE;
}
bool IsRXServiceInstalled(const TCHAR* pszServiceName)
{
	TCHAR tempName[256] = { 0 };
	_RXTsprintf(tempName, _T("SYSTEM\\CurrentControlSet\\Services\\%s"), pszServiceName);

	HKEY hOpenedKey;
	// 打开指定的注册表键
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, tempName, 0, KEY_READ, &hOpenedKey) != ERROR_SUCCESS) {
		return false;
	}
	// 关闭注册表键
	RegCloseKey(hOpenedKey);
	return true;
}


bool RXStartService(const TCHAR* pszServiceName)
{
	SERVICE_STATUS hStatus;
	SC_HANDLE hSCManager;
	SC_HANDLE hService;

	// 打开服务控制管理数据库，并返回服务控制管理数据库的句柄
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL)
	{
		return false;
	}

	// 获得服务句柄
	hService = OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS | DELETE);
	if (hService == NULL)
	{
		CloseServiceHandle(hSCManager);
		return false;
	}

	// 获得服务的当前状态
	QueryServiceStatus(hService, &hStatus);

	// 如果服务处于停止状态，则将其状态设置为启动
	if (hStatus.dwCurrentState == SERVICE_STOPPED)
		//启动服务
	{
		::StartService(hService, 0, NULL);
		// 等待服务停止
		while (::QueryServiceStatus(hService, &hStatus) == TRUE)
		{
			::Sleep(hStatus.dwWaitHint);
			if (hStatus.dwCurrentState == SERVICE_RUNNING)
			{
				::CloseServiceHandle(hService);
				::CloseServiceHandle(hSCManager);
				break;
			}
			::StartService(hService, 0, NULL);
		}
	}

	// 关闭服务句柄
	CloseServiceHandle(hSCManager);
	return true;
}

void RXStopService(const TCHAR* pszServiceName)
{
	SC_HANDLE hSM = NULL, hService = NULL;
	SERVICE_STATUS serviceStatus;

	hSM = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!hSM) {
		
		return;
	}

	hService = OpenService(hSM, pszServiceName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP | DELETE);
	if (!hService) {
		DWORD myerror = GetLastError();
		if (myerror == ERROR_ACCESS_DENIED)
		{
			CloseServiceHandle(hSM);
			return;
		}
		if (myerror == ERROR_SERVICE_DOES_NOT_EXIST)
		{
			CloseServiceHandle(hSM);
			return;
		}
		CloseServiceHandle(hSM);
		return;
	}
	if (!QueryServiceStatus(hService, &serviceStatus)) 
	{		
		CloseServiceHandle(hService);
		CloseServiceHandle(hSM);
		return;
	}
	if (serviceStatus.dwCurrentState != SERVICE_STOPPED) {
		if (::ControlService(hService,
			SERVICE_CONTROL_STOP, &serviceStatus) == FALSE)
		{
			DWORD myerror = GetLastError();

			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSM);
			return;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hService, &serviceStatus) == TRUE)
		{
			::Sleep(serviceStatus.dwWaitHint);
			if (serviceStatus.dwCurrentState == SERVICE_STOPPED)
			{
				::CloseServiceHandle(hService);
				::CloseServiceHandle(hSM);
				break;
			}
		}
	}
	CloseServiceHandle(hService);
	CloseServiceHandle(hSM);
}

bool IsRXServiceRunning(const TCHAR* pszServiceName)
{
	SERVICE_STATUS hStatus;
	SC_HANDLE hSCManager;
	SC_HANDLE hService;

	// 打开服务控制管理数据库，并返回服务控制管理数据库的句柄
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL)
	{
		return false;
	}

	// 获得服务句柄
	hService = OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS | DELETE);
	if (hService == NULL)
	{
		return false;
	}

	// 获得服务的当前状态
	QueryServiceStatus(hService, &hStatus);

	// 如果服务处于停止状态，则将其状态设置为启动
	if (hStatus.dwCurrentState == SERVICE_STOPPED)
		//启动服务
	{
		CloseServiceHandle(hSCManager);
		return false;
	}

	// 关闭服务句柄
	CloseServiceHandle(hSCManager);
	return true;
}


#define VNCDEPENDENCIES    _T("Tcpip\0\0")

int RXInstallService(const TCHAR* pszServiceName,
	const TCHAR* pszPath, const TCHAR* psDesc)
{
	SC_HANDLE scm, service;
	scm = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);
	if (!scm) {
		return 1;
	}

	service = CreateService(scm, pszServiceName, pszServiceName, SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, pszPath,
		NULL, NULL, NULL, NULL, NULL);
	if (!service) {
		DWORD myerror = GetLastError();
		if (myerror == ERROR_ACCESS_DENIED)
		{
			CloseServiceHandle(scm);
			return 1;
		}
		if (myerror == ERROR_SERVICE_EXISTS)
		{
			CloseServiceHandle(scm);
			return 1;
		}

		CloseServiceHandle(scm);
		return 1;
	}
	else
		RXSetServiceDescription(pszServiceName, psDesc, RX::RXFileExtractPath(pszPath).c_str());
	CloseServiceHandle(service);
	CloseServiceHandle(scm);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void  RXUninstallService(const TCHAR* pszServiceName)
{
	SC_HANDLE scm, service;
	SERVICE_STATUS serviceStatus;

	scm = OpenSCManager(0, 0, SC_MANAGER_CONNECT);
	if (!scm) {
		return ;
	}

	service = OpenService(scm, pszServiceName,
		SERVICE_QUERY_STATUS | DELETE);
	if (!service) {
		DWORD myerror = GetLastError();
		if (myerror == ERROR_ACCESS_DENIED)
		{
			CloseServiceHandle(scm);
			return ;
		}
		if (myerror == ERROR_SERVICE_DOES_NOT_EXIST)
		{
#if 0
			MessageBoxSecure(NULL, "Failed: Service is not installed",
				app_name, MB_ICONERROR);
#endif
			CloseServiceHandle(scm);
			return;
		}
		CloseServiceHandle(scm);
		return ;
	}
	if (!QueryServiceStatus(service, &serviceStatus)) 
	{		
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return;
	}
	if (serviceStatus.dwCurrentState != SERVICE_STOPPED) {
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		Sleep(2500);
		RXUninstallService(pszServiceName);
		return;
	}
	if (!DeleteService(service)) {
		CloseServiceHandle(service);
		CloseServiceHandle(scm);
		return;
	}
	CloseServiceHandle(service);
	CloseServiceHandle(scm);
	return;
}


void RXSetServiceDescription(const TCHAR* pszServiceName,
	const TCHAR* psDesc,const TCHAR* pszPath)
{
	DWORD	dw;
	HKEY hKey;
	TCHAR tempName[256] = { 0 };
	_RXTsprintf(tempName, _T("SYSTEM\\CurrentControlSet\\Services\\%s"), pszServiceName);
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		tempName,
		0,
		REG_NONE,
		REG_OPTION_NON_VOLATILE,
		KEY_READ | KEY_WRITE,
		NULL,
		&hKey,
		&dw);
	RegSetValueEx(hKey,
		_T("Description"),
		0,
		REG_SZ,
		(const BYTE*)psDesc,
		_RXTStrlen(psDesc) + 1);
	RegSetValueEx(hKey,
		_T("Path"),
		0,
		REG_SZ,
		(const BYTE*)pszPath,
		_RXTStrlen(pszPath) * sizeof(TCHAR) + 1);

	RegCloseKey(hKey);
}


tstring CheckPath(const tstring& path)
{
	tstring result = path;

	if (result.size() > 0)
	{
		if (result[0] = _T('\"'))
			result = result.substr(1, result.size() - 1);
		if (result.size() > 0)
		{
			if (result[result.size() - 1] = _T('\"'))
				result = result.substr(0, result.size() - 1);
		}
	}

	return result;
}

tstring RXGetInstallPath(const TCHAR* pszServceName)
{
	TCHAR tempName[256] = { 0 };
	_RXTsprintf(tempName, _T("SYSTEM\\CurrentControlSet\\Services\\%s"), pszServceName);
	tstring strResult = RXReadRegistryValue(HKEY_LOCAL_MACHINE, tempName, (WCHAR*)_T("Path"));
	if (strResult.size() <= 0)
	{
		strResult = RXReadRegistryValue(HKEY_LOCAL_MACHINE, tempName, (WCHAR*)_T("ImagePath"));
		strResult = CheckPath(strResult);
		strResult = RX::RXFileExtractPath(strResult.c_str());

	}
	if (strResult.size() > 0)
	{
		int pos = strResult.find(_T('\\'));
		if (pos<0)
		{
			pos = strResult.find(_T('/'));
		}
		if (pos<0)
		{
			strResult = RXReadRegistryValue(HKEY_LOCAL_MACHINE, tempName, (WCHAR*)_T("ImagePath"));
			strResult = CheckPath(strResult);
			strResult = RX::RXFileExtractPath(strResult.c_str());
		}
		if (strResult[strResult.size()-1]!=_T('\\'))
		{
			strResult += _T('\\');
		}

		
	}
	return strResult;
}

// 读取注册表值
tstring RXReadRegistryValue(HKEY hKey, TCHAR* subKey, TCHAR* valueName)
{
	HKEY hOpenedKey;
	DWORD dwType = REG_SZ;  // 默认数据类型为字符串
	TCHAR szValue[512] = { 0 };
	DWORD dwSize = sizeof(szValue);

	// 打开指定的注册表键
	if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hOpenedKey) != ERROR_SUCCESS) {
		return szValue;
	}

	// 查询指定的注册表值
	if (RegQueryValueEx(hOpenedKey, valueName, NULL, &dwType, (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
	}
	else {
	
	}

	// 关闭注册表键
	RegCloseKey(hOpenedKey);
	return szValue;
}
// 读取注册表值
void RXRemoveRegistryValue(HKEY hKey, TCHAR* subKey, TCHAR* valueName)
{
	HKEY hOpenedKey;
	DWORD dwType = REG_SZ;  // 默认数据类型为字符串
	TCHAR szValue[512] = { 0 };
	DWORD dwSize = sizeof(szValue);

	// 打开指定的注册表键
	if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hOpenedKey) != ERROR_SUCCESS) {
		return;
	}

	// 查询指定的注册表值
	if (RegQueryValueEx(hOpenedKey, valueName, NULL, &dwType, (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
	}
	else {
	
	}

	// 关闭注册表键
	RegCloseKey(hOpenedKey);
	return ;
}

void    RXOpenUrl(const TCHAR* pszUrl)
{
	TCHAR szCommand[1024] = {0};
	_RXTsprintf(szCommand, _T("explorer.exe %s"), pszUrl);	
	STARTUPINFO si = { sizeof(si) }; //保存进程的启动信息
	PROCESS_INFORMATION pi;   //保存进程的相关信息
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = 1; //1窗口显示,0表示后台运行
	BOOL bRet = ::CreateProcess  //调用创建进程函数
	(
		NULL,
		szCommand,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);
	if (bRet)
	{
		::CloseHandle(pi.hProcess); //关闭进程句柄
		::CloseHandle(pi.hThread);  //关闭主线程句柄
	}

}