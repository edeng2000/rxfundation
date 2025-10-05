#include "rxtype.h"
#include "rxfile.h"
#include "rxstring.h"
#include "rxdir.h"
#include "rxmath.h"
#include "rxfilemapping.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef> // 对于std::size_t
#include <sys/stat.h>
#if (defined(WIN32) || defined(WIN64))
#include <direct.h>
#include <io.h>
#pragma warning(disable : 4996)
#else
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

#define SDPATH_IS_DELIMITER(x)  (x == '\\' || x == '/')

namespace RX
{

    CRXFile::CRXFile()
        :m_pFileHandle(NULL), m_bFileMapping(FALSE)
    {
    }

    CRXFile::~CRXFile()
    {
        Close();
    }


    BOOL  CRXFile::Open(const TCHAR* szfilename, const TCHAR* type, BOOL bFileMapping)
    {
        m_filepath = szfilename;
        m_bFileMapping = bFileMapping;
        if (m_bFileMapping)
        {
            m_pFileHandle = new SFileMapping();
            return RXFileMapping(*(SFileMapping*)m_pFileHandle,szfilename);
        }
        else
        {
            m_pFileHandle = fopen(_RXTT2A(szfilename).c_str(), _RXTT2A(type).c_str());
        }
        return m_pFileHandle != NULL;
    }

    void  CRXFile::Close()
    {
        if (m_pFileHandle != NULL)
        {
            if (m_bFileMapping)
            {
                RXFileUnMapping(*(SFileMapping*)m_pFileHandle);
                delete (SFileMapping*)m_pFileHandle;
            }
            else
            {
                fclose((FILE*)m_pFileHandle);
            }
            m_pFileHandle = NULL;
        }
    }

    void  CRXFile::Flush()
    {
        if (m_pFileHandle != NULL)
        {
            if (m_bFileMapping)
            {
                SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
                RXFileFlushMapping(pFileMapping->mem, pFileMapping->size);
            }
            else
            {
                fflush((FILE*)m_pFileHandle);
            }
        }
    }

    UINT32  CRXFile::Read(void* pBuf, UINT32 nLen)
    {
        if (m_pFileHandle == NULL)
        {
            return 0;
        }

        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            UINT32 left = (UINT32)(pFileMapping->size -  ( (CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem));
            UINT32 readable =SDMin(left,nLen);
            memcpy(pBuf, pFileMapping->pos ,readable);
            pFileMapping->pos = (CHAR*) pFileMapping->pos +  readable;
            return readable;
        }
        else
        {
            return (UINT32)fread(pBuf, 1, nLen, (FILE*)m_pFileHandle);
        }

    }

    UINT32  CRXFile::Write(const void* pBuf, UINT32 nLen)
    {
        if (m_pFileHandle == NULL)
        {
            return false;
        }

        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            UINT32 left = (UINT32)(pFileMapping->size -  ( (CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem));
            UINT32 writeable =SDMin(left,nLen);
            memcpy(pFileMapping->pos,pBuf,writeable);
            pFileMapping->pos = (CHAR*) pFileMapping->pos +  writeable;
            return writeable;
        }
        else
        {
            return (UINT32)fwrite(pBuf, 1, nLen,(FILE*) m_pFileHandle);
        }
    }

    INT32  CRXFile::Seek( INT32 offset, CRXFile::SeekOffset whence)
    {
        if (m_pFileHandle == NULL)
        {
            return -1;
        }
        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            switch(whence)
            {
            case SK_SET:
                if ((UINT32)abs(offset) <= pFileMapping->size)
                {
                    pFileMapping->pos = (CHAR*)pFileMapping->mem +
                        (offset >0 ? offset : pFileMapping->size + offset);
                    return 0;
                }
                break;

            case SK_END:
                if ((UINT32)abs(offset) <= pFileMapping->size)
                {
                    pFileMapping->pos = (CHAR*)pFileMapping->mem +
                        (offset> 0 ?pFileMapping->size - offset: -offset);
                    return 0;
                }
                break;

            case SK_CUR:
                {
                    UINT32 curPos =  (UINT32)((CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem);
                    if ( (offset >=0  && offset <= (INT32)(pFileMapping->size - curPos))
                        || (offset < 0 && offset + curPos >=0 ))
                    {
                        pFileMapping->pos = (CHAR*)pFileMapping->pos + offset;
                        return 0;
                    }
                }
                break;
            default:
                ;
            }

            return -1;
        }
        else
        {
            return fseek((FILE*)m_pFileHandle, offset, whence);
        }
    }

