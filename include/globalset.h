#ifndef __SERVERPARAMETER_H__
#define __SERVERPARAMETER_H__

// server configs

#include <string.h>
#include "globalmem.h"
#include "type.h"
#include "commonheader.h"
#include "gnserverid.h"
#include "rxlogger.h"

using namespace RX;
using namespace RX::Fundation;

using namespace GNDP;


#if defined(WIN32) || defined(WIN64)
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif
using namespace GNDP;


// common packet object
template<int MEMSIZE, int MEMCOUNT>
class IBaseBuilder
{
	typedef SGlobalMemAlloc<MEMSIZE, MEMCOUNT> SBuilderMem;
public:
	IBaseBuilder() {}
	virtual ~IBaseBuilder() {}

public:
	template <typename T>
	T* ClearBuffer(T*& pT)
	{
		char* pszData = m_oMem.GetData();
		pT = (T*)pszData;
		memset(pT, 0, sizeof(T));

		return pT;
	}

public:
	void*   GetPacketBuf() { return m_oMem.GetData(); }

protected:
	SBuilderMem m_oMem;
};

#pragma pack(push, 1)
// server pipe header: uuid and msgid
struct SPipeHeader
{
	// exchange id
	uint64_t llRoleID;
	uint32 dwClientID;
	uint16 wCompressType;
	// message id
	uint16 wMsgId;
};
#pragma pack(pop)


#endif