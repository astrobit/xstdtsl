#include <xstdtsl_safe_avl_tree>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
#include <xstdtsl_binary_trees_test_common.hpp>
#include <xstdtsl_avl_tree_test.hpp>




typedef xstdtsl::safe_avl_tree<int> 				tree;
typedef xstdtsl::safe_avl_tree<int>::read_iterator 	read_iter;
typedef xstdtsl::safe_avl_tree<int>::read_control 	read_ctrl;
typedef xstdtsl::safe_avl_tree<int>::write_control 	write_ctrl;


int main(int i_nNum_Params, char * i_pParams[])
{
	std::cout << "--------------=============== safe_avl_tree tests ===============--------------" << std::endl;
	tests::basic_test_suite<tree,read_iter,read_ctrl,write_ctrl>();
	tests::avl_balancing_1<tree,read_iter>();
	tests::avl_balancing_2<tree,read_iter>();

	return 0;	
}
