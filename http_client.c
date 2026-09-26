#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 8192

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation error");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Send GET request
    const char *request = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: close\r\n\r\n";
    send(sockfd, request, strlen(request), 0);

    printf(" Request sent to server.\n\n");
    printf("--- Response from Server ---\n");

    // Receive response
    ssize_t bytes;
    while ((bytes = read(sockfd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }

    printf("\n--- End of Response ---\n");
    close(sockfd);
    return 0;
}