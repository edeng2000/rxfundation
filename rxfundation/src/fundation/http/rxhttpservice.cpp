#include "fundation/http/rxhttpservice.h"
#include "curl/curl.h"
#include "fundation/rxfile.h"
#include "fundation/rxdir.h"
#include "fundation/rxtime.h"
#include "fundation/rxlogger.h"
#include "fundation/rxstring.h"
#include "fundation/http/httpresponseparser.h"
#include "fundation/http/response.h"
CDownLoadHttpTask::CDownLoadHttpTask()
{
	m_strName.clear();
	m_dwMsgID = 0;
	m_strLocalPath.clear();
	m_strUrl.clear();
}

CDownLoadHttpTask ::~CDownLoadHttpTask()
{
	m_poResult = NULL;
}

size_t CDownLoadHttpTask::OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	FILE* hFile = dynamic_cast<FILE*>((FILE*)lpVoid);
	if (NULL == hFile || NULL == buffer)
	{
		return -1;
	}

	char* pData = (char*)buffer;
	fwrite(buffer, size * nmemb, 1, hFile);	
	return nmemb;
}

void CDownLoadHttpTask::OnExcuted()
{

}

void CDownLoadHttpTask::OnExcuting()
{
	string strResponse;
	int32_t nRet = CRXHttpService::GetUrlInfo(m_strUrl.c_str(), strResponse);
	if (strResponse.size()<=0)
	{
		return;
	}
	httpparser::HttpResponseParser oParser;
	httpparser::Response reponse;
	oParser.parse(reponse, strResponse.c_str(), &strResponse[strResponse.size() - 1]);
	if (reponse.statusCode != 200)
	{
		return;
	}
	CURL* curl;
	CURLcode res;
	string strResult = "";
	curl = curl_easy_init();
	if (curl) {
		string strName = "";
		string::size_type pos = string(m_strUrl).rfind('/');
		if (pos == tstring::npos)
		{
			strName = m_strUrl;
		}
		strName =  string(m_strUrl).substr(pos + 1);
		if (m_strLocalPath.size()>0)
		{
			if (m_strLocalPath[m_strLocalPath.size()-1]!='\\')
			{
				m_strLocalPath += "\\";
			}
		}
		m_strLocalPath += strName;
		DeleteFileA(m_strLocalPath.c_str());
		FILE* hFile = fopen(m_strLocalPath.c_str(), "wb");
		if (hFile==NULL)
		{
			return;
		}

		curl_easy_setopt(curl, CURLOPT_URL, m_strUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)hFile);
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {
			if (hFile)
			{
				fclose(hFile);
				hFile = NULL;
			}
			char* pszPath = (char*)malloc(MAX_PATH);
			memset(pszPath, 0, MAX_PATH);
			strcpy(pszPath, m_strLocalPath.c_str());
			char* pszName = (char*)malloc(MAX_PATH);
			memset(pszName, 0, MAX_PATH);
			strcpy(pszName, m_strName.c_str());
			if (m_poResult)
			{
				m_poResult->OnResult(GetType(), m_dwMsgID, (WPARAM)pszName, (LPARAM)pszPath);
			}
		}
		/* always cleanup */
		curl_easy_cleanup(curl);
		if (hFile)
		{
			fclose(hFile);
		}
	}
}

void CDownLoadHttpTask::OnRelease()
{
	delete this;
}

void CDownLoadHttpTask::SetName(const char* pszName)
{
	m_strName = pszName;
}

void CDownLoadHttpTask::SetMsgID(const uint32_t& dwMsgID)
{
	m_dwMsgID = dwMsgID;
}

void CDownLoadHttpTask::SetPath(const char* pszPath)
{
	m_strLocalPath = pszPath;
}

void CDownLoadHttpTask::SetUrl(const char* pszUrl)
{
	m_strUrl = pszUrl;
}

CUploadHttpTask::CUploadHttpTask()
{
	m_strName.clear();
	m_dwMsgID = 0;
	m_strLocalPath.clear();
	m_strUrl.clear();
}

CUploadHttpTask ::~CUploadHttpTask()
{
	m_poResult = NULL;
}

void CUploadHttpTask::OnExcuted()
{

}

void CUploadHttpTask::OnExcuting()
{
	FILE* hFile = fopen(m_strLocalPath.c_str(), "rb");
	if (!hFile)
		return;
	struct stat file_info;
	string strResponse;
	int32_t ret = CRXHttpService::GetUrlInfo(m_strUrl, strResponse);
	if (strResponse.size()>0)
	{
		httpparser::HttpResponseParser oParser;
		httpparser::Response reponse;
		oParser.parse(reponse, strResponse.c_str(), &strResponse[strResponse.size() - 1]);
		
		if (fstat(fileno(hFile), &file_info) != 0)
			return;
		if (file_info.st_size == oParser.content_size())
		{
			fclose(hFile);
			hFile = NULL;
			return;
		}
	}
	CURL* poCurl = curl_easy_init();
	if (poCurl) {
		curl_easy_setopt(poCurl, CURLOPT_URL,	m_strUrl.c_str());
		curl_easy_setopt(poCurl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(poCurl, CURLOPT_READDATA, hFile);
		curl_easy_setopt(poCurl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)file_info.st_size);
		curl_easy_setopt(poCurl, CURLOPT_VERBOSE, 1L);
		CURLcode res = curl_easy_perform(poCurl);
		if (res != CURLE_OK) {
			LOG_ERR("curl_easy_perform() failed: %s",	curl_easy_strerror(res));
			fclose(hFile);
			hFile = NULL;
			string strResponse;
			int32_t nRet = CRXHttpService::GetUrlInfo(m_strUrl.c_str(), strResponse);
			if (strResponse.size() > 0)
			{
				httpparser::HttpResponseParser oParser;
				httpparser::Response reponse;
				oParser.parse(reponse, strResponse.c_str(), &strResponse[strResponse.size() - 1]);
				if (reponse.statusCode == 200)
				{
					if (oParser.content_size()==file_info.st_size)
					{
						char* pszPath = (char*)malloc(MAX_PATH);
						memset(pszPath, 0, MAX_PATH);
						strcpy(pszPath, m_strLocalPath.c_str());
						char* pszName = (char*)malloc(MAX_PATH);
						memset(pszName, 0, MAX_PATH);
						strcpy(pszName, m_strName.c_str());
						if (m_poResult)
						{
							m_poResult->OnResult(GetType(), m_dwMsgID, (WPARAM)pszName, (LPARAM)pszPath);
						}				
					}
				}
			}
		}
		else {
			curl_off_t speed_upload, total_time;
			curl_easy_getinfo(poCurl, CURLINFO_SPEED_UPLOAD_T, &speed_upload);
			curl_easy_getinfo(poCurl, CURLINFO_TOTAL_TIME_T, &total_time);
			LOG_INFO("Speed: % lu bytes / sec during % lu. % 06lu seconds",
				(unsigned long)speed_upload,
				(unsigned long)(total_time / 1000000),
				(unsigned long)(total_time % 1000000));
			fclose(hFile);
			hFile = NULL;

		}
		curl_easy_cleanup(poCurl);
	}
}

