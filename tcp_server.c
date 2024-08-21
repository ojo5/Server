#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define DEFAULT_PORT 12345

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
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

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any interface
    server_addr.sin_port = htons(port);       // Port number

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", port);

    // Accept a connection
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sockfd < 0) {
        perror("accept");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Close the sockets
    close(client_sockfd);
    close(sockfd);

    return 0;
}