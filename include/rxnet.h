#ifndef _RX_NET_EXPORT_INCLUDE___
#define _RX_NET_EXPORT_INCLUDE___
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

#ifdef linux
#include "gntype.h"
#include "gnlogger.h"
#include <cstdint>
#ifndef uint8_t
typedef unsigned char      uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short      uint16_t;
#endif

#ifndef uint32_t
//typedef unsigned long      uint32_t;
#endif

#ifndef uint64_t
//typedef unsigned long long      uint64_t;
#endif


typedef unsigned char      uint8;

// unsigned short
typedef unsigned short     uint16;

// unsigned int
typedef unsigned int       uint32;

typedef int                int32;

// unsigned long
typedef unsigned long      dword;

// unsigned long long


// long long
typedef long long          int64;
typedef unsigned long long uint64;


using namespace GNDP;
#define RXAPI 
#define RXNET_API 

#ifndef GNAPI
#define GNAPI 
#endif


extern CSDLogger g_oRXNetLogger;
void RXAPI RXNetSetLogger(ISDLogger* poLogger);
void RXAPI RXNetSetServerLogger(ISDLogger* poLogger);

#define RXNET_LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \	
	g_oRXNetLogger.Debug(buf);\
}

#define RXNET_LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [TORRENT] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oRXNetLogger.Critical(buf);\
}

#define RXNET_LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oRXNetLogger.Info(buf);\
}

#define RXNET_LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oRXNetLogger.Critical(buf);\
}

#define RXNET_LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oRXNetLogger.Warn(buf);\
}

#define RXNET_LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oRXNetLogger.Critical(buf);\
}

#define RXNET_LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oRXNetLogger.Info(buf);\
}
#else

#ifdef WIN32
#include "windows.h"
#include "rxtype.h"
#include "rxlogger.h"
#include "rxnetutils.h"
using namespace RX;
using namespace RX::Fundation;
#ifdef RXNET_EXPORTS
#define RXNET_API __declspec(dllexport)
#else
#define RXNET_API __declspec(dllexport)
#endif

#define RXAPI __stdcall
void RXNET_API RXNetSetLogger(CRXLogger* poLogger);

extern CRXLogger* m_poRXNetLogger;

#define RXNET_LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [TORRENT] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}
#endif


#if defined(__APPLE__) && defined(__MACH__)
#include "rxtype.h"
#include "rxlogger.h"
#include "rxnetutils.h"

#ifdef RXNET_EXPORTS
#define RXNET_API 
#else
#define RXNET_API 
#endif

#define RXAPI 
void RXNET_API RXNetSetLogger(CRXLogger* poLogger);

extern CRXLogger* m_poRXNetLogger;

#define RXNET_LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [TORRENT] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}

#define RXNET_LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXNetLogger)                           \
		m_poRXNetLogger->Log(buf);\
}
#endif

#if defined(LINUX)
#include "gntype.h"
#include "gnlogger.h"
using namespace GNDP;
#define RXAPI 
#define RXNET_API 
extern CSDLogger g_oNetLogger;
void RXAPI GNNetSetLogger(ISDLogger* poLogger);
void RXAPI RXNetSetServerLogger(ISDLogger* poLogger);

#define RXNET_LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Debug(buf);\
}

#define RXNET_LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [TORRENT] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Critical(buf);\
}

#define RXNET_LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Info(buf);\
}

#define RXNET_LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Critical(buf);\
}

#define RXNET_LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Warn(buf);\
}

#define RXNET_LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Critical(buf);\
}

#define RXNET_LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXNET] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	g_oNetLogger.Info(buf);\
}

#endif
#endif

#define MAX_CLIENTS			100
#define ENET_BAND_WIDTH     1024 * 1024 * 32

#define MAX_CHANNEL_NUM     2
enum EP2PWorkModel
{
	P2P_WORK_MODEL_SERVER,
	P2P_WORK_MODEL_CLIENT,
};

