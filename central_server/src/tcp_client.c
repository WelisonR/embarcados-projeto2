/* System header files */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/* Own header files */
#include "tcp_client.h"

/* System definitions */
#define OUT_SERVER_IP "192.168.0.52"
#define OUT_SERVER_PORT 10123

/* Global variables */
struct sockaddr_in tcpc_server_address;
int tcpc_client;

/*!
 * @brief This function is used to check if everything was sent.
 */
int check_return_message(int transmitted_bytes, int required_bytes)
{
    if (transmitted_bytes != required_bytes)
    {
        printf("Erro no envio: numero de bytes enviados diferente do esperado\n");
        return -1;
    }

    return 1;
}

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

/*!
 * @brief This function is used to build the server address structure.
 */
void build_server_struct_c()
{
    memset(&tcpc_server_address, 0, sizeof(struct sockaddr_in)); // Zerando a estrutura de dados
    tcpc_server_address.sin_family = AF_INET;
    tcpc_server_address.sin_addr.s_addr = inet_addr(OUT_SERVER_IP);
    tcpc_server_address.sin_port = htons(OUT_SERVER_PORT);
}

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

/*!
 * @brief This function is used to setup socket configurations.
 */
void initialize_client_socket()
{
    create_client_socker();
    build_server_struct_c();
}

/*!
 * @brief This functions is used to send system data across socket connection.
 */
void send_int_data(int option)
{
    initialize_client_socket();
    connect_to_server();
    int transmitted_bytes = send(tcpc_client, (void *)&option, sizeof(int), 0);
    check_return_message(transmitted_bytes, sizeof(int));
}

/*!
 * @brief This functions is used to send temperature information to distributed server.
 */
void send_temperature_data(int option, float reference_temperature, float hysteresis)
{
    initialize_client_socket();
    connect_to_server();

    int transmitted_bytes = send(tcpc_client, (void *)&option, sizeof(int), 0);
    check_return_message(transmitted_bytes, sizeof(int));

    transmitted_bytes = send(tcpc_client, (void *)&reference_temperature, sizeof(float), 0);
    check_return_message(transmitted_bytes, sizeof(int));

    transmitted_bytes = send(tcpc_client, (void *)&hysteresis, sizeof(float), 0);
    check_return_message(transmitted_bytes, sizeof(int));
}

/*!
 * @brief This function is used to close socket connection.
 */
void handle_client_close()
{
    close(tcpc_client);
}
