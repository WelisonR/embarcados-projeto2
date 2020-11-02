#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 10023

struct data {
    float val1;
    float val2;
};

void TrataClienteTCP(int socketCliente)
{
    struct data dt;
    int tamanhoRecebido = recv(socketCliente, (void *) &dt, sizeof(dt), 0);

    printf("Valores recebidos: %f %f\n", dt.val1, dt.val2);
}

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
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);
    server_address->sin_port = htons(SERVER_PORT);
}

int main(int argc, char *argv[])
{
    int client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    unsigned int client_length;

    int server_socket = create_socket();
    build_server_struct(&server_address);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        printf("Falha no Bind\n");

    if (listen(server_socket, 10) < 0)
        printf("Falha no Listen\n");

    while (1)
    {
        client_length = sizeof(client_address);
        if ((client_socket = accept(server_socket,
                                    (struct sockaddr *)&client_address,
                                    &client_length)) < 0)
            printf("Falha no Accept\n");

        printf("Conexão do Cliente %s\n", inet_ntoa(client_address.sin_addr));

        TrataClienteTCP(client_socket);
        close(client_socket);
    }
    close(server_socket);
}
