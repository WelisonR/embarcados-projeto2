/* System header files */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Own header files */
#include "tcp_client.h"
#include "system_api.h"

/* System definitions */
#define OUT_SERVER_IP "192.168.0.53"
#define OUT_SERVER_PORT 10023

/* Global variables */
struct sockaddr_in server_address_c;
int client_socket_c;

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to create a socket connection.
 */
int create_client_socker()
{
    client_socket_c = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket_c < 0)
    {
        printf("Não foi possível abrir o socket\n");
    }

    return client_socket_c;
}

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to build the server address structure.
 */
void build_server_struct_c()
{
    memset(&server_address_c, 0, sizeof(struct sockaddr_in));
    server_address_c.sin_family = AF_INET;
    server_address_c.sin_addr.s_addr = inet_addr(OUT_SERVER_IP);
    server_address_c.sin_port = htons(OUT_SERVER_PORT);
}

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to setup server-client connection.
 */
void connect_to_server()
{
    int server_connection = connect(client_socket_c, (struct sockaddr *)&server_address_c, sizeof(struct sockaddr_in));
    if (server_connection < 0)
    {
        printf("Falha no estabelecimento da comunicação.\n");
    }
}

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to setup socket configurations.
 */
void initialize_client_socket()
{
    create_client_socker();
    build_server_struct_c();
    connect_to_server();
}

/*!
 * @brief This functions is used to send system data across socket connection.
 */
void send_data(struct system_data *transmitted_data)
{
    initialize_client_socket();
    int transmitted_bytes = send(client_socket_c, (void *)transmitted_data, sizeof(struct system_data), 0);
    if (transmitted_bytes != sizeof(struct system_data))
    {
        printf("Erro no envio: numero de bytes enviados diferente do esperado\n");
    }
}

/*!
 * @brief This function is used to close socket connection.
 */
void handle_client_close()
{
    close(client_socket_c);
}
