#include <xstdtsl_safe_map.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>
//#include <xstdtsl_map_test.hpp>




typedef xstdtsl::safe_map<int,int> 					map;
typedef xstdtsl::safe_map<int,int>::read_iterator 	read_iter;
typedef xstdtsl::safe_map<int,int>::read_control 	read_ctrl;
typedef xstdtsl::safe_map<int,int>::write_control 	write_ctrl;

namespace test
{
	void test(bool i_bResult)
	{
		if (i_bResult)
			std::cout << " ... PASS" << std::endl;
		else
		{
			std::cout << " ... FAIL" << std::endl;
			exit(1);
		}
	}
}
namespace test_map_read_iterator
{
	template <class read_iterator> void confirm_iter_is_valid(read_iterator & i_cIter)
	{
		std::cout << "confirm iterator is valid" ;
		test::test(i_cIter.is_valid());
	}
	template <class read_iterator> void confirm_iter_is_not_valid(read_iterator & i_cIter)
	{
		std::cout << "confirm iterator is not valid" ;
		test::test(!i_cIter.is_valid());
	}
	template <class read_iterator> void set_iter_position(read_iterator & i_cIter, xstdtsl::maps::start_point i_eStart_Point)
	{
		std::cout << "set iterator position to ";
		switch (i_eStart_Point)
		{
		case xstdtsl::binary_trees::start_point::beginning:
			std::cout << "beginning";
			break;
		case xstdtsl::binary_trees::start_point::end:
			std::cout << "end";
			break;
		default:
			std::cout << "FAULT";
			break;
		}
		std::cout << std::endl;
		i_cIter.set_position(i_eStart_Point);
	}

	template <class read_iterator> void move_iter_post_increment(read_iterator & i_cIter)
	{
		std::cout << "advance iterator (post-increment)"  << std::endl;
		i_cIter++;
	}

	template <class read_iterator> void move_iter_post_decrement(read_iterator & i_cIter)
	{
		std::cout << "advance iterator (post-decrement)"  << std::endl;
		i_cIter--;
	}
	/*
	template <class read_iterator> void move_iter_pre_increment(read_iterator & i_cIter)
	{
		std::cout << "advance iterator (pre-increment)"  << std::endl;
		++i_cIter;
	}

	template <class read_iterator> void move_iter_pre_decrement(read_iterator & i_cIter)
	{
		std::cout << "advance iterator (pre-decrement)"  << std::endl;
		--i_cIter;
	}
	*/
	template <class read_iterator> void move_iter_pluseq(read_iterator & i_cIter, int i_nDistance)
	{
		std::cout << "advance iterator (+= " << i_nDistance << ")" << std::endl;
		i_cIter += i_nDistance;
	}

	template <class read_iterator> void move_iter_minuseq(read_iterator & i_cIter, int i_nDistance)
	{
		std::cout << "advance iterator (-= " << i_nDistance << ")" << std::endl;
		i_cIter -= i_nDistance;
	}

	template <class read_iterator> void confirm_read_iterator_key(read_iterator & i_cIter, int i_nKey)
	{
		std::cout << "confirm key at current iterator location (" << i_cIter.get_key() << ") is expected value (" << i_nKey << ")" ;
		test::test(i_cIter.get_key() == i_nKey);
	}

	template <class read_iterator> void confirm_read_iterator_value(read_iterator & i_cIter, int i_nValue)
	{
		std::cout << "confirm value at current iterator location (" << i_cIter.get_value() << ") is expected value (" << i_nValue << ")" ;
		test::test(i_cIter.get_value() == i_nValue);
	}
}

namespace test_map_read_control
{
	template <class read_control> void confirm_has_key(read_control & i_cRC_Tree, int i_nValue)
	{
		std::cout << "confirm read control has key (" << i_nValue << ")" ;
		test::test(i_cRC_Tree.has_key(i_nValue));
	}
	template <class read_control> void confirm_has_not_key(read_control & i_cRC_Tree, int i_nValue)
	{
		std::cout << "confirm read control does not have key (" << i_nValue << ")" ;
		test::test(!i_cRC_Tree.has_key(i_nValue));
	}

	template <class read_control> void confirm_is_empty(read_control & i_cRC_Tree)
	{
		std::cout << "confirm read control is empty" ;
		test::test(i_cRC_Tree.empty());
	}

	template <class read_control> void confirm_is_not_empty(read_control & i_cRC_Tree)
	{
		std::cout << "confirm read control is not empty" ;
		test::test(!i_cRC_Tree.empty());
	}
	template <class read_control> void confirm_at(read_control & i_cRC_Tree, int i_nKey, int i_nValue)
	{
		std::cout << "confirm read control has value (" << i_nValue << ") at key (" << i_nKey << ")";
		test::test(i_cRC_Tree.at(i_nKey) == i_nValue);
	}
}

