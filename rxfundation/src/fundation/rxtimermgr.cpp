#include "fundation/rxtimermgr.h"

CRXTimerMgr::CRXTimerMgr()
{
	m_dwRunTime = 0;
	m_llStartTime = RXTimeMilliSec();
	m_vecRecycleTimer.clear();
}

CRXTimerMgr::~CRXTimerMgr()
{
	UnInit();
}

bool CRXTimerMgr::Init()
{
	m_dwRunTime = 0;
	m_llStartTime = RXTimeMilliSec();
	return true;
}

void CRXTimerMgr::Run()
{
	uint64_t llCur = RXTimeMilliSec();
	uint32 dwNum = (uint32)((llCur - m_llStartTime) / TIMER_BASE_INTERVAL);
	if (m_dwRunTime >= dwNum)
		return;
	m_dwRunTime++;
	vector<STimerBase*>::iterator itrRecycle = m_vecRecycleTimer.begin();
	for (; itrRecycle != m_vecRecycleTimer.end(); itrRecycle++)
	{
		delete (*itrRecycle);
	}
	m_vecRecycleTimer.clear();

	vector<STimerBase*> oVec;
	CObjTimerBaseMapItr itr = m_mapTimer.begin();
	for (; itr != m_mapTimer.end(); itr++)
	{
		CTimerBaseMapItr itrBase = itr->second.begin();
		for (; itrBase != itr->second.end(); itrBase++)
		{
			oVec.push_back(itrBase->second);
		}
	}

	vector<STimerBase*>::iterator itrTimer = oVec.begin();
	for (; itrTimer != oVec.end(); itrTimer++)
	{
		STimerBase* poTimer = (*itrTimer);
		if (poTimer->isDestroy)
			continue;
		if (poTimer->dwLastTimer == 0) {
			poTimer->dwLastTimer = m_dwRunTime;
			continue;
		}
		if (poTimer->dwInterval <= m_dwRunTime - poTimer->dwLastTimer)
		{
			if (poTimer->dwCurTime < poTimer->dwTotalTime) {
				TimerBaseProcessFunc pFunc = poTimer->poFunc;
				poTimer->dwCurTime++;
				poTimer->dwLastTimer = m_dwRunTime;
				(*pFunc)(poTimer->poOwnerObj, m_dwRunTime, poTimer->oParam);
			}
			else
			{
				RemoveTimer(poTimer->poOwnerObj, poTimer->dwTimerID);
			}
		}
	}
}

void CRXTimerMgr::RemoveTimer(void* poOwnerObj, const uint32& dwTimerID)
{
	CObjTimerBaseMapItr itr = m_mapTimer.find(poOwnerObj);
	if (itr != m_mapTimer.end())
	{
		CTimerBaseMapItr itrBase = itr->second.find(dwTimerID);
		if (itrBase != itr->second.end())
		{
			m_vecRecycleTimer.push_back(itrBase->second);
			itrBase->second->isDestroy = true;
			itr->second.erase(itrBase);
		}
	}
}

bool CRXTimerMgr::RemoveTimer(void* poOwnerObj)
{
	CObjTimerBaseMapItr itr = m_mapTimer.find(poOwnerObj);
	if (itr != m_mapTimer.end())
	{
		CTimerBaseMapItr itrBase = itr->second.begin();
		for (; itrBase != itr->second.end(); itrBase++)
		{
			m_vecRecycleTimer.push_back(itrBase->second);
			itrBase->second->isDestroy = true;
		}
		itr->second.clear();
		m_mapTimer.erase(itr);
		return true;
	}
	return false;
}

void CRXTimerMgr::UnInit()
{
	CObjTimerBaseMapItr itr = m_mapTimer.begin();
	for (; itr != m_mapTimer.end(); itr++)
	{
		CTimerBaseMapItr itrBase = itr->second.begin();
		for (; itrBase != itr->second.end(); itrBase++)
		{
			delete itrBase->second;
		}
		itr->second.clear();;
	}
	m_mapTimer.clear();

	vector<STimerBase*>::iterator itrRecycle = m_vecRecycleTimer.begin();
	for (; itrRecycle != m_vecRecycleTimer.end(); itrRecycle++)
	{
		delete (*itrRecycle);
	}
	m_vecRecycleTimer.clear();
}

void  CRXTimerMgr::AddTimer(void* poOwnerObj, const uint32& dwTimerID, TimerBaseProcessFunc poFunc, const uint32 dwInterval, const uint32& dwTotal, const uint64_t& llData)
{
	RemoveTimer(poOwnerObj, dwTimerID);
	STimerBase* poTimer = new STimerBase;
	poTimer->dwCurTime = 0;
	poTimer->dwInterval = dwInterval / TIMER_BASE_INTERVAL;
	poTimer->dwLastTimer = 0;
	poTimer->dwTotalTime = dwTotal;
	poTimer->dwTimerID = dwTimerID;
	poTimer->poFunc = poFunc;
	poTimer->oParam.llData = llData;
	poTimer->oParam.poVoid = NULL;
	poTimer->poOwnerObj = poOwnerObj;
	_AddTimer(poTimer);
}
void  CRXTimerMgr::AddTimerEx(void* poOwnerObj, const uint32& dwTimerID, TimerBaseProcessFunc poFunc, const uint32 dwInterval, const uint32& dwTotal, const void* poVoid)
{
	RemoveTimer(poOwnerObj, dwTimerID);
	STimerBase* poTimer = new STimerBase;
	poTimer->dwCurTime = 0;
	poTimer->dwInterval = dwInterval / TIMER_BASE_INTERVAL;
	poTimer->dwLastTimer = 0;
	poTimer->dwTotalTime = dwTotal;
	poTimer->dwTimerID = dwTimerID;
	poTimer->poFunc = poFunc;
	poTimer->oParam.llData = 0;
	poTimer->oParam.poVoid = (void*)poVoid;
	poTimer->poOwnerObj = poOwnerObj;
	_AddTimer(poTimer);
}

CTimerBaseMap* CRXTimerMgr::FindObjTimer(void* poOwnerObj)
{
	CObjTimerBaseMapItr itr = m_mapTimer.find(poOwnerObj);
	if (itr != m_mapTimer.end())
	{
		return &(itr->second);
	}
	return NULL;
}

void CRXTimerMgr::_AddTimer(STimerBase* poTimer)
{
	CTimerBaseMap* poMap = FindObjTimer(poTimer->poOwnerObj);
	if (poMap)
	{
		poMap->insert(make_pair(poTimer->dwTimerID, poTimer));
	}
	else
	{
		CTimerBaseMap oMap;
		oMap.insert(make_pair(poTimer->dwTimerID, poTimer));
		m_mapTimer.insert(make_pair(poTimer->poOwnerObj, oMap));
	}
}