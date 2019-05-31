/* include my_lock_init */
#include	"unp.h"

static struct flock	lock_it, unlock_it;
static int			lock_fd = -1;
					/* fcntl() will fail if my_lock_init() not called */

void
my_lock_init(char *pathname)
{
    char	lock_file[1024];

		/* 4must copy caller's string, in case it's a constant */
    strncpy(lock_file, pathname, sizeof(lock_file));
    lock_fd = Mkstemp(lock_file);

    Unlink(lock_file);			/* but lock_fd remains open */

	lock_it.l_type = F_WRLCK;
	lock_it.l_whence = SEEK_SET;
	lock_it.l_start = 0;
	lock_it.l_len = 0;

	unlock_it.l_type = F_UNLCK;
	unlock_it.l_whence = SEEK_SET;
	unlock_it.l_start = 0;
	unlock_it.l_len = 0;
}
/* end my_lock_init */

/* include my_lock_wait */
void
my_lock_wait()
{
    int		rc;
    
    while ( (rc = fcntl(lock_fd, F_SETLKW, &lock_it)) < 0) {
		if (errno == EINTR)
			continue;
    	else
			err_sys("fcntl error for my_lock_wait");
	}
}

void
my_lock_release()
{
    if (fcntl(lock_fd, F_SETLKW, &unlock_it) < 0)
		err_sys("fcntl error for my_lock_release");
}
/* end my_lock_wait */
