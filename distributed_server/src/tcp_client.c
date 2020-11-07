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
struct sockaddr_in tcpc_server_address;
int tcpc_client;

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to create a socket connection.
 */
int create_client_socker()
{
    tcpc_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpc_client < 0)
    {
        printf("Não foi possível abrir o socket\n");
    }

    return tcpc_client;
}

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to build the server address structure.
 */
void build_server_struct_c()
{
    memset(&tcpc_server_address, 0, sizeof(struct sockaddr_in));
    tcpc_server_address.sin_family = AF_INET;
    tcpc_server_address.sin_addr.s_addr = inet_addr(OUT_SERVER_IP);
    tcpc_server_address.sin_port = htons(OUT_SERVER_PORT);
}

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to setup server-client connection.
 */
void connect_to_server()
{
    int server_connection = connect(tcpc_client, (struct sockaddr *)&tcpc_server_address, sizeof(struct sockaddr_in));
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
    int transmitted_bytes = send(tcpc_client, (void *)transmitted_data, sizeof(struct system_data), 0);
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
    close(tcpc_client);
}
