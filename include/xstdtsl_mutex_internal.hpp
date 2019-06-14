#pragma once
#include <atomic>
#include <mutex>

namespace xstdtsl_internal
{
	///
	/// The read/write mutex is used for atomic like read/write access to complex types. It allows multiple users to have read access while write access is denied, and a single user to have write access while all reads and other writes are denied.
	///
#if (__cplusplus > 202000L) //c++20 
	template<typename T> concept ReadLockable = requires T::is_read_lockable_type;
	template<typename T> concept WriteLockable = requires T::is_write_lockable_type;
#endif

	class read_write_mutex
	{
	private:
		mutable std::atomic<int>		m_nRead_Users; ///< the count of the number of users currently reading or writing. -1 indicates a write lock, a number greater than 0 or less than -1 indicates the number of read users.
		mutable std::mutex				m_mControl_Mutex; ///< a mutex to limit access to the read users count during attempted locking and unlocking operations
	public:
		///
		/// is_read_lockable_type: flag for c++20 concept ReadLockable
		///
		static const bool is_read_lockable_type;
		///
		/// is_write_lockable_type: flag for c++20 concept ReadLockable
		///
		static const bool is_write_lockable_type;


		///
		/// constructor; sets the initial number of read/write users to 0
		///
		read_write_mutex(void) noexcept
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
		bool try_read_lock(void) const noexcept
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
		bool try_write_lock(void) const noexcept
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
		void read_lock (void) const noexcept
		{
			while (!try_read_lock())
				std::this_thread::yield();
		}
		///
		/// obtains a write lock; blocking
		///
		void write_lock (void) const noexcept
		{
			while (!try_write_lock())
				std::this_thread::yield();
			return m_pData;
		}
		///
		/// releases a read lock; blocking
		///
		void read_unlock(void) const noexcept
		{
			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
			if (m_nRead_Users > 0)
				m_nRead_Users--;
		}
		///
		/// releases a write lock; blocking
		///
		void write_unlock(void) const noexcept
		{
			std::lock_guard<std::mutex> lock(m_mControl_Mutex);
			if (m_nRead_Users == -1)
				m_nRead_Users++;
		}
		///
		/// attemps to obtain a read lock for a fixed duration; blocking for the duration
		/// \returns true if a read lock is obtained; false otherwise
		///
		bool try_read_lock_for( const std::chrono::duration<Rep,Period>& i_cTimeout_Duration ) noexcept
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
		bool try_write_lock_for( const std::chrono::duration<Rep,Period>& i_cTimeout_Duration ) noexcept
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

		bool try_read_lock_until( const std::chrono::time_point<Clock,Duration>& i_cTimeout_Time ) noexcept
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
		bool try_write_lock_until( const std::chrono::time_point<Clock,Duration>& i_cTimeout_Time ) noexcept
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
		explicit read_lock_guard(read_write_mutex & i_tMutex) noexcept : m_tMutex(i_tMutex)
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
		~read_lock_guard(void) noexcept
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
		explicit write_lock_guard(read_write_mutex & i_tMutex) noexcept : m_tMutex(i_tMutex)
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
		~write_lock_guard(void) noexcept
		{
			m_tMutex.write_unlock();
		}
	};
	///
	/// similar to std::lock, but attempts to perform a read lock on multiple read_write_mutex or similar locks that have member try_read_lock()
	/// \returns -1 on success, the index of the lock that was unable to be aquired on partial success or failure
	///
	template<typename lock1, typename lock2, typename... lock3> int try_read_lock(lock1& i_l1, lock2& i_l2, lock3&... i_l3) noexcept
    {
		size_t nIdx = -1;
		bool bSuccess = true;
		auto tLocks = std::tie(i_l1, i_l2, i_l3...);
		do
		{
			bSuccess = std::get<nIdx>(tLocks).try_read_lock();
			nIdx++;
		}
		while (bSuccess && nIdx < std::tuple_size<tLocks>::value)
		if (bSuccess)
			nIdx = -1;
		else
			nIdx--;
		return nIdx;
    }
	///
	/// similar to std::lock, but attempts to perform a read lock on multiple read_write_mutex or similar locks that have member try_read_lock()
	/// \returns -1 on success, the index of the lock that was unable to be aquired on partial success or failure
	///
	template<typename lock1, typename lock2, typename... lock3> int try_write_lock(lock1& i_l1, lock2& i_l2, lock3&... i_l3) noexcept
    {
		size_t nIdx = 0;
		bool bSuccess = true;
		auto tLocks = std::tie(i_l1, i_l2, i_l3...);
		do
		{
			bSuccess = std::get<nIdx>(tLocks).try_write_lock();
			nIdx++;
		}
		while (bSuccess && nIdx < std::tuple_size<tLocks>::value)
		if (bSuccess)
			nIdx = -1;
		else
			nIdx--;
		return nIdx;
    }
	///
	/// similar to std::lock, but performs a read lock on multiple read_write_mutex or similar locks that have member read_lock(); blocking
	///
	template<typename lock1, typename lock2, typename... lock3> void read_lock(lock1& i_l1, lock2& i_l2, lock3&... i_l3) noexcept
    {
		size_t nIdx = 0;
		auto tLocks = std::tie(i_l1, i_l2, i_l3...);
		do
		{
			std::get<nIdx>(tLocks).read_lock();
			nIdx++;
		}
		while (nIdx < std::tuple_size<tLocks>::value)
    }
	///
	/// unlocks multiple read_write_mutex or similar locks that have member read_unlock(); blocking
	///
	template<typename lock1, typename lock2, typename... lock3> void read_unlock(lock1& i_l1, lock2& i_l2, lock3&... i_l3) noexcept
    {
		auto tLocks = std::tie(i_l1, i_l2, i_l3...);
		size_t nIdx = std::tuple_size<tLocks>::value;
		while (nIdx > 0)
		{
			nIdx--;
			std::get<nIdx>(tLocks).read_unlock();
		}
		std::get<0>(tLocks).read_unlock();
    }

