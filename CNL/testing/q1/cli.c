#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTNO 9000

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Use loopback address for testing on the same machine
    address.sin_port = htons(PORTNO);

    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    int arr[5] = {1, 2, 3, 4, 5};
    int size = 5;

    write(sockfd, &size, sizeof(size));
    write(sockfd, arr, sizeof(int) * size);

    int response[5];
    if (read(sockfd, response, sizeof(int) * size) <= 0) {
        perror("read");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", response[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}
