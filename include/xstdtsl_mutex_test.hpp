#pragma once
#include <xstdtsl_mutex>
#include <thread>
#include <atomic>
#include <chrono>

///
/// function prototype for single mutex test functions
///
typedef void (*test_fn_single_mutex) (xstdtsl::read_write_mutex *);
typedef void (*fn_try_until_t)(xstdtsl::read_write_mutex * i_pMutex, const std::chrono::steady_clock::time_point & i_tTime);
typedef void (*fn_try_for_t)(xstdtsl::read_write_mutex * i_pMutex, const std::chrono::milliseconds & i_tDuration);


///
/// flag to indicate that a test is in progress; used by 
///
extern std::atomic_bool g_bWorking;

///
/// test to ensure that the mutex is fully unlocked
///
extern void is_unlocked_test(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet = false) noexcept;
///
/// test to ensure that the mutex is only read locked
///
extern void is_only_read_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet = false) noexcept;
///
/// test to ensure that the mutex is only write locked
///
extern void is_only_write_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet = false) noexcept;

///
/// wrapper to test a lock; throws an exception after some time (default = 250 ms) if the testing function blocks or locks
///
extern void test_lock_nonblocking(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, const char * i_psFault_String, size_t i_nSleep_Length_ms = 250);

///
/// wrapper to test a lock that blocks due to a read or write lock; automatically starts releasing locks after a designated time (default = 250 ms)
///
extern void test_lock_blocking(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms = 250) noexcept;

///
/// test method for thread that should block and uses a try_..._for method
///
extern void test_lock_blocking_for(fn_try_for_t pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms = 250) noexcept;

///
/// test method for thread that should block and uses a try_..._until method
///
extern void test_lock_blocking_until(fn_try_until_t pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms = 250) noexcept;


