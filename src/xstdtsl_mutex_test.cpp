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
	assert (i_pMutex->is_unlocked());
	g_bWorking = true;
	
	// aquire the read lock
//	uint64_t nMax_Values = (uint64_t)(std::numeric_limits<int>::max()) * 2 - 1;

	int nMin = std::numeric_limits<int>::min();
	int nMax = std::numeric_limits<int>::max();
	std::cout << "set read state to maximum positive read" << std::endl;
	i_pMutex->set_lock_status(nMax);
	is_only_read_locked(i_pMutex);
	std::cout << "test rollover to negative read" << std::endl;
	i_pMutex->read_lock();
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == nMin);
	std::cout << "test increment from most negative read" << std::endl;
	i_pMutex->read_lock();
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == (nMin + 1));
	std::cout << "set read state to limit" << std::endl;
	i_pMutex->set_lock_status(-2);
	is_only_read_locked(i_pMutex);
	std::cout << "test maximum read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == -2);
	std::cout << "test maximum read lock status" << std::endl;
	assert(i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::maximum_read_lock));
	std::cout << "test that a try read fails at read limit" << std::endl;
	assert (!i_pMutex->try_read_lock());
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::maximum_read_lock));
	std::cout << "test that a try write fails at read limit" << std::endl;
	assert (!i_pMutex->try_write_lock());
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->test_lock_status(xstdtsl::read_write_mutex::status::maximum_read_lock));

	std::cout << "test read lock unlock from maximum lock" << std::endl;
	i_pMutex->read_unlock();
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == -3);
	std::cout << "test rollover to positive read" << std::endl;
	i_pMutex->set_lock_status(nMin);
	i_pMutex->read_unlock();
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == nMax);

	std::cout << "test try read at negative rollover (should succeed)" << std::endl;
	i_pMutex->set_lock_status(nMin);
	assert (i_pMutex->try_read_lock());
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == (nMin + 1));
	std::cout << "test try write at negative rollover (should fail)" << std::endl;
	i_pMutex->set_lock_status(nMin);
	assert (!i_pMutex->try_write_lock());
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == nMin);

	std::cout << "test try read at positive rollover (should succeed)" << std::endl;
	i_pMutex->set_lock_status(nMax);
	assert (i_pMutex->try_read_lock());
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == nMin);
	std::cout << "test try write at positive rollover (should fail)" << std::endl;
	i_pMutex->set_lock_status(nMax);
	assert (!i_pMutex->try_write_lock());
	is_only_read_locked(i_pMutex);
	std::cout << "test read lock value" << std::endl;
	assert(i_pMutex->get_lock_status() == nMax);

	std::cout << "set status to unlocked" << std::endl;
	i_pMutex->set_lock_status(0);
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

///
/// aquires write lock, then tries to acquire another read and another write lock; both should fail
///

