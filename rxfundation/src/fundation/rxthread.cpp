#include "windows.h"
#include "fundation/rxthread.h"

CRXThread::CRXThread()
{
	m_hThread = NULL;
	m_dwThreadID = 0;
	m_hStopEvent = NULL;
	m_bRuning = FALSE;
	m_hStopEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	::InitializeCriticalSection(&m_hLock);
}

CRXThread::~CRXThread()
{
	CloseHandle(m_hStopEvent);
	m_hStopEvent = NULL;
	::DeleteCriticalSection(&m_hLock);
}

void CRXThread::StartThread()
{
	StopThread();
	m_bRuning = TRUE;
	m_hThread = ::CreateThread(NULL, 0, ThreadFunc, this, 0, (LPDWORD)&m_dwThreadID);
}


DWORD WINAPI CRXThread::ThreadFunc(VOID* lpVoid)
{
	CRXThread* poThis = (CRXThread*)lpVoid;
	if (poThis)
	{
		poThis->_Run();
		SetEvent(poThis->m_hStopEvent);
	}
	return 0;
}

void CRXThread::StopThread()
{
	if (m_bRuning && m_hThread)
	{
		m_bRuning = FALSE;
		::WaitForSingleObject(m_hStopEvent, 500000);
		ResetEvent(m_hStopEvent);
		TerminateThread(m_hThread, 0);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadID = 0;
		_DeleteAll();
	}
}

void CRXThread::_DeleteAll()
{
	::EnterCriticalSection(&m_hLock);
	for (UINT32 m = 0; m < (UINT32)m_vecTask.size(); m++)
	{
		m_vecTask[m]->Release();
	}
	m_vecTask.clear();
	::LeaveCriticalSection(&m_hLock);
}

void CRXThread::AddTask(IRXTask* poTask)
{
	::EnterCriticalSection(&m_hLock);
	m_vecTask.push_back(poTask);
	::LeaveCriticalSection(&m_hLock);
}

void CRXThread::InsertTask(IRXTask* poTask)
{
	::EnterCriticalSection(&m_hLock);
	if (m_vecTask.size()>0)
	{
		vector<IRXTask*>::iterator itr = m_vecTask.begin();
		m_vecTask.insert(itr, poTask);
	}
	else
		m_vecTask.push_back(poTask);
	::LeaveCriticalSection(&m_hLock);
}

IRXTask* CRXThread::_PopTask()
{
	IRXTask* poTask = NULL;
	::EnterCriticalSection(&m_hLock);
	if (m_vecTask.size() > 0)
	{
		std::vector<IRXTask*>::iterator itr = m_vecTask.begin();
		poTask = (*itr);
		m_vecTask.erase(itr);
	}
	::LeaveCriticalSection(&m_hLock);
	return poTask;
}

UINT32 CRXThread::GetCount()
{
	UINT32 dwCount = 0;
	::EnterCriticalSection(&m_hLock);
	dwCount = (UINT32)m_vecTask.size();
	::LeaveCriticalSection(&m_hLock);
	return dwCount;
}

VOID CRXThread::_Run()
{
	while (m_bRuning)
	{
		IRXTask* poTask = _PopTask();
		if (poTask)
		{
			poTask->OnExcuted();
			poTask->Release();
		}
		Sleep(10);
	}

}


CRXThreadEx::CRXThreadEx()
{
	m_hThread = NULL;
	m_dwThreadID = 0;
	m_hStopEvent = NULL;
	m_bRunning = FALSE;
	m_hStopEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hStartEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
}

CRXThreadEx::~CRXThreadEx()
{
	CloseHandle(m_hStopEvent);
	m_hStopEvent = NULL;
	CloseHandle(m_hStartEvent);
	m_hStartEvent = NULL;
}

void CRXThreadEx::SetStarted()
{
	SetEvent(m_hStartEvent);
}

void CRXThreadEx::SetStopEvent()
{
	SetEvent(m_hStopEvent);
}

bool CRXThreadEx::StartThread()
{
	m_bRunning = TRUE;
	m_hThread = ::CreateThread(NULL,
		0,
		ThreadFunc,
		this,
		0,
		(LPDWORD)&m_dwThreadID);
	::WaitForSingleObject(m_hStartEvent, INFINITE);
	return true;
}


DWORD WINAPI CRXThreadEx::ThreadFunc(VOID* lpVoid)
{
	CRXThreadEx* poThis = (CRXThreadEx*)lpVoid;
	if (poThis)
	{
		poThis->Run();
		SetEvent(poThis->m_hStopEvent);
	}
	return 0;
}

void CRXThreadEx::Terminate()
{
	TerminateThread(m_hThread, 0);
	::CloseHandle(m_hThread);
	m_hThread = NULL;
	m_dwThreadID = 0; 
	m_bRunning = FALSE;
}

void CRXThreadEx::StopThread()
{
	if (m_bRunning && m_hThread)
	{
		m_bRunning = FALSE;
		::WaitForSingleObject(m_hStopEvent, 5000);
		TerminateThread(m_hThread, 0);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadID = 0;
	}
}
