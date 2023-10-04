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

    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORTNO);

    int sersize = sizeof(seraddr);
    if (bind(sockfd, (struct sockaddr *)&seraddr, sersize) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        int newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd == -1) {
            perror("accept");
            continue; // Continue to the next client
        }

        int size;
        if (read(newsockfd, &size, sizeof(size)) <= 0) {
            perror("read");
            close(newsockfd);
            continue; // Continue to the next client
        }

        int *arr = malloc(sizeof(int) * size);
        if (arr == NULL) {
            perror("malloc");
            close(newsockfd);
            continue; // Continue to the next client
        }

        if (read(newsockfd, arr, sizeof(int) * size) <= 0) {
            perror("read");
            close(newsockfd);
            free(arr);
            continue; // Continue to the next client
        }

        // Process the data here if needed

        write(newsockfd, arr, sizeof(int) * size);

        free(arr);
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
