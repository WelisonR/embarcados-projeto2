#include "tcp_client.h"

/* Global variables */
struct sockaddr_in server_address;
int client_socket;

/*!
 * @brief This function is used to create a socket connection with server and port above.
 */
int create_socket()
{
    client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0)
    {
        printf("Erro no socket()\n");
        raise(SIGABRT);
    }

    return client_socket;
}

/*!
 * @brief This function is used to build the server address structure.
 */
void build_server_struct()
{
    memset(&server_address, 0, sizeof(struct sockaddr_in)); // Zerando a estrutura de dados
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address.sin_port = htons(SERVER_PORT);
}

/*!
 * @brief This function is used to setup server-client connection.
 */
void connect_to_server()
{
    int server_connection = connect(client_socket, (struct sockaddr *) &server_address, sizeof(struct sockaddr_in));
    if (server_connection < 0)
    {
        printf("Erro no connect()\n");
    }
}

/*!
 * @brief This function is used to setup socket configurations.
 */
void initialize_socket()
{

    create_socket();
    build_server_struct();
}

/*!
 * @brief This functions is used to send system data across socket connection.
 */
void send_data(struct system_data *transmitted_data)
{
    initialize_socket();
    connect_to_server();
    int transmitted_bytes = send(client_socket, (void *)transmitted_data, sizeof(struct system_data), 0);
    if (transmitted_bytes != sizeof(struct system_data))
    {
        printf("Erro no envio: numero de bytes enviados diferente do esperado\n");
    }
}

/*!
 * @brief This function is used to close socket connection.
 */
void close_socket()
{
    close(client_socket);
}
