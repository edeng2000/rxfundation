#ifndef ____RX_TIMER_MGR_H____
#define ____RX_TIMER_MGR_H____

#include <new>
#include <string.h>
#include <stdio.h>
#include "rxtime.h"
#include "rxstring.h"
#include <algorithm>

#include "time.h"
#include "type.h"
#include <map>

using namespace std;

#define  TIMER_BASE_INTERVAL           100
struct STimerParam;
typedef bool (*TimerBaseProcessFunc)(void* poObj, const uint32& dwTimer, const STimerParam& oParam);
struct STimerParam
{
	int64    llData;
	void* poVoid;
	STimerParam()
	{
		llData = 0;
		poVoid = NULL;
	}
	STimerParam(const STimerParam& oInfo)
	{
		llData = oInfo.llData;
		poVoid = oInfo.poVoid;
	}
};

struct STimerBase
{
	uint32   dwTimerID;
	TimerBaseProcessFunc  poFunc;
	uint32   dwInterval;
	uint32   dwCurTime;
	uint32   dwTotalTime;
	uint32   dwLastTimer;
	STimerParam   oParam;
	void* poOwnerObj;
	bool     isDestroy;
	STimerBase()
	{
		dwTimerID = INVALID_32BIT_ID;
		poFunc = NULL;
		dwInterval = INVALID_32BIT_ID;
		dwCurTime = 0;
		dwLastTimer = 0;
		dwTotalTime = INVALID_32BIT_ID;
		poOwnerObj = NULL;
		isDestroy = false;
	}
	STimerBase(const STimerBase& oInfo)
	{
		dwTimerID = oInfo.dwTimerID;
		poFunc = oInfo.poFunc;
		dwInterval = oInfo.dwInterval;
		dwCurTime = oInfo.dwCurTime;
		dwTotalTime = oInfo.dwTotalTime;
		dwLastTimer = oInfo.dwLastTimer;
		oParam = oInfo.oParam;
		poOwnerObj = oInfo.poOwnerObj;
		isDestroy = oInfo.isDestroy;
	}
};

typedef map<uint32, STimerBase*>		CTimerBaseMap;
typedef CTimerBaseMap::iterator			CTimerBaseMapItr;

typedef map<void*, CTimerBaseMap>       CObjTimerBaseMap;
typedef CObjTimerBaseMap::iterator		CObjTimerBaseMapItr;

class CRXTimerMgr
{
public:
	CRXTimerMgr();
	~CRXTimerMgr();
	static CRXTimerMgr* Instance()
	{
		static CRXTimerMgr oInstance;
		return &oInstance;
	}
	bool     Init();
	void     UnInit();
	void     Run();

	void     RemoveTimer(void* poOwnerObj, const uint32& dwTimerID);
	bool     RemoveTimer(void* poOwnerObj);

	void  AddTimer(void* poOwnerObj,
		const uint32& dwTimerID,
		TimerBaseProcessFunc poFunc,
		const uint32 dwInterval,
		const uint32& dwTotal, const uint64_t& llData = 0);

	void  AddTimerEx(void* poOwnerObj,
		const uint32& dwTimerID,
		TimerBaseProcessFunc poFunc,
		const uint32 dwInterval,
		const uint32& dwTotal, const void* poVoid = NULL);
	CTimerBaseMap* FindObjTimer(void* poOwnerObj);
private:
	uint64_t   m_llStartTime;
	uint32   m_dwRunTime;            //  已运行次数

	CObjTimerBaseMap  m_mapTimer;
	vector<STimerBase*>   m_vecRecycleTimer;
private:
	void     _AddTimer(STimerBase* poTimer);
};


#endif
