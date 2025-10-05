#include "fundation/rxutil.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include "tchar.h"
#include <vector>
#include <shellapi.h>
#include "localcharset.h"
#ifdef WIN32
#include "fundation/StringConvert.h"
#endif
#define SDPAGE                (1024*1024)

#if (defined(WIN32) || defined(WIN64))
#define UNICODE_CHARSET      "UCS-2LE"
#else
#define UNICODE_CHARSET      "UCS-4LE"
#endif

#define LOCAL_CHARSET        "gb2312"

#define ASCII_CHARSET             "ASCII"
#define UTF8_CHARSET             "UTF-8"

 const TCHAR* GetModulePath(void)
{
	static TCHAR szPath[MAX_PATH];
	static bool bFirstTime = true;
	if (bFirstTime)
	{
		bFirstTime = false;
		GetModuleFileName(NULL, (TCHAR*)szPath, sizeof(szPath));
		TCHAR* p = _tcsrchr(szPath, _T('\\'));
		*p = _T('\0');
		_tcscat(szPath, _T("\\"));
	}
	return szPath;
}

 tstring& replace_all(tstring& str, const tstring& old_value, const tstring& new_value)
{
	while (true) {

#ifndef _UNICODE
		std::string::size_type pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else break;
#else
		std::wstring::size_type pos(0);
		if ((pos = str.find(old_value)) != std::wstring::npos)
			str.replace(pos, old_value.length(), new_value);
		else break;
#endif
	}
	return str;
}

 std::string replace_allA(std::string& str, const std::string& old_value, const std::string& new_value)
 {
	 size_t start_pos = 0;
	 while (true) {
		 std::string::size_type pos(0);
		 if ((pos = str.find(old_value, start_pos)) != std::string::npos)
		 {
			 str.replace(pos, old_value.length(), new_value);
			 start_pos = pos + new_value.size();
		 }
		 else break;
	 }
	 return str;
 }

bool FileExist(std::string strFile)
{
	FILE* hFile = fopen(strFile.c_str(), "r");
	if (hFile)
	{
		fclose(hFile);
		return true;
	}
	return false;
}

uint64_t FileSize(std::string strFile)
{
	uint64_t llSize = 0;
	FILE* hFile = fopen(strFile.c_str(), "rb");
	if (hFile)
	{
		fseek(hFile, 0, SEEK_END);
		llSize = ftell(hFile);
		fclose(hFile);
	}
	return llSize;
}

