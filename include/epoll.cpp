#include"epoll.h"
struct epoll_event* events;

int epoll_init()
{
    int epoll_fd = epoll_create(LISTENQ);
    if(epoll_fd == -1)
    return -1;
    //events = new epoll_event[MAXEVENTS];
    return epoll_fd;
}

int epoll_add(int epoll_fd, int fd, __uint32_t events){
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    //cout<<"add to epoll"<<fd<<endl;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event)<0)
    {
        perror("epoll_add error");
        return -1;
    }
    return 0;
}
int epoll_mod(int epoll_fd, int fd, __uint32_t events)
{
    struct epoll_event event;
    event.events = events;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&event)<0)
    {
        perror("epoll_mod error");
        return -1;
    }
    return 0;
}

// int epoll_delete(int epoll_fd, int fd, __uint32_t events)
// {
//     struct epoll_event event;
//     event.events = events;
//     if(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,&event))
//     {
//         perror("epoll_del error");
//         return -1;
//     }
//     return 0;
// } 

int my_epoll_wait(int epoll_fd, struct epoll_event* events, int max_events, int timeout)//epoll_fd存放epoll事件表描述符events存放就绪的套接字的事件结构体(可以从里面判断就绪的事件和获取传过来的数据)
{
    int ret_count = epoll_wait(epoll_fd, events, max_events, timeout);
    if (ret_count < 0)
    {
        perror("epoll wait error");
    }
    return ret_count;
}

int setSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
        return -1;

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
        return -1;
    return 0;
}