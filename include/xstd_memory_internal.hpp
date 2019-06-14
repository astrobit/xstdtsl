#pragma once
#include <xstd_types>

namespace xstd_internal
{
	///
	/// The read/write mutex is used for atomic like read/write access to complex types. It allows multiple users to have read access while write access is denied, and a single user to have write access while all reads and other writes are denied.
	///
	class read_write_mutex
	{
	private:
		mutable std::atomic<int>		m_nRead_Users; ///< the count of the number of users currently reading or writing. -1 indicates a write lock, a number greater than 0 or less than -1 indicates the number of read users.
		mutable std::mutex				m_mControl_Mutex; ///< a mutex to limit access to the read users count during attempted locking and unlocking operations
	public:
		///
		/// constructor; sets the initial number of read/write users to 0
		///
		read_write_mutex(void)
		{
			m_nRead_Users = 0;
		}
		///
		/// copy constuctor (deleted)
		///
		read_write_mutex(const read_write_mutex & i_RHO) = delete;
		///
		/// assignment operator (deleted)
		///
		read_write_mutex & operator =(const read_write_mutex & i_cRHO) = delete;
		///
		/// destructor; claims write lock to ensure no other read/write users prior to destruction
		///
		~read_write_mutex(void) noexcept
		{
			write_lock();
//			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
//			if (m_nRead_Users != 0)
//				throw mutex_destroyed_while_locked
		}
		///
		/// checks to see if the mutex is currently read locked; non-blocking
		/// \returns true if the mutex is locked due to reading; false otherwise
		///
		inline bool is_read_locked(void) const  noexcept
		{
			int nCurr_Read_Users = m_nRead_Users;
			return (nCurr_Read_Users > 0 || nCurr_Read_Users <= -2);
		}
		///
		/// checks to see if the mutex is currently write locked; non-blocking
		/// \returns true if the mutex is locked due to writing; false otherwise
		///
		inline bool is_write_locked(void) const noexcept
		{
			return m_nRead_Users == -1;
		}
		///
		/// checks to see if the mutex is currently locked for reading or writing; non-blocking
		/// \returns true if the mutex is not locked for reading or writing; false otherwise
		///
		inline bool is_unlocked(void) const noexcept
		{
			return m_nRead_Users == 0;
		}
		///
		/// gets the current number of users of the read or write lock
		/// \returns 0 if no read or write locks exist; -1 if a write lock exists; and a number > 0 or <= -2 if a read lock exists. If the number is negative, there are (std::numeric_limits<int>.max() + (<return value> - std::numeric_limits<int>.min())) users
		///
		int lock_status(void) const noexcept
		{
			return m_nRead_Users;
		}

		///
		/// attemps to obtain a read lock; non-blocking
		/// \returns true if a read lock is obtained; false otherwise
		///
		bool try_read_lock(void) const
		{
			bool bSuccess = false;
			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
			bSuccess = (m_nRead_Users >= 0 || m_nRead_Users < -2);
			if (bSuccess)
			{
				m_nRead_Users++;
			}
			return bSuccess;
		}
		///
		/// attemps to obtain a write lock; non-blocking
		/// \returns true if a write lock is obtained; false otherwise
		///
		bool try_write_lock(void) const
		{
			bool bSuccess = false;
			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
			bSuccess = (m_nRead_Users == 0);
			if (bSuccess)
			{
				m_nRead_Users--; /// -1 indicates write in progress
			}
			return bSuccess;
		}
		///
		/// obtains a read lock; blocking
		///
		void read_lock (void) const
		{
			while (!try_read_lock())
				std::this_thread::yield();
		}
		///
		/// obtains a write lock; blocking
		///
		void write_lock (void) const
		{
			while (!try_write_lock())
				std::this_thread::yield();
			return m_pData;
		}
		///
		/// releases a read lock; blocking
		///
		void read_unlock(void) const
		{
			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
			if (m_nRead_Users > 0)
				m_nRead_Users--;
		}
		///
		/// releases a write lock; blocking
		///
		void write_unlock(void) const
		{
			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
			if (m_nRead_Users == -1)
				m_nRead_Users++;
		}
		///
		/// attemps to obtain a read lock for a fixed duration; blocking for the duration
		/// \returns true if a read lock is obtained; false otherwise
		///
		bool try_read_lock_for( const std::chrono::duration<Rep,Period>& i_cTimeout_Duration )
		{
			auto tStart = std::chrono::steady_clock::now();
			bool bRet = try_read_lock();
			while (!bRet && (std::chrono::steady_clock::now() - tStart) < i_cTimeout_Duration)
			{
				std::this_thread::yield();
				bRet = try_read_lock();
			}
			return bRet;
		}
		///
		/// attemps to obtain a write lock for a fixed duration; blocking for the duration
		/// \returns true if a write lock is obtained; false otherwise
		///
		bool try_write_lock_for( const std::chrono::duration<Rep,Period>& i_cTimeout_Duration )
		{
			auto tStart = std::chrono::steady_clock::now();
			bool bRet = try_write_lock();
			while (!bRet && (std::chrono::steady_clock::now() - tStart) < i_cTimeout_Duration)
			{
				std::this_thread::yield();
				bRet = try_write_lock();
			}
			return bRet;
		}
		///
		/// attemps to obtain a read lock until a certain time; blocking for the duration
		/// \returns true if a read lock is obtained; false otherwise
		///

