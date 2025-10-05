#ifndef __GATE_COMMON_H__
#define __GATE_COMMON_H__

#include "commonheader.h"
#include "globalmem.h"
#include "globalset.h"

#ifndef MAX_PACKET_SIZE
#define MAX_PACKET_SIZE (1024 * 1024)
#endif

#define MAX_GT_MEM_COUNT 7
typedef SGlobalMemAlloc<MAX_PACKET_SIZE, MAX_GT_MEM_COUNT> SGTGlobalMemPool;
typedef IBaseBuilder<MAX_PACKET_SIZE, MAX_GT_MEM_COUNT> SGTBuildMem;

#endif