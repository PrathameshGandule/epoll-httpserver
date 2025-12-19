#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    int bytes_read;
    socklen_t client_len;

    // 1️⃣ Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2️⃣ Bind to address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3️⃣ Listen for connections
    if (listen(server_fd, 5) < 0)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080...\n");

    // 4️⃣ Accept connection
    client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

    // 5️⃣ Echo loop
    const char *response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, world!";
    // while ((bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
    // {
    //     buffer[bytes_read] = '\0';
    //     printf("Received: %s\n", buffer);
    // }
    while(1){
        send(client_fd, response, strlen(response), 0);
    }

    // if (bytes_read == 0)
    //     printf("Client disconnected.\n");
    // else if (bytes_read < 0)
    //     perror("recv failed");

    // 6️⃣ Clean up
    close(client_fd);
    close(server_fd);
    return 0;
}
