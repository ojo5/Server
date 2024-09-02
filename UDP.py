# Created with chatGPT
import socket

# 1. Create a UDP socket
udp_server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# 2. Bind the socket to an IP address and port
server_address = ('localhost', 12345)  # Use any IP address and port number
udp_server_socket.bind(server_address)

print("UDP server is up and listening on {}:{}".format(server_address[0], server_address[1]))

# 3. Listen for incoming datagrams
while True:
    # 4. Receive data from the client
    data, client_address = udp_server_socket.recvfrom(1024)  # Buffer size is 1024 bytes
    
    print("Received message:", data.decode(), "from", client_address)
    
    # 5. (Optional) Send a response to the client
    response_message = "Message received"
    udp_server_socket.sendto(response_message.encode(), client_address)

# 6. Close the socket (This will never be reached in this example as the server runs indefinitely)
udp_server_socket.close()