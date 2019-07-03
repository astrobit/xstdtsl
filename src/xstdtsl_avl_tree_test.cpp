#include <xstdtsl_safe_avl_tree>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>

#include <xstdtsl_avl_tree_test.hpp>


int main(int i_nNum_Params, char * i_pParams[])
{
	std::cout << "--------------=============== safe_vector tests ===============--------------" << std::endl;

	std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
	{
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "confirm tree is empty" << std::endl;
		assert(cTI.empty());
		std::cout << "destruct empty tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element" << std::endl;
		cTI.insert(2);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(2));
		std::cout << "insert an element" << std::endl;
		cTI.insert(3);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(3));
		std::cout << "insert an element" << std::endl;
		cTI.insert(1);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(1));
		std::cout << "confirm tree is not empty" << std::endl;
		assert(!cTI.empty());
		std::cout << "destruct non-empty tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element" << std::endl;
		cTI.insert(2);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(2));
		std::cout << "insert an element" << std::endl;
		cTI.insert(3);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(3));
		std::cout << "insert an element" << std::endl;
		cTI.insert(1);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(1));
		std::cout << "clear the tree" << std::endl;
		cTI.clear();
		std::cout << "confirm tree is empty" << std::endl;
		assert(cTI.empty());
		std::cout << "destruct empty tree" << std::endl;
	}

	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element" << std::endl;
		cTI.insert(20);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(20));
		std::cout << "insert an element" << std::endl;
		cTI.insert(30);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(30));
		std::cout << "insert an element" << std::endl;
		cTI.insert(10);
		std::cout << "confirm tree has the key" << std::endl;
		assert(cTI.has_key(10));
		std::cout << "erase a leaf node" << std::endl;
		cTI.erase(10);
		std::cout << "confirm other nodes havent been lost" << std::endl;
		assert(cTI.has_key(20));
		assert(cTI.has_key(30));
		std::cout << "confirm node was erased" << std::endl;
		assert(!cTI.has_key(10));
		std::cout << "re-insert erased node" << std::endl;
		cTI.insert(10);
		std::cout << "confirm node exists" << std::endl;
		assert(cTI.has_key(10));
		std::cout << "erase root node" << std::endl;
		cTI.erase(20);
		std::cout << "confirm other nodes havent been lost" << std::endl;
		assert(cTI.has_key(10));
		assert(cTI.has_key(30));
		std::cout << "confirm root was erased" << std::endl;
		assert(!cTI.has_key(20));
		std::cout << "re-insert erased node" << std::endl;
		cTI.insert(20);
		std::cout << "confirm node exists" << std::endl;
		assert(cTI.has_key(20));
		
		std::cout << "destruct non-empty tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "confirm all elements are in the tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		std::cout << "erase right child of root" << std::endl;
		cTI.erase(30);		
		std::cout << "confirm correct contents of tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(!cTI.has_key(30));
		assert(cTI.has_key(35));
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "confirm all elements are in the tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		std::cout << "erase root" << std::endl;
		cTI.erase(20);		
		std::cout << "confirm correct contents of tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(!cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "confirm all elements are in the tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		std::cout << "erase left child of root" << std::endl;
		cTI.erase(10);		
		std::cout << "confirm correct contents of tree" << std::endl;
		assert(cTI.has_key(5));
		assert(!cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "confirm all elements are in the tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		std::cout << "construct a read iterator starting at the beginning of the tree" << std::endl;
		xstdtsl::safe_avl_tree<int>::read_iterator cIter(cTI,xstdtsl::safe_avl_tree<int>::iterator_base::start_point::beginning);
		std::cout << "confirm iterator is valid" << std::endl;
		assert(cIter.is_valid());
		std::cout << "confirm correct value for beginning of tree" << std::endl;
		assert(cIter.load() == 5);
		for (int nI = 10; nI < 40; nI+=5)
		{
			std::cout << "advance iterator using post-increment" << std::endl;
			cIter++;
			std::cout << "confirm iterator is valid" << std::endl;
			assert(cIter.is_valid());
			std::cout << "confirm correct value of iterator (" << nI << ")" << std::endl;
			assert(cIter.load() == nI);
		}
		std::cout << "advance iterator using post-increment" << std::endl;
		cIter++;
		std::cout << "confirm iterator is not valid" << std::endl;
		assert(!cIter.is_valid());
		std::cout << "set iterator to root" << std::endl;
		cIter.set_position(xstdtsl::safe_avl_tree<int>::iterator_base::start_point::root);
		std::cout << "confirm iterator is valid" << std::endl;
		assert(cIter.is_valid());
		std::cout << "confirm correct value for root of tree" << std::endl;
		assert(cIter.load() == 20);
		for (int nI = 25; nI < 40; nI+=5)
		{
			std::cout << "advance iterator using post-increment" << std::endl;
			cIter++;
			std::cout << "confirm iterator is valid" << std::endl;
			assert(cIter.is_valid());
			std::cout << "confirm correct value of iterator (" << nI << ")" << std::endl;
			assert(cIter.load() == nI);
		}
		std::cout << "set iterator to root" << std::endl;
		cIter.set_position(xstdtsl::safe_avl_tree<int>::iterator_base::start_point::root);
		std::cout << "confirm iterator is valid" << std::endl;
		assert(cIter.is_valid());
		std::cout << "confirm correct value for root of tree" << std::endl;
		assert(cIter.load() == 20);
		for (int nI = 15; nI > 0; nI-=5)
		{
			std::cout << "advance iterator using post-decrement" << std::endl;
			cIter--;
			std::cout << "confirm iterator is valid" << std::endl;
			assert(cIter.is_valid());
			std::cout << "confirm correct value of iterator (" << nI << ")" << std::endl;
			assert(cIter.load() == nI);
		}
		std::cout << "set iterator to end" << std::endl;
		cIter.set_position(xstdtsl::safe_avl_tree<int>::iterator_base::start_point::end);
		std::cout << "confirm iterator is valid" << std::endl;
		assert(cIter.is_valid());
		std::cout << "confirm correct value for end of tree" << std::endl;
		assert(cIter.load() == 35);
		for (int nI = 30; nI > 0; nI-=5)
		{
			std::cout << "advance iterator using post-decrement" << std::endl;
			cIter--;
			std::cout << "confirm iterator is valid" << std::endl;
			assert(cIter.is_valid());
			std::cout << "confirm correct value of iterator (" << nI << ")" << std::endl;
			assert(cIter.load() == nI);
		}

		std::cout << "set iterator to root" << std::endl;
		cIter.set_position(xstdtsl::safe_avl_tree<int>::iterator_base::start_point::root);
		std::cout << "confirm iterator is valid" << std::endl;
		assert(cIter.is_valid());
		std::cout << "confirm correct value for end of tree" << std::endl;
		assert(cIter.load() == 20);
		std::cout << "traverse right" << std::endl;
		cIter.right();
		std::cout << "confirm correct value" << std::endl;
		assert(cIter.load() == 30);
		std::cout << "traverse up" << std::endl;
		cIter.up();
		std::cout << "confirm correct value" << std::endl;
		assert(cIter.load() == 20);
		std::cout << "traverse left" << std::endl;
		cIter.left();
		std::cout << "confirm correct value" << std::endl;
		assert(cIter.load() == 10);

	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "confirm all elements are in the tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		{
			std::cout << "construct read controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			xstdtsl::safe_avl_tree<int>::read_control cTIC(cTI);

			std::cout << "confirm read controller reports empty" << std::endl;
			assert(!cTIC.empty());
			std::cout << "confirm read controller reports existance of all keys in tree" << std::endl;
			assert(cTIC.has_key(5));
			assert(cTIC.has_key(10));
			assert(cTIC.has_key(15));
			assert(cTIC.has_key(20));
			assert(cTIC.has_key(25));
			assert(cTIC.has_key(30));
			assert(cTIC.has_key(35));
			std::cout << "confirm read controller doesn't have nonexistent data" << std::endl;
			assert(!cTIC.has_key(0));
			{
				std::cout << "construct read controller for safe_avl_tree with constructor taking a reference to existing read controller" << std::endl;
				xstdtsl::safe_avl_tree<int>::read_control cTIC2(cTIC);
				std::cout << "confirm read controller reports empty" << std::endl;
				assert(!cTIC2.empty());
				std::cout << "confirm read controller reports existance of all keys in tree" << std::endl;
				assert(cTIC2.has_key(5));
				assert(cTIC2.has_key(10));
				assert(cTIC2.has_key(15));
				assert(cTIC2.has_key(20));
				assert(cTIC2.has_key(25));
				assert(cTIC2.has_key(30));
				assert(cTIC2.has_key(35));
				std::cout << "confirm read controller doesn't have nonexistent data" << std::endl;
				assert(!cTIC2.has_key(0));
				std::cout << "destruct read controller 2" << std::endl;
			}
				std::cout << "destruct read controller 1" << std::endl;
		}
		std::cout << "destruct tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		{
			std::cout << "construct write controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			xstdtsl::safe_avl_tree<int>::write_control cTIC(cTI);
			std::cout << "insert an element (root)" << std::endl;
			cTIC.insert(20);
			std::cout << "insert an element (right child)" << std::endl;
			cTIC.insert(30);
			std::cout << "insert an element (left child)" << std::endl;
			cTIC.insert(10);
			std::cout << "insert an element (left of left child)" << std::endl;
			cTIC.insert(5);
			std::cout << "insert an element (right of left child)" << std::endl;
			cTIC.insert(15);
			std::cout << "insert an element (left of right child)" << std::endl;
			cTIC.insert(25);
			std::cout << "insert an element (right of right child)" << std::endl;
			cTIC.insert(35);
			std::cout << "confirm all elements are in the tree controller" << std::endl;
			assert(cTIC.has_key(5));
			assert(cTIC.has_key(10));
			assert(cTIC.has_key(15));
			assert(cTIC.has_key(20));
			assert(cTIC.has_key(25));
			assert(cTIC.has_key(30));
			assert(cTIC.has_key(35));
			std::cout << "destruct write controller" << std::endl;
		}
		std::cout << "confirm all elements are in the tree" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		{
			std::cout << "construct another write controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			xstdtsl::safe_avl_tree<int>::write_control cTIC(cTI);
			std::cout << "confirm all elements are in the tree controller" << std::endl;
			assert(cTIC.has_key(5));
			assert(cTIC.has_key(10));
			assert(cTIC.has_key(15));
			assert(cTIC.has_key(20));
			assert(cTIC.has_key(25));
			assert(cTIC.has_key(30));
			assert(cTIC.has_key(35));
			std::cout << "erase an element" << std::endl;
			cTIC.erase(20);
			std::cout << "confirm all elements are in the tree controller except the erased one" << std::endl;
			assert(cTIC.has_key(5));
			assert(cTIC.has_key(10));
			assert(cTIC.has_key(15));
			assert(!cTIC.has_key(20));
			assert(cTIC.has_key(25));
			assert(cTIC.has_key(30));
			assert(cTIC.has_key(35));
			std::cout << "destruct write controller" << std::endl;
		}
		std::cout << "confirm all elements are in the tree except the erased one" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(!cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		{
			std::cout << "construct another write controller for safe_avl_tree with constructor taking a reference to a tree" << std::endl;
			xstdtsl::safe_avl_tree<int>::write_control cTIC(cTI);
			std::cout << "clear the tree" << std::endl;
			cTIC.clear();
			std::cout << "confirm all tree is empty" << std::endl;
			assert(cTIC.empty());
			assert(!cTIC.has_key(5));
			std::cout << "destruct write controller" << std::endl;
		}
		std::cout << "confirm tree is empty" << std::endl;
		assert(cTI.empty());
		std::cout << "destruct tree" << std::endl;
	}

	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "construct a tree using the copy constructor" << std::endl;
		{
			xstdtsl::safe_avl_tree<int> cTI2(cTI);
			std::cout << "confirm all elements in the original tree" << std::endl;
			assert(cTI.has_key(5));
			assert(cTI.has_key(10));
			assert(cTI.has_key(15));
			assert(cTI.has_key(20));
			assert(cTI.has_key(25));
			assert(cTI.has_key(30));
			assert(cTI.has_key(35));
			std::cout << "confirm all elements in the duplicate tree" << std::endl;
			assert(cTI2.has_key(5));
			assert(cTI2.has_key(10));
			assert(cTI2.has_key(15));
			assert(cTI2.has_key(20));
			assert(cTI2.has_key(25));
			assert(cTI2.has_key(30));
			assert(cTI2.has_key(35));
			std::cout << "erase an element from the duplicate tree" << std::endl;
			cTI2.erase(20);
			std::cout << "confirm all elements in the original tree" << std::endl;
			assert(cTI.has_key(5));
			assert(cTI.has_key(10));
			assert(cTI.has_key(15));
			assert(cTI.has_key(20));
			assert(cTI.has_key(25));
			assert(cTI.has_key(30));
			assert(cTI.has_key(35));
			std::cout << "confirm all elements in the duplicate tree except the erased one" << std::endl;
			assert(cTI2.has_key(5));
			assert(cTI2.has_key(10));
			assert(cTI2.has_key(15));
			assert(!cTI2.has_key(20));
			assert(cTI2.has_key(25));
			assert(cTI2.has_key(30));
			assert(cTI2.has_key(35));
			std::cout << "destruct duplicate tree" << std::endl;
		}
		std::cout << "destruct original tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "insert an element (root)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (right child)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (left child)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (left of left child)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (right of left child)" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (left of right child)" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (right of right child)" << std::endl;
		cTI.insert(35);
		std::cout << "construct a tree using the default constructor" << std::endl;
		{
			xstdtsl::safe_avl_tree<int> cTI2;
			std::cout << "copy original tree using = operator" << std::endl;
			cTI2 = cTI;
			std::cout << "confirm all elements in the original tree" << std::endl;
			assert(cTI.has_key(5));
			assert(cTI.has_key(10));
			assert(cTI.has_key(15));
			assert(cTI.has_key(20));
			assert(cTI.has_key(25));
			assert(cTI.has_key(30));
			assert(cTI.has_key(35));
			std::cout << "confirm all elements in the duplicate tree" << std::endl;
			assert(cTI2.has_key(5));
			assert(cTI2.has_key(10));
			assert(cTI2.has_key(15));
			assert(cTI2.has_key(20));
			assert(cTI2.has_key(25));
			assert(cTI2.has_key(30));
			assert(cTI2.has_key(35));
			std::cout << "erase an element from the duplicate tree" << std::endl;
			cTI2.erase(20);
			std::cout << "confirm all elements in the original tree" << std::endl;
			assert(cTI.has_key(5));
			assert(cTI.has_key(10));
			assert(cTI.has_key(15));
			assert(cTI.has_key(20));
			assert(cTI.has_key(25));
			assert(cTI.has_key(30));
			assert(cTI.has_key(35));
			std::cout << "confirm all elements in the duplicate tree except the erased one" << std::endl;
			assert(cTI2.has_key(5));
			assert(cTI2.has_key(10));
			assert(cTI2.has_key(15));
			assert(!cTI2.has_key(20));
			assert(cTI2.has_key(25));
			assert(cTI2.has_key(30));
			assert(cTI2.has_key(35));
			std::cout << "destruct duplicate tree" << std::endl;
		}
		std::cout << "destruct original tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		std::cout << "insert an element (5)" << std::endl;
		cTI.insert(5);
		std::cout << "insert an element (10)" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (15) --- should result in rebalance" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (20)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (25) --- should result in rebalance" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (30) --- should result in rebalance" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (35) --- should result in rebalance" << std::endl;
		cTI.insert(35);
		std::cout << "confirm all elements in the duplicate tree except the erased one" << std::endl;
		assert(cTI.has_key(5));
		assert(cTI.has_key(10));
		assert(cTI.has_key(15));
		assert(cTI.has_key(20));
		assert(cTI.has_key(25));
		assert(cTI.has_key(30));
		assert(cTI.has_key(35));
		std::cout << "destruct tree" << std::endl;
	}
	{
		std::cout << "construct safe_avl_tree with default constructor for base type int" << std::endl;
		xstdtsl::safe_avl_tree<int> cTI;
		std::cout << "perform ordered insert into tree: will require tree to balance" << std::endl;
		std::cout << "insert an element (35)" << std::endl;
		cTI.insert(35);
		std::cout << "insert an element (30)" << std::endl;
		cTI.insert(30);
		std::cout << "insert an element (25) --- should result in rebalance" << std::endl;
		cTI.insert(25);
		std::cout << "insert an element (20)" << std::endl;
		cTI.insert(20);
		std::cout << "insert an element (15) --- should result in rebalance" << std::endl;
		cTI.insert(15);
		std::cout << "insert an element (10) --- should result in rebalance" << std::endl;
		cTI.insert(10);
		std::cout << "insert an element (5) --- should result in rebalance" << std::endl;
		cTI.insert(5);
		std::cout << "confirm all elements in the duplicate tree except the erased one" << std::endl;
		assert(cTI.has_key(35));
		assert(cTI.has_key(30));
		assert(cTI.has_key(25));
		assert(cTI.has_key(20));
		assert(cTI.has_key(15));
		assert(cTI.has_key(10));
		assert(cTI.has_key(5));
		std::cout << "destruct tree" << std::endl;
	}
	return 0;	
}