		bool try_read_lock_until( const std::chrono::time_point<Clock,Duration>& i_cTimeout_Time )
		{
			bool bRet = try_read_lock();
			while (!bRet && i_cTimeout_Time.clock::now() < i_cTimeout_Time)
			{
				std::this_thread::yield();
				bRet = try_read_lock();
			}
			return bRet;
		}
		///
		/// attemps to obtain a write lock until a certain time; blocking for the duration
		/// \returns true if a write lock is obtained; false otherwise
		///
		bool try_write_lock_until( const std::chrono::time_point<Clock,Duration>& i_cTimeout_Time )
		{
			bool bRet = try_write_lock();
			while (!bRet && i_cTimeout_Time.clock::now() < i_cTimeout_Time)
			{
				std::this_thread::yield();
				bRet = try_write_lock();
			}
			return bRet;
		}
	};


	///
	/// similar to std::lock_guard, but claiming read access for a read_write_mutex
	///
	class read_lock_guard
	{
	private:
		read_write_mutex & m_tMutex; ///< a reference to the mutex to use
	public:
		///
		/// constructor; establishes a read lock; blocking
		///
		read_lock_guard(read_write_mutex & i_tMutex) : m_tMutex(i_tMutex)
		{
			m_tMutex.read_lock();
		}
		///
		/// copy constructor; deleted
		///
		read_lock_guard(const read_lock_guard & i_cRHO) = delete;
		///
		/// assignment operator; deleted
		///
		read_lock_guard operator =(const read_lock_guard & i_cRHO) = delete;
		///
		/// destructor; releases a read lock; blocking
		///
		~read_lock_guard(void)
		{
			m_tMutex.read_unlock();
		}
	};

	///
	/// similar to std::lock_guard, but claiming read access for a read_write_mutex
	///
	class write_lock_guard
	{
	private:
		read_write_mutex & m_tMutex; ///< a reference to the mutex to use
	public:
		///
		/// constructor; establishes a read lock; blocking
		///
		write_lock_guard(read_write_mutex & i_tMutex) : m_tMutex(i_tMutex)
		{
			m_tMutex.write_lock();
		}
		///
		/// copy constructor; deleted
		///
		write_lock_guard(const write_lock_guard & i_cRHO) = delete;
		///
		/// assignment operator; deleted
		///
		write_lock_guard operator =(const write_lock_guard & i_cRHO) = delete;
		///
		/// destructor; releases a read lock; blocking
		///
		~write_lock_guard(void)
		{
			m_tMutex.write_unlock();
		}
	};
}

//	template <type T> class read_unique_lock

	template <typename T> class atomic
	{
	private:
		T 							m_tData;
		mutable read_write_mutex	m_mMutex;
	public:
		atomic(const T & i_tValue) noexcept
		{
			write_lock_guard<read_write_mutex> lock(m_mMutex);
			m_tData = i_tValue;
		}
		atomic(atomic & i_cRHO) noexcept
		{
			read_lock_guard<read_write_mutex> lockRHO(i_cRHO.m_mMutex);
			write_lock_guard<read_write_mutex> lockThis(m_mMutex);
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
