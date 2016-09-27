#include "my_rwlock.h"

static rwlock_methods rwlock_type_list[RWLOCK_PRIO_COUNT];

void register_rwlock_methods(const rwlock_methods *rm, rwlock_prio prio_scheme) {
	rwlock_type_list[prio_scheme] = *rm;
}

rwlock_t* rw_lock_init (rwlock_prio prio_scheme) 
{
	rwlock_t *my_rwlock = (rwlock_t *) malloc (sizeof(rwlock_t));
	if (my_rwlock) {
		my_rwlock->prio_scheme = prio_scheme;
		(*rwlock_type_list[prio_scheme].pfn_rwlock_init)(my_rwlock);
	}
	return my_rwlock;
}


void rw_read_lock (rwlock_t *rw_lock)
{
	if (rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_read_lock)
		(*rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_read_lock)(rw_lock);
	
}

void rw_read_unlock  (rwlock_t *rw_lock) 
{
	if (rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_read_unlock)
		(*rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_read_unlock)(rw_lock);
	
}

void rw_write_lock   (rwlock_t *rw_lock)
{
	if (rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_write_lock)
		(*rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_write_lock)(rw_lock);
	
}

void rw_write_unlock (rwlock_t *rw_lock)
{
	if (rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_write_unlock)
		(*rwlock_type_list[rw_lock->prio_scheme].pfn_rwlock_write_unlock)(rw_lock);
	
}
