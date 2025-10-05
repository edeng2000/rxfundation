#include "rxdir.h"
#include "rxfile.h"
#include "rxstring.h"
#include "rxutil.h"
#include "rxfilemapping.h"
#include "type.h"
#include <vector>
#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#include <errno.h>
#pragma warning(disable : 4996)
#else
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
using namespace std;
namespace RX
{

    CRXDirectory::CRXDirectory()
    {
#if (defined(WIN32) || defined(WIN64))
        m_szPath    = _RXT("");
        m_fHandle   = INVALID_HANDLE_VALUE;
#else
        m_dir       = NULL;
        memset(&m_curDir, 0, sizeof(m_curDir));
#endif

    }

    CRXDirectory::~CRXDirectory()
    {

    }

    BOOL CRXDirectory::Open( const TCHAR* path )
    {
#if (defined(WIN32) || defined(WIN64))
        m_szPath = path;
        m_szPath += _RXT("\\*");
        m_fHandle = INVALID_HANDLE_VALUE;
        CHAR szOldPathDir[1024] = {0};
        getcwd(szOldPathDir,1024);
        std::string strPath = _RXTT2A(path);
        if(0 != chdir(strPath.c_str()))
        {
            return FALSE;
        }
        chdir(szOldPathDir);
#else
		SDStrncpy(m_curDir, _SDTT2A(path), sizeof(m_curDir)); 
        m_dir = opendir(_SDTT2A(path));
        if (m_dir == NULL)
        {
            return false;
        }
#endif

        return TRUE;
    }

    void CRXDirectory::Close()
    {
#if (defined(WIN32) || defined(WIN64))
        if (m_fHandle != INVALID_HANDLE_VALUE)
        {
            FindClose(m_fHandle);
            m_fHandle = INVALID_HANDLE_VALUE;
        }
#else
        if (m_dir != NULL)
        {
            closedir(m_dir);
            m_dir = NULL;
        }
#endif
    }

