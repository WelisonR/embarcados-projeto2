#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include "tcp_server.h"
#include "system_api.h"

/* System definitions */
#define IN_SERVER_PORT 10123
#define DEVICES_LENGTH 6
#define SENSORS_LENGTH 8

/* Global variables */
int client_socket_s;
int server_socket_s;

/*!
 * @brief This function is used to receive all system data from client (distributed server)
 */
void process_tcp_client(struct system_data *all_environment_data)
{
    int option = 0;
    int received_length = recv(client_socket_s, (void *)&option, sizeof(int), 0);
    if (received_length != sizeof(int))
    {
        printf("Houve um problema ao receber os dados.");
    }

    if (option >= 0 || option <= 5)
    {
        invert_device_state(all_environment_data->devices, &all_environment_data->air_temperature, option);
    }

    if (option == AIR_CONDITIONING_1_POS || option == AIR_CONDITIONING_2_POS)
    {
        float reference_temperature, hysteresis;
        received_length = recv(client_socket_s, (void *)&reference_temperature, sizeof(float), 0);
        if (received_length != sizeof(float))
        {
            printf("Houve um problema ao receber os dados.");
        }
        received_length = recv(client_socket_s, (void *)&hysteresis, sizeof(float), 0);
        if (received_length != sizeof(float))
        {
            printf("Houve um problema ao receber os dados.");
        }

        if (reference_temperature != -1.0f)
        {
            all_environment_data->air_temperature.reference_temperature = reference_temperature;
        }

        if (hysteresis != -1.0f)
        {
            all_environment_data->air_temperature.hysteresis = hysteresis;
        }
    }
}

/* TODO: Create top file with this function */
/*!
 * @brief Create a TCP/IP socket connection.
 */
int create_server_socket_s()
{
    int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_socket < 0)
    {
        printf("Não foi possível abrir o socket\n");
        raise(SIGABRT);
    }

    return tcp_socket;
}

/* TODO: Create top file with this function */
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

/* TODO: Create top file with this function */
/*!
 * @brief This function is used to reuse the connection port address of socket server.
 */
void build_server_setsocket()
{
    int option = 1;
    int status = setsockopt(server_socket_s, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
    if (status < 0)
    {
        printf("Não foi possível configurar o socket com setsocket.\n");
    }
}

/* TODO: Create top file with this function */
/*!
 * @brief This functions is used to bind server with address.
 */
void bind_server(struct sockaddr_in *server_address)
{
    int status = bind(server_socket_s, (struct sockaddr *)server_address, sizeof(struct sockaddr_in));
    if (status < 0)
    {
        printf("Não foi possível realizar o bind do socket.\n");
        raise(SIGABRT);
    }
}

/* TODO: Create top file with this function */
/*!
 * @brief This functions is used to setup server listen with queue size.
 */
void listen_server()
{
    int queue_size = 100;
    int status = listen(server_socket_s, queue_size);
    if (status < 0)
    {
        printf("Não foi possível realizar o listen do socket\n");
        raise(SIGABRT);
    }
}

/*!
 * @brief This functions is used to initialize the tcp server activities.
 */
void *initialize_tcp_server(void *args)
{
    struct system_data *all_environment_data = (struct system_data *)args;

    struct sockaddr_in server_address, client_address;
    unsigned int client_length;

    server_socket_s = create_server_socket_s();
    build_server_struct(&server_address);
    build_server_setsocket();
    bind_server(&server_address);
    listen_server();

    while (1)
    {
        client_length = sizeof(client_address);
        client_socket_s = accept(server_socket_s, (struct sockaddr *)&client_address, &client_length);
        if (client_socket_s < 0)
        {
            printf("Falha no estabelecimento da comunicação.\n");
            sleep(1);
        }

        process_tcp_client(all_environment_data);

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
