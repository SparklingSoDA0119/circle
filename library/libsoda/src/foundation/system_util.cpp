#include "libsoda/foundation/system_util.h"

#if defined(_WIN64)
#include <Windows.h>
#include <NetCon.h>
#include <iphlpapi.h>
#include <tchar.h>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

#pragma warning(disable:4996)
#else
#include "foundation/string_util.h"

#include <ctime>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <sys/types.h>
#if defined(__linux__)
#include <linux/version.h>
#endif
#include <dirent.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <set>
#include <algorithm>
#endif

#include <map>
#include <fstream>
#include <chrono>
#include <stdlib.h>

#if defined(_WIN64)
	#define PIPE_OPEN	_popen
	#define PIPE_CLOSE  _pclose
#else
	#define PIPE_OPEN	popen
	#define PIPE_CLOSE  pclose
#endif

_namespace_soda_util_begin

namespace tick {

	uint64_t currentTick()
	{
		using std::chrono::milliseconds;
		using std::chrono::steady_clock;
		using std::chrono::duration_cast;

		return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
	}

	uint64_t diffTick(uint64_t t1, uint64_t t2)   // t1 should be prior to t2.
	{
		return (t1 <= t2) ? t2 - t1 : (std::numeric_limits<uint64_t>::max() - t1) + t2 + 1;
	}

	uint64_t diffFromCurrentTick(uint64_t tick)
	{
		uint64_t currTick = currentTick();

		return (currTick <= tick) ? 0 : tick - currTick;
	}

}	// namespace tick

_namespace_soda_util_end