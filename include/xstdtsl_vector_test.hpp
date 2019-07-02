#pragma once
#include <xstdtsl_safe_vector>
#include <thread>
#include <atomic>
#include <chrono>


///
/// flag to indicate that a test is in progress; used by 
///
extern std::atomic_bool g_bWorking;


