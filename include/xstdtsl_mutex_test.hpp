#pragma once
#include <xstdtsl_mutex>
#include <thread>
#include <atomic>
#include <chrono>
// function prototype for single mutex test functions
typedef void (*test_fn_single_mutex) (xstdtsl::read_write_mutex *);
// function prototype for single mutex test functions

extern std::atomic_bool g_bWorking;

///
/// test to ensure that the mutex is fully unlocked
///
extern void is_unlocked_test(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet = false);
///
/// test to ensure that the mutex is only read locked
///
extern void is_only_read_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet = false);
///
/// test to ensure that the mutex is only write locked
///
extern void is_only_write_locked(xstdtsl::read_write_mutex * i_pMutex, bool i_bQuiet = false);

void test_lock_thread_safe(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, const char * i_psFault_String, size_t i_nSleep_Length_ms = 250);

// test method for thread that should block due to attempting read locks
void test_lock_blocking_thread(test_fn_single_mutex pFn, xstdtsl::read_write_mutex * i_pMutex, size_t i_nSleep_Length_ms = 250);