enum EP2PSessionType
{
	P2P_SESSION_TYPE_NONE = 0,
	P2P_SESSION_TYPE_SERVER,
	P2P_SESSION_TYPE_MAIN,
	P2P_SESSION_TYPE_PEER_PASSIVE,
	P2P_SESSION_TYPE_PEER_ACTIVE, 
	P2P_SESSION_TYPE_SERVER_DATA_CHANNEL,
	P2P_SESSION_TYPE_MAIN_DATA_CHANNEL,
	P2P_SESSION_TYPE_PEER_DATA_CHANNEL_ACTIVE,
	P2P_SESSION_TYPE_PEER_DATA_CHANNEL_PASSIVE,
};

enum ETerminateType
{
	TERMINATE_TYPE_CONNECT = 0,  //  ����ʧ��
	TERMINATE_TYPE_LOST,   // ���ӶϿ�
	TERMINATE_TYPE_DISCONNECT,   // �������ӶϿ�
	TERMINATE_TYPE_SHUTDOWN
};



enum ERXNetCode
{
	RXNET_ERROR_NONE = 0,
	RXNET_ERROR_CONNECT_TRY_TIME,
	RXNET_ERROR_CONNECT_TIMEOUT,
	RXNET_ERROR_CONNECT_DIS,
};

enum EKCPStatus
{
	KCP_STATUS_NONE = -1,
	KCP_STATUS_START_CONNECTING = 0, //  ��������״̬
	KCP_STATUS_START_CONNECTED,      //  �յ�����������״̬���ȴ��Է�Ӧ��
	KCP_STATUS_CONNECTED,            //  ���ӽ���״̬
};
class IRXPacketParser
{
public:
	virtual INT32 ParsePacket(const char* pBuf, UINT32 dwLen) = 0;
};
class IP2PSession;
class IP2PConnection
{
public:
	virtual int32_t		Send(const char* pszData, const uint32_t& dwLen) = 0;
	virtual uint64_t	GetKey() = 0;
	virtual uint32_t	GetNetworkDelay() = 0;
	virtual bool        IsConnected() = 0;
	virtual void        Disconnect() = 0;
	virtual const char* GetName() = 0;
#ifdef WIN32
	virtual IP2PSession* GetP2PSession() = 0;
#else
	virtual IP2PSession* GetP2PSession() = 0;
#endif
	
};

#define MAX_P2P_CONNECT_TIME       3

enum ETerminateResult
{
	P2P_TERMINATE_RESULT_NONE = 0,  //  ��������
	P2P_TERMINATE_RESULT_RECONNECT,  // ��Ҫ����
};

class IP2PSession
{
public:
	virtual void  SetType(const EP2PSessionType& enType) = 0;
	virtual EP2PSessionType GetType() = 0;
	virtual void  OnEstablish(IP2PConnection* poConnection,const uint32_t& dwCurConnectTime) = 0;
	virtual ETerminateResult OnTerminate(const ETerminateType& enType) = 0;
	virtual void  OnRecvData(const char* pszData, const uint32_t& dwLen) = 0;
	virtual uint64_t GetSessionID() = 0;
	virtual void  SetConnection(IP2PConnection* poConnection) = 0;	
	virtual void  Release() = 0;
};
class ITCPClient;
class ITCPSession
{
public:
	virtual void SetConnection(ITCPClient* poConnection) = 0;
	virtual void OnEstablish(void) =0 ;
	virtual void OnTerminate(void) = 0;
	virtual bool OnError(INT32 nModuleErr, INT32 nSysErr) = 0;
	virtual void OnRecv(const char* pBuf, UINT32 dwLen) = 0;
	virtual void OnSend(const char* pBuf, UINT32 dwLen) = 0;
	virtual void Release(void) = 0;
	virtual void OnHeart() = 0;
};

class IP2PSessionFactory
{
public:
	virtual IP2PSession* CreateSession(const uint64_t& llSessionID)	= 0;
};

class IP2PServer
{
public:
	virtual void SetFactory(IP2PSessionFactory* poFactory) = 0;
	virtual bool StartServer(const char* pszIP,const uint16_t& wPort) = 0;
	virtual int32_t SendTo(const uint64_t& llSessionID, const char* pszData, const uint32_t& dwLen) = 0;
	virtual void Disconnect(const uint64_t& llSessionID) = 0;
	virtual void StopServer() = 0;
	virtual void RunService() = 0;
};


