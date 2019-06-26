#if defined _WIN32 || defined _WIN64
#define __XSTDTSL_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

class system_data
{
public:
	size_t		m_nWord_Size;
	size_t		m_nPage_Size;

	system_data(void)
	{
#ifdef __XSTDTSL_WINDOWS
		SYSTEM_INFO sSys_Info;
		GetNativeSystemInfo(&sSys_Info);
		m_nPage_Size = sSys_Info.dwPageSize;
#else
		m_nPage_Size = sysconf(_SC_PAGE_SIZE);
#endif
		if (sizeof(long double) == 16) /// use long double as an indicator of 64-bit vs 32-bit memory alighnment; this is actually compiler dependent, but it is a reasonable guess
			m_nWord_Size = 8;
		else
			m_nWord_Size = 4;
	}

};
system_data	g_cSystem_Data;
bool		g_bAllow_Exceptions = true;

size_t get_word_size(void)
{
	return g_cSystem_Data.m_nWord_Size;
}

size_t get_available_memory(void)
{
	size_t nRet;
#ifdef __XSTDTSL_WINDOWS
	MEMORYSTATUSEX cMem_Status;
	GlobalMemoryStatusEx(&cMem_Status);
	nRet = cMem_Status.ullAvailPhys;
#else
	nRet = sysconf(_SC_AVPHYS_PAGES) * g_cSystem_Data.m_nPage_Size;    /* available pages */
#endif
	return nRet;
}

#ifdef __XSTDTSL_WINDOWS
#undef __XSTDTSL_WINDOWS
#endif

