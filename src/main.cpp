
#include"epoll.h"

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

const int PORT = 8888;

typedef vector<struct epoll_event> EventList;

void acceptConnection(int listen_fd, int epoll_fd)
{
    struct sockaddr_in client_addr;
    //memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t client_addr_len = 0;
    int accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(accept_fd > 0)
    {
        
        int ret = setSocketNonBlocking(accept_fd);
        if (ret < 0)
        {
            perror("Set non block failed!");
            return;
        }
        
        __uint32_t _epo_event = EPOLLIN | EPOLLET | EPOLLONESHOT;
        epoll_add(epoll_fd, accept_fd, _epo_event);
    }
}
// void readconnection(int listen_fd, int epoll_fd,struct epoll_event* events)
// {
//     struct sockaddr_in client_addr;
//     memset(&client_addr, 0, sizeof(struct sockaddr_in));
//     socklen_t client_addr_len = 0;
//     int accept_fd = 0;
//     while((accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len)) > 0)
//     {
//         if (accept_fd < 0)
//         continue;

//         char buf[1024] = {0};
//         int ret = read(accept_fd, buf, 1024);
//         if (ret == -1)
//             ERR_EXIT("read");
//         if (ret == 0)
//         {
//             std::cout<<"client close"<<std::endl;
//             close(accept_fd);

//             epoll_delete(epoll_fd,accept_fd, events);
            
//             continue;
//         }

//         std::cout<<buf;
//         write(accept_fd, buf, strlen(buf));
//         }
// }

int socket_bind_listen(int port)
{
    if(port<1024 || port>65535)
        return -1;
    int listen_fd;
    if((listen_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        return -1;
     int on = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		return -1;
    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    // if(bind(listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))== -1)
    //     return -1;
    // if(listen(listen_fd,LISTENQ)<0)
    //     ERR_EXIT("listen");
        
    // if(listen_fd == -1)
    //     close(listen_fd);
    //     return listen_fd;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");

	if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		ERR_EXIT("bind");
	if (listen(listen_fd, SOMAXCONN) < 0)
		ERR_EXIT("listen");
    return listen_fd;
}
    

int main()
{
    signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);

    int epoll_fd = epoll_init();
    if (epoll_fd < 0)
    {
        perror("epoll init failed");
        return 1;
    }
    int listen_fd = socket_bind_listen(PORT);
    // struct epoll_event event;
	if (listen_fd < 0) 
    {
        perror("socket bind failed");
        return 1;
    }
    if (setSocketNonBlocking(listen_fd) < 0)
    {
        perror("set socket non block failed");
        return 1;
    }
    
    
    // __uint32_t event = EPOLLIN | EPOLLET;
    struct epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    // epoll_add(epoll_fd, listen_fd,event);
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_fd,&event)<0)
    {
        perror("epoll_add error");
        return -1;
    }
    //return 0;

    
    // struct sockaddr_in client_addr;
    //memset(&client_addr, 0, sizeof(struct sockaddr_in));
    // socklen_t client_addr_len ;
    
    while(true)
    {   

    //    int ret =  my_epoll_wait(epoll_fd, events, MAXEVENTS,-1);
        struct epoll_event *events;
        events = new epoll_event[MAXEVENTS];
        int ret = epoll_wait(epoll_fd, events, MAXEVENTS, -1);
            // if (ret < 0)
            // {
            //     perror("epoll wait error");
            // }
            //return ret;
        cout<<"num of event = "<<ret<<endl;
        if (ret < 0)
		{
			if (errno == EINTR)
				continue;
			
			ERR_EXIT("epoll_wait");
            }
        if (ret == 0)
            continue;
        
        for(int i = 0; i < ret; i++)
        {   
            
            if(events[i].data.fd == listen_fd)
            {   
                
                //cout << "This is listen_fd" << endl;
                acceptConnection(listen_fd, epoll_fd);
            }
            else if (events[i].events & EPOLLIN)
			{   
                //int accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
                int accept_fd =  events[i].data.fd;
                if (accept_fd < 0)
                continue;

                char buf[1024] = {0};
                int ret = read(accept_fd, buf, 1024);
                if (ret == -1)
                    ERR_EXIT("read");
                if (ret == 0)
                {
                    std::cout<<"client close"<<std::endl;
                    close(accept_fd);
                    
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, accept_fd, events);
                    
                    continue;
                }

                std::cout<<buf;
                write(accept_fd, buf, strlen(buf));
                }
            
			}
        }

    }




