#ifndef __GLOBAL_MEM_H__
#define __GLOBAL_MEM_H__

#include "dynamicpool.h"
#include <string.h>

template<int BLOCKSIZE>
struct SAllocMemData
{
	char szBuff[BLOCKSIZE];
};

namespace NGlobalDataSpace
{
	template<int BLOCKSIZE, int BLOCKCOUNT, typename locker>
	void MemOpt(char*& pszBuff, bool bFetch)
	{
		typedef SAllocMemData<BLOCKSIZE> SRealAllocMemData;

		static CDynamicPoolEx<SRealAllocMemData, locker> s_oDynamicPool;
		static bool s_bInit = false;

		if (!s_bInit)
		{
			s_oDynamicPool.Init(BLOCKCOUNT, 1);
			s_bInit = true;
		}

		if (bFetch)
		{
			SRealAllocMemData* pstBuff = s_oDynamicPool.FetchObj();
			pszBuff = pstBuff->szBuff;
		}
		else
		{
			SRealAllocMemData* pstBuff = (SRealAllocMemData*)pszBuff;
			s_oDynamicPool.ReleaseObj(pstBuff);
		}
	}
}

template<int MEMSIZE, int MEMCOUNT, typename locker = CNonMutex>
struct SGlobalMemAlloc
{
	SGlobalMemAlloc()
	{
		NGlobalDataSpace::MemOpt<MEMSIZE, MEMCOUNT, locker>(m_pBuff, true);
	}
	~SGlobalMemAlloc()
	{
		NGlobalDataSpace::MemOpt<MEMSIZE, MEMCOUNT, locker>(m_pBuff, false);
	}

	char* GetData()      { return m_pBuff; }
	int   GetSize()const { return MEMSIZE; }
	void  ClearData()    { memset(m_pBuff, 0, MEMSIZE); }

	template<typename T>
	T* ToType()
	{
		assert(sizeof(T) <= MEMSIZE);
		return new (m_pBuff) T();
	}

	template <typename T>
	operator T&()
	{
		return *(ToType<T>());
	}

	template <typename T>
	operator T*()
	{
		return ToType<T>();
	}

private:
	char* m_pBuff;
};

#endif