void CUploadHttpTask::OnRelease()
{
	delete this;
}

void CUploadHttpTask::SetName(const char* pszName)
{
	m_strName = pszName;
}

void CUploadHttpTask::SetMsgID(const uint32_t& dwMsgID)
{
	m_dwMsgID = dwMsgID;
}

void CUploadHttpTask::SetPath(const char* pszPath)
{
	m_strLocalPath = pszPath;
}

void CUploadHttpTask::SetUrl(const char* pszUrl)
{
	m_strUrl = pszUrl;
}

size_t CUploadHttpTask::OnReadData(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t retcode;
	curl_off_t nread;
	retcode = fread(ptr, size, nmemb, (FILE*)stream);
	nread = (curl_off_t)retcode;
	return retcode;
}


RX_IMPLEMENT_SINGLETON(CRXHttpService)
CRXHttpService::CRXHttpService()
{
	m_poResult = NULL;
}

CRXHttpService::~CRXHttpService()
{

}

void CRXHttpService::Run()
{
	while (IsRunning())
	{
		IHttpTask* poTask = _PopTask();
		if (poTask)
		{
			poTask->OnExcuting();
			poTask->OnExcuted();
			poTask->OnRelease();
		}
		RXSleep(10);
	}
}

bool CRXHttpService::StartThread()
{
	return CRXThreadEx::StartThread();
}

void CRXHttpService::StopThread()
{
	CRXThreadEx::StopThread();
	_DeleteAll();
}

size_t CRXHttpService::OnGetUrlData(void* ptr, size_t size, size_t nmemb, void* stream)
{
	string* str = (string*)stream;
	(*str).append((char*)ptr, size * nmemb);
	return size * nmemb;
}

void CRXHttpService::AddTask(IHttpTask* poTask)
{
	m_oLock.Lock();
	m_vecTask.push_back(poTask);
	m_oLock.Unlock();
}

IHttpTask* CRXHttpService::_PopTask()
{
	IHttpTask* poTask = NULL;
	m_oLock.Lock();
	if (m_vecTask.size()>0)
	{
		vector<IHttpTask*>::iterator itr = m_vecTask.begin();
		poTask = *itr;
		m_vecTask.erase(itr);
	}
	m_oLock.Unlock();
	return poTask;
}


int32_t CRXHttpService::GetUrlInfo(const std::string& url, std::string& response)
{
	CURL* curl = curl_easy_init();
	CURLcode res;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  // 忽略SSL证书验证
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CRXHttpService::OnGetUrlData);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&response);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return res;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string*)lpVoid);
	if (NULL == str || NULL == buffer)
	{
		return -1;
	}

	char* pData = (char*)buffer;
	str->append(pData, size * nmemb);
	return nmemb;
}
string CRXHttpService::GetUrl(const char* pszUrl)
{
	CURL* curl;
	CURLcode res;
	string strResult = "";
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, pszUrl);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  // 忽略SSL证书验证
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResult);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (CURLE_OK == res) {

		}

	}
	return strResult;
}

string CRXHttpService::GetUrls(const char* pszUrl)
{
	const char* cert_path = "certificate.pem";
	const char* key_path = "private_key.pem";
	const char* ca_info = "ca_certificate.pem"; // 如果需要验证服务器的CA证书也可以指定此文件路径
	CURL* curl;
	CURLcode res;
	string strResult = "";
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, pszUrl);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strResult);
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, 1);
		//	curl_easy_setopt(curl, CURLOPT_SSLCERT, cert_path); // 设置客户端证书路径
		//	curl_easy_setopt(curl, CURLOPT_SSLKEY, key_path); // 设置客户端私钥路径
		//	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // 验证服务器的SSL证书（需要CA证书）
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		//		curl_easy_setopt(curl, CURLOPT_CAINFO, ca_info); // 设置CA证书路径，用于验证服务器证书（可选）
		res = curl_easy_perform(curl);
		if (CURLE_OK == res) {

		}
		LOG_INFO("Result:%d", res);
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	return strResult;
}

void CRXHttpService::_DeleteAll()
{
	m_oLock.Lock();
	for (uint32_t i = 0; i < (uint32_t)m_vecTask.size(); i++)
	{
		m_vecTask[i]->OnRelease();
	}
	m_vecTask.clear();
	m_oLock.Unlock();
}