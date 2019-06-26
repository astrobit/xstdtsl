#include <xstdtsl_safe_vector>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
#include <xstdtsl_vector_test.hpp>

// function prototype for single mutex test functions

std::atomic_bool g_bWorking(false);


