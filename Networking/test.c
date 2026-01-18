#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "Server.h"
#include "HTTPRequest.h"

void launch(struct Server *server)
{
    char buffer[30000];
    int cnt = 1;
    char *hello = "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/html;\r\n"
                  "Content-Length: 13\r\n"
                  "Connection: close\r\n"
                  "\r\n"
                  "Hello World\r\n";
    int addr_len;
    int new_socket;
    while (1)
    {
        // printf("%d\n", cnt);
        printf("==== Waiting for connection %d ====\n", cnt);
        addr_len = sizeof(server->address);
        new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&addr_len);
        read(new_socket, buffer, 30000);
        printf("%s\n\n\n\n", buffer);
        http_request_constructor(buffer);
        write(new_socket, hello, strlen(hello));
        close(new_socket);
        printf("==== Closing connection ====\n");
        addr_len = 0;
        new_socket = -1;
        memset(buffer, 0, 30000);
        cnt++;
    }
}

int main(int argc, char const *argv[])
{
    int port;
    if (argv[1])
        port = atoi(argv[1]);
    else
        port = 3000;
    printf("PORT : %d\n", port);
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 10, launch);
    server.launch(&server);
    return 0;
}
