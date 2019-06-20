#pragma once
#include <xstd_types>

namespace xstdtsl_internal
{
	class atomic_storage
	{
	private:
		size_t						m_nData_Size;
		void *						m_ptData;
		mutable read_write_mutex	m_mMutex;
	public:
		atomic(size_t i_nData_Size) noexcept
		{
			write_lock_guard cLock(m_mMutex);
			if (i_nData_Size != 0)
			{
				m_nData_Size = i_nData_Size;
				m_ptData = new char[i_nData_Size];
			}
		}
		atomic(atomic & i_cRHO) noexcept
		{
			dual_read_write_lock_guard cLock(i_cRHO.m_mMutex,m_mMutex);
			m_tData = i_cRHO.m_tData;
		}
		void store(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			m_tData = i_tValue;
		}
		atomic & operator =(const T & i_tValue) noexcept
		{
			store(i_tValue);
			return *this;
		}
		T load(void) const noexcept
		{
			T tRet;
			read_lock_guard<read_write_mutex> lock(m_mMutex);
			tRet = m_tData;
			return tRet;
		}
		operator T(void) const noexcept
		{
			return load();
		}
		bool is_lock_free(void) const
		{
			return false;
		}
		bool is_lock_free(void) const
		{
			return false;
		}
		T exchange(T & io_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			T tTemp  = 	m_tData;
			m_tData = io_tValue;
			io_tValue = tTemp;
		}
		bool compare_exchange_weak(T & io_tExpected, T i_tDesired)
		{
			bool bRet = false;
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			if (std::memcmp(&m_tData,&io_tExpected) == 0)
			{
				m_tData = i_tDesired;
				bRet = true;
			}
			else
				io_tExpected = m_tData;
			return bRet;		
		}	
		bool compare_exchange_strong(T & io_tExpected, T i_tDesired)
		{
			bool bRet = false;
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			if (std::memcmp(&m_tData,&io_tExpected) == 0)
			{
				m_tData = i_tDesired;
				bRet = true;
			}
			else
				io_tExpected = m_tData;
			return bRet;		
		}	
		T fetch_add(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			T tRet = m_tData;
			m_tData += i_tValue;
			return tRet;
		}
		T fetch_sub(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			T tRet = m_tData;
			m_tData -= i_tValue;
			return tRet;
		}
		T fetch_and(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			T tRet = m_tData;
			m_tData &= i_tValue;
			return tRet;
		}
		T fetch_or(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			T tRet = m_tData;
			m_tData |= i_tValue;
			return tRet;
		}
		T fetch_xor(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			T tRet = m_tData;
			m_tData ^= i_tValue;
			return tRet;
		}

		T operator++(void)
		{
			return fetch_add(1);
		}
		T operator++(int i_nI)
		{
			return fetch_add(1) + 1;
		}
		T operator--(void)
		{
			return fetch_add(-1);
		}
		T operator--(int i_nI)
		{
			return fetch_sub(1) - 1;
		}
		T operator+=(int i_nI)
		{
			return fetch_add(i_nI) + i_nI;
		}
		T operator-=(int i_nI)
		{
			return fetch_sub(i_nI) - i_nI;
		}
		T operator&=(const T & i_tValue)
		{
			return fetch_and(i_tValue) & i_tValue;
		}
		T operator|=(const T & i_tValue)
		{
			return fetch_or(i_tValue) | i_tValue;
		}
		T operator^=(const T & i_tValue)
		{
			return fetch_xor(i_tValue) ^ i_tValue;
		}

			

	};



	class shared_ptr_data
	{
	private:
		void * 							m_pData;
		allocator 						m_pfnAllocator;
		deleter 						m_pfnDeleter;
		std::atomic<size_t>				m_nInstance_Count;
		std::atomic<size_t>				m_nAllocation_Size;
		mutable std::mutex				m_mMutex;
		std::atomic<bool>				m_bManual_Lock;
		read_write_mutex				m_mRead_Write_Mutex;

		void clear(void)
		{
			m_nAllocation_Size = 0;
			m_nInstance_Count = 0;
			m_pfnAllocator = nullptr;
			m_pfnDeleter = nullptr;
			m_pData = nullptr;
			m_nRead_Users = 0;
		}
		void nl_decrement(void)
		{
			if (m_nInstance_Count > 0)	
				m_nInstance_Count--;
			if (m_nInstance_Count == 0 && m_pData != nullptr)
			{
				m_pfnDeleter(m_pData,m_nAllocation_Size);
				m_pData = nullptr;
			}
		}
			

	public:
		void * get_read_lock (void) const
		{
			m_mRead_Write_Mutex.read_lock();
			return m_pData;
		}
		void * get_write_lock (void) const
		{
			m_mRead_Write_Mutex.write_lock();
			return m_pData;
		}
		void release_read_lock(void) const
		{
			std::lock_guard<std::mutex> lock(m_mRead_Write_Control_Mutex);
			if (m_nRead_Users > 0)
				m_nRead_Users--;
		}
		void release_write_lock(void) const
		{
			std::lock_guard<std::mutex> lock(m_mRead_Write_Control_Mutex);
			if (m_nRead_Users == -1)
				m_nRead_Users++;
		}

