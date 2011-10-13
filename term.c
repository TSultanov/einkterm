#include "common.h"

#include "pty_hell.h"
#include "eink.h"
#include "keyboard.h"

#define X_SIZE 600
#define Y_SIZE 800

enum events {READ, WRITE, RW};

struct entry {
	//struct pollfd;
	int fd;
	void (*callback)();
	enum events event;
};

struct pollfd* fds=NULL;

int entries=0,allow=1;
struct entry* reactor=NULL;

/*void *waiter(void *vptr_args)
{
	wait_epd();
	allow = 1;
	pthread_exit(NULL);
}

void init_waiter(void)
{
	pthread_t* thr;
	int rc;
	rc = pthread_create(thr, NULL, waiter, NULL);
}*/



void init_reactor(int sz)
{
	reactor = realloc(reactor, sz * (sizeof(struct entry)));
	fds = realloc(fds, sz * (sizeof(struct pollfd)));
}

void add_callback(int fd, void* callback, enum events event)
{
	entries++;
	
	reactor[entries-1].fd = fd;
	reactor[entries-1].event = event;
	reactor[entries-1].callback = callback;
	
	fds[entries-1].fd = fd;
	if(event == READ)
	{
		fds[entries-1].events = POLLIN;
	} else
	{
		fds[entries-1].events = POLLOUT;
	}
}

void update_disp(void)
{
	read_ptm();
//	if(allow){
		update_part();
//		allow = 0;
//		init_waiter();
//	}
}

void loop(void)
{
	int i;
	while(1){
		if(poll(fds, entries, 10))
		{
			//put_str("Poll accept\n");
			//update_disp();
			for(i = 0; i<entries; i++)
			{
				if((fds[i].revents&POLLIN)||(fds[i].revents&POLLOUT))
				{
					/*if(i==1){
						put_str("keyboard\n");
						update_part();
					}
					if(i==0){
						put_str("ptm\n");
						update_part();
					}*/
					reactor[i].callback();
				}
			}
		}
	}
}

int main(int argc, char** argv)
{
	init_eink();
	init_ptm();
	open_keyboard();
	//put_str("HELLO");
	//draw_bitmap();
	put_str("HELLO\n");
	update_part();
	init_reactor(10);
	add_callback(get_ptm(), update_disp, READ);
	add_callback(get_keyboard(), read_keyboard, READ);
	//add_callback(get_disp(), disp_allow, WRITE);
	//if(entries==2) { put_str("2\n"); }
	loop();
    return 0;
}