    INT32  CRXFile::SeekToBegin()
    {
        return this->Seek(0, SK_SET);
    }

    INT32  CRXFile::SeekToEnd()
    {
        return this->Seek(0, SK_END);
    }

    LONG  CRXFile::GetPosition()
    {
        if (m_pFileHandle == NULL)
        {
            return -1;
        }
        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            return (LONG)((CHAR*)pFileMapping->pos - (CHAR*)pFileMapping->mem);
        }
        else
        {
            return ftell((FILE*)m_pFileHandle);
        }

    }

    void  CRXFile::SetLength(LONG newLen)
    {
        if (m_pFileHandle == NULL)
        {
            return;
        }
        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            RXFileUnMapping(*pFileMapping);
            RXFileMapping(*pFileMapping,m_filepath.c_str(),0,newLen);
        }
        else
        {

#if (defined(WIN32) || defined(WIN64))
            _chsize(_fileno((FILE*)m_pFileHandle), newLen);
#else
            ftruncate(fileno((FILE*)m_pFileHandle), newLen);
#endif
        }
    }

    BOOL  CRXFile::Eof()
    {
        if (m_pFileHandle == NULL)
        {
            return FALSE;
        }

        if (m_bFileMapping)
        {
            SFileMapping* pFileMapping = (SFileMapping*) m_pFileHandle;
            return pFileMapping->pos == ((CHAR*)pFileMapping->mem + pFileMapping->size);
        }
        else
        {
            LONG thispos = GetPosition();
            SeekToEnd();
            LONG endpos = GetPosition();
            if(thispos != endpos)
            {
                Seek(thispos, SK_SET);
                return FALSE;
            }
        }
        return TRUE;
    }

    BOOL  CRXFile::GetFileStatus(SFileStatus &fStatus)
    {
        if (m_pFileHandle == NULL)
        {
            return FALSE;
        }

        return  RXGetFileStatus(m_filepath.c_str(), fStatus);
    }

    BOOL  RXGetFileStatus( const TCHAR* szfilename, SFileStatus& fStatus )
    {
        if(NULL == szfilename)
        {
            return FALSE;
        }
#if (defined(WIN32) || defined(WIN64))
        struct _stat st;
        if (_tstat(szfilename, &st) != 0)
        {
            return FALSE;
        }
#else
        struct stat st;
        if (stat(_SDTT2A(szfilename), &st) < 0)
        {
            return FALSE;
        }
#endif

        fStatus.gid = st.st_gid;
        fStatus.atime = st.st_atime;
        fStatus.ctime = st.st_ctime;
        fStatus.dev = st.st_dev;
        fStatus.inode = st.st_ino;
        fStatus.mode = st.st_mode;
        fStatus.mtime = st.st_mtime;
        fStatus.nlink = st.st_nlink;
        fStatus.rdev = st.st_rdev;
        fStatus.size = st.st_size;
        fStatus.uid = st.st_uid;

        return TRUE;
    }

    BOOL  RXFileRemove( const TCHAR* szfilename )
    {
#if (defined(WIN32) || defined(WIN64))
        return _tunlink(szfilename) == 0;
#else
        return unlink(_RXTT2A(szfilename).c_str()) == 0;
#endif
    }

    INT32  RXFileRename( const TCHAR* oldname, const TCHAR* newname ,BOOL bForce)
    {
        if (bForce)
        {
            if (RXFileExists(newname))
            {
                RXFileRemove(newname);
            }
            return rename(_RXTT2A(oldname).c_str(), _RXTT2A(newname).c_str());
        }
        else
        {
            return rename(_RXTT2A(oldname).c_str(), _RXTT2A(newname).c_str());
        }
    }
    // 增加强制创建目标目录  是否覆盖目标文件 增加文档注释
    INT32  RXFileMove( const TCHAR* oldname, const TCHAR* newname ,BOOL bForce   )
    {
        return RXFileRename(oldname,newname,bForce);

        //TODO 实现文件跨目录拷贝,或移动.
        if ( RXIsDirectory(oldname))
        {
            UINT32 newNameLen = _RXTStrlen(newname);
            if (SDPATH_IS_DELIMITER(newname[newNameLen-1]))
            {

            }
            return RXFileRename(oldname,newname,bForce);
        }
        else
        {
            return RXFileRename(oldname,newname,bForce);
        }
    }

    INT32  RXFileAccess( const TCHAR *path, AccessMode mode )
    {
        if(NULL == path)
        {
            return -1;
        }
#if (defined(WIN32) || defined(WIN64))
        return _taccess(path, mode);
#else
        return access(_SDTT2A(path), mode);
#endif
    }


    const TCHAR*  RXGetModulePath()
    {
        static TCHAR szPath[RX_MAX_PATH] = {0};

        static bool bFirstTime = true;

        if (bFirstTime)
        {
#if (defined(WIN32) || defined(WIN64))
            bFirstTime = false;
            GetModuleFileName(NULL, (TCHAR*)szPath, sizeof(szPath));
            TCHAR *p = _tcsrchr(szPath, _RXT('\\'));
            *p = _RXT('\0');
            _tcscat(szPath, _RXT("\\"));
#else
            CHAR szTmp[SD_MAX_PATH];
            getcwd(szTmp,sizeof(szTmp));
            CHAR szTempPath[SD_MAX_PATH];
            sprintf(szTempPath,"%s//",szTmp);
            _RXTStrncpy(szPath,_RXTA2T(szTempPath).c_str(),SD_MAX_PATH);
#endif //
        }
        return szPath;
    }

	const TCHAR* RXGetModuleName()
	{
#if (defined(WIN32) || defined(WIN64))
		static TCHAR szPath[MAX_PATH];
		static bool bFirstTime = true;

		if (bFirstTime)
		{
			bFirstTime = false;
			GetModuleFileName(NULL, (TCHAR*)szPath, sizeof(szPath));
		}

		return szPath;
#else
		pid_t pid = getpid();
		static char strProcessPath[1024] = { 0 };
		static char strProcessName[256] = { 0 };
		static bool bFirstTime = true;
		if (bFirstTime)
		{
			bFirstTime = false;
			if (readlink("/proc/self/exe", strProcessPath, 1024) <= 0)
			{
				return strProcessName;
			}
			char* pszProcessName = strrchr(strProcessPath, '/');
			if (pszProcessName)
			{
				strncpy(strProcessName, pszProcessName + 1, 256);
			}
		}
        return strProcessName;
#endif
    }

    const TCHAR*  RXGetWorkPath()
    {
#if (defined(WIN32) || defined(WIN64))
        return _tgetcwd(NULL,0);
#else
        static CHAR workPath[SD_MAX_PATH];
        getcwd(workPath,sizeof(workPath));
        return _RXTA2T(workPath).c_str();
#endif
    }

    tstring   RXFileExtractPath(const TCHAR* strFileName)
    {
        if(NULL == strFileName)
        {
            return _RXT("");
        }
        tstring::size_type pos = tstring(strFileName).rfind('\\') ;
        if (pos == tstring::npos)
        {
            pos =  tstring(strFileName).rfind('/');
            if (pos == tstring::npos)
            {
                return _RXT("");
            }
        }

        return tstring(strFileName).substr(0, pos);
    }

    tstring   RXFileExtractName(const TCHAR* strFileName)
    {
        if(NULL == strFileName)
        {
            return _RXT("");
        }
        tstring::size_type pos = tstring(strFileName).rfind('\\');
        if (pos == tstring::npos)
        {
            return strFileName;
        }
        return tstring( strFileName).substr(pos + 1);
    }

    tstring   RXFileExtractExt(const TCHAR* strFileName)
    {
        if(NULL == strFileName)
        {
            return _RXT("");
        }
        tstring::size_type pos = tstring(strFileName).find_last_of(_RXT(":.\\"));
        if (pos == tstring::npos || strFileName[pos] != '.')
        {
            return _RXT("");
        }
        return tstring(strFileName).substr(pos + 1);
    }

    tstring   RXFileExcludeTrailingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _RXT("");
        }
        int len = _RXTStrlen(strPath);

        if (len == 0)
        {
            return _RXT("");
        }

        if (SDPATH_IS_DELIMITER(strPath[len - 1]))
        {
            return tstring(strPath).substr(0, len - 1);
        }
        else
        {
            return strPath;
        }
    }

    uint64_t RXFileSize(const TCHAR* pszFileName)
    {
        return RXFileGetSize(pszFileName);
    }

    tstring   RXFileExcludeLeadingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _RXT("");
        }
        size_t pos = 0;
        size_t len = _RXTStrlen(strPath);
        while (pos < len )
        {
            if (!SDPATH_IS_DELIMITER(strPath[pos]))
            {
                break;
            }
            pos++;
        }
        return tstring(strPath).substr(pos);
    }

    tstring  RXFileIncludeTrailingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _RXT("");
        }
        int len = _RXTStrlen(strPath);
        if (len == 0 )
        {
            return _RXT("");
        }
        if (!SDPATH_IS_DELIMITER(strPath[len - 1]))
        {
            return tstring(strPath) + _RXT("\\");
        }
        else
        {
            return strPath;
        }
    }

    tstring   RXFileIncludeLeadingDelimiter(const TCHAR* strPath)
    {
        if(NULL == strPath)
        {
            return _RXT("");
        }
        if (_RXTStrlen(strPath) == 0)
        {
            return _RXT("\\");
        }
        if (!SDPATH_IS_DELIMITER(strPath[0]))
        {
            return tstring(_RXT("\\")) + strPath;
        }
        else
        {
            return strPath;
        }
    }

    tstring  RXFileReplaceDelimiter(const TCHAR * strPath,TCHAR delimiter)
    {
        tstring path = strPath;
        for (tstring::iterator itr = path.begin(); itr != path.end(); ++itr)
        {
            if (SDPATH_IS_DELIMITER(*itr))
            {
                *itr = delimiter;
            }
        }
        return path;
    }

    BOOL  RXFileExists(const TCHAR* strFileName)
    {
        FILE* fp = fopen(_RXTT2A(strFileName).c_str(), "r");
        if (fp == NULL)
        {
            return FALSE;
        }

        fclose(fp);

        return TRUE;
    }

    uint64_t  RXFileGetSize(const TCHAR* strFileName)
    {
        std::ifstream infile;
        infile.open(_RXTT2A(strFileName).c_str(), std::ios::binary);
        if(infile.is_open())
        {
			// 保存当前读取位置
			std::streampos currentPosition = infile.tellg();

			// 移动到文件末尾并获取大小
            infile.seekg(0, std::ios::end);
			std::streampos endPosition = infile.tellg();
            
			// 回到原来的读取位置
            infile.seekg(currentPosition);
			uint64_t llSize = static_cast<uint64_t>(endPosition);
            infile.close();
            return llSize;
        }
        return 0;
    }

    BOOL RXFileCreate(const TCHAR* strFileName)
    {
        if (RXFileExists(strFileName))
        {
            return FALSE;
        }

        tstring strPath = RXFileExtractPath(strFileName);
        if(!strPath.empty())
        {
            if(!RXDirectoryExists(strPath.c_str()))
            {
                if (!RXCreateDirectory(strPath.c_str()))
                {
                    return FALSE;
                }
            }
        }


        FILE* fp = fopen(_RXTT2A(strFileName).c_str(), ("a+"));
        if (NULL == fp)
        {
            return FALSE;
        }

        fclose(fp);

        return TRUE;
    }

    BOOL  RXFileCopy(const TCHAR * pszExistFile, const TCHAR * pNewFile ,BOOL bFailIfExists)
    {
#if (defined(WIN32) || defined(WIN64))
        return CopyFile(pszExistFile,pNewFile,bFailIfExists); 
#else 
        int fps, fpd;
        struct stat statbuf;
        void *bufs, *bufd;

        if (RXFileExists(pNewFile))
        {
            if (bFailIfExists)
            {
                return FALSE; 
            }
            else 
            {
                RXFileRemove(pNewFile); 
            }
        }

        if((fps = open(_RXTT2A(pszExistFile).c_str(), O_RDONLY)) == -1) 
        {
            return FALSE; 
        }

        if((fpd = open(_RXTT2A(pNewFile).c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1) 
        {
            return FALSE; 
        }

        if(fstat(fps, &statbuf) == -1) 
        {
            return FALSE; 
        }

        if(pwrite(fpd, "", 1, statbuf.st_size - 1) != 1) 
        {
            return FALSE; 
        }

        if((bufs = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fps, 0)) == MAP_FAILED) 
        {
            return FALSE; 
        }

        if((bufd = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fpd, 0)) == MAP_FAILED) 
        {
            return FALSE; 
        }

        memcpy (bufd, bufs, statbuf.st_size);

        munmap(bufd, statbuf.st_size);
        munmap(bufs, statbuf.st_size);

        close(fpd);
        close(fps);

        return TRUE; 
#endif //		
    }
	typedef unsigned char LBYTE;	//
	typedef unsigned short LWORD;	//
	typedef unsigned int LDWORD;
	typedef long LLONG;				//

	//