		void * get(void) const
		{
			return m_pData;
		}
		size_t count(void) const
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			return m_nInstance_Count;
		}

		shared_ptr_data(void)
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			m_bManual_Lock = false;
			clear();
		}
		shared_ptr_data(allocator i_pfnAllocator, deleter i_pfnDeleter, size_t i_nAllocation_Size)
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			m_bManual_Lock = false;
			m_pfnAllocator = i_pfnAllocator;
			m_pfnDeleter = i_pfnDeleter;
			m_nAllocation_Size = i_nAllocation_Size;
			m_pData = m_pfnAllocator(i_nAllocation_Size);
			if (m_pData != nullptr)
				m_nInstance_Count = 1;
			else
				clear();
		}
		~shared_ptr_data(void)
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			if (m_nInstance_Count > 0)
			{
				//@@TODO: throw an exception or otherwise indicate a problem if this class is being deleted before all users have exited
				if (m_pData != nullptr)
					m_pfnDeleter(m_pData,m_nAllocation_Size);
				m_pData = nullptr;
			}
		}



		///
		/// pre-increment (++a)
		///
		shared_ptr_data & operator++(void) const
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			m_nInstance_Count++;
			return *this;
		}
		///
		/// pre-decement (--a)
		///
		shared_ptr_data& operator--(void) const
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			nl_decrement();
			return *this;
		}
		///
		/// post-increment (a++)
		///
		shared_ptr_data operator++(void) const
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			m_nInstance_Count++;
			return *this;
		}
		///
		/// post-decement (--a)
		///
		shared_ptr_data operator--(void) const
		{
			std::lock_guard<std::mutex> lock(m_mMutex);
			nl_decrement();
			return *this;
		}

		bool is_manual_lock(void) const
		{
			return m_bManual_Lock;
		}

		void lock(void) const
		{
			m_mtx.lock();
			m_bManual_Lock = true;
		}
		bool try_lock(void) const
		{
			return (m_bManual_Lock = m_mtx.try_lock());
		}
		void unlock(void) const
		{
			m_bManual_Lock = false;
			m_mtx.unlock();
		}
	};



	class shared_ptr
	{
	private:
		shared_ptr_data * 		m_pData;

		mutable std::mutex		m_mtx;
		bool					m_bManual_Lock;


		void nl_reset(void)
		{
			if (m_pPointer != nullptr)
			{
				if (m_pData != nullptr)
				{
					(*m_pData)--;
					if (m_pData.count() == 0)
						delete m_pData;
				}
				m_pData = nullptr;
			}
		}
		void copy(shared_ptr & i_cRHO)
		{
			std::shared_lock lock(m_mtx,i_cRHO.m_mtx);
			nl_reset();
			(*i_cRHO.m_pData)++;
			m_pData = i_cRHO.m_pData;
		}
	public:
		void * get_read_lock(void) const
		{
		}

		void * get(void) const
		{
			void * pRet = nullptr;
			std::lock_guard<std::mutex> lock(m_mtx);
			if (m_pData != nullptr)
				pRet = m_pData->get();
			return pRet;
		}

		void reset(void)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			nl_reset();
		}

		inline bool is_this_manual_lock(void) const
		{
			return m_bManual_Lock;
		}

		bool is_manual_lock(void) const
		{
			return is_this_manual_lock() || (m_pData != nullptr && m_pData->is_manual_lock());
		}


		shared_ptr(allocator i_pfnAllocator, deleter i_pfnDeleter, size_t i_nArray_Size)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_bManual_Lock = false;
			m_pData = nullptr;
			m_nRead_Users = 0;
			if (i_pfnAllocator != nullptr && i_pfnDeleter != nullptr)
				m_pData = new shared_ptr_data(i_pfnAllocator,i_pfnDeleter,i_nArray_Size);
		}
		shared_ptr(shared_ptr & i_cRHO)
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			m_nRead_Users = 0;
			m_bManual_Lock = false;
			m_pData = nullptr;
			copy(i_cRHO);
		}
		~shared_ptr(void)
		{
			if (m_bManual_Lock)
				unlock();
			reset();
		}

		void lock(void) const
		{
			m_mtx.lock();
			m_bManual_Lock = true;
			if (m_pData != nullptr)
				m_pData->lock();
		}
		void try_lock(void) const
		{
			// the try lock must hold the mutex of this class to ensure that the underlying data does not change while the lock is held. 
			m_mtx.lock();
			m_bManual_Lock = true;
			bool bLocked = false;
			if (m_pData != nullptr)
				bLocked = m_pData->try_lock();
			if (!bLocked) // the lock failed for some reason - unlock the mutex of this class
			{
				m_bManual_Lock = false;
				m_mtx.unlock();
			}
		}
		void unlock(void) const
		{
			if (m_pData != nullptr)
				m_pData->unlock();
			m_bManual_Lock = false;
			m_mtx.unlock();
		}
	};
}
