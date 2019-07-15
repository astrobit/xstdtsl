//#include <xstdtsl_safe_binary_tree.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
#include <xstdtsl_binary_trees_test_common.hpp>

#include <xstdtsl_binary_tree_test.hpp>


typedef xstdtsl::safe_binary_tree<int> 				tree;
typedef xstdtsl::safe_binary_tree<int>::read_iterator 	read_iter;
typedef xstdtsl::safe_binary_tree<int>::read_control 	read_ctrl;
typedef xstdtsl::safe_binary_tree<int>::write_control 	write_ctrl;

int main(int i_nNum_Params, char * i_pParams[])
{
	std::cout << "--------------=============== safe_binary_tree tests ===============--------------" << std::endl;

	tests::basic_test_suite<tree,read_iter,read_ctrl,write_ctrl>();
	return 0;	
}
