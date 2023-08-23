#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void send_http_request(const char* host, int port, const char* path) {
    int client_socket;
    struct sockaddr_in server_addr;
    char request[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    // if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
    //     perror("Invalid address/ Address not supported");
    //     exit(1);
    // }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(1);
    }

    // Prepare the HTTP request
    sprintf(request,
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: MyHTTPClient\r\n"
            "Connection: close\r\n"
            "\r\n", path, host);

    // Send the request using write
    ssize_t bytes_written = write(client_socket, request, strlen(request));
    if (bytes_written == -1) {
        perror("Request sending failed");
        exit(1);
    }

    // Receive and display the response
    char response[1024];
    ssize_t bytes_received;
    while ((bytes_received = recv(client_socket, response, sizeof(response) - 1, 0)) > 0) {
        response[bytes_received] = '\0';
        printf("%s", response);
    }

    // Close the socket
    close(client_socket);
}

int main() {
    const char* host = "info.cern.ch";  // Replace with the target server's hostname
    int port = 80;                     // Use 443 for HTTPS
    const char* path = "/";            // The path of the resource you want to retrieve

    send_http_request(host, port, path);

    return 0;
}
