#pragma once
#ifndef XSTDTSL_BINARY_TREES_TEST_COMMON_H
#define XSTDTSL_BINARY_TREES_TEST_COMMON_H

#include <iostream>
#include <xstdtsl_enums.hpp>

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
namespace tree_read_iterator
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
	template <class read_iterator> void set_iter_position(read_iterator & i_cIter, xstdtsl::binary_trees::start_point i_eStart_Point)
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
		case xstdtsl::binary_trees::start_point::root:
			std::cout << "root";
			break;
		default:
			std::cout << "FAULT";
			break;
		}
		std::cout << std::endl;
		i_cIter.set_position(i_eStart_Point);
	}

	template <class read_iterator> void move_iter_up(read_iterator & i_cIter)
	{
		std::cout << "advance iterator to parent"  << std::endl;
		i_cIter.up();
	}

	template <class read_iterator> void move_iter_right(read_iterator & i_cIter)
	{
		std::cout << "advance iterator to right child"  << std::endl;
		i_cIter.right();
	}

	template <class read_iterator> void move_iter_left(read_iterator & i_cIter)
	{
		std::cout << "advance iterator to left child"  << std::endl;
		i_cIter.left();
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

	template <class read_iterator> void confirm_read_iterator_load(read_iterator & i_cIter, int i_nValue)
	{
		std::cout << "confirm value at current iterator location (" << i_cIter.load() << ") is expected value (" << i_nValue << ")" ;
		test::test(i_cIter.load() == i_nValue);
	}
}

namespace tree_read_control
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
}

namespace tree_tree
{
	template <class treetype> void confirm_has_key(treetype & i_cTree, int i_nValue)
	{
		std::cout << "confirm tree has key (" << i_nValue << ")" ;
		test::test(i_cTree.has_key(i_nValue));
	}
	template <class treetype>void confirm_has_not_key(treetype & i_cTree, int i_nValue)
	{
		std::cout << "confirm tree does not have key (" << i_nValue << ")" ;
		test::test(!i_cTree.has_key(i_nValue));
	}

	template <class treetype>void confirm_is_empty(treetype & i_cTree)
	{
		std::cout << "confirm tree is empty" ;
		test::test(i_cTree.empty());
	}

	template <class treetype>void confirm_is_not_empty(treetype & i_cTree)
	{
		std::cout << "confirm tree is not empty" ;
		test::test(!i_cTree.empty());
	}

	template <class treetype>void insert_element(treetype & i_cTree, int i_nValue)
	{
		std::cout << "insert value (" << i_nValue << ")"  << std::endl;
		i_cTree.insert(i_nValue);
		confirm_has_key<treetype>(i_cTree,i_nValue);
	}

	template <class treetype>void erase_element(treetype & i_cTree, int i_nValue)
	{
		std::cout << "erase value (" << i_nValue << ")"  << std::endl;
		i_cTree.erase(i_nValue);
		confirm_has_not_key<treetype>(i_cTree,i_nValue);
	}

	template <class treetype>void clear_tree(treetype & i_cTree)
	{
		std::cout << "clear tree"  << std::endl;
		i_cTree.clear();
	}
}

namespace tree_write_control
{
	template <class write_control>void confirm_has_key(write_control & i_cWC_Tree, int i_nValue)
	{
		std::cout << "confirm tree has key via write control (" << i_nValue << ")" ;
		test::test(i_cWC_Tree.has_key(i_nValue));
	}
	template <class write_control>void confirm_has_not_key(write_control & i_cWC_Tree, int i_nValue)
	{
		std::cout << "confirm tree does not have key via write control (" << i_nValue << ")" ;
		test::test(!i_cWC_Tree.has_key(i_nValue));
	}

	template <class write_control>void confirm_is_empty(write_control & i_cWC_Tree)
	{
		std::cout << "confirm tree is empty via write control" ;
		test::test(i_cWC_Tree.empty());
	}

	template <class write_control>void confirm_is_not_empty(write_control & i_cWC_Tree)
	{
		std::cout << "confirm tree is not empty via write control" ;
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
		std::cout << "clear tree via write control"  << std::endl;
		i_cWC_Tree.clear();
	}
}