	///
	/// similar to std::lock, but performs a write lock on multiple read_write_mutex or similar locks that have member write_lock(); blocking
	///
	template<typename lock1, typename lock2, typename... lock3> void write_lock(lock1& i_l1, lock2& i_l2, lock3&... i_l3) noexcept
    {
		size_t nIdx = 0;
		auto tLocks = std::tie(i_l1, i_l2, i_l3...);
		do
		{
			std::get<nIdx>(tLocks).write_lock();
			nIdx++;
		}
		while (nIdx < std::tuple_size<tLocks>::value)
    }
	///
	/// unlocks multiple read_write_mutex or similar locks that have member write_unlock(); blocking
	///
	template<typename lock1, typename lock2, typename... lock3> void read_unlock(lock1& i_l1, lock2& i_l2, lock3&... i_l3) noexcept
    {
		auto tLocks = std::tie(i_l1, i_l2, i_l3...);
		size_t nIdx = std::tuple_size<tLocks>::value;
		while (nIdx > 0)
		{
			nIdx--;
			std::get<nIdx>(tLocks).write_unlock();
		}
    }

	///
	/// similar to std::scoped_lock, but operating on two read_write_mutex to gain read lock on both, ensuring a deadlock doesn't occur; blocking
	///
	class dual_read_lock
	{
	private:
		//std::vector<read_write_lock&> m_vMutex_Info; ///< list of all mutex controlled by this lock
		read_write_mutex &m_Mutex1;
		read_write_mutex &m_Mutex2;
	public:
		///
		/// constructor; locks both mutexes for reading; blocking
		///
		dual_read_lock(
			read_write_mutex & i_Mutex1, ///< the first read_write_mutex to lock for writing
			read_write_mutex & i_Mutex2 ///< the second read_write_mutex to lock for writing
			) noexcept : m_Mutex1(i_Mutex1), m_Mutex1(i_Mutex2)
		{
			bool bWhich = true;
			bool bDone = false;
			do
			{
				read_write_mutex * pM1 = &m_Mutex1;
				read_write_mutex * pM2 = &m_Mutex2;
				bWhich = !bWhich;

				if (bWhich)
				{
					pM1 = m_Mutex2;
					pM2 = m_Mutex1;
				}

				if (pM1->try_read_lock())
				{
					if (pM2->try_read_lock())
						bDone = true;
					else
					{
						pM1->read_unlock();
						std::this_thread::yield();
					}
				}
				else
					std::this_thread::yield();
			} while (!bDone);
		}
		///
		/// copy constructor; deleted
		///
		dual_read_lock(const dual_read_lock & i_cRHO) = delete;
		///
		/// assignment operator; deleted
		///
		dual_read_lock & operator =(const dual_read_lock & i_cRHO) = delete;
		///
		/// destructor; releases read lock on both mutexes
		///
		~dual_read_lock(void)
		{
			m_Mutex2.read_unlock();
			m_Mutex1.read_unlock();
		}
	};

