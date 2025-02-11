#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       // For close()
#include <arpa/inet.h>    // For socket functions

#define PORT 8080
#define BUFFER_SIZE 8192

void send_response(int client, const char *message) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s", strlen(message), message);
    send(client, response, strlen(response), 0);
}

void handle_client(int client) {
    char buffer[BUFFER_SIZE] = {0};
    read(client, buffer, BUFFER_SIZE - 1);

    if (strstr(buffer, "GET /lookup?key=") != NULL) {
        send_response(client, "<h2>Response from Linux Server</h2>");
    }

    close(client);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server, client;
    socklen_t client_size = sizeof(client);

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    listen(server_socket, 5);
    printf("Server running at http://localhost:%d\n", PORT);

    // Automatically open browser
    system("xdg-open https://kapileswar-rout.github.io/key-lookup/");

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client, &client_size);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}
