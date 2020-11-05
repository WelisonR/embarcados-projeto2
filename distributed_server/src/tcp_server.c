#include "tcp_server.h"
#include "system_api.h"

int client_socket_s;
int server_socket_s;

/*!
 * @brief This functions is used to receive all system data from client (distributed server)
 */
void process_tcp_client(struct system_data *all_environment_data)
{
    int option = 0;
    int received_length = recv(client_socket_s, (void *)&option, sizeof(int), 0);
    if (received_length != sizeof(int))
    {
        printf("Houve um problema ao receber os dados.");
    }

    if(option >= 0 || option <= 5) {
        invert_device_state(all_environment_data->devices, &all_environment_data->air_temperature, option);
    }
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

void build_server_setsocket()
{
    int option = 1;
    int status = setsockopt(server_socket_s, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
    if (status < 0)
    {
        printf("Setsocket não atualizado.\n");
    }
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
void* initialize_tcp_server(void* args)
{
    struct system_data *all_environment_data = (struct system_data *) args;

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
            printf("Falha no Accept\n");
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
