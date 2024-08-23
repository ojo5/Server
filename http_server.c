#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Receive data from the client
    bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("recv");
        close(client_socket);
        return;
    }
    buffer[bytes_received] = '\0';  // Null-terminate the received data

    // Print the received data
    printf("Received data:\n%s\n", buffer);

    // Check if the request is a POST request
    if (strncmp(buffer, "POST", 4) == 0) {
        // Look for the specific string in the POST data
        if (strstr(buffer, "post_data") != NULL) {
            // Respond with a 302 Found and a redirect location
            const char *response = 
                "HTTP/1.1 302 Found\r\n"
                "Location: http://pwnd.com/\r\n"
                "Content-Length: 0\r\n"
                "\r\n";
            send(client_socket, response, strlen(response), 0);
        } else {
            // If the POST data doesn't contain the specific string, send a 200 OK response
            const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nNot correct POST data";
            send(client_socket, response, strlen(response), 0);
        }
    } else {
        // For non-POST requests, respond with a 200 OK
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nNormal 200 Code!";
        send(client_socket, response, strlen(response), 0);
    }

    // Close the client socket
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept an incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        // Handle the client connection
        handle_client(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}