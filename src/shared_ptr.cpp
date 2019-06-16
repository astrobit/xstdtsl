#include <xstd_memory>
#include <xstd_memory_internal.hpp>


void * xstd::new_shared_ptr(xstd::allocator i_pfnAllocator, xstd::deleter i_pfnDeleter, size_t i_nArray_Size)
{
	return reinterpret_cast<void *> new xstd_internal::shared_ptr(i_pfnAllocator, i_pfnDeleter, i_nArray_Size);
}
void xstd::delete_shared_ptr(void * i_pPointer)
{
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		delete pPointer;
	}
}

void * xstd::shared_ptr_get(void * i_pPointer)
{
	void * pRet = nullptr;
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		pRet = pPointer->get();
	}
}

void xstd::shared_ptr_reset(void * i_pPointer)
{
	void * pRet = nullptr;
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		pPointer->reset();
	}
}

void xstd::shared_ptr_lock(void * i_pPointer)
{
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		pPointer->lock();
	}
}

void xstd::shared_ptr_unlock(void * i_pPointer)
{
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		pPointer->unlock();
	}
}

bool xstd::shared_ptr_try_lock(void * i_pPointer)
{
	bool bRet = false;
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		bRet = pPointer->try_lock();
	}
}

bool xstd::shared_ptr_is_manual_lock(void * i_pPointer)
{
	bool bRet = false;
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		bRet = pPointer->is_manual_lock();
	}
}


bool xstd::shared_ptr_is_this_manual_lock(void * i_pPointer)
{
	bool bRet = false;
	if (i_pPointer != nullptr)
	{
		xstd_internal::shared_ptr * pPointer = reinterpret_cast<xstd_internal::shared_ptr *> i_pPointer;
		bRet = pPointer->is_this_manual_lock();
	}
}

