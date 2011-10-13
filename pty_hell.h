#include "common.h"
int login_tty(int fd);
int
openpty (int *amaster, int *aslave, char *name, struct termios *termp,
	 struct winsize *winp);
int
forkpty (int *amaster, char *name, struct termios *termp, struct winsize *winp);
int write_ptm(char* buf, int sz);
int read_ptm(void);
void init_ptm(void);
int get_ptm(void);


