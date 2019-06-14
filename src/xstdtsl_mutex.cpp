#include <xstdtsl_mutex_internal.hpp>
#include <xstdtsl_mutex>


xstdtsl_rwm_t	xstdtsl_rwm_new_read_write_mutex(void)
{
	return new xstdtsl_internal::read_write_mutex;
}

bool	xstdtsl_rwm_is_read_locked(xstdtsl_rwm_t i_pMutex)
{
	bool bRet = false;
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		bRet = pMutex->is_read_locked();
	}
	return bRet;
}
bool	xstdtsl_rwm_is_write_locked(xstdtsl_rwm_t i_pMutex)
{
	bool bRet = false;
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		bRet = pMutex->is_write_locked();
	}
	return bRet;
}
bool	xstdtsl_rwm_is_unlocked(xstdtsl_rwm_t i_pMutex)
{
	bool bRet = false;
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		bRet = pMutex->is_unlocked();
	}
	return bRet;
}
int		xstdtsl_rwm_lock_status(xstdtsl_rwm_t i_pMutex)
{
	int iRet = 0;
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		iRet = pMutex->lock_status();
	}
	return iRet;
}
void	xstdtsl_rwm_read_lock(xstdtsl_rwm_t i_pMutex)
{
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		pMutex->read_lock();
	}
}
void	xstdtsl_rwm_write_lock(xstdtsl_rwm_t i_pMutex)
{
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		pMutex->write_lock();
	}
}
bool	xstdtsl_xstdtsl_rwm_try_read_lock(xstdtsl_rwm_t i_pMutex);
{
	bool bRet = false;
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		bRet = pMutex->try_read_lock();
	}
	return bRet;
}
bool	xstdtsl_xstdtsl_rwm_try_write_lock(xstdtsl_rwm_t i_pMutex);
{
	bool bRet = false;
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		bRet = pMutex->try_write_lock();
	}
	return bRet;
}
void	xstdtsl_rwm_delete_read_write_mutex(xstdtsl_rwm_t & io_pMutex)
{
	if (i_pMutex != nullptr)
	{
		xstdtsl_internal::read_write_mutex * pMutex = reinterpret_cast<xstdtsl_internal::read_write_mutex *>(i_pMutex);
		delete pMutex;
		io_pMutex = nullptr;
	}
}

