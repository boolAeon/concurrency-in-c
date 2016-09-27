#include "my_rwlock.h"

static void rwlock_init(rwlock_t *rw_lock) 
{
    rw_lock->read_count = 0;
    sem_init(&rw_lock->rmutex, 0, 1);
    sem_init(&rw_lock->wrt, 0, 1);    
}

static void rwlock_read_lock(rwlock_t *rw_lock)
{
    sem_wait(&rw_lock->rmutex);
        rw_lock->read_count++;
        if (rw_lock->read_count == 1) {
            sem_wait(&rw_lock->wrt);    
        }
    sem_post(&rw_lock->rmutex);
}

static void rwlock_read_unlock(rwlock_t *rw_lock)
{
    sem_wait(&rw_lock->rmutex);
        rw_lock->read_count--;
        if (rw_lock->read_count == 1) {
            sem_post(&rw_lock->wrt);    
        }
    sem_post(&rw_lock->rmutex);
}

static void rwlock_write_lock(rwlock_t *rw_lock)
{
    sem_wait(&rw_lock->wrt);    
}

static void rwlock_write_unlock(rwlock_t *rw_lock)
{
    sem_post(&rw_lock->wrt);    
}

__attribute__ ((constructor))
static void init_rwlock_rprio(void)
{
	static const rwlock_methods rm = {
		rwlock_init,
		rwlock_read_lock,
		rwlock_read_unlock,
		rwlock_write_lock,
		rwlock_write_unlock
	};
	register_rwlock_methods(&rm, RWLOCK_READ_PRIO);
}
