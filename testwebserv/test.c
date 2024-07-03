#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 8080

void    print_ip(const struct sockaddr_in address)
{
    printf("IP = %s\n", inet_ntoa(address.sin_addr));
}


struct sockaddr_in  fill_struct_sockaddr_in(void)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    print_ip(address);
    return (address);
}

void    bind_socket(int *server_fd, const struct sockaddr_in address)
{
    const struct sockaddr_in address = fill_struct_sockaddr_in();
    if(bind(*server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        close(*server_fd);
        exit(EXIT_FAILURE);
    }
}

// Create socket and return is fd id. arg are domain here AF_INET = IP, type of service here SOCK_STREAM = TCP, protocol no option default=0, check if creation failed.
void    create_socket(int *server_fd)
{
    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(*server_fd == -1)
    {
        perror("socket failed to create");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    int server_fd;//fd of the server.
    const struct sockaddr_in address;
    create_socket(&server_fd);
    bind_socket(&server_fd, &address);
    listen(server_fd, backlog);
    return 0;
}
