#if defined _WIN32 || defined _WIN64
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
#if defined _WIN32 || defined _WIN64
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


class array_internal
{
private:
	void	* 			m_pList;
	size_t 				m_nSize;
	size_t				m_nCapacity;
	size_t				m_nType_Size;
	size_t				m_nBlock_Allocation_Size;
//	optimization_hint	m_eOptimization;
	std::mutex			m_mtx;
	allocator			m_fnAllocator;
	deleter				m_fndeleter;


public:
	array_internal(size_t i_nType_Size, allocator i_pfnAllocator, deleter i_pfndeleter, size_t i_nInitial_Capacity)
	{
		m_pList = nullptr;
		m_nSize = 0;
		m_nCapacity = 0;
		m_nType_Size = i_nType_Size;
		m_nBlock_Allocation_Size = i_nType_Size;
		if (m_nBlock_Allocation_Size > 0 && g_cSystem_Data.m_nWord_Size > 0) 
		{
			// determine how many of typeT should get allocated per block in order to achieve system word alignment
			while (m_nBlock_Allocation_Size & 0x01 == 0)
				m_nBlock_Allocation_Size >>= 1;
			m_nBlock_Allocation_Size *= g_cSystem_Data.m_nWord_Size;
		}
		else if (g_bAllow_Exceptions)
			throw std::invalid_argument("Type size must be > 0");
	}
private:
	void nl_reserve(size_t i_nNew_Capacity)
	{
		if (m_nBlock_Allocation_Size > 0) 
		{
			size_t nTotal_Alloc_Size = (i_nNew_Capacity / m_nBlock_Allocation_Size) * m_nBlock_Allocation_Size;
			if ((i_nNew_Capacity % m_nBlock_Allocation_Size) > 0)
				nTotal_Alloc_Size += m_nBlock_Allocation_Size;
			if (nTotal_Alloc_Size > m_nCapacity)
			{
				void * pNew = m_fnAllocator(nTotal_Alloc_Size);
				if (m_nSize > 0 && m_pList != nullptr && pNew != nullptr)
					std::memcpy(pNew,m_pList,m_nSize * m_nType_Size);
				if (m_pList != nullptr)
					m_fndeleter(m_pList,m_nCapacity);
				m_nCapacity = nTotal_Alloc_Size;
				m_pList = pNew;
			}
		}
	}
	void nl_shrink_to_fit(void)
	{
		// save existing data
		void * pCurr = m_pList;
		size_t nCurr_Size = m_nSize;
		size_t nCurr_Capacity = m_nCapacity;
		// clear current member variables so reserve can be used
		m_nAlloc_Size = m_nSize = 0;
		m_pList = nullptr;
		nl_reserve(nCurr_Size);
		// copy existing data (if any) into current member variables allocated by reserve
		if (nCurr_Size > 0 && pCurr != nullptr && m_pList != nullptr)
			std::memcpy(m_pList,pCurr,nCurr_Size * m_nType_Size);
		m_nSize = nCurr_Size;
		// delete old data pointer
		if (pCurr != nullptr)
			m_fndeleter(pCurr,nCurr_Capacity);
	}


public:
	size_t size(void) const  noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		return m_nSize;
	}
	size_t capacity(void) const  noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		return m_nCapacity;
	}
	size_t max_size(void) const  noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		return m_nCapacity;
	}
		
	bool empty(void) const noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		return m_nSize == 0;
	}
	void clear(void) noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		m_nSize = 0;
	}


	void * at (size_t i_nIndex) noexcept(false)
	{
		void * pRet = nullptr;
		std::lock_guard<std::mutex> lLock(m_mtx);
		if (m_pList != nullptr && i_nIndex < m_nSize)
			pRet = m_pList + m_nType_Size * i_nIndex;
		else if (g_bAllow_Exceptions)
			throw std::out_of_range("vector operator[] called with index out of range");
		return pRet;
	}
	const typeT& at (size_t i_nIndex) const noexcept(false)
	{
		void * pRet = nullptr;
		std::lock_guard<std::mutex> lLock(m_mtx);
		if (m_pList != nullptr && i_nIndex < m_nSize)
			pRet = m_pList + m_nType_Size * i_nIndex;
		else if (g_bAllow_Exceptions)
			throw std::out_of_range("vector operator[] called with index out of range");
		return pRet;
	}
	typeT & operator [] (size_t i_nIndex) noexcept(false)
	{
		return at(i_nIndex);
	}
	const typeT & operator [] (size_t i_nIndex) const noexcept(false)
	{
		return at(i_nIndex);
	}

	void reserve(size_t i_nNew_Capacity)
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		nl_reserve(i_nNew_Capacity);
	}
	void shrink_to_fit(void)
	{

		std::lock_guard<std::mutex> lLock(m_mtx);
		nl_shrink_to_fit();
	}
	void resize(size_t i_nCount)
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		if (m_nSize > i_nCount)
		{
			m_nSize = i_nCount;
			nl_shrink_to_fit();
		}
		else
		{
			nl_reserve(i_nCount);
		}
	}
	void push_back(void * i_pData) noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		nl_reserve(m_nSize + 1);
		if (m_pList != nullptr)
		{
			memcpy(m_pList + m_nSize * m_nType_Size,i_pData,m_nType_Size);
			m_nSize++;
		}
	}
	void pop_back(void)
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		if (m_pList != nullptr && m_nSize > 0)
		{
			m_nSize--;
		}
	}
	void * front(void)
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		void * pRet = nullptr;
		if (m_pList != nullptr && m_nSize > 0)
		{
			pRet = m_pList;
		}
		return pRet;
	}
	void * back(void) noexcept(false)
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		void * pRet = nullptr;
		if (m_pList != nullptr && m_nSize > 0)
		{
			pRet = m_pList + (m_nSize - 1) * m_nType_Size;
		}
		return pRet;
	}
	void * data(void) noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		return m_pList;
	}

	const void * data(void) const noexcept
	{
		std::lock_guard<std::mutex> lLock(m_mtx);
		return m_pList;
	}

	///
	/// swap the contents of two arrays; this is better done in the wrapper than here
	///
	void swap(array & i_cArray)
	{
		// assume c++17 is available; need to develop this for c++14 and earlier

		std::scoped_lock lock(m_mtx,i_cArray.m_mtx);
//		std::lock_guard<std::mutex> lLock(m_mtx);
//		std::lock_guard<std::mutex> lLock2(i_cArray.m_mtx);
		if (m_nType_Size == i_cArray.m_nType_Size) // make sure they at least have the same element sizes, even if we can't determine the types
		{
			std::swap<void *>(m_pList,i_cArray.m_pList);
			std::swap<size_t>(m_nSize,i_cArray.m_nSize);
			std::swap<size_t>(m_nCapacity,i_cArray.m_nCapacity);
			std::swap<allocator>(m_fnAllocator,i_cArray.m_fnAllocator);
			std::swap<deleter>(m_fndeleter,i_cArray.m_fndeleter);
		}
	}

			
};

