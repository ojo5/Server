# Server-in-C
An implementation of some servers in C


## Overview
This project demonstrates a basic TCP server that listens for incoming connections, handles communication with clients, and processes data.

### Features
Listen for connections on a specified port.
Accept multiple client connections.
Handle data exchange with clients.
Basic error handling.

## Getting Started
### Requirements
C compiler (e.g., gcc)
Basic understanding of networking and C programming


### Build and Run
Compile the server:

```sh
gcc -o tcp_server tcp_server.c
```

Run the server:
```sh
./tcp_server
```

The server will start listening on localhost at port 12345. You can change the port in the source code if needed.

## Usage
Connect to the server using a TCP client or tool (e.g., telnet).
The server will echo back any data received from clients.


## References:
https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

  chatGPT