namespace tests
{
	template<class treetype> void construct_destruct(void)
	{
		std::cout << "-----===== contructor test =====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::confirm_is_empty<treetype>(cTI);
		std::cout << "destruct empty tree"  << std::endl;
	}
	template<class treetype> void construct_insert_destruct(void)
	{
		std::cout << "-----===== insertion/has_key test =====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_is_not_empty<treetype>(cTI);
		std::cout << "destruct non-empty tree"  << std::endl;
	}
	template<class treetype> void construct_insert_clear_destruct(void)
	{
		std::cout << "-----===== clear test =====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::clear_tree<treetype>(cTI);
		tree_tree::confirm_is_empty<treetype>(cTI);
		std::cout << "destruct empty tree after clear"  << std::endl;
	}

	template<class treetype> void construct_insert_erase_1_destruct(void)
	{
		std::cout << "-----===== erase test #1 =====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::erase_element<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::erase_element<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,20);
		std::cout << "destruct tree"  << std::endl;
	}
	template<class treetype> void construct_insert_erase_2_destruct(void)
	{
		std::cout << "-----===== erase test #2 (right child of root of large tree)=====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		tree_tree::erase_element<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_not_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		std::cout << "destruct non-empty tree"  << std::endl;
	}
	template<class treetype> void construct_insert_erase_3_destruct(void)
	{
		std::cout << "-----===== erase test #3 (root of large tree)=====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		tree_tree::erase_element<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_not_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		std::cout << "destruct non-empty tree"  << std::endl;
	}
	template<class treetype> void construct_insert_erase_4_destruct(void)
	{
		std::cout << "-----===== erase test #4 (left child of root of large tree)=====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		tree_tree::erase_element<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_not_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		std::cout << "destruct non-empty tree"  << std::endl;
	}
	template<class treetype, class read_iterator> void read_iterator_1(void)
	{
		std::cout << "-----===== read iterator test #1 =====-----"  << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		std::cout << "construct a read iterator starting at the beginning of the tree"  << std::endl;
		read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::beginning);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,5);
		for (int nI = 10; nI < 40; nI+=5)
		{
			tree_read_iterator::move_iter_post_increment<read_iterator>(cIter);
			tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,nI);
		}
		tree_read_iterator::move_iter_post_increment<read_iterator>(cIter);
		tree_read_iterator::confirm_iter_is_not_valid<read_iterator>(cIter);
		tree_read_iterator::set_iter_position<read_iterator>(cIter,xstdtsl::binary_trees::start_point::root);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
		for (int nI = 25; nI < 40; nI+=5)
		{
			tree_read_iterator::move_iter_post_increment<read_iterator>(cIter);
			tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,nI);
		}
		tree_read_iterator::set_iter_position<read_iterator>(cIter,xstdtsl::binary_trees::start_point::root);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		for (int nI = 15; nI > 0; nI-=5)
		{
			tree_read_iterator::move_iter_post_decrement<read_iterator>(cIter);
			tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,nI);
		}
		tree_read_iterator::set_iter_position<read_iterator>(cIter,xstdtsl::binary_trees::start_point::end);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,35);
		for (int nI = 30; nI > 0; nI-=5)
		{
			tree_read_iterator::move_iter_post_decrement<read_iterator>(cIter);
			tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,nI);
		}

		tree_read_iterator::set_iter_position<read_iterator>(cIter,xstdtsl::binary_trees::start_point::root);
		tree_read_iterator::move_iter_right<read_iterator>(cIter);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,30);
		tree_read_iterator::move_iter_up<read_iterator>(cIter);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
		tree_read_iterator::move_iter_left<read_iterator>(cIter);
		tree_read_iterator::confirm_iter_is_valid<read_iterator>(cIter);
		tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
		std::cout << "destruct read iterator"  << std::endl;
	}
	template<class treetype, class read_control> void read_controller_1(void)
	{
		std::cout << "-----===== read controller test #1 =====-----" << std::endl;
		std::cout << "void constructor"  << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		{
			std::cout << "construct read controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			read_control cTIC(cTI);
			tree_read_control::confirm_is_not_empty(cTIC);
			tree_read_control::confirm_has_key<read_control>(cTIC,5);
			tree_read_control::confirm_has_key<read_control>(cTIC,10);
			tree_read_control::confirm_has_key<read_control>(cTIC,15);
			tree_read_control::confirm_has_key<read_control>(cTIC,20);
			tree_read_control::confirm_has_key<read_control>(cTIC,25);
			tree_read_control::confirm_has_key<read_control>(cTIC,30);
			tree_read_control::confirm_has_key<read_control>(cTIC,35);
			tree_read_control::confirm_has_not_key<read_control>(cTIC,0);
			{
				std::cout << "construct read controller for safe_avl_tree with constructor taking a reference to existing read controller" << std::endl;
				read_control cTIC2(cTIC);
				tree_read_control::confirm_is_not_empty<read_control>(cTIC2);
				tree_read_control::confirm_has_key<read_control>(cTIC2,5);
				tree_read_control::confirm_has_key<read_control>(cTIC2,10);
				tree_read_control::confirm_has_key<read_control>(cTIC2,15);
				tree_read_control::confirm_has_key<read_control>(cTIC2,20);
				tree_read_control::confirm_has_key<read_control>(cTIC2,25);
				tree_read_control::confirm_has_key<read_control>(cTIC2,30);
				tree_read_control::confirm_has_key<read_control>(cTIC2,35);
				tree_read_control::confirm_has_not_key<read_control>(cTIC2,0);
				std::cout << "destruct read controller 2" << std::endl;
			}
			std::cout << "destruct read controller 1" << std::endl;
		}
		std::cout << "destruct tree" << std::endl;
	}
	template<class treetype, class write_control> void write_controller_1(void)
	{
		std::cout << "-----===== write controller test #1 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		{
			std::cout << "construct write controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			write_control cTIC(cTI);
			tree_write_control::insert_element<write_control>(cTIC,20);
			tree_write_control::insert_element<write_control>(cTIC,30);
			tree_write_control::insert_element<write_control>(cTIC,10);
			tree_write_control::insert_element<write_control>(cTIC,5);
			tree_write_control::insert_element<write_control>(cTIC,15);
			tree_write_control::insert_element<write_control>(cTIC,25);
			tree_write_control::insert_element<write_control>(cTIC,35);
			tree_write_control::confirm_has_key<write_control>(cTIC,5);
			tree_write_control::confirm_has_key<write_control>(cTIC,10);
			tree_write_control::confirm_has_key<write_control>(cTIC,15);
			tree_write_control::confirm_has_key<write_control>(cTIC,20);
			tree_write_control::confirm_has_key<write_control>(cTIC,25);
			tree_write_control::confirm_has_key<write_control>(cTIC,30);
			tree_write_control::confirm_has_key<write_control>(cTIC,35);
			tree_write_control::confirm_has_not_key<write_control>(cTIC,0);
			std::cout << "destruct write controller" << std::endl;
		}
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		{
			std::cout << "construct another write controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			write_control cTIC(cTI);
			tree_write_control::confirm_has_key<write_control>(cTIC,5);
			tree_write_control::confirm_has_key<write_control>(cTIC,10);
			tree_write_control::confirm_has_key<write_control>(cTIC,15);
			tree_write_control::confirm_has_key<write_control>(cTIC,20);
			tree_write_control::confirm_has_key<write_control>(cTIC,25);
			tree_write_control::confirm_has_key<write_control>(cTIC,30);
			tree_write_control::confirm_has_key<write_control>(cTIC,35);
			tree_write_control::confirm_has_not_key<write_control>(cTIC,0);
			tree_write_control::erase_element<write_control>(cTIC,20);
			tree_write_control::confirm_has_key<write_control>(cTIC,5);
			tree_write_control::confirm_has_key<write_control>(cTIC,10);
			tree_write_control::confirm_has_key<write_control>(cTIC,15);
			tree_write_control::confirm_has_not_key<write_control>(cTIC,20);
			tree_write_control::confirm_has_key<write_control>(cTIC,25);
			tree_write_control::confirm_has_key<write_control>(cTIC,30);
			tree_write_control::confirm_has_key<write_control>(cTIC,35);
			tree_write_control::confirm_has_not_key<write_control>(cTIC,0);
			std::cout << "destruct write controller" << std::endl;
		}
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_not_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		{
			std::cout << "construct another write controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			write_control cTIC(cTI);
			tree_write_control::clear_tree<write_control>(cTIC);
			tree_write_control::confirm_is_empty<write_control>(cTIC);
			tree_write_control::confirm_has_not_key<write_control>(cTIC,20);
			std::cout << "destruct write controller" << std::endl;
		}
		tree_tree::confirm_is_empty<treetype>(cTI);
		std::cout << "destruct tree" << std::endl;
	}

	template<class treetype> void copy_constructor(void)
	{
		std::cout << "-----===== copy constructor tests =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		{
			std::cout << "construct a tree using the copy constructor" << std::endl;
			treetype cTI2(cTI);
			std::cout << "confirm all elements in the duplicate tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI2,5);
			tree_tree::confirm_has_key<treetype>(cTI2,10);
			tree_tree::confirm_has_key<treetype>(cTI2,15);
			tree_tree::confirm_has_key<treetype>(cTI2,20);
			tree_tree::confirm_has_key<treetype>(cTI2,25);
			tree_tree::confirm_has_key<treetype>(cTI2,30);
			tree_tree::confirm_has_key<treetype>(cTI2,35);
			std::cout << "confirm all elements in the original tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI,5);
			tree_tree::confirm_has_key<treetype>(cTI,10);
			tree_tree::confirm_has_key<treetype>(cTI,15);
			tree_tree::confirm_has_key<treetype>(cTI,20);
			tree_tree::confirm_has_key<treetype>(cTI,25);
			tree_tree::confirm_has_key<treetype>(cTI,30);
			tree_tree::confirm_has_key<treetype>(cTI,35);
			std::cout << "erase an element from the duplicate tree" << std::endl;
			tree_tree::erase_element<treetype>(cTI2,20);
			std::cout << "confirm all elements in the duplicate tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI2,5);
			tree_tree::confirm_has_key<treetype>(cTI2,10);
			tree_tree::confirm_has_key<treetype>(cTI2,15);
			tree_tree::confirm_has_not_key<treetype>(cTI2,20);
			tree_tree::confirm_has_key<treetype>(cTI2,25);
			tree_tree::confirm_has_key<treetype>(cTI2,30);
			tree_tree::confirm_has_key<treetype>(cTI2,35);
			std::cout << "confirm all elements in the original tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI,5);
			tree_tree::confirm_has_key<treetype>(cTI,10);
			tree_tree::confirm_has_key<treetype>(cTI,15);
			tree_tree::confirm_has_key<treetype>(cTI,20);
			tree_tree::confirm_has_key<treetype>(cTI,25);
			tree_tree::confirm_has_key<treetype>(cTI,30);
			tree_tree::confirm_has_key<treetype>(cTI,35);
			std::cout << "destruct duplicate tree" << std::endl;
		}
		std::cout << "destruct original tree" << std::endl;
	}
	template<class treetype> void copy_assignment(void)
	{
		std::cout << "-----===== copy via assignement operator tests =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,20);
		tree_tree::insert_element<treetype>(cTI,30);
		tree_tree::insert_element<treetype>(cTI,10);
		tree_tree::insert_element<treetype>(cTI,5);
		tree_tree::insert_element<treetype>(cTI,15);
		tree_tree::insert_element<treetype>(cTI,25);
		tree_tree::insert_element<treetype>(cTI,35);
		{
			std::cout << "construct a tree using the copy constructor" << std::endl;
			treetype cTI2;
			std::cout << "copy original tree using = operator" << std::endl;
			cTI2 = cTI;
			std::cout << "confirm all elements in the duplicate tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI2,5);
			tree_tree::confirm_has_key<treetype>(cTI2,10);
			tree_tree::confirm_has_key<treetype>(cTI2,15);
			tree_tree::confirm_has_key<treetype>(cTI2,20);
			tree_tree::confirm_has_key<treetype>(cTI2,25);
			tree_tree::confirm_has_key<treetype>(cTI2,30);
			tree_tree::confirm_has_key<treetype>(cTI2,35);
			std::cout << "confirm all elements in the original tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI,5);
			tree_tree::confirm_has_key<treetype>(cTI,10);
			tree_tree::confirm_has_key<treetype>(cTI,15);
			tree_tree::confirm_has_key<treetype>(cTI,20);
			tree_tree::confirm_has_key<treetype>(cTI,25);
			tree_tree::confirm_has_key<treetype>(cTI,30);
			tree_tree::confirm_has_key<treetype>(cTI,35);
			std::cout << "erase an element from the duplicate tree" << std::endl;
			tree_tree::erase_element<treetype>(cTI2,20);
			std::cout << "confirm all elements in the duplicate tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI2,5);
			tree_tree::confirm_has_key<treetype>(cTI2,10);
			tree_tree::confirm_has_key<treetype>(cTI2,15);
			tree_tree::confirm_has_not_key<treetype>(cTI2,20);
			tree_tree::confirm_has_key<treetype>(cTI2,25);
			tree_tree::confirm_has_key<treetype>(cTI2,30);
			tree_tree::confirm_has_key<treetype>(cTI2,35);
			std::cout << "confirm all elements in the original tree" << std::endl;
			tree_tree::confirm_has_key<treetype>(cTI,5);
			tree_tree::confirm_has_key<treetype>(cTI,10);
			tree_tree::confirm_has_key<treetype>(cTI,15);
			tree_tree::confirm_has_key<treetype>(cTI,20);
			tree_tree::confirm_has_key<treetype>(cTI,25);
			tree_tree::confirm_has_key<treetype>(cTI,30);
			tree_tree::confirm_has_key<treetype>(cTI,35);
			std::cout << "destruct duplicate tree" << std::endl;
		}
		std::cout << "destruct original tree" << std::endl;
	}

	template<class treetype,class read_iterator> void avl_balancing_1(void)
	{
		std::cout << "-----===== AVL tree balancing tests #1 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,5);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,5);
		}
		tree_tree::insert_element<treetype>(cTI,10);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,5);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
		}
		tree_tree::insert_element<treetype>(cTI,15); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,15);
		}
		tree_tree::insert_element<treetype>(cTI,20);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,15);
		}
		tree_tree::insert_element<treetype>(cTI,25); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
		}
		tree_tree::insert_element<treetype>(cTI,30); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,25);
		}
		tree_tree::insert_element<treetype>(cTI,35); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,30);
		}
		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		std::cout << "destruct tree" << std::endl;
	}
	template<class treetype,class read_iterator> void avl_balancing_2(void)
	{
		std::cout << "-----===== AVL tree balancing tests #2 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		tree_tree::insert_element<treetype>(cTI,35);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
		}
		tree_tree::insert_element<treetype>(cTI,30);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
		}
		tree_tree::insert_element<treetype>(cTI,25); // right rotate
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
		}
		tree_tree::insert_element<treetype>(cTI,20);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
		}
		tree_tree::insert_element<treetype>(cTI,15); // right rotate at 25
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 15);
		}
		tree_tree::insert_element<treetype>(cTI,10); // rotation at root
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 15);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 10);
		}
		tree_tree::insert_element<treetype>(cTI,5); // rotation at 15
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 10);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 5);
		}
		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		std::cout << "destruct tree" << std::endl;
	}

	template<class treetype, class read_iterator> void avl_balancing_3(void)
	{
		std::cout << "-----===== AVL tree balancing tests #3 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		tree_tree::insert_element<treetype>(cTI, 35);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
		}
		tree_tree::insert_element<treetype>(cTI, 30);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
		}
		tree_tree::insert_element<treetype>(cTI, 25); // right rotate
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
		}
		tree_tree::insert_element<treetype>(cTI, 20);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
		}
		tree_tree::insert_element<treetype>(cTI, 15); // right rotate at 25
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 15);
		}
		tree_tree::insert_element<treetype>(cTI, 10); // rotation at root
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 15);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 10);
		}
		tree_tree::insert_element<treetype>(cTI, 5); // rotation at 15
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 10);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 5);
		}
		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI, 5);
		tree_tree::confirm_has_key<treetype>(cTI, 10);
		tree_tree::confirm_has_key<treetype>(cTI, 15);
		tree_tree::confirm_has_key<treetype>(cTI, 20);
		tree_tree::confirm_has_key<treetype>(cTI, 25);
		tree_tree::confirm_has_key<treetype>(cTI, 30);
		tree_tree::confirm_has_key<treetype>(cTI, 35);
		std::cout << "destruct tree" << std::endl;
	}

	template<class treetype,class read_iterator> void rb_balancing_1(void)
	{
		std::cout << "-----===== red-black tree balancing tests #1 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		tree_tree::insert_element<treetype>(cTI,5);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,5);
		}
		tree_tree::insert_element<treetype>(cTI,10);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,5);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
		}
		tree_tree::insert_element<treetype>(cTI,15); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,15);
		}
		tree_tree::insert_element<treetype>(cTI,20);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,15);
		}
		tree_tree::insert_element<treetype>(cTI,25); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
		}
		tree_tree::insert_element<treetype>(cTI,30);
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,25);
		}
		tree_tree::insert_element<treetype>(cTI,35); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,30);
		}
		tree_tree::insert_element<treetype>(cTI,40); // rebalance
		{
			read_iterator cIter(cTI,xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,20);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,30);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter,35);
		}
		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		tree_tree::confirm_has_key<treetype>(cTI,40);
		std::cout << "destruct tree" << std::endl;
	}
	template<class treetype,class read_iterator> void rb_balancing_2(void)
	{
		std::cout << "-----===== red-black tree balancing tests #2 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		tree_tree::insert_element<treetype>(cTI,40);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 40);
		}
		tree_tree::insert_element<treetype>(cTI,35);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 40);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
		}
		tree_tree::insert_element<treetype>(cTI,30); // rebalance
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
		}
		tree_tree::insert_element<treetype>(cTI,25);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
		}
		tree_tree::insert_element<treetype>(cTI,20); // rotation at 30
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
		}
		tree_tree::insert_element<treetype>(cTI,15);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 20);
		}
		tree_tree::insert_element<treetype>(cTI,10); // rotation at 20
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 15);
		}
		tree_tree::insert_element<treetype>(cTI,5); // rotation at root
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 15);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 10);
		}
		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI,5);
		tree_tree::confirm_has_key<treetype>(cTI,10);
		tree_tree::confirm_has_key<treetype>(cTI,15);
		tree_tree::confirm_has_key<treetype>(cTI,20);
		tree_tree::confirm_has_key<treetype>(cTI,25);
		tree_tree::confirm_has_key<treetype>(cTI,30);
		tree_tree::confirm_has_key<treetype>(cTI,35);
		tree_tree::confirm_has_key<treetype>(cTI,40);
		std::cout << "destruct tree" << std::endl;
	}

	template<class treetype, class read_iterator> void rb_balancing_3(void)
	{
		std::cout << "-----===== red-black tree balancing tests #3 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		tree_tree::insert_element<treetype>(cTI, 40);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 40);
		}
		tree_tree::insert_element<treetype>(cTI, 30);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 40);
			tree_read_iterator::move_iter_left<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 30);
		}
		tree_tree::insert_element<treetype>(cTI, 35); // right rotate
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 40);
		}
		tree_tree::insert_element<treetype>(cTI, 37);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 40);
		}
		tree_tree::insert_element<treetype>(cTI, 38);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 35);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 38);
		}

		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI, 30);
		tree_tree::confirm_has_key<treetype>(cTI, 35);
		tree_tree::confirm_has_key<treetype>(cTI, 37);
		tree_tree::confirm_has_key<treetype>(cTI, 38);
		tree_tree::confirm_has_key<treetype>(cTI, 40);
		std::cout << "destruct tree" << std::endl;
	}
	template<class treetype, class read_iterator> void rb_balancing_4(void)
	{
		std::cout << "-----===== red-black tree balancing tests #4 =====-----" << std::endl;
		std::cout << "void constructor" << std::endl;
		treetype cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		tree_tree::insert_element<treetype>(cTI, 5);
		tree_tree::insert_element<treetype>(cTI, 10);
		tree_tree::insert_element<treetype>(cTI, 15);
		tree_tree::insert_element<treetype>(cTI, 20);
		tree_tree::insert_element<treetype>(cTI, 25);
		tree_tree::insert_element<treetype>(cTI, 30);

		tree_tree::erase_element<treetype>(cTI, 20);
		{
			read_iterator cIter(cTI, xstdtsl::binary_trees::start_point::root);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 10);
			tree_read_iterator::move_iter_right<read_iterator>(cIter);
			tree_read_iterator::confirm_read_iterator_load<read_iterator>(cIter, 25);
		}

		std::cout << "confirm all elements in the tree" << std::endl;
		tree_tree::confirm_has_key<treetype>(cTI, 5);
		tree_tree::confirm_has_key<treetype>(cTI, 10);
		tree_tree::confirm_has_key<treetype>(cTI, 15);
		tree_tree::confirm_has_key<treetype>(cTI, 25);
		tree_tree::confirm_has_key<treetype>(cTI, 30);
		std::cout << "destruct tree" << std::endl;
	}
	template<class treetype, class read_iterator, class read_control, class write_control> void basic_test_suite(void)
	{
		construct_destruct<treetype>();
		construct_insert_destruct<treetype>();
		construct_insert_clear_destruct<treetype>();
		construct_insert_erase_1_destruct<treetype>();
		construct_insert_erase_2_destruct<treetype>();
		construct_insert_erase_3_destruct<treetype>();
		construct_insert_erase_4_destruct<treetype>();
		read_iterator_1<treetype,read_iterator>();
		read_controller_1<treetype,read_control>();
		write_controller_1<treetype,write_control>();
		copy_constructor<treetype>();
		copy_assignment<treetype>();
	}
}
#endif //#ifndef XSTDTSL_BINARY_TREES_TEST_COMMON_H