	///
	/// similar to std::scoped_lock, but operating on two read_write_mutex to gain write lock on both, ensuring a deadlock doesn't occur; blocking
	///
	class dual_write_lock
	{
	private:
		read_write_mutex & m_Mutex1; ///< reference to the first read_write_mutex
		read_write_mutex & m_Mutex2; ///< reference to the second read_write_mutex

	public:
		///
		/// constructor; locks both mutexes for reading; blocking
		///
		dual_write_lock(
			read_write_mutex & i_Mutex1, ///< the first read_write_mutex to lock for writing
			read_write_mutex & i_Mutex2 ///< the second read_write_mutex to lock for writing
			) noexcept : m_Mutex1(i_Mutex1), m_Mutex1(i_Mutex2)
		{
			bool bWhich = true;
			bool bDone = false;
			do
			{
				read_write_mutex * pM1 = &m_Mutex1;
				read_write_mutex * pM2 = &m_Mutex2;
				bWhich = !bWhich;

				if (bWhich)
				{
					pM1 = m_Mutex2;
					pM2 = m_Mutex1;
				}

				if (pM1->try_write_lock())
				{
					if (pM2->try_write_lock())
						bDone = true;
					else
					{
						pM1->write_unlock();
						std::this_thread::yield();
					}
				}
				else
					std::this_thread::yield();
			} while (!bDone);
		}
		///
		/// copy constructor; deleted
		///
		dual_write_lock(const scoped_write_lock & i_cRHO) = delete;
		///
		/// assignment operator; deleted
		///
		dual_write_lock & operator =(const scoped_write_lock & i_cRHO) = delete;
		///
		/// destructor; releases write lock on both mutexes
		///
		~dual_write_lock(void)
		{
			m_Mutex2.write_unlock();
			m_Mutex1.write_unlock();
		}
	};

	///
	/// similar to std::scoped_lock, but operating on two read_write_mutex to gain read lock on one and write lock on the other, ensuring a deadlock doesn't occur; blocking
	///
	class dual_read_write_lock
	{
	private:
		read_write_mutex & m_Mutex_Read; ///< reference to the first read_write_mutex that will be locked for read
		read_write_mutex & m_Mutex_Write; ///< reference to the second read_write_mutex that will be locked for write

	public:
		///
		/// constructor; locks both mutexes for reading; blocking
		///
		dual_read_write_lock(
			read_write_mutex & i_Mutex_Read, ///< the read_write_mutex to lock for reading
			read_write_mutex & i_Mutex_Write ///< the read_write_mutex to lock for writing
			) noexcept : m_Mutex_Read(i_Mutex_Read), m_Mutex_Write(i_Mutex_Write)
		{
			bool bWhich = true;
			bool bDone = false;
			do
			{
				bWhich = !bWhich;
				auto fnA = m_Mutex_Read.try_read_lock;
				auto fnB = m_Mutex_Write.try_write_lock;
				auto fnAu = m_Mutex_Read.read_unlock;

				if (bWhich)
				{
					fnB = m_Mutex_Read.try_read_lock;
					fnA = m_Mutex_Write.try_write_lock;
					fnAu = m_Mutex_Write.write_unlock;
				}

				if (fnA())
				{
					if (fnB())
						bDone = true;
					else
					{
						fnAu();
						std::this_thread::yield();
					}
				}
				else
					std::this_thread::yield();
			} while (!bDone);
		}
		///
		/// copy constructor; deleted
		///
		dual_read_write_lock(const scoped_read_write_lock & i_cRHO) = delete;
		///
		/// assignment operator; deleted
		///
		dual_read_write_lock & operator =(const scoped_read_write_lock & i_cRHO) = delete;
		///
		/// destructor; releases lock on both mutexes
		///
		~dual_read_write_lock(void)
		{
			m_Mutex_Write.write_unlock();
			m_Mutex_Read.read_unlock();
		}
	};
	
}