    BOOL CRXDirectory::Read( TCHAR* name, INT32 buflen, SFileAttr* pAttr)
    {
#if (defined(WIN32) || defined(WIN64))
        WIN32_FIND_DATAA find_data;
        if(m_fHandle == INVALID_HANDLE_VALUE)
        {
            m_fHandle = FindFirstFileA(_RXTT2A(m_szPath.c_str()).c_str(), &find_data);
            if (m_fHandle == INVALID_HANDLE_VALUE)
            {
                return FALSE;
            }
        }
        else
        {
            if (FindNextFileA(m_fHandle, &find_data) == 0)
            {
                return FALSE;
            }
        }

        if (strcmp(find_data.cFileName, ".") == 0 ||
                strcmp(find_data.cFileName, "..") == 0 )
        {
            return Read(name, buflen, pAttr);
        }

		_RXTsnprintf(name, buflen, _RXT("%s"), _RXTLocal2T(find_data.cFileName));

        if (pAttr != NULL)
        {
            pAttr->isDir = (find_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
        }

#else
        if (m_dir == NULL)
        {
            return FALSE;
        }

        struct dirent* pdir = readdir(m_dir);
        if (pdir == NULL)
        {
            return FALSE;
        }

        if (strcmp(pdir->d_name, ".") == 0 ||
                strcmp(pdir->d_name, "..") == 0 )
        {
            return Read(name, buflen, pAttr);
        }

		_RXTsnprintf(name, buflen, _RXT("%s"), _SDTA2T(pdir->d_name));

        if (pAttr != NULL )
        {
			CHAR szFullPath[SD_MAX_PATH]; 
			snprintf(szFullPath, buflen, ("%s/%s"), m_curDir, (pdir->d_name));

            struct stat statbuf;
            if (lstat(szFullPath, &statbuf) < 0 )
            {
                return FALSE;
            }

            pAttr->isDir = S_ISDIR(statbuf.st_mode);
        }

#endif

        return TRUE;
    }

    BOOL  RXRenameDirectory(const TCHAR* pszDirName, const TCHAR* pszNewDirName)
    {
#if (defined(WIN32) || defined(WIN64))
        MoveFile(pszDirName, pszNewDirName);
#else
        rename(_RXTT2UTF8(pszDirName).c_str(), _RXTT2UTF8(pszNewDirName).c_str());
#endif
        return TRUE;
    }


    BOOL  RXCreateDirectory(const TCHAR *dirName, BOOL bforce)
    {

#if (defined(WIN32) || defined(WIN64))
        //Nonzero if successful; otherwise 0
        int ret = 0;
        if (bforce)
        {
            vector<tstring> vec = _RXTSplitStrings(dirName, _T('\\'));
            tstring strCurDir = _T("");
            for (uint32_t t=0;t<(uint32_t)vec.size();t++)
            {
                strCurDir += vec[t];
                strCurDir += _T('\\');
                CreateDirectory(strCurDir.c_str(), NULL);
            }
            ret = TRUE;
        }
        else
        {
			ret = CreateDirectory(dirName, NULL);
			if (0 != ret)
			{
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
        }

#else
        //returns zero on success, or -1 if an error occurred
        int ret = mkdir(_RXTT2A(dirName).c_str(), S_IREAD | S_IWRITE);
        if(0 == ret)
        {
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
#endif
        if (!bforce)
        {
            return ret;
        }

        if (FALSE == ret)
        {
            if(bforce)
            {
                tstring strPath = dirName;
                strPath = replace_all(strPath, _T("/"), _T("\\"));
                std::vector<tstring> vecPath = _RXTSplitStrings(strPath, _T('\\'));
                if (vecPath.size()>=2)
                {
                    tstring strTruePath = vecPath[0] + _T("\\");
                    for (uint32 t = 1; t <(uint32)vecPath.size();t ++)
                    {
                        strTruePath += vecPath[t];
                        CreateDirectory(strTruePath.c_str(), NULL);
                        strTruePath += _T("\\");
                    }
                }
            }
            else//bforce == false
            {
                return FALSE;
            }
        }
        return TRUE;
    }

    BOOL  RXDeleteDirectory( const TCHAR *dirName, BOOL bforce/* = false*/ )
    {
        if(NULL == dirName)
        {
            return FALSE;
        }
        if(!bforce)
        {
#if (defined(WIN32) || defined(WIN64))
            return _rmdir(_RXTT2A(dirName).c_str()) == 0;
#else
            return rmdir(_SDTT2A(dirName)) == 0;
#endif
        }
        else
        {
            vector<tstring> vecAll;
            RXGetAllFiles((TCHAR*)dirName, vecAll);
            for (uint32 t=0;t<(uint32)vecAll.size();t++)
            {
                if (FALSE == RXFileRemove(vecAll[t].c_str()))
                {
                    return FALSE;
                }
            }

            vecAll.clear();
            RXGetAllDirectory((TCHAR*)dirName, vecAll);
            while (vecAll.size()>1)
            {
				for (uint32 t = 0; t < (uint32)vecAll.size(); t++)
				{
					if (RXIsEmptyDirectory(vecAll[t].c_str()))
					{
#if (defined(WIN32) || defined(WIN64))
						return _rmdir(_RXTT2A(vecAll[t].c_str()).c_str()) == 0;
#else
						return rmdir(_RXTT2A(vecAll[t].c_str()).c_str()) == 0;
#endif
					}
				}
				vecAll.clear();
				RXGetAllDirectory((TCHAR*)dirName, vecAll);
            }
     
            CRXDirectory dir;
            if(dir.Open(dirName))
            {
                TCHAR _filename[RX_MAX_PATH] = {0};
                SFileAttr attr = {0};
                while(dir.Read(_filename, RX_MAX_PATH, &attr))
                {
                    TCHAR _subfilepath[RX_MAX_PATH] = {0};
					_RXTStrncpy(_subfilepath, dirName, RX_MAX_PATH);
                    _RXTStrcat(_subfilepath, _RXT("\\") );
                    _RXTStrcat(_subfilepath, _filename );
                    if(attr.isDir)
                    {

                        if(!RXDeleteDirectory(_subfilepath, bforce))
                        {
                            return FALSE;
                        }
                    }
                    else
                    {
#if (defined(WIN32) || defined(WIN64))
                        if(_unlink(_RXTT2A(_subfilepath).c_str()) != 0)
#else
                        if(unlink(_RXTT2A(_subfilepath).c_str()) != 0)
#endif
                        {
                            return FALSE;
                        }
                    }
                }
                dir.Close();
#if (defined(WIN32) || defined(WIN64))
                return _rmdir(_RXTT2A(dirName).c_str()) == 0;
#else
                return rmdir(_RXTT2A(dirName).c_str()) == 0;
#endif
            }
            return FALSE;
        }
    }

    BOOL  RXDirectoryExists(const TCHAR *dirName)
    {
#if (defined(WIN32) || defined(WIN64))
        int nCode = GetFileAttributesA(_RXTT2Local(dirName).c_str());
        if (nCode == -1)
        {
            return false;
        }
        return FILE_ATTRIBUTE_DIRECTORY & nCode;
#else
        struct stat fileStat;
        int ret = stat(_RXTT2A(dirName).c_str(), &fileStat);
        if ( ret == 0)
        {
            return S_ISDIR(fileStat.st_mode);
        }
        return FALSE;
#endif
    }

    BOOL  RXIsDirectory(const TCHAR * dirName)
    {
#if (defined(WIN32) || defined(WIN64))
        int nCode = GetFileAttributesA(_RXTT2A(dirName).c_str());
        if (nCode == -1)
        {
            return false;
        }
        return FILE_ATTRIBUTE_DIRECTORY & nCode;
#else
        struct stat fileStat;
        int ret = stat(_RXTT2A(dirName).c_str(), &fileStat);
        if ( ret == 0)
        {
            return S_ISDIR(fileStat.st_mode);
        }
        return FALSE;
#endif
    }

    BOOL  RXIsEmptyDirectory(const TCHAR* dirName)
    {
        if (RXIsDirectory(dirName))
        {
#if (defined(WIN32) || defined(WIN64))
            HANDLE hFind = INVALID_HANDLE_VALUE;

            tstring dirFile = RXFileIncludeTrailingDelimiter(dirName);

            BOOL isEmpty = TRUE;
            dirFile+= _RXT("*");
            WIN32_FIND_DATA find_data;
            hFind = FindFirstFile(dirFile.c_str(), &find_data);
            if (hFind)
            {
                while( FindNextFile(hFind,&find_data))
                {
                    if (_tcscmp(find_data.cFileName,_RXT(".")) == 0 || _tcscmp(find_data.cFileName,_RXT("..")) == 0)
                    {
                        continue;
                    }
                    isEmpty = FALSE;
                    break;
                }

                FindClose(hFind);

                return isEmpty;
            }
#else
			DIR * pFd = opendir(_SDTT2A(dirName));
			if (pFd == NULL)
			{
				return FALSE; 
			}

			struct dirent* pdir = readdir(pFd);
			if (pdir == NULL)
			{
				closedir(pFd); 
				return FALSE;
			}

			while (pdir != NULL)
			{
				if (strcmp(pdir->d_name, ".") == 0 ||
					strcmp(pdir->d_name, "..") == 0 )
				{
					pdir = readdir(pFd); 
					continue; 
				}
				else 
				{
					closedir(pFd); 
					return FALSE; 
				}
			}
			
			closedir(pFd); 
#endif // 
        }
        return TRUE;
    }
	void RXGetAllDirectory(TCHAR* lpPath, std::vector<tstring>& fileList)
	{
		TCHAR szFind[MAX_PATH] = { 0 };
		WIN32_FIND_DATA FindFileData;
		_RXTStrcpy(szFind, lpPath);
		_RXTStrcat(szFind, _T("\\*.*"));
		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)  return;
		while (true)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (FindFileData.cFileName[0] != _T('.'))
				{
					TCHAR szFile[MAX_PATH] = { 0 };
					_RXTStrcpy(szFile, lpPath);
					_RXTStrcat(szFile, _T("\\"));
					_RXTStrcat(szFile, FindFileData.cFileName);
					fileList.push_back(tstring(szFile));
                    RXGetAllDirectory(szFile, fileList);
				}
			}
			if (!FindNextFile(hFind, &FindFileData))  break;
		}
		FindClose(hFind);
	}
	void RXGetAllFiles(TCHAR* lpPath, std::vector<tstring> &fileList)
	{
        TCHAR szFind[MAX_PATH] = {0};
		WIN32_FIND_DATA FindFileData;
		_RXTStrcpy(szFind, lpPath);
		_RXTStrcat(szFind, _T("\\*.*"));
		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)  return;
		while (true)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (FindFileData.cFileName[0] != _T('.'))
				{
                    TCHAR szFile[MAX_PATH] = {0};
                    _RXTStrcpy(szFile, lpPath);
                    _RXTStrcat(szFile, _T("\\"));
                    _RXTStrcat(szFile, FindFileData.cFileName);
                    RXGetAllFiles(szFile, fileList);
				}
			}
			else
			{
                if (FindFileData.nFileSizeHigh>0||FindFileData.nFileSizeLow>0)
                {
					TCHAR szFile[MAX_PATH] = { 0 };
					_RXTsprintf(szFile, _T("%s\\%s"), lpPath, FindFileData.cFileName);
					fileList.push_back(tstring(szFile));
                }

			}
			if (!FindNextFile(hFind, &FindFileData))  break;
		}
		FindClose(hFind);
	}
    void RXGetAllFilesEx(const TCHAR* lpPath, const TCHAR* pszExt, std::vector<tstring>& fileList)
    {
		TCHAR szFind[MAX_PATH] = { 0 };
		WIN32_FIND_DATA FindFileData;
		_RXTStrcpy(szFind, lpPath);
		_RXTStrcat(szFind, _T("\\*.*"));
		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)  return;
		while (true)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (FindFileData.cFileName[0] != _T('.'))
				{
					TCHAR szFile[MAX_PATH] = { 0 };
					_RXTStrcpy(szFile, lpPath);
					_RXTStrcat(szFile, _T("\\"));
					_RXTStrcat(szFile, FindFileData.cFileName);
					RXGetAllFilesEx(szFile, pszExt, fileList);
				}
			}
			else
			{
				TCHAR szFile[MAX_PATH] = { 0 };
				_RXTsprintf(szFile, _T("%s\\%s"), lpPath, FindFileData.cFileName);
                tstring strExt = RXFileExtractExt(szFile);
                if (_RXTStrcmp(strExt.c_str(),pszExt) == 0)
    				fileList.push_back(tstring(szFile));
			}
			if (!FindNextFile(hFind, &FindFileData))  break;
		}
		FindClose(hFind);
    }

	void RXGetAllFileName(const TCHAR* lpPath, std::vector<tstring>& fileList)
	{
		TCHAR szFind[MAX_PATH] = { 0 };
		WIN32_FIND_DATA FindFileData;
		_RXTStrcpy(szFind, lpPath);
		_RXTStrcat(szFind, _T("\\*.*"));
		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)  return;
		while (true)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (FindFileData.cFileName[0] != _T('.'))
				{
					TCHAR szFile[MAX_PATH] = { 0 };
					_RXTStrcpy(szFile, lpPath);
					_RXTStrcat(szFile, _T("\\"));
					_RXTStrcat(szFile, FindFileData.cFileName);
                    RXGetAllFileName(szFile, fileList);
				}
			}
			else
			{
				fileList.push_back(tstring(FindFileData.cFileName));
			}
			if (!FindNextFile(hFind, &FindFileData))  break;
		}
		FindClose(hFind);
	}
	SFileInfo::SFileInfo()
	{
		bIsFile = false;
		memset(szFullPath, 0, sizeof(TCHAR) * MAX_PATH);
		memset(szName, 0, sizeof(TCHAR) * MAX_PATH);
	}
    SFileInfo::SFileInfo(const SFileInfo& oInfo)
	{
		bIsFile = oInfo.bIsFile;
		memset(szFullPath, 0, sizeof(TCHAR) * MAX_PATH);
		memset(szName, 0, sizeof(TCHAR) * MAX_PATH);
		llTime = oInfo.llTime;
		_RXTStrcpy(szName, oInfo.szName);
		_RXTStrcpy(szFullPath, oInfo.szFullPath);
	}

