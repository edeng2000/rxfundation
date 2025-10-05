#ifndef _GNDP_HASHHELPER_H_
#define _GNDP_HASHHELPER_H_

// has function for linux

#if defined(WIN32) || defined(WIN64)

// just ignore in windows environment

#else

// linux hash function
#include <string>
#include <ext/hash_map>

namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& x ) const
		{
			return hash< const char* >()( x.c_str() );
		}
	};

	template<> struct hash<long long> {
		size_t operator()(long long __x) const { return __x; }
	};
	template<> struct hash<const long long> {
		size_t operator()(const long long __x) const { return __x; }
	};


	template<> struct hash<unsigned long long> {
		size_t operator()(unsigned long long __x) const { return __x; }
	};
	template<> struct hash<const unsigned long long> {
		size_t operator()(const unsigned long long __x) const { return __x; }
	};

}

#endif

#endif
