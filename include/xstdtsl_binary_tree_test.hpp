#pragma once
#include <xstdtsl_safe_binary_tree>
#include <thread>
#include <atomic>
#include <chrono>


///
/// flag to indicate that a test is in progress; used by 
///
extern std::atomic_bool g_bWorking;


