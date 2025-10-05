#ifndef  __GNMUTEX_H__
#define  __GNMUTEX_H__

#if defined(WIN32) || defined(WIN64) || defined(_M_X64)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "windows.h"
#ifndef GNAPI
#define GNAPI __stdcall
#endif

#define mutex_t           CRITICAL_SECTION
#define mutex_init(m)     InitializeCriticalSection(m)
#define mutex_acquire(m)  EnterCriticalSection(m)
#define mutex_release(m)  LeaveCriticalSection(m)
#define mutex_destroy(m)  DeleteCriticalSection(m)

#else

#include <pthread.h>
#define GNAPI
#define mutex_t           pthread_mutex_t
#define mutex_init(m)     pthread_mutex_init(m, NULL)
#define mutex_acquire(m)  pthread_mutex_lock(m)
#define mutex_release(m)  pthread_mutex_unlock(m)
#define mutex_destroy(m)  pthread_mutex_destroy(m)

#endif

namespace GNDP
{
	class IGNMutex
	{
	public:
		virtual ~IGNMutex() {}

		virtual void GNAPI Lock()  = 0;
		virtual void GNAPI Unlock() = 0;
		virtual void GNAPI Release() = 0;
	};

	// locker
	class CLocker : public IGNMutex
	{
	public:
		CLocker()
		{
			mutex_init(&m_oCS);
		}
		virtual ~CLocker()
		{
			mutex_destroy(&m_oCS);
		}

		virtual void GNAPI Lock()
		{
			mutex_acquire(&m_oCS);
		}

		virtual void GNAPI Unlock()
		{
			mutex_release(&m_oCS);
		}

		virtual void GNAPI Release()
		{
		}

	protected:
		mutex_t m_oCS;
	};

	// ¼ÙËø
	class CNonMutex : public IGNMutex
	{
	public:
		CNonMutex() {}
		virtual ~CNonMutex() {}

	public:
		virtual void GNAPI Lock()  {}
		virtual void GNAPI Unlock() {}
		virtual void GNAPI Release() {}
	};

	// ×Ô¶¯Ëø
	template <typename MT = CNonMutex>
	class CAutoLock
	{
	public:
		CAutoLock(MT& oLocker) : m_oLocker(oLocker)
		{
			m_oLocker.Lock();
		}
		~CAutoLock()
		{
			m_oLocker.Unlock();
		}

	protected:
		MT& m_oLocker;
	};

	// common mutex module
	IGNMutex* GNAPI GetGNMutex();
}

#endif
