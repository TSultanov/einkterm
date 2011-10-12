#include "common.h"

#include "pty_hell.h"
#include "eink.h"
#include "keyboard.h"

#define X_SIZE 600
#define Y_SIZE 800

enum events {READ, WRITE};

struct entry {
	//struct pollfd;
	int fd;
	void *callback;
	enum events event;
};

struct pollfd* fds=NULL;

int entries=0;
struct entry* reactor=NULL;

void add_callback(int fd, void* callback, enum events event)
{
	entries++;
	reactor = realloc(reactor, entries * (sizeof(struct entry)));
	reactor[entries-1].fd = fd;
	reactor[entries-1].event = event;
	reactor[entries-1].callback = callback;
	fds = realloc(fds, entries * (sizeof(struct pollfd)));
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
	update_part();
}

void loop(void)
{
	while(1){
		poll(fds, entries, 10);
	}
}

int main(int argc, char* argv)
{
	init_eink();
	init_ptm();
	put_str("HELLO");
	add_callback(get_ptm(), update_disp, READ);
	add_callback(get_keyboard(), read_keyboard, READ);
    return 0;
}
