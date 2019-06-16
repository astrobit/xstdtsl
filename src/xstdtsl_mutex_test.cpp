#include <xstdtsl_mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>

#include <xstdtsl_mutex_test.hpp>


///
/// aquires read lock, assuming that the mutex is not locked prior to attempt
///

void read_lock(xstdtsl::read_write_mutex * i_pMutex)
{
	std::cout << "checking to ensure read_write_mutex can be locked for read" << std::endl;
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	// aquire the read lock
	i_pMutex->read_lock();
	/// test to ensure correct read lock
	is_only_read_locked(i_pMutex);
	std::cout << "checking to ensure read_write_mutex can be unlocked after read lock" << std::endl;
	// release the lock
	i_pMutex->read_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

///
/// aquires write lock, assuming that the mutex is not locked prior to attempt
///

void write_lock(xstdtsl::read_write_mutex * i_pMutex)
{
	std::cout << "checking to ensure read_write_mutex can be locked for write" << std::endl;
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	// aquire the read lock
	i_pMutex->write_lock();
	/// test to ensure correct read lock
	is_only_write_locked(i_pMutex);
	std::cout << "checking to ensure read_write_mutex can be unlocked after write lock" << std::endl;
	// release the lock
	i_pMutex->write_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

///
/// attempts to aquire read lock, assuming that the mutex is not locked prior to attempt
///

void try_read_lock(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex can be locked for read using try_read_lock" << std::endl;
	// aquire the read lock
	assert(i_pMutex->try_read_lock()); /// try_read_lock should return true if the lock is aquired
	/// test to ensure correct read lock
	is_only_read_locked(i_pMutex);
	std::cout << "checking to ensure read_write_mutex can be unlocked after read lock" << std::endl;
	// release the lock
	i_pMutex->read_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

///
/// attempts to aquire read lock, assuming that the mutex is not locked prior to attempt
///

void try_write_lock(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex can be locked for write using try_write_lock" << std::endl;
	// aquire the write lock
	assert(i_pMutex->try_write_lock());
	/// test to ensure correct read lock
	is_only_write_locked(i_pMutex);
	std::cout << "checking to ensure read_write_mutex can be unlocked after write lock" << std::endl;
	// release the lock
	i_pMutex->write_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}


///
/// aquires two read locks, assuming that the mutex is not locked prior to attempt
///

void two_read_lock(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex can be locked twice in a row for read" << std::endl;
	// aquire the read lock
	i_pMutex->read_lock();
	// aquire the read lock again
	i_pMutex->read_lock();
	/// test to ensure correct read lock
	is_only_read_locked(i_pMutex);
	std::cout << "checking to ensure read_write_mutex is still locked after a single unlock" << std::endl;
	// release one lock
	i_pMutex->read_unlock();
	/// after a single unlock it should still be read locked
	is_only_read_locked(i_pMutex);
	std::cout << "checking to ensure read_write_mutex is unlocked after the second unlock" << std::endl;
	// release final lock
	i_pMutex->read_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

///
/// aquires maximum number of read locks, assuming that the mutex is not locked prior to attempt
///

void read_lock_limits(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex correctly handles the maximum number of read locks" << std::endl;
	// aquire the read lock
//	uint64_t nMax_Values = (uint64_t)(std::numeric_limits<int>::max()) * 2 - 1;
	std::cout << "locking stage 1" << std::endl;
	while (i_pMutex->lock_status() >= 0)
	{
		i_pMutex->read_lock();
		is_only_read_locked(i_pMutex,true);
	}
	std::cout << "locking stage 2" << std::endl;
	while (i_pMutex->lock_status() < -2)
	{
		i_pMutex->read_lock();
		is_only_read_locked(i_pMutex,true);
	}
	std::cout << "checking to ensure read_write_mutex correctly handles unlocking after the maximum number of read locks" << std::endl;
	std::cout << "unlocking stage 1" << std::endl;
	// release locks
//	size_t nCount = 0;
	while (i_pMutex->lock_status() <= -2)
	{
//		std::cout << nCount << " --- " << i_pMutex->lock_status() << std::endl;
		is_only_read_locked(i_pMutex,true);
		i_pMutex->read_unlock();
//		nCount++;
	}
	std::cout << "unlocking stage 2" << std::endl;
	while (i_pMutex->lock_status() > 0)
	{
//		if (nCount % 10000 == 0)
//			std::cout << nCount << " --- " << i_pMutex->lock_status() << std::endl;
		is_only_read_locked(i_pMutex,true);
		i_pMutex->read_unlock();
//		nCount++;
	}
	std::cout << "multi-read lock test complete" << std::endl;
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

///
/// aquires write lock, then tries to acquire another read and another write lock; both should fail
///

void write_then_try(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex doesnt allow a read lock or additional write lock when locked for write" << std::endl;
	// aquire the write lock
	i_pMutex->write_lock();
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// try to lock for reading -- should fail and not block
	assert(!i_pMutex->try_read_lock());
	// try to lock for writing -- should fail and not block
	assert(!i_pMutex->try_write_lock());
	std::cout << "checking to ensure read_write_mutex correctly unlocks after a write lock in which read and write lock attempts are subsequently made" << std::endl;
	// unlock
	i_pMutex->write_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

///
/// aquires write lock then tries a blocking write lock; should block until controlling thread releases
///

void write_then_write(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex blocks on second write lock attempt" << std::endl;
	// aquire the write lock
	i_pMutex->write_lock();
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// aquire the write lock ---  blocking; should hold here until controlling thread releases
	i_pMutex->write_lock();
	g_bWorking = false;
}

///
/// aquires write lock then tries a blocking read lock; should block until controlling thread releases
///

void write_then_read(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex blocks on read lock attempt while write locked" << std::endl;
	// aquire the write lock
	i_pMutex->write_lock();
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// aquire the read lock ---  blocking; should hold here until controlling thread releases
	i_pMutex->read_lock();
	g_bWorking = false;
}

///
/// aquires write lock then tries a blocking read lock; should block until controlling thread releases
///

void read_then_write(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	std::cout << "checking to ensure read_write_mutex blocks on write lock attempt while read locked" << std::endl;
	g_bWorking = true;
	// aquire the read lock
	i_pMutex->read_lock();
	// confirm read lock
	is_only_read_locked(i_pMutex);
	// aquire the write lock ---  blocking; should hold here until controlling thread releases
	i_pMutex->write_lock();
	g_bWorking = false;
}


int main(int i_nNum_Params, char * i_pParams[])
{
	xstdtsl::read_write_mutex cMutex;
	std::cout << "checking to ensure read_write_mutex constructor creates an unlocked mutex" << std::endl;
	// test to ensure the mutex is unlocked after the constructor
	is_unlocked_test(&cMutex);
	// test to ensure we can aquire read lock using read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_thread_safe(read_lock,&cMutex,"read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire read lock using try_read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_thread_safe(try_read_lock,&cMutex,"try read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire read lock using try_read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_thread_safe(two_read_lock,&cMutex,"2x read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire read lock using try_read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
//	test_lock_thread_safe(read_lock_limits,&cMutex,"maximum read lock or read unlock blocked unnecessarily. terminating.",1400000 );
	// test to ensure we can aquire write lock using write_lock  when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_thread_safe(write_lock,&cMutex,"write lock or write unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire write lock usng try_write_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_thread_safe(try_write_lock,&cMutex,"try write lock or write unlock blocked unnecessarily. terminating." );
	// test to ensure that read and write locks are unavailable after a write lock
	test_lock_thread_safe(write_then_try,&cMutex,"write lock then try write lock or write unlock blocked unnecessarily. terminating." );

	// blocking tests
	// aquire write then attempt another write
	test_lock_blocking_thread(write_then_write,&cMutex);
	// aquire write then attempt a read
	test_lock_blocking_thread(write_then_read,&cMutex);
	// aquire read then attempt a write
	test_lock_blocking_thread(read_then_write,&cMutex);

	//@@TODO: try ... until; try ... for
	//@@TODO: lock guard
	//@@TODO: dual_read, dual_write, dual_read_write	

	return 0;	
}
