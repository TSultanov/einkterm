#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pty.h>
#include <sys/poll.h>



int ptm, pts;

int main(){
	struct pollfd* fds;
	int c,i,pid;
	char buf[256];
	fds = malloc(sizeof(struct pollfd)*3);
	pid = forkpty(&ptm, NULL, NULL, NULL);
	if(pid == 0){
		execl("/bin/bash", "", NULL);
	} else{
			fds[0].fd = ptm;
			fds[0].events = POLLIN;
			
			fds[1].fd = 0;
			fds[1].events = POLLIN;
			
			while(1){
				if(poll(fds, 2, 100)>0){
					for(i=0;i<2;i++){
						if(fds[i].revents&POLLIN){
							if(fds[i].fd == ptm){
								c = read(ptm, buf, 1);
								write(1, buf, c);
							} if(fds[i].fd == 0){
								c = read(0, buf, 1);
								//write(1, buf, с);
								write(ptm, buf, c);
							}
						}
					}
				}
			}
	}
	free(fds);
	return 0;
}
