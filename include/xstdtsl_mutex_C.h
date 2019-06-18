#pragma once

typedef void * xstdtsl_rwm_t;
extern "C"
{
	xstdtsl_rwm_t	xstdtsl_rwm_new_read_write_mutex(void);
	bool	xstdtsl_rwm_is_read_locked(xstdtsl_rwm_t);
	bool	xstdtsl_rwm_is_write_locked(xstdtsl_rwm_t);
	bool	xstdtsl_rwm_is_unlocked(xstdtsl_rwm_t);
	int		xstdtsl_rwm_lock_status(xstdtsl_rwm_t);
	void	xstdtsl_rwm_read_lock(xstdtsl_rwm_t);
	void	xstdtsl_rwm_write_lock(xstdtsl_rwm_t);
	void	xstdtsl_rwm_read_unlock(xstdtsl_rwm_t);
	void	xstdtsl_rwm_write_unlock(xstdtsl_rwm_t);
	bool	xstdtsl_rwm_try_read_lock(xstdtsl_rwm_t);
	bool	xstdtsl_rwm_try_write_lock(xstdtsl_rwm_t);
	void	xstdtsl_rwm_delete_read_write_mutex(xstdtsl_rwm_t &);
	void	xstdtsl_rwm_set_status(xstdtsl_rwm_t, int);
}

