#include <xstdtsl_safe_binary_tree>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
#include <xstdtsl_binary_tree_test.hpp>

// function prototype for single mutex test functions

std::atomic_bool g_bWorking(false);


