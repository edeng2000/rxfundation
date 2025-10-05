#ifndef _RX_FUNDATION_INCLUDE_
#define _RX_FUNDATION_INCLUDE_

#ifdef RXFUNDATION_EXPORTS
#define RXFUNDATION_API __declspec(dllexport)
#else
#define RXFUNDATION_API __declspec(dllimport)
#endif
#ifndef RXAPI
#define RXAPI    __stdcall
#endif

#ifndef DOUBLE
typedef double              DOUBLE;
#endif

#endif