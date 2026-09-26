#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define WWW_ROOT "www"

void send_file(int client_fd, const char *filepath) {
    int file_fd = open(filepath, O_RDONLY);
    if (file_fd < 0) {
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<h1>404 Not Found</h1>";
        send(client_fd, not_found, strlen(not_found), 0);
        return;
    }

    struct stat st;
    fstat(file_fd, &st);
    long file_size = st.st_size;

    char *content_type = "application/octet-stream";
    if (strstr(filepath, ".html")) content_type = "text/html";

    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: close\r\n"
             "\r\n", content_type, file_size);

    send(client_fd, header, strlen(header), 0);

    char buffer[BUFFER_SIZE];
    ssize_t bytes;
    while ((bytes = read(file_fd, buffer, sizeof(buffer))) > 0) {
        send(client_fd, buffer, bytes, 0);
    }
    close(file_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("HTTP Server started successfully!\n");
    printf("Open your browser and go to: http://127.0.0.1:8080\n\n");

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            continue;
        }

        read(client_fd, buffer, BUFFER_SIZE - 1);

        char *get_pos = strstr(buffer, "GET ");
        if (get_pos) {
            char path[256] = {0};
            sscanf(get_pos + 4, "%255s", path);
            if (strcmp(path, "/") == 0) strcpy(path, "/index.html");

            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s%s", WWW_ROOT, path);
            send_file(client_fd, filepath);
        }

        close(client_fd);
    }
}
