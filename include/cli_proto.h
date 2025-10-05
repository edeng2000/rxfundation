#ifndef __CLI_PROTO_H__
#define __CLI_PROTO_H__


#include <tchar.h>
#include <windows.h>
#include <winsock2.h>
#include <string.h>
#include "rxtype.h"
#include "type.h"

#define MAX_PACKET_SIZE   64* 1024

#pragma pack(push,1)
struct SCliProHead
{
    uint16  wDataLen;      // data len
	uint16  wMsgID;
};
#pragma pack(pop)

// build packet head
inline uint32 BuildPacket(char* pPacket, uint32 dwPackSize, const uint16& wMsgID, const char* pData, uint16 wLen)
{
	if (dwPackSize < sizeof(SCliProHead) + wLen)
	{
		return 0;
	}

	SCliProHead* pstHead = (SCliProHead*)pPacket;
	pstHead->wDataLen = htons(wLen);
	pstHead->wMsgID = htons(wMsgID);
	char* pszBody = pPacket + sizeof(SCliProHead);
	pstHead = (SCliProHead*)pszBody;
	memcpy(pszBody, pData, wLen);
	return sizeof(SCliProHead) + wLen;
}

inline int CheckPacket(const char* pData, uint16 wLen)
{
	if (wLen < sizeof(SCliProHead))
	{
		return 0;
	}

	const SCliProHead* pstCliHead = (const SCliProHead*)pData;

	uint16 wDataLen = ntohs(pstCliHead->wDataLen);
	
	if (wLen >= wDataLen + sizeof(SCliProHead))
	{
		return wDataLen + sizeof(SCliProHead);
	}

	return 0;
}


#endif
