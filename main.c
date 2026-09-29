#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define MAX_LISTEN_QUEUE 10
#define PORT 3000
#define REQUEST_BUFFER_SIZE 1024
#define SERVER_RESPONSE_BUFFER_SIZE 0xFFFF

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Methods
// Only handles a small set of the existing ones for now
typedef enum
{
    INVALID = -1,
    GET,
    POST
} http_request_method_type;

typedef struct
{
    http_request_method_type type;
    char *resource;
    char *http_version;
} http_request;

http_request_method_type http_request_method_type_from_string(const char *s)
{

    if (strcmp(s, "GET") == 0)
    {
        return GET;
    }
    else if (strcmp(s, "POST") == 0)
    {
        return POST;
    }

    return INVALID;
}

/**
 * @brief Parse raw HTTP received from the client in a struct
 *
 * @param raw_request Raw HTTP request
 * @param result struct that will contain the parsed request
 */
void http_raw_request_parse(char *raw_request, http_request *result)
{
    const char *DELIMITERS = " \n";

    // Split on each lines
    // TODO: Only treat the start line for now, need to treat headers later.
    char *tokens = strtok(raw_request, DELIMITERS);

    result->type = http_request_method_type_from_string(tokens);
    tokens = strtok(NULL, DELIMITERS);

    result->resource = tokens;
    tokens = strtok(NULL, DELIMITERS);

    result->http_version = tokens;

    // FIXME: Debug only
    printf("Parsed HTTP request:\n");
    printf("Type: %d\n", result->type);
    printf("Resource: %s\n", result->resource);
    printf("Version: %s\n", result->http_version);
}

void static println_error(const char *s)
{
    fprintf(stderr, "%s\n", s);
}

int main()
{
    int server_fd;
    const struct sockaddr_in sa_in = {
        .sin_family = AF_INET,  // IPv4
        .sin_addr = INADDR_ANY, // 0.0.0.0
        .sin_port = htons(PORT)};
    const size_t sa_in_len = sizeof(sa_in);

    if ((server_fd = socket(AF_INET, SOCK_STREAM /* TCP */, 0)) < 0)
    {
        perror("Couldn't open a socket...");
        return 1;
    }
    printf("Opened socket (sockfd: %d)\n", server_fd);

    // Bind opened socket to address and port
    if (bind(server_fd, (struct sockaddr *)&sa_in, sizeof(sa_in)) < 0)
    {
        perror("Couldn't bind a socket...");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_LISTEN_QUEUE) < 0)
    {
        perror("Couldn't listen on socket...");
        return 1;
    }

    // Wait for client to connect
    printf("Waiting for connection on %d:%d...\n", sa_in.sin_addr.s_addr, ntohs(sa_in.sin_port));
    int client_socket;
    if ((client_socket = accept(server_fd, (struct sockaddr *)&sa_in, (socklen_t *)&sa_in_len)) < 0)
    {
        println_error("Couldn't accept an incoming socket connection...\n");
        return 1;
    }

    // Receive data client->server
    char request_buffer[REQUEST_BUFFER_SIZE];
    read(client_socket, request_buffer, REQUEST_BUFFER_SIZE);
    printf("Raw request received '%s' from client\n", request_buffer); // TODO: DEBUG PRINT

    // TODO: for now, we only parse the request without the headers. Take that into account later
    http_request parsed_request = {};
    http_raw_request_parse(request_buffer, &parsed_request);

    // Server response
    // https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Messages
    char server_response[SERVER_RESPONSE_BUFFER_SIZE];
    switch (parsed_request.type)
    {
    case GET:
        // FIXME: Get index.html file content instead of hardcoded response, just for tesing purposes for now.
        sprintf(server_response, "%s 200 OK\nServer: custom-c/0.0.1\nContent-Type: text/html\n\n<!DOCTYPE html><html lang=\"en\"><body><h1>Hello from custom C http server!</h1></body></html>", parsed_request.http_version);
        break;

        // FIXME: Not implemented yet
    case POST:
    default:
        strcpy(server_response, "Not implemented yet");
        break;
    }
    size_t server_response_len = strnlen(server_response, SERVER_RESPONSE_BUFFER_SIZE);

    send(client_socket, server_response, server_response_len, 0);
    printf("Message sent back to the client\n");

    close(client_socket);
    if (close(server_fd) < 0)
    {
        perror("Couldn't close socket...");
    }

    return 0;
}