#pragma pack(2)
	typedef struct
	{
		LWORD bfType;
		LDWORD bfSize;
		LWORD bfReserved1;
		LWORD bfReserved2;
		LDWORD bfOffBits;
	}LBITMAPFILEHEADER;
	// #pragma pack(pop)

	typedef struct
	{
		LDWORD biSize;
		LLONG biWidth;
		LLONG biHeight;
		LWORD biPlanes;
		LWORD biBitCount;
		LDWORD biCompression;
		LDWORD biSizeImage;
		LLONG biXPelsPerMeter;
		LLONG biYPelsPerMeter;
		LDWORD biClrUsed;
		LDWORD biClrImportant;
	}LBITMAPINFOHEADER;

	typedef struct
	{
		LBYTE rgbBlue;
		LBYTE rgbGreen;
		LBYTE rgbRed;
		LBYTE rgbReserved;
	}LRGBQUAD;

	typedef struct
	{
		unsigned char* buf;
		int width_x;
		int height_y;
		int deepth;
		LRGBQUAD* palette;
	}LBmpbase;

    BOOL RXSaveBmp(const TCHAR* pszFilePath, const BYTE* imgBuffer, const uint32_t& dwWidth, const uint32_t& dwHeight, const uint32_t& dwBitcount)
    {
		if (!imgBuffer)
		{
			return FALSE;
		}
		int colorTablesize = 1024;	//灰度图像颜色表
		int lineByte = (dwWidth * dwBitcount / 8 + 3) / 4 * 4;
		FILE* fp = _tfopen(pszFilePath, _T("wb"));
		if (!fp)
		{
			return FALSE;
		}
		LBITMAPFILEHEADER filehead;
		filehead.bfType = 0x4D42;
		filehead.bfSize = sizeof(LBITMAPFILEHEADER) + sizeof(LBITMAPINFOHEADER) +
			colorTablesize + lineByte * dwHeight;
		filehead.bfReserved1 = 0;
		filehead.bfReserved2 = 0;
		filehead.bfOffBits = 54 + colorTablesize;
		//写位图文件头进文件
		fwrite(&filehead, sizeof(LBITMAPFILEHEADER), 1, fp);
		//申请位图文件信息头结构变量， 填写文件信息头信息
		LBITMAPINFOHEADER infoHead;
		infoHead.biBitCount = dwBitcount;
		infoHead.biClrImportant = 0;
		infoHead.biClrUsed = 0;
		infoHead.biSize = 40;
		infoHead.biWidth = dwWidth;
		infoHead.biHeight = dwHeight;
		infoHead.biPlanes = 1;
		infoHead.biCompression = 0;
		infoHead.biSizeImage = lineByte * dwHeight;
		infoHead.biXPelsPerMeter = 0;
		infoHead.biYPelsPerMeter = 0;
		fwrite(&infoHead, sizeof(LBITMAPINFOHEADER), 1, fp);
		LRGBQUAD* pColorTable = new LRGBQUAD[256];
		for (int i = 0; i < 256; i++)
		{
			pColorTable[i].rgbBlue = i;
			pColorTable[i].rgbGreen = i;
			pColorTable[i].rgbRed = i;
		}
		fwrite(pColorTable, sizeof(LRGBQUAD), 256, fp);
		//写位图数据进文件
		fwrite(imgBuffer, dwHeight * lineByte, 1, fp);
		fclose(fp);
		return TRUE;
    }
}

