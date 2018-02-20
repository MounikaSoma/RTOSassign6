#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
//defining the maximum number of data bytes that the client can send at one go.
#define MAXDATASIZE 100 
int clientSocket;
//client will connect to 3340 port
//signal handler
void my_sigint_handler(int signalnumber)
{
  char ans[2];
  if (signalnumber == SIGINT)
  {
    printf("received SIGINT\n");
    printf("Program received a CTRL-C\n");
    printf("Terminate Y/N : "); 
    scanf("%s", ans);
    if ((strcmp(ans,"Y") == 0)||(strcmp(ans,"y") == 0))
    {
       printf("Exiting ....\n");
       if (send(clientSocket, "End of Connection", MAXDATASIZE-1, 0) == -1){
            close(clientSocket);
            perror("send");
        }
       close(clientSocket);
       exit(0); 
    }
    else
    {
       printf("Continuing ..\n");
    }
  }
}




void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
    int sockfd;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];


//Signal handler for sigint
    if (signal(SIGINT, my_sigint_handler) == SIG_ERR)
      printf("\ncan't catch SIGINT\n");



    if (argc != 3) {
        fprintf(stderr,"usage: ./client <hostname> <portnumber>\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); 
    clientSocket = sockfd;
    while(1){
        scanf("%s", buf);
        if (send(sockfd, buf, MAXDATASIZE-1, 0) == -1){
            close(sockfd);
            perror("send");
        }
    }

    

    close(sockfd);

    return 0;
}
