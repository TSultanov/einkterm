#include "common.h"
#include "pty_hell.h"

int login_tty(int fd)
{
	(void) setsid();
#ifdef TIOCSCTTY
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
#else
	{
	  /* This might work.  */
	  char *fdname = ttyname (fd);
	  int newfd;
	  if (fdname)
	    {
	      if (fd != 0)
		(void) close (0);
	      if (fd != 1)
		(void) close (1);
	      if (fd != 2)
		(void) close (2);
	      newfd = open (fdname, O_RDWR);
	      (void) close (newfd);
	    }
	}
#endif
	(void) dup2(fd, 0);
	(void) dup2(fd, 1);
	(void) dup2(fd, 2);
	if (fd > 2)
		(void) close(fd);
	return (0);
}

int
openpty (int *amaster, int *aslave, char *name, struct termios *termp,
	 struct winsize *winp)
{
  //char _buf[512];

  char buf[512];


  int master, slave;

  //master = posix_openpt (O_RDWR);
  master = open("/mnt/MOVIEXT2/ptmx", O_RDWR);
  if (master == -1)
    return -2;

  if (grantpt (master))
    goto fail;

  if (unlockpt (master))
    goto fail;

#if 0
 if (pts_name (master, &buf, sizeof (_buf)))
#else
 if (ptsname_r (master, buf, sizeof buf))
#endif
   goto fail;

  slave = open (buf, O_RDWR | O_NOCTTY);
  if (slave == -1)
    {
#if 0
      if (buf != _buf)
	free (buf);
#endif
      goto fail;
    }

  /* XXX Should we ignore errors here?  */
  if(termp)
    tcsetattr (slave, TCSAFLUSH, termp);
  if (winp)
    ioctl (slave, TIOCSWINSZ, winp);

  *amaster = master;
  *aslave = slave;
  if (name != NULL)
    strcpy (name, buf);

#if 0
  if (buf != _buf)
    free (buf);
#endif
  return 0;

 fail:
  close (master);
  return -1;
}


int
forkpty (int *amaster, char *name, struct termios *termp, struct winsize *winp)
{
  int master, slave, pid, t;
  t = openpty(&master, &slave, name, termp, winp);
  if (t == -1)
    return -2;
  if(t == -2)
	return -3;

  switch (pid = fork ())
    {
    case -1:
      return -1;
    case 0:
      /* Child.  */
      close (master);
      if (login_tty (slave))
	_exit (1);

      return 0;
    default:
      /* Parent.  */
      *amaster = master;
      close (slave);

      return pid;
    }
}

int write_ptm(char* buf, int sz)
{
	return write(ptm, buf, sz);
}

void init_ptm(void)
{
	ptm = forkpty(&ptm, NULL, NULL, NULL);
	if(ptm == -1)
	{
		put_str("Failed to fork");
	}
	if(ptm == 0)
	{
		execl("/bin/bash", "", NULL);
	}
}

int get_ptm(void)
{
	return ptm;
}
