#ifndef ___RX_PROCESS_H____
#define ___RX_PROCESS_H____

#include <windows.h>
#include <vector>
#include "type.h"
#include <map>
#include "rxtype.h"
#include "rxstring.h"
using namespace std;

class CRXProcess
{
public:
	CRXProcess();
	~CRXProcess();
	VOID     Start(const TCHAR* pszCMDLine);
	VOID     Stop();
private:
	SECURITY_ATTRIBUTES m_saAttr;
	HANDLE m_hReadFromChild;
	HANDLE m_hWriteToParent;
	HANDLE m_hWriteToChild;
	HANDLE m_hReadFromParent;

	BOOL m_bPipeFull;
	int m_nPipeFullCount;

	int m_nPipeBufferSize;

	// MPlayer process related member variables
	STARTUPINFO m_siStartupInfo;
	PROCESS_INFORMATION m_piProcessInfo;

private:
	void      _DestroyCmdPipe();
	void      _CreateCmdPipe();
};

struct SProcessInfo
{
	char szName[MAX_PATH];
	DWORD dwProcessID;
	DWORD dwSessionID;
	HANDLE   hProcess;
	SProcessInfo()
	{
		memset(szName, 0, MAX_PATH);
		dwSessionID = 0;
		dwProcessID = 0;
		hProcess = NULL;
	}
	SProcessInfo(const SProcessInfo& oInfo)
	{
		memset(szName, 0, MAX_PATH);
		strcpy(szName, oInfo.szName);
		dwSessionID = oInfo.dwSessionID;
		dwProcessID = oInfo.dwProcessID;
		hProcess = oInfo.hProcess;
	}
};

typedef vector<SProcessInfo>    CProcessInfoVec;
typedef CProcessInfoVec::iterator CProcessInfoVecItr;

typedef map<tstring, CProcessInfoVec>  CProcessInfoMap;
typedef CProcessInfoMap::iterator     CProcessInfoMapItr;

void ClearAllProccess();

BOOL   KillProcessByName(const TCHAR* pszName);


bool  IsRXServiceInstalled(const TCHAR* pszServiceName);
bool  RXStartService(const TCHAR* pszServiceName);
VOID  RXStopService(const TCHAR* pszServiceName);
bool  IsRXServiceRunning(const TCHAR* pszServiceName);
int   RXInstallService(const TCHAR* pszServiceName,
	const TCHAR* pszPath,const TCHAR* psDesc);
void  RXUninstallService(const TCHAR* pszServiceName);
void  RXSetServiceDescription(const TCHAR* pszServiceName,
	const TCHAR* psDesc, const TCHAR* pszPath);
tstring RXGetInstallPath(const TCHAR* pszServceName);
tstring RXReadRegistryValue(HKEY hKey, TCHAR* subKey, TCHAR* valueName);
void    RXRemoveRegistryValue(HKEY hKey, TCHAR* subKey, TCHAR* valueName);
void    RXOpenUrl(const TCHAR* pszUrl);
#endif