#include "tcp_client.h"

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
    memset(server_address, 0, sizeof(struct sockaddr_in)); // Zerando a estrutura de dados
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address->sin_port = htons(SERVER_PORT);
}

void connect_to_server(int client_socket, struct sockaddr_in *server_address)
{
    int server_connection = connect(client_socket, (struct sockaddr *)server_address, sizeof(struct sockaddr_in));
    if (server_connection < 0)
    {
        printf("Erro no connect()\n");
    }
}

int initialize_socket()
{
    struct sockaddr_in server_address;

    int client_socket = create_socket();
    build_server_struct(&server_address);
    connect_to_server(client_socket, &server_address);

    return client_socket;
}

void send_data(int client_socket, struct system_data *transmitted_data)
{
    int transmitted_bytes = send(client_socket, (void *)transmitted_data, sizeof(struct system_data), 0);
    if (transmitted_bytes != sizeof(struct system_data))
    {
        printf("Erro no envio: numero de bytes enviados diferente do esperado\n");
    }
}

void close_socket(int client_socket)
{
    close(client_socket);
}