#define EPOCH_DIFF 116444736000000000
#define RATE_DIFF 10000000
#define TIME_DIFF 8*60*60
	static uint64_t FileTime2TimeStamp(FILETIME const& filetime)
	{
		ULARGE_INTEGER    ularge;
		ularge.LowPart = filetime.dwLowDateTime;
		ularge.HighPart = filetime.dwHighDateTime;
		return (ularge.QuadPart - EPOCH_DIFF) / RATE_DIFF - TIME_DIFF;
	}

	void RXGetCurDirAllFileName(const TCHAR* lpPath, std::vector<SFileInfo*>& fileList, std::vector<SFileInfo*>& dirList)
	{
		TCHAR szFind[MAX_PATH] = { 0 };
		WIN32_FIND_DATA FindFileData;
		_RXTStrcpy(szFind, lpPath);
		_RXTStrcat(szFind, _T("*.*"));
		HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
		if (INVALID_HANDLE_VALUE == hFind)  return;
		while (true)
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (FindFileData.cFileName[0] != _T('.'))
				{
					TCHAR szFile[MAX_PATH] = { 0 };
					_RXTStrcpy(szFile, lpPath);
					_RXTStrcat(szFile, FindFileData.cFileName);
                    SFileInfo* poInfo = (SFileInfo*)malloc(sizeof(SFileInfo));
                    memset(poInfo, 0, sizeof(SFileInfo));
                    poInfo->bIsFile = false;					
                    _RXTStrcpy(poInfo->szFullPath, szFile);
                    _RXTStrcpy(poInfo->szName, FindFileData.cFileName);
                    poInfo->llTime = (FindFileData.ftLastWriteTime);
                    dirList.push_back(poInfo);
				}
			}
			else
			{
				TCHAR szFile[MAX_PATH] = { 0 };
				_RXTStrcpy(szFile, lpPath);
				_RXTStrcat(szFile, FindFileData.cFileName);
				SFileInfo* poInfo = (SFileInfo*)malloc(sizeof(SFileInfo));
				memset(poInfo, 0, sizeof(SFileInfo));
				poInfo->bIsFile = TRUE;
				
				_RXTStrcpy(poInfo->szFullPath, szFile);
				_RXTStrcpy(poInfo->szName, FindFileData.cFileName);
				poInfo->llTime = (FindFileData.ftLastWriteTime);
				fileList.push_back(poInfo);
			}
			if (!FindNextFile(hFind, &FindFileData))  break;
		}
		FindClose(hFind);
	}

    tstring RXGetUpDirectory(const TCHAR* pszDir)
    {
        tstring strResult = _T("");
		vector<tstring> vec = _RXTSplitStrings(pszDir, _T('\\'));
        for (vector<tstring>::iterator itr = vec.begin();itr!=vec.end();)
        {
            if ((*itr).size() <= 0)
            {
                itr = vec.erase(itr);
            }
            else if (_RXTStrcmp((*itr).c_str(), _T("/")) == 0)
            {
                itr = vec.erase(itr);
            }
            else
                itr++;
        }

        if (vec.size()>1)
        {
            for (uint32_t t = 0; t < (uint32_t)vec.size() - 1; t++)
            {
                strResult += vec[t];
                strResult += _T('\\');
            }
        }
        else
        {
            strResult = pszDir;
        }
        return strResult;
    }


	tstring RXGetUpDirectoryForMac(const TCHAR* pszDir)
	{
		tstring strResult = _T("/");
		vector<tstring> vec = _RXTSplitStrings(pszDir, _T('/'));
		for (vector<tstring>::iterator itr = vec.begin(); itr != vec.end();)
		{
			if ((*itr).size() <= 0)
			{
				itr = vec.erase(itr);
			}
			else if (_RXTStrcmp((*itr).c_str(), _T("/")) == 0)
			{
				itr = vec.erase(itr);
			}
			else
				itr++;
		}

		if (vec.size() > 1)
		{
			for (uint32_t t = 0; t < (uint32_t)vec.size() - 1; t++)
			{
				strResult += vec[t];
				strResult += _T('/');
			}
		}
		else
		{
			strResult = pszDir;
		}
		return strResult;
	}
} // namespace GNDP

