#pragma once
#ifndef EXPORT
#define EXPORT
#endif

typedef void * xstdtsl_rwm_t;
extern "C"
{
	EXPORT xstdtsl_rwm_t	xstdtsl_rwm_new_read_write_mutex(void);
	EXPORT bool	xstdtsl_rwm_is_read_locked(xstdtsl_rwm_t);
	EXPORT bool	xstdtsl_rwm_is_write_locked(xstdtsl_rwm_t);
	EXPORT bool	xstdtsl_rwm_is_unlocked(xstdtsl_rwm_t);
	EXPORT int		xstdtsl_rwm_lock_status(xstdtsl_rwm_t);
	EXPORT void	xstdtsl_rwm_read_lock(xstdtsl_rwm_t);
	EXPORT void	xstdtsl_rwm_write_lock(xstdtsl_rwm_t);
	EXPORT void	xstdtsl_rwm_read_unlock(xstdtsl_rwm_t);
	EXPORT void	xstdtsl_rwm_write_unlock(xstdtsl_rwm_t);
	EXPORT bool	xstdtsl_rwm_try_read_lock(xstdtsl_rwm_t);
	EXPORT bool	xstdtsl_rwm_try_write_lock(xstdtsl_rwm_t);
	EXPORT void	xstdtsl_rwm_delete_read_write_mutex(xstdtsl_rwm_t &);
	EXPORT void	xstdtsl_rwm_set_status(xstdtsl_rwm_t, int);
}

#undef EXPORT
