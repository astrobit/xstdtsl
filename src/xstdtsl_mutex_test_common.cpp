#include <xstdtsl_mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
#include <xstdtsl_mutex_test.hpp>

// function prototype for single mutex test functions
typedef void (*test_fn_single_mutex) (xstdtsl::read_write_mutex *);
// function prototype for single mutex test functions

std::atomic_bool g_bWorking(false);

///
/// test to ensure that the mutex is fully unlocked
///
void is_unlocked_test(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet)
{
	if (!i_bQuiet)
		std::cout << "checking to ensure read_write_mutex is unlocked" << std::endl;
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
void is_only_read_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet)
{
	if (!i_bQuiet)
		std::cout << "checking to ensure read_write_mutex is locked for read only" << std::endl;
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
void is_only_write_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet)
{
	if (!i_bQuiet)
		std::cout << "checking to ensure read_write_mutex is locked for write only" << std::endl;
	// test to ensure constructor does not lock the mutex for read
	assert (!i_pMutex->is_read_locked());
	// test to ensure constructor does not lock the mutex for write
	assert (i_pMutex->is_write_locked());
	// test to ensure constructor leaves mutex unlocked
	assert (!i_pMutex->is_unlocked());
	// test to ensure constructor leave state as "0"
	assert (i_pMutex->lock_status() == -1);
}

void test_lock_thread_safe(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, const char * i_psFault_String, size_t i_nSleep_Length_ms)
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
	std::cout << "this test complete and passed" << std::endl;
}

// test method for thread that should block due to attempting read locks
void test_lock_blocking_thread(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms)
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
	std::cout << "this test complete and passed" << std::endl;
}
