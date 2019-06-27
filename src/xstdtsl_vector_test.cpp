#include <xstdtsl_safe_vector>
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <cassert>

#include <xstdtsl_vector_test.hpp>


int main(int i_nNum_Params, char * i_pParams[])
{
	std::cout << "--------------=============== safe_vector tests ===============--------------" << std::endl;

	std::cout << "construct safe_vector with default constructor for base type int" << std::endl;
	xstdtsl::safe_vector<int> cSFI;
	std::cout << "check that size is 0" << std::endl;
	assert(cSFI.size() == 0);
	std::cout << "check that vector is empty" << std::endl;
	assert(cSFI.empty());
	std::cout << "add data to vector using push_back" << std::endl;
	cSFI.push_back(1);
	std::cout << "confirm size is 1" << std::endl;
	assert(cSFI.size() == 1);
	std::cout << "confirm vector is not empty" << std::endl;
	assert(!cSFI.empty());
	std::cout << "size " << cSFI.size() << " capacity " << cSFI.capacity() << std::endl;
	std::cout << "confirm capacity is larger or equal to size" << std::endl;
	assert(cSFI.size() <= cSFI.capacity());
	std::cout << "add data to vector using push_back" << std::endl;
	cSFI.push_back(2);
	std::cout << "confirm size is 2" << std::endl;
	assert(cSFI.size() == 2);
	std::cout << "confirm vector is not empty" << std::endl;
	assert(!cSFI.empty());
	std::cout << "confirm capacity is larger or equal to size" << std::endl;
	assert(cSFI.size() <= cSFI.capacity());
	std::cout << "add data to vector using push_back" << std::endl;
	cSFI.push_back(3);
	std::cout << "confirm size is 3" << std::endl;
	assert(cSFI.size() == 3);
	std::cout << "confirm vector is not empty" << std::endl;
	assert(!cSFI.empty());
	std::cout << "confirm capacity is larger or equal to size" << std::endl;
	assert(cSFI.size() <= cSFI.capacity());
	std::cout << "confirm data[0] is correct" << std::endl;
	assert(cSFI.load(0) == 1);
	std::cout << "confirm data[1] is correct" << std::endl;
	assert(cSFI.load(1) == 2);
	std::cout << "confirm data[2] is correct" << std::endl;
	assert(cSFI.load(2) == 3);
	std::cout << "store data to index 0" << std::endl;
	cSFI.store(0,4);
	std::cout << "confirm data stored correctly" << std::endl;
	assert(cSFI.load(0) == 4);
	std::cout << "clear vector" << std::endl;
	cSFI.clear();
	std::cout << "confirm vector is empty" << std::endl;
	assert(cSFI.empty());
	std::cout << "confirm capacity is larger or equal to size" << std::endl;
	assert(cSFI.size() <= cSFI.capacity());
	std::cout << "shrink vector" << std::endl;
	cSFI.shrink_to_fit();
	std::cout << "confirm vector is empty" << std::endl;
	assert(cSFI.empty());
	std::cout << "confirm capacity is 0" << std::endl;
	assert(cSFI.capacity() == 0);
	size_t nCapacity = 4;
	std::cout << "reserve space" << std::endl;
	cSFI.reserve(nCapacity);
	std::cout << "confirm space is reserved" << std::endl;
	assert(nCapacity <= cSFI.capacity());
	nCapacity = cSFI.capacity();
	std::cout << "confirm vector is empty" << std::endl;
	assert(cSFI.empty());
	std::cout << "add data to vector using push_back" << std::endl;
	cSFI.push_back(1);
	cSFI.push_back(2);
	cSFI.push_back(3);
	std::cout << "confirm capacity hasnt changed" << std::endl;
	assert(nCapacity == cSFI.capacity());
	std::cout << "reserve additional space" << std::endl;
	nCapacity += 4;
	cSFI.reserve(nCapacity);
	std::cout << "confirm space is reserved" << std::endl;
	assert(nCapacity <= cSFI.capacity());
	nCapacity = cSFI.capacity();
	std::cout << "confirm vector is empty" << std::endl;
	assert(!cSFI.empty());
	std::cout << "confirm vector size is unchanged" << std::endl;
	assert(cSFI.size() == 3);
	std::cout << "confirm data is unchanged" << std::endl;
	assert(cSFI.load(0) == 1);
	assert(cSFI.load(1) == 2);
	assert(cSFI.load(2) == 3);
	std::cout << "confirm capacity hasnt changed" << std::endl;
	assert(nCapacity == cSFI.capacity());

	std::cout << "instantiate new empty int vector" << std::endl;
	xstdtsl::safe_vector<int> cSFIas;
	std::cout << "confirm size is 0" << std::endl;
	assert(cSFIas.size() == 0);
	std::cout << "perform = operator" << std::endl;
	cSFIas = cSFI;
	std::cout << "confirm original vector size is unchanged" << std::endl;
	assert(cSFI.size() == 3);
	std::cout << "confirm new vector size the same (" << cSFIas.size() << ")" << std::endl;
	assert(cSFIas.size() == 3);
	std::cout << "confirm original data is unchanged" << std::endl;
	assert(cSFI.load(0) == 1);
	assert(cSFI.load(1) == 2);
	assert(cSFI.load(2) == 3);
	std::cout << "confirm new data is correct" << std::endl;
	assert(cSFIas.load(0) == 1);
	assert(cSFIas.load(1) == 2);
	assert(cSFIas.load(2) == 3);
	std::cout << "confirm capacity of new vector is larger or equal to original vector" << std::endl;
	assert(cSFIas.capacity() >= cSFIas.size());

	
	std::cout << "instantiate new int vector using copy constructor" << std::endl;
	xstdtsl::safe_vector<int> cSFIcc(cSFI);
	std::cout << "confirm original vector size is unchanged" << std::endl;
	assert(cSFI.size() == 3);
	std::cout << "confirm new vector size the same (" << cSFIas.size() << ")" << std::endl;
	assert(cSFIas.size() == 3);
	std::cout << "confirm original data is unchanged" << std::endl;
	assert(cSFI.load(0) == 1);
	assert(cSFI.load(1) == 2);
	assert(cSFI.load(2) == 3);
	std::cout << "confirm new data is correct" << std::endl;
	assert(cSFIas.load(0) == 1);
	assert(cSFIas.load(1) == 2);
	assert(cSFIas.load(2) == 3);
	std::cout << "confirm capacity of new vector is larger or equal to original vector" << std::endl;
	assert(cSFIas.capacity() >= cSFIas.size());

	std::cout << "construct safe_vector with default constructor for base type float" << std::endl;
	xstdtsl::safe_vector<float> cSFF;
	std::cout << "confirm size is 0" << std::endl;
	assert(cSFF.size() == 0);
	std::cout << "confirm vector is empty" << std::endl;
	assert(cSFF.empty());

	std::cout << "--------------=============== read_iterator tests ===============--------------" << std::endl;
	{
		std::cout << "construct read iterator with int vector (forward iterator)" << std::endl;
		xstdtsl::safe_vector<int>::read_iterator cIter(cSFI,xstdtsl::safe_vector<int>::iterator_base::beginning);
		size_t nI = 1;
		std::cout << "test contents using iterator" << std::endl;
		assert(!cIter.is_at_beginning());
		assert(!cIter.is_at_end());
		while (!cIter.is_at_end())
		{
			assert(cIter.load() == nI);
			cIter++;
			nI++;
		}
		assert(!cIter.is_at_beginning());
		assert(cIter.is_at_end());
	}
	{
		std::cout << "construct read iterator with int vector (reverse iterator)" << std::endl;
		xstdtsl::safe_vector<int>::read_iterator cIter(cSFI,xstdtsl::safe_vector<int>::iterator_base::end);
		size_t nI = 3;
		std::cout << "test contents using iterator" << std::endl;
		assert(!cIter.is_at_beginning());
		assert(!cIter.is_at_end());
		while (!cIter.is_at_beginning())
		{
			assert(cIter.load() == nI);
			cIter--;
			nI--;
		}
		assert(cIter.is_at_beginning());
		assert(!cIter.is_at_end());
	}

	std::cout << "--------------=============== write_iterator tests ===============--------------" << std::endl;
	{
		std::cout << "construct write iterator with int vector (forward iterator)" << std::endl;
		xstdtsl::safe_vector<int>::write_iterator cIter(cSFI,xstdtsl::safe_vector<int>::iterator_base::beginning);
		size_t nI = 1;
		std::cout << "test contents using iterator" << std::endl;
		assert(!cIter.is_at_beginning());
		assert(!cIter.is_at_end());
		while (!cIter.is_at_end())
		{
			assert(cIter.load() == nI);
			cIter.store(nI + 10);
			assert(cIter.load() == (nI + 10));
			++cIter;
			nI++;
		}
		assert(!cIter.is_at_beginning());
		assert(cIter.is_at_end());
	}
	{
		std::cout << "construct write iterator with int vector (reverse iterator)" << std::endl;
		xstdtsl::safe_vector<int>::write_iterator cIter(cSFI,xstdtsl::safe_vector<int>::iterator_base::end);
		size_t nI = 3;
		std::cout << "test contents using iterator" << std::endl;
		assert(!cIter.is_at_beginning());
		assert(!cIter.is_at_end());
		while (!cIter.is_at_beginning())
		{
			assert(cIter.load() == (nI + 10));
			cIter.store(nI + 20);
			assert(cIter.load() == (nI + 20));
			--cIter;
			nI--;
		}
		assert(cIter.is_at_beginning());
		assert(!cIter.is_at_end());
	}
	return 0;	
}
