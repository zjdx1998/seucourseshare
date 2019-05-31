/* Our own header for the programs that use threads.
   Include this file, instead of "unp.h". */

#ifndef	__unp_pthread_h
#define	__unp_pthread_h

#include	"unp.h"

void	Pthread_create(pthread_t *, const pthread_attr_t *,
					   void * (*)(void *), void *);
void	Pthread_join(pthread_t, void **);
void	Pthread_detach(pthread_t);
void	Pthread_kill(pthread_t, int);

void	Pthread_mutexattr_init(pthread_mutexattr_t *);
void	Pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
void	Pthread_mutex_init(pthread_mutex_t *, pthread_mutexattr_t *);
void	Pthread_mutex_lock(pthread_mutex_t *);
void	Pthread_mutex_unlock(pthread_mutex_t *);

void	Pthread_cond_broadcast(pthread_cond_t *);
void	Pthread_cond_signal(pthread_cond_t *);
void	Pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);
void	Pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *,
							   const struct timespec *);

void	Pthread_key_create(pthread_key_t *, void (*)(void *));
void	Pthread_setspecific(pthread_key_t, const void *);
void	Pthread_once(pthread_once_t *, void (*)(void));

#endif	/* __unp_pthread_h */
