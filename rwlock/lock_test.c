#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "my_rwlock.h"

static int buff;

int main()
{
	rwlock_t *mylock = rw_lock_init(RWLOCK_READ_PRIO);
	rw_read_lock(mylock);
	printf("%d", buff);
	rw_read_unlock(mylock);
	return 1;
}
