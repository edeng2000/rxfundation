#include "rxfilemapping.h"
#include "rxfile.h"
#include "rxstring.h"
#if !(defined(WIN32) || defined(WIN64))
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#endif

namespace RX
{

    BOOL  RXFileMapping(SFileMapping& fm, const TCHAR* file, UINT32 beg, UINT32 size)
    {
#if (defined(WIN32) || defined(WIN64))
        HANDLE filehandle = CreateFileA(_RXTT2A(file).c_str(), GENERIC_READ | GENERIC_WRITE,
                                        FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                                        FILE_ATTRIBUTE_HIDDEN, NULL);
        if (filehandle == INVALID_HANDLE_VALUE)
        {
            return FALSE;
        }

        fm.maphandle = CreateFileMappingA(filehandle, NULL,
                                          PAGE_READWRITE, 0, size, _RXTT2A(file).c_str());

        if (fm.maphandle == NULL)
        {
            CloseHandle(filehandle);
            return FALSE;
        }

        fm.mem = MapViewOfFile(fm.maphandle, FILE_MAP_READ|FILE_MAP_WRITE, 0, beg, size);
        fm.pos = fm.mem;
        if (fm.mem == NULL)
        {
            CloseHandle(filehandle);
            CloseHandle(fm.maphandle);

            return FALSE;
        }

        if (size == 0)
        {
            SFileStatus filestatus;
            if (RXGetFileStatus(file, filestatus) == FALSE)
            {
                return FALSE;
            }
            size = filestatus.size - beg;
        }

        CloseHandle(filehandle);
#else
        if (size == 0)
        {
            SFileStatus filestatus;
            if (RXGetFileStatus((file), filestatus) == FALSE)
            {
                return FALSE;
            }
            size = filestatus.size - beg;
        }

        int fd = open(_RXTT2A(file).c_str(), O_RDWR);
        if (fd < 0)
        {
            return FALSE;
        }

        fm.mem = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, beg);
        if (fm.mem == MAP_FAILED)
        {
            printf("%d:%s\n", errno,strerror(errno));
            close(fd);
            return FALSE;
        }

        close(fd);
#endif
        fm.size = size;

        return TRUE;
    }

    void  RXFileUnMapping(SFileMapping& fm)
    {
#if (defined(WIN32) || defined(WIN64))
        UnmapViewOfFile(fm.mem);
        CloseHandle(fm.maphandle);
#else
        munmap(fm.mem, fm.size);
#endif
    }

    void  RXFileFlushMapping(void* mem, UINT32 len)
    {
#if (defined(WIN32) || defined(WIN64))
        FlushViewOfFile(mem, len);
#else
        msync(mem, len, 0);
#endif
    }

}

