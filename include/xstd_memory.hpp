#pragma once
#include <xstd_types>

namespace xstd
{
	void * new_shared_ptr(allocator i_pfnAllocator, deleter i_pfnDeleter, size_t i_nArray_Size);
	void delete_shared_ptr(void * i_pPointer);
	void * shared_ptr_get(void * i_pPointer);
	void shared_ptr_reset(void * i_pPointer);
	void shared_ptr_lock(void * i_pPointer);
	void shared_ptr_unlock(void * i_pPointer);
	bool shared_ptr_try_lock(void * i_pPointer);
	bool shared_ptr_is_manual_lock(void * i_pPointer);
	bool shared_ptr_is_this_manual_lock(void * i_pPointer);


	template <typename T> void * allocator(size_t i_nInstances)
	{
		void * pRet = nullptr;
		if (i_nInstances > 1)
			pRet = new T[i_nInstances];
		else
			pRet = new T;
		return pRet;
	}
	template <typename T> void deleter(void * i_pPtr, size_t i_nInstances)
	{
		if (i_pPtr != nullptr)
		{
			if (i_nInstances > 1)
				delete [] i_pPtr;
			else
				delete i_pPtr;
		}
	}
			

	template <typename T> class shared_ptr
	{
	private:
		void * m_pInternal_Data;
	public:
		shared_ptr(size_t i_nArray_Size = 1)
		{
			m_pInternal_Data = new_shared_ptr(allocator<T>,deleter<T>,i_nArray_Size);
		}
		shared_ptr(size_t i_nArray_Size = 1)
		{
			m_pInternal_Data = new_shared_ptr(allocator<T>,deleter<T>,i_nArray_Size);
		}
		
	};

}
