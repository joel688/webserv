#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 2

void    respond_to_client(int *new_socket)
{
    char *hello = "Hello from the serv!";
    write(*new_socket, hello, strlen(hello));
}

void    read_from_client(int *new_socket)
{
    int valread;
    char buffer[1024] = {0};
    valread = read(*new_socket, buffer, 1024);
    printf("%s\n", buffer);
    if(valread < 0)
    {
        printf("No bytes are there to read");
    }
}

void    accept_connection(int *server_fd, int *new_socket, struct sockaddr_in *client_address)
{
    socklen_t client_addr_len = sizeof(*client_address);

    while(1)
    {
        printf("\n-----WAITING FOR CONNECTION-----\n\n");
        if((*new_socket = accept(*server_fd, (struct sockaddr *)client_address, &client_addr_len)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        printf("Accepted connection from %s\n", inet_ntoa(client_address->sin_addr));
        read_from_client(new_socket);
        respond_to_client(new_socket);
    }
    close(*new_socket);
    close(*server_fd);
}

void    listen_for_connection(int *server_fd)
{
    if(listen(*server_fd, BACKLOG) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
}

void    print_ip(struct sockaddr_in *address)
{
    printf("IP = %s\n", inet_ntoa(address->sin_addr));
}

void    fill_struct_sockaddr_in(struct sockaddr_in *address)
{
    memset(address, 0, sizeof(*address));
    address->sin_family = AF_INET;
    address->sin_port = htons(PORT);
    address->sin_addr.s_addr = INADDR_ANY;
}

void    bind_socket(int *server_fd, struct sockaddr_in *address)
{
    fill_struct_sockaddr_in(address);
    if(bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0)
    {
        perror("Bind failed");
        close(*server_fd);
        exit(EXIT_FAILURE);
    }
}

// Create socket and return is fd id. arg are domain here AF_INET = IP, type of service here SOCK_STREAM = TCP, protocol no option default=0, check if creation failed.
void    create_socket(int *server_fd)
{
    int opt = 1;
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(*server_fd <= 0)
    {
        perror("socket failed to create");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(*server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in server_address, client_address;

    create_socket(&server_fd);
    bind_socket(&server_fd, &server_address);
    print_ip(&server_address);
    listen_for_connection(&server_fd);
    accept_connection(&server_fd, &new_socket, &client_address);
    return 0;
}