void try_write_until_blocking(xstdtsl::read_write_mutex * i_pMutex, const std::chrono::steady_clock::time_point & i_tTime)
{
	assert (i_pMutex != nullptr);
	assert (std::chrono::steady_clock::now() < i_tTime);

	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex blocks for a designated time using try_write_lock_until " << std::endl;
	// aquire the write lock
	i_pMutex->write_lock();
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// try to lock for writing -- should fail and block until the designated time
	assert(!i_pMutex->try_write_lock_until(i_tTime));
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// unlock
	i_pMutex->write_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

void try_write_for_blocking(xstdtsl::read_write_mutex * i_pMutex, const std::chrono::milliseconds & i_tTime)
{
	assert (i_pMutex != nullptr);

	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex blocks for a designated time using try_write_lock_for " << std::endl;
	// aquire the write lock
	i_pMutex->write_lock();
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// try to lock for writing -- should fail and block until the designated time
	assert(!i_pMutex->try_write_lock_for(i_tTime));
	// confirm write lock
	is_only_write_locked(i_pMutex);
	// unlock
	i_pMutex->write_unlock();
	/// test to ensure lock correctly released
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

void try_read_until_blocking(xstdtsl::read_write_mutex * i_pMutex, const std::chrono::steady_clock::time_point & i_tTime)
{
	assert (i_pMutex != nullptr);
	assert (std::chrono::steady_clock::now() < i_tTime);

	g_bWorking = true;
	i_pMutex->set_lock_status(-2);
	std::cout << "checking to ensure read_write_mutex blocks for a designated time using try_read_lock_until " << std::endl;
	// try to lock for writing -- should fail and block until the designated time
	assert(!i_pMutex->try_read_lock_until(i_tTime));
	g_bWorking = false;
}
void try_read_for_blocking(xstdtsl::read_write_mutex * i_pMutex, const std::chrono::milliseconds & i_tTime)
{
	assert (i_pMutex != nullptr);

	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex blocks for a designated time using try_read_lock_for " << std::endl;
	i_pMutex->set_lock_status(-2);
	// try to lock for writing -- should fail and block until the designated time
	assert(!i_pMutex->try_read_lock_for(i_tTime));
	g_bWorking = false;
}


void try_read_until_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);

	g_bWorking = true;
	i_pMutex->set_lock_status(0);
	std::cout << "checking to ensure read_write_mutex blocks for a designated time using try_read_lock_until " << std::endl;
	std::chrono::steady_clock::time_point tEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
	// try to lock for writing -- should fail and block until the designated time
	assert(i_pMutex->try_read_lock_until(tEnd));
	// confirm read lock
	is_only_read_locked(i_pMutex);
	i_pMutex->set_lock_status(0);

	g_bWorking = false;
}
void try_read_for_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	std::chrono::milliseconds cTime(50);

	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex doesnt block for a designated time using try_read_lock_for " << std::endl;
	i_pMutex->set_lock_status(0);
	// try to lock for reading -- should succeed and not block
	assert(i_pMutex->try_read_lock_for(cTime));
	// confirm read lock
	is_only_read_locked(i_pMutex);
	// try to lock for reading -- should succeed and not block
	assert(i_pMutex->try_read_lock_for(cTime));
	// confirm read lock
	is_only_read_locked(i_pMutex);
	i_pMutex->set_lock_status(0);
	g_bWorking = false;
}

void try_write_until_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);

	g_bWorking = true;
	i_pMutex->set_lock_status(0);
	std::cout << "checking to ensure read_write_mutex blocks for a designated time using try_write_lock_until " << std::endl;
	std::chrono::steady_clock::time_point tEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
	// try to lock for writing -- should fail and block until the designated time
	assert(i_pMutex->try_write_lock_until(tEnd));
	// confirm read lock
	is_only_write_locked(i_pMutex);
	i_pMutex->set_lock_status(0);

	g_bWorking = false;
}
void try_write_for_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	std::chrono::milliseconds cTime(50);

	g_bWorking = true;
	std::cout << "checking to ensure read_write_mutex doesnt block for a designated time using try_write_lock_for " << std::endl;
	i_pMutex->set_lock_status(0);
	// try to lock for writing -- should fail and block until the designated time
	assert(i_pMutex->try_write_lock_for(cTime));
	// confirm read lock
	is_only_write_locked(i_pMutex);
	i_pMutex->set_lock_status(0);
	g_bWorking = false;
}

void read_lock_guard_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure read_lock_guard locks performs a single read lock  then releases lock when out of scope" << std::endl;
	i_pMutex->set_lock_status(0);
	{
		xstdtsl::read_lock_guard cLock(*i_pMutex);
		// confirm read lock
		is_only_read_locked(i_pMutex);
		assert (i_pMutex->get_lock_status() == 1);
	}
	std::cout << "out of scope of lock guard; ensure unlocked" << std::endl;
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

void read_lock_guard_exception_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	std::cout << "checking to ensure read_lock_guard locks performs a single read lock and releases lock when an exception occurs " << std::endl;
	g_bWorking = true;
	i_pMutex->set_lock_status(0);
	try
	{
		xstdtsl::read_lock_guard cLock(*i_pMutex);
		// confirm read lock
		is_only_read_locked(i_pMutex);
		assert (i_pMutex->get_lock_status() == 1);
		throw 1;
	}
	catch (int i_iExc)
	{
		std::cout << "caught exception" << std::endl;
		if (i_iExc != 1) // some other exception occurred
			throw 1;
	}
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}
void write_lock_guard_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	g_bWorking = true;
	std::cout << "checking to ensure write_lock_guard locks performs a single write lock  then releases lock when out of scope" << std::endl;
	i_pMutex->set_lock_status(0);
	{
		xstdtsl::write_lock_guard cLock(*i_pMutex);
		// confirm read lock
		is_only_write_locked(i_pMutex);
	}
	std::cout << "out of scope of lock guard; ensure unlocked" << std::endl;
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

