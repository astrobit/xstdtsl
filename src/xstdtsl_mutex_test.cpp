#include <xstdtsl_mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>

std::atomic_bool g_bWorking(false);

///
/// test to ensure that the mutex is fully unlocked
///
void is_unlocked_test(xstdtsl::read_write_mutex * i_pMutex)
{
	// test to ensure constructor does not lock the mutex for read
	assert (!i_pMutex->is_read_locked());
	// test to ensure constructor does not lock the mutex for write
	assert (!i_pMutex->is_write_locked());
	// test to ensure constructor leaves mutex unlocked
	assert (i_pMutex->is_unlocked());
	// test to ensure constructor leave state as "0"
	assert (i_pMutex->lock_status() == 0);
}
///
/// test to ensure that the mutex is only read locked
///
void is_only_read_locked(xstdtsl::read_write_mutex * i_pMutex)
{
	// test to ensure constructor does not lock the mutex for read
	assert (i_pMutex->is_read_locked());
	// test to ensure constructor does not lock the mutex for write
	assert (!i_pMutex->is_write_locked());
	// test to ensure constructor leaves mutex unlocked
	assert (!i_pMutex->is_unlocked());
	// test to ensure constructor leave state as "0"
	assert (i_pMutex->lock_status() > 0 || i_pMutex->lock_status() < -1);
}
///
/// test to ensure that the mutex is only write locked
///
void is_only_write_locked(xstdtsl::read_write_mutex * i_pMutex)
{
	// test to ensure constructor does not lock the mutex for read
	assert (!i_pMutex->is_read_locked());
	// test to ensure constructor does not lock the mutex for write
	assert (i_pMutex->is_write_locked());
	// test to ensure constructor leaves mutex unlocked
	assert (!i_pMutex->is_unlocked());
	// test to ensure constructor leave state as "0"
	assert (i_pMutex->lock_status() == -1);
}


///
/// aquires read lock, assuming that the mutex is not locked prior to attempt
///

void read_lock(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	// aquire the read lock
	i_pMutex->read_lock();
	/// test to ensure correct read lock
	is_only_read_locked(i_pMutex);
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
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	// aquire the read lock
	i_pMutex->write_lock();
	/// test to ensure correct read lock
	is_only_write_locked(i_pMutex);
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
	// aquire the read lock
	assert(i_pMutex->try_read_lock()); /// try_read_lock should return true if the lock is aquired
	/// test to ensure correct read lock
	is_only_read_locked(i_pMutex);
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
	// aquire the write lock
	assert(i_pMutex->try_write_lock());
	/// test to ensure correct read lock
	is_only_write_locked(i_pMutex);
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
	// aquire the read lock
	i_pMutex->read_lock();
	// aquire the read lock again
	i_pMutex->read_lock();
	/// test to ensure correct read lock
	is_only_read_locked(i_pMutex);
	// release one lock
	i_pMutex->read_unlock();
	/// after a single unlock it should still be read locked
	is_only_read_locked(i_pMutex);
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
	// aquire the read lock
	uint64_t nMax_Values = (uint64_t)(std::numeric_limits<int>::max()) * 2 - 1;
	std::cout << "locking" << std::endl;
	while (i_pMutex->lock_status() >= 0)
	{
		i_pMutex->read_lock();
		is_only_read_locked(i_pMutex);
	}
	std::cout << "locking stage 2" << std::endl;
	while (i_pMutex->lock_status() < -2)
	{
		i_pMutex->read_lock();
		is_only_read_locked(i_pMutex);
	}
	std::cout << "unlocking" << std::endl;
	// release locks
//	size_t nCount = 0;
	while (i_pMutex->lock_status() <= -2)
	{
//		std::cout << nCount << " --- " << i_pMutex->lock_status() << std::endl;
		is_only_read_locked(i_pMutex);
		i_pMutex->read_unlock();
//		nCount++;
	}
	std::cout << "unlocking stage 2" << std::endl;
	while (i_pMutex->lock_status() > 0)
	{
//		if (nCount % 10000 == 0)
//			std::cout << nCount << " --- " << i_pMutex->lock_status() << std::endl;
		is_only_read_locked(i_pMutex);
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
	// aquire the write lock
	i_pMutex->write_lock();
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// try to lock for reading -- should fail and not block
	assert(!i_pMutex->try_read_lock());
	// try to lock for writing -- should fail and not block
	assert(!i_pMutex->try_write_lock());
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
	g_bWorking = true;
	// aquire the read lock
	i_pMutex->read_lock();
	// confirm read lock
	is_only_read_locked(i_pMutex);
	// aquire the write lock ---  blocking; should hold here until controlling thread releases
	i_pMutex->write_lock();
	g_bWorking = false;
}

// function prototype for single mutex test functions
typedef void (*test_fn_single_mutex) (xstdtsl::read_write_mutex *);
// function prototype for single mutex test functions

void test_lock_thread_safe(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, const char * i_psFault_String, size_t i_nSleep_Length_ms = 250)
{
	// test to ensure we can aquire read lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	std::thread cThr(pFn,i_pMutex);
	// wait for thread to start working
	while (!g_bWorking)
		std::this_thread::yield();
	//
	if (i_nSleep_Length_ms != (size_t)(-1))
	{
		std::chrono::steady_clock::time_point tEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(i_nSleep_Length_ms);
//		std::this_thread::sleep_for(std::chrono::milliseconds(i_nSleep_Length_ms)); // sleep for 250 ms; should be plenty of time to complete the read lock attempt
		while (g_bWorking && std::chrono::steady_clock::now() < tEnd)
			std::this_thread::yield();
		// ensure the thread completed
		if (g_bWorking)
		{
			std::cerr << i_psFault_String << std::endl;
			throw (1);
		}
	}
	cThr.join();
}

// test method for thread that should block due to attempting read locks
void test_lock_blocking_thread(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms = 250)
{
	// test to ensure we can aquire read lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	std::thread cThr(pFn,i_pMutex);
	// wait for thread to start working
	while (!g_bWorking)
		std::this_thread::yield();
	//
	std::chrono::steady_clock::time_point tEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(i_nSleep_Length_ms);
//		std::this_thread::sleep_for(std::chrono::milliseconds(i_nSleep_Length_ms)); // sleep for 250 ms; should be plenty of time to complete the read lock attempt
	while (g_bWorking && std::chrono::steady_clock::now() < tEnd)
		std::this_thread::yield();

	// ensure the thread is blocking
	assert(g_bWorking);

	while (g_bWorking)
	{
		std::this_thread::yield();
		if (i_pMutex->is_read_locked())
			i_pMutex->read_unlock();
		else if (i_pMutex->is_write_locked())
			i_pMutex->write_unlock();
	}
	cThr.join();
	// ensure all locks released
	while (i_pMutex->lock_status() != 0)
	{
		if (i_pMutex->lock_status() > 0 || i_pMutex->lock_status() <= -2)
			i_pMutex->read_unlock();
		else 
			i_pMutex->write_unlock();
	}
}

int main(void)
{
	xstdtsl::read_write_mutex cMutex;
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
}
