#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include<cstring>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
const int MAXEVENTS = 5000;
const int LISTENQ = 1024;


    int epoll_init();
    int epoll_add(int epoll_fd, int fd, __uint32_t events);
    int epoll_mod(int epoll_fd, int fd, __uint32_t events);
    // int epoll_del(int epoll_fd, int fd, __uint32_t events);
    int my_epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout);
    
    int setSocketNonBlocking(int fd);

