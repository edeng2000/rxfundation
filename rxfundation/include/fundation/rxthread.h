#ifndef _RX_THREAD_H____
#define _RX_THREAD_H____
#if defined (WIN32) || defined (WIN64)
#include <windows.h>
#endif
#include <vector>
#include "rxtype.h"
using namespace std;

class IRXTask
{
public:
	virtual  void OnExcuted() = 0;
	virtual  void Release() = 0;
};


class CRXThread
{
public:
	CRXThread();
	~CRXThread();
	virtual void StartThread();
	virtual void StopThread();
	UINT32  GetCount();
	void    AddTask(IRXTask* poTask);
	void    InsertTask(IRXTask* poTask);
private:
	UINT32		m_dwThreadID;
	SDHANDLE		m_hThread;
	SDHANDLE		m_hStopEvent;
	BOOL		m_bRuning;
#if defined (WIN32) || defined (WIN64)
	CRITICAL_SECTION m_hLock;
#endif
	vector<IRXTask*>  m_vecTask;

private:
#if defined (WIN32) || defined (WIN64)
	static DWORD WINAPI ThreadFunc(VOID* lpVoid);
#endif


#if defined (__APPLE__) && defined (__MACH__)
	static void* ThreadFunc(void* lpVoid);
#endif
	VOID   _Run();
	void   _DeleteAll();
	IRXTask* _PopTask();
};

class CRXThreadEx
{
public:
	CRXThreadEx();
	~CRXThreadEx();
	virtual bool StartThread();
	virtual void StopThread();
	virtual VOID Run() = 0;
	BOOL IsRunning() { return m_bRunning; }
	SDHANDLE GetThreadHandle() { return m_hThread; }
	UINT32 GetThreadID() { return m_dwThreadID; }
	void   SetStarted();
	void   Terminate();
	void   SetStopEvent();
private:
	UINT32		m_dwThreadID;
	SDHANDLE		m_hThread;
	SDHANDLE		m_hStopEvent;
	BOOL		m_bRunning;
	SDHANDLE		m_hStartEvent;
private:
#if defined (WIN32) || defined (WIN64)
static DWORD WINAPI ThreadFunc(VOID* lpVoid);
#endif


#if defined (__APPLE__) && defined (__MACH__)
static void* ThreadFunc(void* lpVoid);
#endif
};



#endif // _RX_THREAD_H____