std::string GetDataTimeString(long long llTime)
{
	char szData[256] = {0};
	long long milli = llTime + (long long)8 * 60 * 60 * 1000;//此处转化为东八区北京时间，如果是其它时区需要按需求修改
	auto mTime = std::chrono::milliseconds(milli);
	auto tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(mTime);
	auto tt = std::chrono::system_clock::to_time_t(tp);
	std::tm* now = std::gmtime(&tt);
	sprintf(szData,"%4d-%02d-%02d %02d:%02d:%02d\n", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
	return szData;
}



#define EPOCH_1970_1601 116444736000000000ULL
static uint64_t  SDGetSystemTime()
{	
	uint64_t time = 0;
#if (defined(WIN32) || defined(WIN64))
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	ULARGE_INTEGER li;
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	time = (li.QuadPart - EPOCH_1970_1601) / 10;
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	//assert( now );
	time = now.tv_sec;
	time = time * 1000000;
	time += now.tv_usec;
#endif
	return time;
}

std::wstring Utf82Unicode(const std::string& utf8string)
{
	WCHAR* pszDesc = (WCHAR*)malloc(SDPAGE * sizeof(WCHAR));
	memset(pszDesc, 0, sizeof(WCHAR) * SDPAGE);
	size_t nSize = SDPAGE * sizeof(WCHAR);
	RXIconvCovert((CHAR*)UNICODE_CHARSET, (CHAR*)UTF8_CHARSET, (CHAR*)utf8string.c_str(), utf8string.size(), (CHAR*)pszDesc, nSize);
	std::wstring strResult = pszDesc;
	free(pszDesc);
	return strResult;
}

std::string WideByte2Acsi(const std::wstring& wstrcode)
{
	CHAR* pszDesc = (CHAR*)malloc(SDPAGE * sizeof(CHAR));
	memset(pszDesc, 0, sizeof(CHAR) * SDPAGE);
	size_t nSize = SDPAGE * sizeof(CHAR);
	RXIconvCovert((CHAR*)LOCAL_CHARSET,
		(CHAR*)UNICODE_CHARSET,
		(CHAR*)wstrcode.c_str(),
		wstrcode.size() * sizeof(TCHAR),
		(CHAR*)pszDesc,
		nSize);
	std::string strResult = pszDesc;
	free(pszDesc);
	return strResult;
}



std::string UTF_82ASCII(const std::string& strUtf8Code)
{
	CHAR* pszDesc = (CHAR*)malloc(SDPAGE * sizeof(CHAR));
	memset(pszDesc, 0, sizeof(CHAR) * SDPAGE);
	size_t nSize = SDPAGE * sizeof(CHAR);
	RXIconvCovert((CHAR*)LOCAL_CHARSET, (CHAR*)UTF8_CHARSET, (CHAR*)strUtf8Code.c_str(), strUtf8Code.size(), (CHAR*)pszDesc, nSize);
	std::string strResult = pszDesc;
	free(pszDesc);
	return strResult;
}


std::wstring Acsi2WideByte(const std::string& strascii)
{
	WCHAR* pszDesc = (WCHAR*)malloc(SDPAGE * sizeof(WCHAR));
	memset(pszDesc, 0, sizeof(WCHAR) * SDPAGE);
	size_t nSize = SDPAGE * sizeof(WCHAR);
	RXIconvCovert((CHAR*)UNICODE_CHARSET, (CHAR*)LOCAL_CHARSET, (CHAR*)strascii.c_str(), strascii.size(), (CHAR*)pszDesc, nSize);
	std::wstring strResult = pszDesc;
	free(pszDesc);
	return strResult;
}

std::string Unicode2Utf8(const std::wstring& widestring)
{
	CHAR* pszDesc = (CHAR*)malloc(SDPAGE * sizeof(CHAR));
	memset(pszDesc, 0, sizeof(CHAR) * SDPAGE);
	size_t nSize = SDPAGE * sizeof(CHAR);
	RXIconvCovert((CHAR*)UTF8_CHARSET, (CHAR*)UNICODE_CHARSET, (CHAR*)widestring.c_str(), widestring.size() * sizeof(TCHAR), (CHAR*)pszDesc, nSize);
	std::string strResult = pszDesc;
	free(pszDesc);
	return strResult;
}

std::string ASCII2UTF_8(const std::string& strAsciiCode)
{
	CHAR* pszDesc = (CHAR*)malloc(SDPAGE * sizeof(CHAR));
	memset(pszDesc, 0, sizeof(CHAR) * SDPAGE);
	size_t nSize = SDPAGE * sizeof(CHAR);
	RXIconvCovert((CHAR*)UTF8_CHARSET, (CHAR*)LOCAL_CHARSET, (CHAR*)strAsciiCode.c_str(), strAsciiCode.size(), (CHAR*)pszDesc, nSize);
	std::string strResult = pszDesc;
	free(pszDesc);
	return strResult;
}


#define ID_MEDIA_TYPE_NONE                        -1
#define ID_MEDIA_TYPE_OTHER_ICON                   60062
#define ID_MEDIA_TYPE_AAC_ICON                     60033
#define ID_MEDIA_TYPE_AC3_ICON                     60034
#define ID_MEDIA_TYPE_AIFF_ICON                    60035
#define ID_MEDIA_TYPE_ALAC_ICON                    60036
#define ID_MEDIA_TYPE_AMR_ICON                     60037
#define ID_MEDIA_TYPE_APE_ICON                     60038
#define ID_MEDIA_TYPE_AU_ICON                      60039
#define ID_MEDIA_TYPE_AVI_ICON                     60040
#define ID_MEDIA_TYPE_BINK_ICON                    60041
#define ID_MEDIA_TYPE_CDA_ICON                     60042
#define ID_MEDIA_TYPE_DSM_ICON                     60043
#define ID_MEDIA_TYPE_DTS_ICON                     60044
#define ID_MEDIA_TYPE_FLAC_ICON                    60045
#define ID_MEDIA_TYPE_FLIC_ICON                    60046
#define ID_MEDIA_TYPE_FLV_ICON                     60047
#define ID_MEDIA_TYPE_IFO_ICON                     60048
#define ID_MEDIA_TYPE_IVF_ICON                     60049
#define ID_MEDIA_TYPE_MID_MEDIA_TYPE_ICON                    60050
#define ID_MEDIA_TYPE_MKA_ICON                     60051
#define ID_MEDIA_TYPE_MKV_ICON                     60052
#define ID_MEDIA_TYPE_MOV_ICON                     60053
#define ID_MEDIA_TYPE_MP3_ICON                     60054
#define ID_MEDIA_TYPE_MP4_ICON                     60055
#define ID_MEDIA_TYPE_MPA_ICON                     60056
#define ID_MEDIA_TYPE_MPC_ICON                     60057
#define ID_MEDIA_TYPE_MPEG_ICON                    60058
#define ID_MEDIA_TYPE_OFR_ICON                     60059
#define ID_MEDIA_TYPE_OGG_ICON                     60060
#define ID_MEDIA_TYPE_OGM_ICON                     60061
#define ID_MEDIA_TYPE_PLAYLIST_ICON                60063
#define ID_MEDIA_TYPE_RA_ICON                      60064
#define ID_MEDIA_TYPE_RM_ICON                      60065
#define ID_MEDIA_TYPE_RT_ICON                      60066
#define ID_MEDIA_TYPE_SMK_ICON                     60067
#define ID_MEDIA_TYPE_SWF_ICON                     60068
#define ID_MEDIA_TYPE_TS_ICON                      60069
#define ID_MEDIA_TYPE_TTA_ICON                     60070
#define ID_MEDIA_TYPE_VOB_ICON                     60071
#define ID_MEDIA_TYPE_WAV_ICON                     60072
#define ID_MEDIA_TYPE_WEBM_ICON                    60073
#define ID_MEDIA_TYPE_WMA_ICON                     60074
#define ID_MEDIA_TYPE_WMV_ICON                     60075
#define ID_MEDIA_TYPE_WV_ICON                      60076


int GetIconIndex(LPCTSTR ext)
{
	int iconIndex = ID_MEDIA_TYPE_NONE;

	if (_tcsicmp(ext, _T(".3g2")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MOV_ICON;
	}
	else if (_tcsicmp(ext, _T(".3ga")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".3gp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".3gp2")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MOV_ICON;
	}
	else if (_tcsicmp(ext, _T(".3gpp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".aac")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AAC_ICON;
	}
	else if (_tcsicmp(ext, _T(".ac3")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AC3_ICON;
	}
	else if (_tcsicmp(ext, _T(".aif")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AIFF_ICON;
	}
	else if (_tcsicmp(ext, _T(".aifc")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AIFF_ICON;
	}
	else if (_tcsicmp(ext, _T(".aiff")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AIFF_ICON;
	}
	else if (_tcsicmp(ext, _T(".alac")) == 0) {
		iconIndex = ID_MEDIA_TYPE_ALAC_ICON;
	}
	else if (_tcsicmp(ext, _T(".amr")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AMR_ICON;
	}
	else if (_tcsicmp(ext, _T(".amv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".aob")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".ape")) == 0) {
		iconIndex = ID_MEDIA_TYPE_APE_ICON;
	}
	else if (_tcsicmp(ext, _T(".apl")) == 0) {
		iconIndex = ID_MEDIA_TYPE_APE_ICON;
	}
	else if (_tcsicmp(ext, _T(".asf")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WMV_ICON;
	}
	else if (_tcsicmp(ext, _T(".asx")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".au")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AU_ICON;
	}
	else if (_tcsicmp(ext, _T(".avi")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AVI_ICON;
	}
	else if (_tcsicmp(ext, _T(".bdmv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".bik")) == 0) {
		iconIndex = ID_MEDIA_TYPE_BINK_ICON;
	}
	else if (_tcsicmp(ext, _T(".cda")) == 0) {
		iconIndex = ID_MEDIA_TYPE_CDA_ICON;
	}
	else if (_tcsicmp(ext, _T(".divx")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".dsa")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DSM_ICON;
	}
	else if (_tcsicmp(ext, _T(".dsm")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DSM_ICON;
	}
	else if (_tcsicmp(ext, _T(".dss")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DSM_ICON;
	}
	else if (_tcsicmp(ext, _T(".dsv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DSM_ICON;
	}
	else if (_tcsicmp(ext, _T(".dts")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DTS_ICON;
	}
	else if (_tcsicmp(ext, _T(".dtshd")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DTS_ICON;
	}
	else if (_tcsicmp(ext, _T(".dtsma")) == 0) {
		iconIndex = ID_MEDIA_TYPE_DTS_ICON;
	}
	else if (_tcsicmp(ext, _T(".evo")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".f4v")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLV_ICON;
	}
	else if (_tcsicmp(ext, _T(".flac")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLAC_ICON;
	}
	else if (_tcsicmp(ext, _T(".flc")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLIC_ICON;
	}
	else if (_tcsicmp(ext, _T(".fli")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLIC_ICON;
	}
	else if (_tcsicmp(ext, _T(".flic")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLIC_ICON;
	}
	else if (_tcsicmp(ext, _T(".flv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLV_ICON;
	}
	else if (_tcsicmp(ext, _T(".hdmov")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".iflv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_FLV_ICON;
	}
	else if (_tcsicmp(ext, _T(".ifo")) == 0) {
		iconIndex = ID_MEDIA_TYPE_IFO_ICON;
	}
	else if (_tcsicmp(ext, _T(".ivf")) == 0) {
		iconIndex = ID_MEDIA_TYPE_IVF_ICON;
	}
	else if (_tcsicmp(ext, _T(".m1a")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPA_ICON;
	}
	else if (_tcsicmp(ext, _T(".m1v")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".m2a")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPA_ICON;
	}
	else if (_tcsicmp(ext, _T(".m2p")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".m2t")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".m2ts")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".m2v")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".m3u")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".m3u8")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".m4a")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AAC_ICON;
	}
	else if (_tcsicmp(ext, _T(".m4b")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AAC_ICON;
	}
	else if (_tcsicmp(ext, _T(".m4r")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AAC_ICON;
	}
	else if (_tcsicmp(ext, _T(".m4v")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".mid")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MID_MEDIA_TYPE_ICON;
	}
	else if (_tcsicmp(ext, _T(".mID_MEDIA_TYPE")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MID_MEDIA_TYPE_ICON;
	}
	else if (_tcsicmp(ext, _T(".mka")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MKA_ICON;
	}
	else if (_tcsicmp(ext, _T(".mkv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MKV_ICON;
	}
	else if (_tcsicmp(ext, _T(".mlp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".mov")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MOV_ICON;
	}
	else if (_tcsicmp(ext, _T(".mp2")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPA_ICON;
	}
	else if (_tcsicmp(ext, _T(".mp2v")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".mp3")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP3_ICON;
	}
	else if (_tcsicmp(ext, _T(".mp4")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".mp4v")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpa")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPA_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpc")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPC_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpcpl")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpe")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpeg")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpg")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpls")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpv2")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".mpv4")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MP4_ICON;
	}
	else if (_tcsicmp(ext, _T(".mts")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".ofr")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OFR_ICON;
	}
	else if (_tcsicmp(ext, _T(".ofs")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OFR_ICON;
	}
	else if (_tcsicmp(ext, _T(".oga")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OGG_ICON;
	}
	else if (_tcsicmp(ext, _T(".ogg")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OGG_ICON;
	}
	else if (_tcsicmp(ext, _T(".ogm")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OGM_ICON;
	}
	else if (_tcsicmp(ext, _T(".ogv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OGM_ICON;
	}
	else if (_tcsicmp(ext, _T(".opus")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".pls")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".pva")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MPEG_ICON;
	}
	else if (_tcsicmp(ext, _T(".ra")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RA_ICON;
	}
	else if (_tcsicmp(ext, _T(".ram")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RM_ICON;
	}
	else if (_tcsicmp(ext, _T(".rm")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RM_ICON;
	}
	else if (_tcsicmp(ext, _T(".rmi")) == 0) {
		iconIndex = ID_MEDIA_TYPE_MID_MEDIA_TYPE_ICON;
	}
	else if (_tcsicmp(ext, _T(".rmm")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RM_ICON;
	}
	else if (_tcsicmp(ext, _T(".rmvb")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".rp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RT_ICON;
	}
	else if (_tcsicmp(ext, _T(".rt")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RT_ICON;
	}
	else if (_tcsicmp(ext, _T(".smil")) == 0) {
		iconIndex = ID_MEDIA_TYPE_RT_ICON;
	}
	else if (_tcsicmp(ext, _T(".smk")) == 0) {
		iconIndex = ID_MEDIA_TYPE_SMK_ICON;
	}
	else if (_tcsicmp(ext, _T(".snd")) == 0) {
		iconIndex = ID_MEDIA_TYPE_AU_ICON;
	}
	else if (_tcsicmp(ext, _T(".swf")) == 0) {
		iconIndex = ID_MEDIA_TYPE_SWF_ICON;
	}
	else if (_tcsicmp(ext, _T(".tp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".trp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".ts")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".rec")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TS_ICON;
	}
	else if (_tcsicmp(ext, _T(".tak")) == 0) {
		iconIndex = ID_MEDIA_TYPE_OTHER_ICON;
	}
	else if (_tcsicmp(ext, _T(".tta")) == 0) {
		iconIndex = ID_MEDIA_TYPE_TTA_ICON;
	}
	else if (_tcsicmp(ext, _T(".vob")) == 0) {
		iconIndex = ID_MEDIA_TYPE_VOB_ICON;
	}
	else if (_tcsicmp(ext, _T(".wav")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WAV_ICON;
	}
	else if (_tcsicmp(ext, _T(".wax")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".webm")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WEBM_ICON;
	}
	else if (_tcsicmp(ext, _T(".wm")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WMV_ICON;
	}
	else if (_tcsicmp(ext, _T(".wma")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WMA_ICON;
	}
	else if (_tcsicmp(ext, _T(".wmp")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WMV_ICON;
	}
	else if (_tcsicmp(ext, _T(".wmv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WMV_ICON;
	}
	else if (_tcsicmp(ext, _T(".wmx")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}
	else if (_tcsicmp(ext, _T(".wv")) == 0) {
		iconIndex = ID_MEDIA_TYPE_WV_ICON;
	}
	else if (_tcsicmp(ext, _T(".wvx")) == 0) {
		iconIndex = ID_MEDIA_TYPE_PLAYLIST_ICON;
	}

	return iconIndex;
}


tstring GetFileExt(const tstring& strFilePath)
{
	int pos = (int)strFilePath.rfind(_T('.'));
	if (pos >= 0)
	{
		return strFilePath.substr(pos, strFilePath.size() - pos);
	}
	return _T("");
}



CRXLock::CRXLock()
{
	::InitializeCriticalSection(&m_hLock);
}

CRXLock::~CRXLock()
{
	::DeleteCriticalSection(&m_hLock);
}

void CRXLock::Lock()
{
	::EnterCriticalSection(&m_hLock);
}

void CRXLock::Unlock()
{
	::LeaveCriticalSection(&m_hLock);
}

CRXAutoLock::CRXAutoLock(CRXLock& oLock):m_oLock(oLock)
{
	m_oLock = oLock;
	m_oLock.Lock();
}

CRXAutoLock::~CRXAutoLock()
{
	m_oLock.Unlock();
}

CVideoDataBuff::CVideoDataBuff()
{
	m_dwImageSize = 0;
	m_dwHeight = 0;
	m_dwWidth = 0;
	m_pszData = NULL;
	m_llLocalTime = 0;
	m_llTime = 0;
}

CVideoDataBuff::~CVideoDataBuff()
{
	m_dwImageSize = 0;
	m_dwHeight = 0;
	m_dwWidth = 0;
	if (m_pszData)
	{
		free(m_pszData);
		m_pszData = NULL;
	}
	m_pszData = NULL;
}

void CVideoDataBuff::PushData(const char* pszData,
	const uint32_t& dwWidth,
	const uint32_t& dwHeight,
	const uint32_t& dwImageSize)
{
	if (m_dwImageSize != dwImageSize)
	{
		if (m_pszData)
		{
			free(m_pszData);
			m_pszData = NULL;
		}
		m_pszData = (char*)malloc(dwImageSize);
	}
	if (NULL == m_pszData)
	{
		m_pszData = (char*)malloc(dwImageSize);
	}
	memcpy(m_pszData, pszData, dwImageSize);
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_dwImageSize = dwImageSize;
}

void CVideoDataBuff::Alloc(const uint32_t& dwImageSize)
{
	if (m_dwImageSize != dwImageSize)
	{
		if (m_pszData)
		{
			free(m_pszData);
			m_pszData = NULL;
		}
		m_pszData = (char*)malloc(dwImageSize);
	}
	m_dwImageSize = dwImageSize;
}

std::wstring GetTimeByValue(const uint64_t& llValue)
{
	tstring strResult;
	uint64_t llSecond = (uint64_t)(llValue / 1000);
	if (llSecond < 60)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld 秒"), llSecond);
		strResult = szData;
	}
	else if (llSecond < 60 * 60)
	{
		uint64_t llMinute = llSecond / 60;
		uint64_t llLessSecond = llSecond % 60;
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld分钟%02lld秒"), llMinute, llLessSecond);
		strResult = szData;
	}
	else if (llSecond < 60 * 60 * 24)
	{

		uint64_t dwHour = llSecond / (60 * 60);
		uint64_t dwLessSecond = llSecond % (60 * 60);
		uint64_t dwMinute = dwLessSecond / 60;
		dwLessSecond = dwLessSecond % 60;

		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld小时%02lld分钟%02lld秒"), dwHour, dwMinute, dwLessSecond);
		strResult = szData;
	}
	else
	{
		uint64_t dwDay = llSecond / (60 * 60 * 24);
		uint64_t dwSecond = llSecond % (60 * 60 * 24);
		uint64_t dwHour = dwSecond / (60 * 60);
		uint64_t dwLessSecond = dwSecond % (60 * 60);
		uint64_t dwMinute = dwLessSecond / 60;
		dwLessSecond = dwLessSecond % 60;

		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld天%02lld小时%02lld分钟%02lld秒"), dwDay, dwHour, dwMinute, dwLessSecond);
		strResult = szData;
	}
	return strResult;
}
tstring  GetTextByValue(const uint64_t& llValue)
{
	tstring strResult;
	if (llValue >= 1024 * 1024 * 1024)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fGB"), (double)llValue / (double)(1024 * 1024 * 1024));
		strResult = szData;
	}
	else if (llValue >= 1024 * 1024)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fMB"), (double)llValue / (double)(1024 * 1024));
		strResult = szData;
	}
	else if (llValue >= 1024)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fKB"), (double)llValue / (double)(1024));
		strResult = szData;
	}
	else
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fB"), (double)llValue);
		strResult = szData;
	}
	return strResult;
}

void RXSetAutoRunator(const TCHAR* pszName,const TCHAR* pszPath)
{
	HKEY hKey = NULL;
	DWORD rc;
	rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WOW64_64KEY | KEY_ALL_ACCESS,
		NULL,
		&hKey,
		NULL);
	if (rc == ERROR_SUCCESS)
	{
		rc = RegSetValueEx(hKey,
			pszName,
			0,
			REG_SZ,
			(const BYTE*)pszPath,
			_RXTStrlen(pszPath)*sizeof(TCHAR));
		if (rc == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
		}
	}
}

void RXCancelAutoRunator(const TCHAR* pszName, const TCHAR* pszPath)
{
	HKEY hKey = NULL;
	DWORD rc;
	rc = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WOW64_64KEY | KEY_ALL_ACCESS,
		NULL,
		&hKey,
		NULL);
	if (rc == ERROR_SUCCESS)
	{
		rc = RegDeleteKeyValue(hKey,
			pszName,
			(const TCHAR*)pszPath);
		if (rc == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
		}
	}
}

std::wstring RXGetTextBySize(const uint64_t& llValue)
{
	tstring strResult;
	if (llValue >= 1024 * 1024 * 1024)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fGB"), (double)llValue / (double)(1024 * 1024 * 1024));
		strResult = szData;
	}
	else if (llValue >= 1024 * 1024)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fMB"), (double)llValue / (double)(1024 * 1024));
		strResult = szData;
	}
	else if (llValue >= 1024)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fKB"), (double)llValue / (double)(1024));
		strResult = szData;
	}
	else
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%0.2fB"), (double)llValue);
		strResult = szData;
	}
	return strResult;
}

std::wstring RXGetTimeByValue(const uint64_t& llValue)
{
	tstring strResult;
	uint64_t llSecond = (uint64_t)(llValue / 1000);
	if (llSecond < 60)
	{
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld 秒"), llSecond);
		strResult = szData;
	}
	else if (llSecond < 60 * 60)
	{
		uint64_t llMinute = llSecond / 60;
		uint64_t llLessSecond = llSecond % 60;
		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld分钟%02lld秒"), llMinute, llLessSecond);
		strResult = szData;
	}
	else if (llSecond < 60 * 60 * 24)
	{

		uint64_t dwHour = llSecond / (60 * 60);
		uint64_t dwLessSecond = llSecond % (60 * 60);
		uint64_t dwMinute = dwLessSecond / 60;
		dwLessSecond = dwLessSecond % 60;

		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld小时%02lld分钟%02lld秒"), dwHour, dwMinute, dwLessSecond);
		strResult = szData;
	}
	else
	{
		uint64_t dwDay = llSecond / (60 * 60 * 24);
		uint64_t dwSecond = llSecond % (60 * 60 * 24);
		uint64_t dwHour = dwSecond / (60 * 60);
		uint64_t dwLessSecond = dwSecond % (60 * 60);
		uint64_t dwMinute = dwLessSecond / 60;
		dwLessSecond = dwLessSecond % 60;

		TCHAR szData[256] = { 0 };
		_RXTsprintf(szData, _T("%02lld天%02lld小时%02lld分钟%02lld秒"), dwDay, dwHour, dwMinute, dwLessSecond);
		strResult = szData;
	}
	return strResult;
}

#include <windows.h>
#include <iostream>
#include <string>

std::string GetDefaultBrowser() {
	HKEY hKey;
	char szData[1024];
	DWORD dwSize = sizeof(szData) - 1;
	std::string strBrowser;

	// 打开注册表项
	if (RegOpenKeyExA(HKEY_CLASSES_ROOT, "http\\shell\\open\\command", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		// 读取默认值
		if (RegQueryValueExA(hKey, NULL, NULL, NULL, (LPBYTE)szData, &dwSize) == ERROR_SUCCESS) {
			szData[dwSize] = '\0'; // 确保字符串以null终止
			// 查找引号内的路径，即为浏览器路径
			char* pos = strchr(szData, '"');
			if (pos) {
				pos++; // 跳过引号
				char* endPos = strchr(pos, '"');
				if (endPos) {
					*endPos = '\0'; // 截断字符串在引号处
				}
				strBrowser = pos; // 浏览器路径
			}
		}
		RegCloseKey(hKey); // 关闭注册表项
	}
	else {
		std::cerr << "无法打开注册表项" << std::endl;
	}
	return strBrowser;
}


string GetEdgeBrowser()
{
	HKEY hKey;
	LONG result;
	DWORD dwType = 0;
	char szData[1024] = {0}; // 缓冲区用于存储查询到的数据
	DWORD dwSize = sizeof(szData); // 缓冲区大小
	string strResult = "";
	result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msedge.exe"), 0, KEY_READ , &hKey);
	if (result == ERROR_SUCCESS) {
		result = RegQueryValueExA(hKey, NULL, NULL, NULL, (LPBYTE)szData, &dwSize);
		if (result == ERROR_SUCCESS) {
			std::cout << "Path: " << szData << std::endl;
			strResult = szData;
		}
		else {
			std::cout << "Failed to query value." << std::endl;
		}
		RegCloseKey(hKey);
	}
	else {
		std::cout << "Failed to open registry key." << std::endl;
	}

	return strResult;
}

string GetIEBrowser()
{
	HKEY hKey;
	LONG result;
	DWORD dwType = 0;
	char szData[1024] = {0}; // 缓冲区用于存储查询到的数据
	DWORD dwSize = sizeof(szData); // 缓冲区大小
	string strResult = "";
	result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Internet Explorer\\Main"), 0, KEY_READ , &hKey);
	if (result == ERROR_SUCCESS) {
		result = RegQueryValueExA(hKey, "x86AppPath", NULL, NULL, (LPBYTE)szData, &dwSize);
		if (result == ERROR_SUCCESS) {
			std::cout << "Path: " << szData << std::endl;
			strResult = szData;
		}
		else {
			std::cout << "Failed to query value." << std::endl;
		}
		RegCloseKey(hKey);
	}
	else {
		std::cout << "Failed to open registry key." << std::endl;
	}

	return strResult;
}

void OpenUrl(const TCHAR* pszUrl, const bool& bIsIE)
{		
	string command = GetEdgeBrowser();
	if(command.size()<=0)
		command = GetDefaultBrowser();
	if (bIsIE)
	{
		command = GetIEBrowser();
	}
	HINSTANCE hInst = ShellExecute(NULL,_T("open"), RXUTF82W(command.c_str()).c_str(), pszUrl, NULL, SW_SHOW);
	if (NULL == hInst)
	{
		hInst = ShellExecute(NULL, _T("open"),_T("explore.exe"), pszUrl, NULL, SW_SHOW);
		if(NULL == hInst)
			ShellExecute(NULL, _T("open"), _T("chromeX.exe"), pszUrl, NULL, SW_SHOW);
	}
}