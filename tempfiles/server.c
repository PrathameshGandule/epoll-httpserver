#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
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
#include <bits/sigaction.h>
#include <asm-generic/signal-defs.h>

#define PORT "3000"

#define BACKLOG 10

void sigchld_handler(int s)
{
    (void)s;
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    char response[4096];
    char buffer[1024];
    int bytes_read;
    socklen_t client_len;
    char *token;
    const char *t1, *t2, *t3;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo : %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        break;
        ;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connection...\n");
    // char *template =
    //     "HTTP/1.1 200 OK\r\n"
    //     "Content-Type: text/html\r\n";
    // "Content-Length: 13\r\n"
    // "\r\n";

    strcpy(response,
           "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n");

    while (1)
    {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);
        bytes_read = recv(new_fd, buffer, sizeof(buffer) - 1, 0);
        buffer[bytes_read] = '\0';
        printf("Received this : \n%s\n\n\n\n", buffer);
        token = strtok(buffer, "\n");
        t1 = strtok(token, " ");
        t2 = strtok(NULL, " ");
        // t3 = strtok(NULL, " ");
        printf("t2 : %s\n", t2);

        if (strcmp(t2, "/employee") == 0)
        {
            strcat(response, "Content-Length: 21\r\n");
            strcat(response, "\r\n");
            strcat(response, "this is employee page");
        }
        else if (strcmp(t2, "/") == 0)
        {
            strcat(response, "Content-Length: 20\r\n");
            strcat(response, "\r\n");
            strcat(response, "Hello from main page");
        }
        else
        {
            strcat(response, "Content-Length: 27\r\n");
            strcat(response, "\r\n");
            strcat(response, "response for any other page");
        }

        if (!fork())
        {
            close(sockfd);
            if (send(new_fd, response, strlen(response), 0) == -1)
            {
                perror("send");
            }
            close(new_fd);
            exit(0);
        }

        close(new_fd);
    }
}