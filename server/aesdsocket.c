#include <stdio.h>
#include<stdbool.h>
#include<fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "9000"  // the port users will be connecting to
#define BUF_SIZE 1024
#define BACKLOG 10   // how many pending connections queue will hold
char *rbuf = NULL;

void sigchld_handler(int s);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    int file =0;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    bool packcomplete = false;
    rbuf = (char *)malloc(BUF_SIZE * sizeof(char));
    int rbytes=0; int wbytes = 0;
    int bread = 0;
    int bsent = 0;
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

	file = open("/var/tmp/aesdsocketdata", O_CREAT | O_RDWR| O_APPEND,S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	do{
		rbytes = recv(new_fd,rbuf,BUF_SIZE,0);
		wbytes = write(file,rbuf,rbytes);
		if(NULL != (memchr(rbuf,'\n',rbytes))){
			packcomplete = true;
		}
	}while(!packcomplete);
	packcomplete=false;
	do{
		bread = read(file,rbuf,BUF_SIZE);
		if(bread >0){
			bsent = send(new_fd,rbuf,bread,0);
		}
	}while(bread>0);
	close(file);
	close(new_fd);
    }

    return 0;
}

void sigchld_handler(int s)
{
    if((s == SIGINT) || (s == SIGTERM))
    {
            free(rbuf);
            remove("/var/tmp/aesdsocketdata");
            exit(EXIT_SUCCESS);
    }
}


