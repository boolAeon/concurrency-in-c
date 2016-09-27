#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

typedef enum _rwlock_prio {
    RWLOCK_READ_PRIO = 0,
    RWLOCK_WRITE_PRIO,
    RWLOCK_FIFO_PRIO,
    RWLOCK_PRIO_COUNT
} rwlock_prio;

typedef struct _rwlock_t {
    rwlock_prio prio_scheme;
    int read_count;
    sem_t rmutex, wrt;    
} rwlock_t;

typedef struct _rwlock_methods {
    void (*pfn_rwlock_init)         (rwlock_t *rw_lock);
    void (*pfn_rwlock_read_lock)    (rwlock_t *rw_lock);
    void (*pfn_rwlock_read_unlock)  (rwlock_t *rw_lock);
    void (*pfn_rwlock_write_lock)   (rwlock_t *rw_lock);
    void (*pfn_rwlock_write_unlock)(rwlock_t *rw_lock);
} rwlock_methods;

extern void register_rwlock_methods(const rwlock_methods *rw_lock_methods, rwlock_prio);

extern rwlock_t* rw_lock_init (rwlock_prio prio_scheme);
extern void rw_read_lock    (rwlock_t *rw_lock);
extern void rw_read_unlock  (rwlock_t *rw_lock);
extern void rw_write_lock   (rwlock_t *rw_lock);
extern void rw_write_unlock (rwlock_t *rw_lock);
