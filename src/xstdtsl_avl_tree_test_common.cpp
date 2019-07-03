#include <xstdtsl_safe_avl_tree>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
#include <xstdtsl_avl_tree_test.hpp>

// function prototype for single mutex test functions

std::atomic_bool g_bWorking(false);


