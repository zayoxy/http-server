#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define MAX_LISTEN_QUEUE 10
#define PORT 3000

void static println_error(const char* s)
{
    fprintf(stderr, "%s\n", s);
}

int main()
{
    int server_fd;
    const struct sockaddr_in sa_in = {
        .sin_family = AF_INET, // IPv4
        .sin_addr = INADDR_ANY, // 0.0.0.0
        .sin_port = htons(PORT)
    };
    constexpr size_t sa_in_len = sizeof(sa_in);

    if ((server_fd = socket(AF_INET, SOCK_STREAM /* TCP */, 0)) < 0)
    {
        println_error("Couldn't open a socket...\n");
        return 1;
    }
    printf("Opened socket (sockfd: %d)\n", server_fd);

    // Bind opened socket to address and port
    if (bind(server_fd, (struct sockaddr*)&sa_in, sizeof(sa_in)) < 0)
    {
        println_error("Couldn't bind a socket...\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_LISTEN_QUEUE) < 0)
    {
        println_error("Couldn't listen on socket...\n");
        return 1;
    }

    // Wait for client to connect
    printf("Waiting for connection on %d:%d...\n", sa_in.sin_addr.s_addr, ntohs(sa_in.sin_port));
    int client_socket;
    if ((client_socket = accept(server_fd, (struct sockaddr*)&sa_in, (socklen_t*)&sa_in_len)) < 0)
    {
        println_error("Couldn't accept an incoming socket connection...\n");
        return 1;
    }

    constexpr int BUF_SIZE = 10;
    char buf[BUF_SIZE];

    // Receive data client->server
    read(client_socket, buf, BUF_SIZE);
    printf("Received value '%s' from client\n", buf);

    // Sent data back server->client
    send(client_socket, buf, strlen(buf), 0);
    printf("Message sent back to the client\n");

    close(client_socket);

    if (close(server_fd) < 0)
    {
        println_error("Couldn't close socket...\n");
    }

    return 0;
}
