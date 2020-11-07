/* System header files */
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* Own header files */
#include "tcp_server.h"
#include "alarm.h"

/* System definitions */
#define IN_SERVER_PORT 10023

/* Global variables */
int tcps_client;
int tcps_server;

/*!
 * @brief This functions is used to receive all system data from client (distributed server)
 */
void process_tcp_client(struct all_system_data *all_enviroment_data)
{
    int received_length = recv(tcps_client, (void *)&all_enviroment_data->system_data,
                               sizeof(struct system_data), 0);
    if (received_length != sizeof(struct system_data))
    {
        printf("Houve um problema ao receber os dados.");
    }

    update_alarm_status(all_enviroment_data->system_data.sensors,
                        &all_enviroment_data->alarm_state.alarm_status,
                        &all_enviroment_data->alarm_state.is_alarm_enabled);
}

/*!
 * @brief Create a TCP/IP socket connection.
 */
int create_tcps_server()
{
    int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket < 0)
    {
        printf("Não foi possível abrir o socket\n");
        raise(SIGABRT);
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
 * @brief This function is used to reuse the connection port address of socket server.
 */
void build_server_setsocket()
{
    int option = 1;
    int status = setsockopt(tcps_server, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
    if (status < 0)
    {
        printf("Não foi possível configurar o socket com setsocket.\n");
    }
}

/*!
 * @brief This functions is used to bind server with address.
 */
void bind_server(struct sockaddr_in *server_address)
{
    int status = bind(tcps_server, (struct sockaddr *)server_address, sizeof(struct sockaddr_in));
    if (status < 0)
    {
        printf("Não foi possível realizar o bind do socket.\n");
        raise(SIGABRT);
    }
}

/*!
 * @brief This functions is used to setup server listen with queue size.
 */
void listen_server()
{
    int queue_size = 100;
    int status = listen(tcps_server, queue_size);
    if (status < 0)
    {
        printf("Não foi possível realizar o listen do socket\n");
        raise(SIGABRT);
    }
}

/*!
 * @brief This functions is used to initialize the tcp server activities.
 */
int initialize_tcp_server(struct all_system_data *all_enviroment_data)
{
    struct sockaddr_in server_address, client_address;
    unsigned int client_length;

    tcps_server = create_tcps_server();
    build_server_struct(&server_address);
    build_server_setsocket();
    bind_server(&server_address);
    listen_server();

    while (1)
    {
        client_length = sizeof(client_address);
        tcps_client = accept(tcps_server, (struct sockaddr *)&client_address, &client_length);
        if (tcps_client < 0)
        {
            printf("Falha no estabelecimento da comunicação.\n");
            sleep(1);
        }

        process_tcp_client(all_enviroment_data);

        close(tcps_client);
    }

    close(tcps_server);

    return 0;
}

/*!
 * @brief This functions is used to close all tcp_server connections.
 */
void handle_server_close()
{
    close(tcps_server);
    close(tcps_client);
}
