#ifndef _RX_SHARED_FILE_INCLUDE___
#define _RX_SHARED_FILE_INCLUDE___

#include <windows.h>
#include <vector>
#include "rxtype.h"
#include "rxconfig.h"
#include "rxstring.h"
#include "type.h"
#pragma pack(push,1)
enum PROTOCOLID
{
	PROTOCOL_ANY = -1,
	PROTOCOL_NULL = 0,
	PROTOCOL_G1 = 1,
	PROTOCOL_G2 = 2,
	PROTOCOL_ED2K = 3,
	PROTOCOL_HTTP = 4,
	PROTOCOL_FTP = 5,
	PROTOCOL_BT = 6,
	PROTOCOL_KAD = 7,
	PROTOCOL_DC = 8,
	PROTOCOL_LAST = 9
};

enum EShareFileStatus
{
	share_file_none = 0,
	share_file_checking_files,
	share_file_downloading_metadata,
	share_file_downloading,
	share_file_finished,
	share_file_seeding,
	share_file_checking_resume_data,
	share_file_paused,
	share_file_stopped,
	share_file_deleted,
};

enum EIdentify
{
	IDENTIFY_TORRENT_CLIENT = 0,
	IDENTIFY_TORRENT_MONITOR,
};

struct SFileProgress
{
	UINT32  dwID;
	UINT64  llFileSize;
	UINT32  dwSpeed;
	UINT64  llDownloadedSize;
	UINT32  dwPeersNum;
	UINT64  llProgress_ppm;
	EShareFileStatus enStatus;
	BOOL    bSave;
	SFileProgress()
	{
		dwPeersNum = 0;
		dwID = 0;
		dwSpeed = 0;
		llFileSize = 0;
		llDownloadedSize = 0;
		enStatus = share_file_none;
		llProgress_ppm = 0;
		bSave = FALSE;
	}
	SFileProgress(const SFileProgress& oInfo)
	{
		dwPeersNum = oInfo.dwPeersNum;;
		dwID = oInfo.dwID;
		dwSpeed = oInfo.dwSpeed;
		llFileSize = oInfo.llFileSize;
		llDownloadedSize = oInfo.llDownloadedSize;
		enStatus = oInfo.enStatus;
		llProgress_ppm = oInfo.llProgress_ppm;
		bSave = oInfo.bSave;
	}
};


#define MSG_SHARE_FILE_PROGREE_ID			1
#define MSG_SHARE_FILE_FINISHED_ID			2
#define MSG_SHARE_FILE_TERMINATE_ID			3
#define MSG_SHARE_FILE_START_ID				4
#define MSG_SHARE_FILE_PAUSE_REQ_ID			5
#define MSG_SHARE_FILE_PAUSE_ACK_ID			6
#define MSG_SHARE_FILE_STOP_REQ_ID			7
#define MSG_SHARE_FILE_STOP_ACK_ID			8
#define MSG_SHARE_FILE_RESUME_REQ_ID		9
#define MSG_SHARE_FILE_RESUME_ACK_ID		10

#define MSG_SHARE_FILE_LOGIN_REQ_ID			11
#define MSG_SHARE_FILE_LOGIN_ACK_ID			12

#define MSG_CLIENT_HEART_NTF_ID				13

struct SPauseReq
{
	uint32  dwID;
};

struct SPauseAck
{
	int32   nRet;
	uint32  dwID;
};
struct SStopReq
{
	uint32  dwID;
};
struct SStopAck
{
	int32   nRet;
	uint32  dwID;
};

struct SResumeReq
{
	uint32  dwID;
};

struct SResumeAck
{
	int32   nRet;
	uint32  dwID;
};

struct SLoginReq
{
	EIdentify  enType;
	uint32     dwID;
};

struct SLoginAck
{
	INT32      nRet;
	EIdentify  enType;
};

struct SCheckMediaReq
{
	uint32    dwID;
	char      szSavePath[256];
	char      szStatsPath[256];
};

struct SHeartNtf
{
	uint64_t    curTime;
};

#pragma pack(pop)
#endif