namespace test_map
{
	template <class map> void confirm_has_key(map & i_cTree, int i_nValue)
	{
		std::cout << "confirm map has key (" << i_nValue << ")" ;
		test::test(i_cTree.has_key(i_nValue));
	}
	template <class map>void confirm_has_not_key(map & i_cTree, int i_nValue)
	{
		std::cout << "confirm map does not have key (" << i_nValue << ")" ;
		test::test(!i_cTree.has_key(i_nValue));
	}
	template <class map> void confirm_has_value(map & i_cTree, int i_nKey, int i_nValue)
	{
		std::cout << "confirm map has value (" << i_nValue << ") at key (" << i_nKey << ")" ;
		test::test(i_cTree.at(i_nKey) == i_nValue);
	}

	template <class map>void confirm_is_empty(map & i_cTree)
	{
		std::cout << "confirm map is empty" ;
		test::test(i_cTree.empty());
	}

	template <class map>void confirm_is_not_empty(map & i_cTree)
	{
		std::cout << "confirm map is not empty" ;
		test::test(!i_cTree.empty());
	}

	template <class map>void insert_element(map & i_cTree, int i_nKey, int i_nValue)
	{
		std::cout << "insert value (" << i_nValue << ") at ("  << i_nKey << ")" << std::endl;
		i_cTree.insert(i_nKey,i_nValue);
		confirm_has_key<map>(i_cTree,i_nKey);
		confirm_has_value<map>(i_cTree,i_nKey,i_nValue);
	}

	template <class map>void store_element(map & i_cTree, int i_nKey, int i_nValue)
	{
		std::cout << "store value (" << i_nValue << ") at ("  << i_nKey << ")" << std::endl;
		i_cTree.store(i_nKey,i_nValue);
		confirm_has_key<map>(i_cTree,i_nKey);
		confirm_has_value<map>(i_cTree,i_nKey,i_nValue);
	}

	template <class map>void erase_element(map & i_cTree, int i_nValue)
	{
		std::cout << "erase value (" << i_nValue << ")"  << std::endl;
		i_cTree.erase(i_nValue);
		confirm_has_not_key<map>(i_cTree,i_nValue);
	}

	template <class map>void clear_map(map & i_cTree)
	{
		std::cout << "clear map"  << std::endl;
		i_cTree.clear();
	}
	template <class map> void confirm_at(map & i_cTree, int i_nKey, int i_nValue)
	{
		std::cout << "confirm map has value (" << i_nValue << ") at key (" << i_nKey << ")";
		test::test(i_cTree.at(i_nKey) == i_nValue);
	}
}

namespace test_map_write_control
{
	template <class write_control>void confirm_has_key(write_control & i_cWC_Tree, int i_nValue)
	{
		std::cout << "confirm map has key via write control (" << i_nValue << ")" ;
		test::test(i_cWC_Tree.has_key(i_nValue));
	}
	template <class write_control>void confirm_has_not_key(write_control & i_cWC_Tree, int i_nValue)
	{
		std::cout << "confirm map does not have key via write control (" << i_nValue << ")" ;
		test::test(!i_cWC_Tree.has_key(i_nValue));
	}

	template <class write_control>void confirm_is_empty(write_control & i_cWC_Tree)
	{
		std::cout << "confirm map is empty via write control" ;
		test::test(i_cWC_Tree.empty());
	}

	template <class write_control>void confirm_is_not_empty(write_control & i_cWC_Tree)
	{
		std::cout << "confirm map is not empty via write control" ;
		test::test(!i_cWC_Tree.empty());
	}

	template <class write_control>void insert_element(write_control & i_cWC_Tree, int i_nValue)
	{
		std::cout << "insert value via write control (" << i_nValue << ")"  << std::endl;
		i_cWC_Tree.insert(i_nValue);
		confirm_has_key<write_control>(i_cWC_Tree,i_nValue);
	}

	template <class write_control>void erase_element(write_control & i_cWC_Tree, int i_nValue)
	{
		std::cout << "erase value via write control (" << i_nValue << ")"  << std::endl;
		i_cWC_Tree.erase(i_nValue);
		confirm_has_not_key<write_control>(i_cWC_Tree,i_nValue);
	}

	template <class write_control>void clear_tree(write_control & i_cWC_Tree)
	{
		std::cout << "clear map via write control"  << std::endl;
		i_cWC_Tree.clear();
	}
}

int main(int i_nNum_Params, char * i_pParams[])
{
	std::cout << "--------------=============== safe_map tests ===============--------------" << std::endl;
	{
		map cMap;
	}
	{
		map cMap;
		test_map::insert_element(cMap,1,1);
		test_map::insert_element(cMap,2,2);
		test_map::store_element(cMap,1,3);
		test_map::store_element(cMap,3,4);
		test_map::erase_element(cMap,3);
		test_map::store_element(cMap,3,5);

	}

	return 0;	
}
