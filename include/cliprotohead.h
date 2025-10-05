#ifndef __CLIPROTOHEADER_H__
#define __CLIPROTOHEADER_H__


#include <tchar.h>
#include <windows.h>
#include <winsock2.h>
#include <string.h>
#include "rxtype.h"
#include "type.h"

#pragma pack(push,1)
struct SCliProHead
{
    uint16  wDataLen;      // data len
	uint16  wMsgID;
};
#pragma pack(pop)

#define PARSE_CLI_PROTO(proto, oProto, pszData, dwLen)           \
	if (NULL == (pszData))                                   \
	    return 0;                                        \
		                                                     \
	if (!oProto.ParseFromArray(pszData, (dwLen)))            \
	{                                                        \
	    LOG_CRIT(_T("parse %s proto error"), #proto);\
	    return 0;                                        \
	}                                                        \

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
