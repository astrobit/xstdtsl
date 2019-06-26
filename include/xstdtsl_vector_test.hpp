#pragma once
#include <xstdtsl_mutex>
#include <thread>
#include <atomic>
#include <chrono>


///
/// flag to indicate that a test is in progress; used by 
///
extern std::atomic_bool g_bWorking;


