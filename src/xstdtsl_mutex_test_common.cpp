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
void is_unlocked_test(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet) noexcept
{
	if (!i_bQuiet)
		std::cout << "checking to ensure read_write_mutex is unlocked" << std::endl;
	// test to ensure read lock is not set
	assert (!i_pMutex->is_read_locked());
	// test to ensure write lock is not set
	assert (!i_pMutex->is_write_locked());
	// test to ensure the mutex is unlocked
	assert (i_pMutex->is_unlocked());
	// test to ensure the status is 0
	assert (i_pMutex->get_lock_status() == 0);
	// test to ensure the status is unlocked
	assert (i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::unlocked));
	// test to ensure the status is not write locked
	assert (!i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::write_locked));
	// test to ensure the status is not write locked
	assert (!i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::maximum_read_lock));
}
///
/// test to ensure that the mutex is only read locked
///
void is_only_read_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet) noexcept
{
	if (!i_bQuiet)
		std::cout << "checking to ensure read_write_mutex is locked for read only" << std::endl;
	// test to ensure mutex is read locked
	assert (i_pMutex->is_read_locked());
	// test to ensure mutex is not write locked
	assert (!i_pMutex->is_write_locked());
	// test to ensure mutex is not unlocked
	assert (!i_pMutex->is_unlocked());
	// test to ensure state is read lock
	assert (i_pMutex->get_lock_status() > 0 || i_pMutex->get_lock_status() < -1);
	// test to ensure the status is not unlocked
	assert (!i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::unlocked));
	// test to ensure the status is not write locked
	assert (!i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::write_locked));
}
///
/// test to ensure that the mutex is only write locked
///
void is_only_write_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet) noexcept
{
	if (!i_bQuiet)
		std::cout << "checking to ensure read_write_mutex is locked for write only" << std::endl;
	// test to ensure mutex is not read locked
	assert (!i_pMutex->is_read_locked());
	// test to ensure mutex is write locked
	assert (i_pMutex->is_write_locked());
	// test to ensure mutex is not unlocked
	assert (!i_pMutex->is_unlocked());
	// test to ensure constructor leave state as "0"
	assert (i_pMutex->get_lock_status() == -1);
	// test to ensure the status is unlocked
	assert (!i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::unlocked));
	// test to ensure the status is not write locked
	assert (i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::write_locked));
	// test to ensure the status is not write locked
	assert (!i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::maximum_read_lock));
}

void test_lock_nonblocking(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, const char * i_psFault_String, size_t i_nSleep_Length_ms)
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
void test_lock_blocking(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms) noexcept
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
		i_pMutex->set_lock_status(0);
	}
	cThr.join();
	// ensure all locks released
	i_pMutex->set_lock_status(0);
	std::cout << "this test complete and passed" << std::endl;
}


///
/// test method for thread that should block and uses a try_..._until method
///
void test_lock_blocking_until(fn_try_until_t pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms) noexcept
{
	std::chrono::steady_clock::time_point tEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(i_nSleep_Length_ms);
	std::chrono::steady_clock::time_point tEnd2 = std::chrono::steady_clock::now() + std::chrono::milliseconds(i_nSleep_Length_ms * 2);
	// test to ensure we can aquire read lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	std::thread cThr(pFn,i_pMutex,tEnd);
	// wait for thread to start working
	while (!g_bWorking)
		std::this_thread::yield();
	//
//		std::this_thread::sleep_for(std::chrono::milliseconds(i_nSleep_Length_ms)); // sleep for 250 ms; should be plenty of time to complete the read lock attempt
	while (g_bWorking && std::chrono::steady_clock::now() < tEnd)
		std::this_thread::yield();

	// ensure the thread is blocking
	assert(g_bWorking || std::chrono::steady_clock::now() >= tEnd);
	// give the thread longer to complete
	while (g_bWorking && std::chrono::steady_clock::now() < tEnd2)
		std::this_thread::yield();
	// ensure the thread completed and held for the required time
	assert(!g_bWorking && std::chrono::steady_clock::now() >= tEnd);
	//
	cThr.join();
	i_pMutex->set_lock_status(0);
	std::cout << "this test complete and passed" << std::endl;
}


///
/// test method for thread that should block and uses a try_..._for method
///
void test_lock_blocking_for(fn_try_for_t pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms) noexcept
{
	std::chrono::milliseconds tGrace_Dur(i_nSleep_Length_ms * 2);
	std::chrono::milliseconds tDur(i_nSleep_Length_ms);
	std::chrono::steady_clock::time_point tStart = std::chrono::steady_clock::now();
	// test to ensure we can aquire read lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	std::thread cThr(pFn,i_pMutex,tDur);
	// wait for thread to start working
	while (!g_bWorking)
		std::this_thread::yield();
	//
//		std::this_thread::sleep_for(std::chrono::milliseconds(i_nSleep_Length_ms)); // sleep for 250 ms; should be plenty of time to complete the read lock attempt
	while (g_bWorking && (std::chrono::steady_clock::now() < (tStart + tDur)))
		std::this_thread::yield();

	// ensure the thread is blocking
	assert(g_bWorking || std::chrono::steady_clock::now() >= (tStart + tDur));
	// give the thread longer to complete
	while (g_bWorking && std::chrono::steady_clock::now() < (tStart + tGrace_Dur))
		std::this_thread::yield();
	// ensure the thread completed
	assert(!g_bWorking && std::chrono::steady_clock::now() >= (tStart + tDur));
	//
	cThr.join();
	i_pMutex->set_lock_status(0);
	std::cout << "this test complete and passed" << std::endl;
}
