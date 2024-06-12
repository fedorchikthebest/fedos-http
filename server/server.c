#include "server.h"
#include "http/responce_handler.h"

#include <sys/epoll.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_EVENTS 64
#define BUFFER_SIZE 5120

int run_server(char* addr, unsigned short int port){
    struct epoll_event ev, events[MAX_EVENTS];
    int listen_sock, conn_sock, nfds, epollfd, n, buffer_size;

    char buffer[5];

    epollfd = epoll_create1(0);

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(port); 
    server_addr.sin_addr.s_addr = inet_addr(addr); 

    if (bind(listen_sock, (struct sockaddr*)&server_addr, sizeof(server_addr))) return -1;

    listen(listen_sock, 8);

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) return -1;

    while (1){
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                conn_sock = accept(listen_sock, NULL, NULL);
                if (conn_sock == -1) exit(EXIT_FAILURE);
                ev.events = EPOLLIN;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                            &ev) == -1) exit(EXIT_FAILURE);
            } else {
                buffer_size = recv(events[n].data.fd, &buffer, 5, 0);
                if (buffer_size == 0){
                    close(events[n].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
                }
                else{
                    add_to_pool(events[n].data.fd);
                }
            }
        }
    }
}