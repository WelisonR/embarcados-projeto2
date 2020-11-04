#include "tcp_server.h"

int client_socket_s;
int server_socket_s;

/*!
 * @brief This functions is used to receive all system data from client (distributed server)
 */
void process_tcp_client()
{
    int value = 0;
    int received_length = recv(client_socket_s, (void *)&value, sizeof(int), 0);
    if (received_length != sizeof(int))
    {
        printf("Houve um problema ao receber os dados.");
    }

    printf("Valor recebido: %d\n", value);
}

/*!
 * @brief Create a TCP/IP socket connection.
 */
int create_server_socket_s()
{
    int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket < 0)
    {
        printf("Erro no socket()\n");
    }

    return tcp_socket;
}

/*!
 * @brief This functions is used to build the server address structure.
 */
void build_server_struct(struct sockaddr_in *server_address)
{
    memset(server_address, 0, sizeof(struct sockaddr_in)); // Zerando a estrutura de dados
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);
    server_address->sin_port = htons(IN_SERVER_PORT);
}

/*!
 * @brief This functions is used to bind server with address.
 */
void bind_server(struct sockaddr_in *server_address)
{
    int status = bind(server_socket_s, (struct sockaddr *)server_address, sizeof(struct sockaddr_in));
    if (status < 0)
    {
        printf("Falha no Bind\n");
    }
}

/*!
 * @brief This functions is used to setup server listen with queue size.
 */
void listen_server()
{
    int queue_size = 100;
    int status = listen(server_socket_s, queue_size);
    if (status < 0)
    {
        printf("Falha no Listen\n");
    }
}

/*!
 * @brief This functions is used to initialize the tcp server activities.
 */
void* initialize_tcp_server()
{
    struct sockaddr_in server_address, client_address;
    unsigned int client_length;

    server_socket_s = create_server_socket_s();
    build_server_struct(&server_address);
    bind_server(&server_address);
    listen_server();

    while (1)
    {
        client_length = sizeof(client_address);
        client_socket_s = accept(server_socket_s, (struct sockaddr *)&client_address, &client_length);
        if (client_socket_s < 0)
        {
            printf("Falha no Accept\n");
        }

        process_tcp_client();

        close(client_socket_s);
    }

    close(server_socket_s);
}

/*!
 * @brief This functions is used to close all tcp_server connections.
 */
void handle_server_close()
{
    close(client_socket_s);
    close(server_socket_s);
}
