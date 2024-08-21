#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define DEFAULT_PORT 12345
#define BUFFER_SIZE 1024

// Global variables
int server_running = 1;
int sockfd; // Global socket file descriptor


// Function to create and configure a socket
int create_socket(int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

// Function to handle client communication
void handle_client(int client_sockfd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Handle communication with the client
    while ((bytes_received = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received: %s\n", buffer);


        const char *message = "Message echoed back from server: \n";
        ssize_t message_length = strlen(message);
        if (send(client_sockfd, message, message_length, 0) < 0) {
            perror("send");
            close(client_sockfd);
            return;
        }

        // Echo the data back to the client
        if (send(client_sockfd, buffer, bytes_received, 0) < 0) {
            perror("send");
            close(client_sockfd);
            return;
        }
    }

    if (bytes_received < 0) {
        perror("recv");
    }

    printf("Client disconnected.\n");
    close(client_sockfd);
}

int main(int argc, char *argv[]) {
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int port = DEFAULT_PORT;

    // Check if a port number is provided as a command-line argument
    if (argc == 2) {
        port = atoi(argv[1]);
        if (port <= 0) {
            fprintf(stderr, "Invalid port number. Using default port %d.\n", DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Create and configure the socket
    sockfd = create_socket(port);

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", port);

    // Main server loop
    while (server_running) {
        // Accept a connection
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd < 0) {
            if (server_running) { // Only print error if server is still running
                perror("accept");
            }
            continue; // Continue to accept other clients
        }

        printf("Client connected.\n");

        // Handle client communication
        handle_client(client_sockfd);
    }

    // Close the server socket
    close(sockfd);

    printf("Server has shut down.\n");

    return 0;
}