void write_lock_guard_exception_nonblocking(xstdtsl::read_write_mutex * i_pMutex)
{
	assert (i_pMutex != nullptr);
	std::cout << "checking to ensure write_lock_guard locks performs a single write lock and releases lock when an exception occurs " << std::endl;
	g_bWorking = true;
	i_pMutex->set_lock_status(0);
	try
	{
		xstdtsl::write_lock_guard cLock(*i_pMutex);
		// confirm read lock
		is_only_write_locked(i_pMutex);
		throw 1;
	}
	catch (int i_iExc)
	{
		std::cout << "caught exception" << std::endl;
		if (i_iExc != 1) // some other exception occurred
			throw 1;
	}
	is_unlocked_test(i_pMutex);
	g_bWorking = false;
}

int main(int i_nNum_Params, char * i_pParams[])
{
	xstdtsl::read_write_mutex cMutex;
	std::cout << "checking to ensure read_write_mutex constructor creates an unlocked mutex" << std::endl;
	// test to ensure the mutex is unlocked after the constructor
	is_unlocked_test(&cMutex);
	// test to ensure we can aquire read lock using read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_nonblocking(read_lock,&cMutex,"read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire read lock using try_read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_nonblocking(try_read_lock,&cMutex,"try read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire read lock using try_read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_nonblocking(two_read_lock,&cMutex,"2x read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire read lock using try_read_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_nonblocking(read_lock_limits,&cMutex,"maximum read lock or read unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire write lock using write_lock  when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_nonblocking(write_lock,&cMutex,"write lock or write unlock blocked unnecessarily. terminating." );
	// test to ensure we can aquire write lock usng try_write_lock when unlocked. Use thread to ensure that if a block occurs that it will be broken
	test_lock_nonblocking(try_write_lock,&cMutex,"try write lock or write unlock blocked unnecessarily. terminating." );
	// test to ensure that read and write locks are unavailable after a write lock
	test_lock_nonblocking(write_then_try,&cMutex,"write lock then try write lock or write unlock blocked unnecessarily. terminating." );
	// test to ensure that try_write_for aquires lock when unlocked
	test_lock_nonblocking(try_write_for_nonblocking,&cMutex,"try_write_for blocked unnecessarily. terminating." );
	// test to ensure that try_write_until aquires lock when unlocked
	test_lock_nonblocking(try_write_until_nonblocking,&cMutex,"try_write_until blocked unnecessarily. terminating." );
	// test to ensure that try_read_for aquires lock when unlocked
	test_lock_nonblocking(try_read_for_nonblocking,&cMutex,"try_read_for blocked unnecessarily. terminating." );
	// test to ensure that try_read_until aquires lock when unlocked
	test_lock_nonblocking(try_read_until_nonblocking,&cMutex,"try_read_until blocked unnecessarily. terminating." );

	// blocking tests
	// aquire write then attempt another write
	test_lock_blocking(write_then_write,&cMutex);
	// aquire write then attempt a read
	test_lock_blocking(write_then_read,&cMutex);
	// aquire read then attempt a write
	test_lock_blocking(read_then_write,&cMutex);


	// aquire write when attempt a write until a given time
	test_lock_blocking_until(try_write_until_blocking,&cMutex);
	// aquire write when attempt a write until a given time
	test_lock_blocking_for(try_write_for_blocking,&cMutex);
	// test read until blocking
	test_lock_blocking_until(try_read_until_blocking,&cMutex);
	// test read for blocking
	test_lock_blocking_for(try_read_for_blocking,&cMutex);



	// test to ensure that read_lock_guard aquires a read lock then releases when out of scope
	test_lock_nonblocking(read_lock_guard_nonblocking,&cMutex,"read_lock_guard blocked unnecessarily. terminating." );
	// test to ensure that read_lock_guard aquires a read lock then releases when out of scope when an exception occurs
	test_lock_nonblocking(read_lock_guard_exception_nonblocking,&cMutex,"read_lock_guard blocked unnecessarily when there was an exception. terminating." );
	// test to ensure that write_lock_guard aquires a read lock then releases when out of scope
	test_lock_nonblocking(write_lock_guard_nonblocking,&cMutex,"write_lock_guard blocked unnecessarily. terminating." );
	// test to ensure that write_lock_guard aquires a read lock then releases when out of scope when an exception occurs
	test_lock_nonblocking(write_lock_guard_exception_nonblocking,&cMutex,"write_lock_guard blocked unnecessarily when there was an exception. terminating." );

	//@@TODO: dual_read, dual_write, dual_read_write	

	return 0;	
}
