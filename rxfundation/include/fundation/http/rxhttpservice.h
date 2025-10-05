#pragma once
#include "fundation/rxthread.h"
#include "fundation/rxsingleton.h"
#include "mkmutex.h"
#include <string.h>
#include <string>
#include <vector>
using namespace std;

enum EHttpTaskType
{
	HTTP_TASK_TYPE_NONE = -1,
	HTTP_TASK_TYPE_DOWNLOAD = 0,
	HTTP_TASK_TYPE_UPLOAD,
};
class IHttpResult
{
public:
	virtual void OnResult(const EHttpTaskType& enType, const uint32_t& dwID, WPARAM wParam, LPARAM lParam) = 0;
};
class IHttpTask
{
public:
	virtual void OnExcuting() = 0;
	virtual void OnExcuted() = 0;
	virtual void OnRelease() = 0;
	virtual EHttpTaskType GetType() = 0;
	virtual void SetResult(IHttpResult* poResult) = 0;
private:
};



class CDownLoadHttpTask : public IHttpTask
{
public:
	CDownLoadHttpTask ();
	~CDownLoadHttpTask ();
	virtual void OnExcuting();
	virtual void OnExcuted();
	virtual void OnRelease();
	void  SetMsgID(const uint32_t& dwMsgID);
	void  SetName(const char* pszName);
	void  SetUrl(const char* pszUrl);
	void  SetPath(const char* pszPath);
	virtual void SetResult(IHttpResult* poResult)
	{
		m_poResult = poResult;
	}
	virtual EHttpTaskType GetType() { return HTTP_TASK_TYPE_DOWNLOAD; }
private:
	string m_strUrl;
	string m_strLocalPath;
	string m_strName;
	uint32_t m_dwMsgID;
	IHttpResult* m_poResult;
private:
	static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid);

};

class CUploadHttpTask : public IHttpTask
{
public:
	CUploadHttpTask();
	~CUploadHttpTask();
	virtual void OnExcuting();
	virtual void OnExcuted();
	virtual void OnRelease();
	virtual EHttpTaskType GetType() { return HTTP_TASK_TYPE_UPLOAD; }
	void  SetMsgID(const uint32_t& dwMsgID);
	void  SetName(const char* pszName);
	void  SetUrl(const char* pszUrl);
	void  SetPath(const char* pszPath);
	virtual void SetResult(IHttpResult* poResult)
	{
		m_poResult = poResult;
	}
private:
	string m_strUrl;
	string m_strLocalPath;
	string m_strName;
	uint32_t m_dwMsgID;
	IHttpResult* m_poResult;
private:
	static size_t OnReadData(void* ptr, size_t size, size_t nmemb, void* stream);
};


class CRXHttpService : public CRXThreadEx
{
	RX_DECLARE_SINGLETON(CRXHttpService)
public:
	CRXHttpService();
	~CRXHttpService();
	virtual VOID Run();
	virtual bool StartThread();
	virtual void StopThread();
	void  SetResult(IHttpResult* poResult) 
	{
		m_poResult = poResult; 
	}
	IHttpResult* GetResult() { return m_poResult; }
	void  AddTask(IHttpTask* poTask);
	static string GetUrl(const char* pszUrl);
	static string GetUrls(const char* pszUrl);
	static int32_t GetUrlInfo(const std::string& url, std::string& response);
	static size_t OnGetUrlData(void* ptr, size_t size, size_t nmemb, void* stream);
private:
	GNDP::CLocker m_oLock;
	vector<IHttpTask*> m_vecTask;
	IHttpResult* m_poResult;
private:
	void  _DeleteAll();
	IHttpTask* _PopTask();
};