class ITCPClient
{
public:
	virtual void  SetPacketParser(IRXPacketParser* poParser) = 0;
	virtual void  SetBufferSize(const uint32_t& dwSendBuffSize, const uint32_t& dwRecvBuffSize) = 0;
	virtual int32_t  Connect(const char* pszIP, const uint16_t& wPort) = 0;
	virtual void  SetSession(ITCPSession* poSession) = 0;
	virtual void  Disconnect() = 0;
	virtual int32_t  Send(const char* pszData, const uint32_t& dwLen) = 0;
	virtual bool  IsConnected() = 0;
	virtual const char* GetRemoteIP() = 0;
	virtual uint16_t GetRemotePort() = 0;
	virtual void  RunService() = 0;
	virtual void  Release() = 0;
};
class ITCPClientConnection;
class ITCPClientSession
{
public:
	virtual void	SetConnection(ITCPClientConnection* poConnection) = 0;
	virtual void	OnEstablish(void) = 0;
	virtual void	OnTerminate(void) = 0;;
	virtual bool	OnError(INT32 nModuleErr, INT32 nSysErr) = 0;
	virtual void	OnRecv(const char* pBuf, UINT32 dwLen) = 0;
	virtual void	OnSend(const char* pBuf, UINT32 dwLen) = 0;
	virtual void	Release(void) = 0;;
	
};
class ITCPClientConnection
{
public:
	virtual int32_t Send(const char* pszData, const uint32_t& dwLen,const uint32_t& dwReliable = 1) = 0;
	virtual void	Release() = 0;
	virtual void	Disconnect() = 0;
	virtual const char*  GetRemoteIPStr() = 0;
	virtual uint16_t  GetRemotePort() = 0;
	virtual bool	IsConnected() = 0;
};
class ITCPSessionFactory
{
public:
	virtual ITCPClientSession* CreateSession(ITCPClientConnection* poConnection) = 0;	
};

class ITCPServer
{
public: 
	virtual bool	Start(const char* pszIP,const uint32_t& dwPort) = 0;
	virtual void	Stop() = 0;
	virtual void	SetPacketParser(IRXPacketParser* poParser) = 0;
	virtual void	SetSessionFactory(ITCPSessionFactory* poFactory) = 0;
	virtual void	SetBufferSize(const uint32_t& dwSendBuffSize, const uint32_t& dwRecvBuffSize) = 0;
	virtual void	RunService() = 0;
	virtual void	Release() = 0;
};


class IRXNetModel
{
public:
	virtual bool Start(const char* pszName) = 0;
	virtual void Stop() = 0;
	virtual void SetPacketParser(IRXPacketParser* poParser) = 0;
	virtual void SetFactory(IP2PSessionFactory* poFactory) = 0;
	virtual bool ConnectServer(const char* pszIP, const uint16_t& wPort) = 0;
	virtual bool ConnectPeer(const uint64_t& llKey) = 0;
	virtual bool Stun(const char* pszName) = 0;	
	virtual int32_t SendTo(const uint64_t& llSessionID, const char* pszData, const uint32_t& dwLen) = 0;
	virtual uint64_t GetLocalKey() = 0;
	virtual bool IsReady() = 0;
	virtual void RunService() = 0;	
	virtual void PrepareBindRequest(const uint64_t& llKey) = 0;
	virtual bool CreateDataChannel(const char* pszName) = 0;
};


IP2PServer* RXAPI RXNetCreateServer();


RXNET_API ITCPClient* RXAPI RXNetCreateTCPClient();

RXNET_API ITCPServer* RXAPI RXNetCreateTCPServer();
RXNET_API IRXNetModel* RXAPI RXNetCreateModel();

RXNET_API string RXAPI RXNetGetLocalIP();
RXNET_API void  RXAPI RXNetSleep(int  time);

RXNET_API void RXAPI RXNetExcuteUpnp(const uint32_t& dwLocalPort,const uint32_t& dwOutPort,const char* pszOutIP);


#endif


