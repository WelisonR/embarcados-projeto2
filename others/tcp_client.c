#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "192.168.0.53"
#define SERVER_PORT 10023

int create_socket()
{
    int client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0)
    {
        printf("Erro no socket()\n");
    }

    return client_socket;
}

void build_server_struct(struct sockaddr_in *server_address)
{
    memset(server_address, 0, sizeof(server_address)); // Zerando a estrutura de dados
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address->sin_port = htons(SERVER_PORT);
}

void connect_to_server(int client_socket, struct sockaddr_in *server_address, int server_size)
{
    int server_connection = connect(client_socket, (struct sockaddr *)server_address, server_size);
    if (server_connection < 0)
    {
        printf("Erro no connect()\n");
    }
}

struct data {
    float val1;
    float val2;
};

int main(int argc, char *argv[])
{
    struct sockaddr_in server_address;

    int client_socket = create_socket();
    build_server_struct(&server_address);
    connect_to_server(client_socket, &server_address, sizeof(server_address));

    struct data dt;
    dt.val1 = 10.0f;
    dt.val2 = 19.0f;

    if (send(client_socket, (void *) &dt, sizeof(dt), 0) != sizeof(dt))
        printf("Erro no envio: numero de bytes enviados diferente do esperado\n");

    close(client_socket);
    exit(0);
}
