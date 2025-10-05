#ifndef _MYSQLDYNAMICPOOL_H_
#define _MYSQLDYNAMICPOOL_H_

#include <list>
#include <time.h>
#include <algorithm>
#include <assert.h>
#include "mkmutex.h"
#include "type.h"
using namespace std;

// malloc just for c compiler, not only for c++

template <typename T, class Locker = CNonMutex>
class CDynamicPoolEx
{
	typedef list<T*> CObjList;
	typedef typename list<T*>::iterator CObjListIter;

	struct Skiller
	{
		void operator() (T* poBuff)
		{
			free (poBuff);
		}
	};

public:
	explicit CDynamicPoolEx() : m_nInitSize(0), m_nGrowSize(0)
	{
		m_listAllT.clear();
		m_listFreeT.clear();
	}

	~CDynamicPoolEx()
	{	
		m_oFreeLocker.Lock();
		m_listFreeT.clear();
		m_oFreeLocker.Unlock();

		// release all resources.
		std::for_each(m_listAllT.begin(), m_listAllT.end(), Skiller());
		m_listAllT.clear();
	}

public:
	bool Init(uint32 nInitSize, uint32 nGrowSize)
	{
		m_nInitSize = nInitSize;
		m_nGrowSize = nGrowSize;

		if (0 == m_nInitSize && 0 == m_nGrowSize)
		{
			return false;
		}
		return _Allocate(m_nInitSize);
	}

	T* FetchObj(void)
	{
		T* poShellT = NULL;

		// lock it
		m_oFreeLocker.Lock();

		if (m_listFreeT.empty())
		{
            if (!_Allocate(m_nGrowSize))
			{
				m_oFreeLocker.Unlock();

				return NULL;
			}
		}

		// get front resource.
		poShellT = m_listFreeT.front();
		if (NULL == poShellT)
		{
			m_oFreeLocker.Unlock();

			return NULL;
		}
		m_listFreeT.pop_front();

		// unlock
		m_oFreeLocker.Unlock();
		new (poShellT) T();
		return poShellT;
	}

	void ReleaseObj(T* poT)
	{
		if (NULL == poT) {
			return;
		}

		T* poShellT = static_cast<T*>(poT);
		if (NULL == poShellT) {
			return ;
		}
		// call deallocate to c++ object.
		poShellT->~T();

		// reallocate pShellT
		m_oFreeLocker.Lock();
		m_listFreeT.push_back(poShellT);
		m_oFreeLocker.Unlock();
	}

protected:
	bool _Allocate(int nSize)
	{
		T* pstArray = (T*)malloc(sizeof(T) * nSize);
		if (NULL == pstArray)
		{
			return false;
		}
		for (int i = 0; i < nSize; i++)
		{
			m_listFreeT.push_back(&pstArray[i]);
			
		}
		m_listAllT.push_back(pstArray);
		return true;
	}

protected:
	// Locker   m_oAllLocker;// all resources locker
	CObjList m_listAllT;     // all resources

	CObjList m_listFreeT;    // can allocated resource list
	Locker   m_oFreeLocker;  // allocated resource locker

	uint32      m_nInitSize;    // init size
	uint32      m_nGrowSize;    // grow size
};

#endif