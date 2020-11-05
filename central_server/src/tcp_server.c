#include "tcp_server.h"

int client_socket_s;
int server_socket_s;

/*!
 * @brief This functions is used to receive all system data from client (distributed server)
 */
void process_tcp_client(struct all_system_data *all_enviroment_data)
{
    int received_length = recv(client_socket_s, (void *)&all_enviroment_data->system_data,
                               sizeof(struct system_data), 0);
    if (received_length != sizeof(struct system_data))
    {
        printf("Houve um problema ao receber os dados.");
    }

    update_alarm_status(all_enviroment_data->system_data.sensors,
                        &all_enviroment_data->alarm_state.alarm_status,
                        &all_enviroment_data->alarm_state.is_alarm_enabled);

    // for (int i = 0; i < DEVICES_LENGTH; i++)
    // {
    //     printf("Device %d: %d\n", all_enviroment_data->system_data->devices[i].gpio, all_enviroment_data->system_data->devices[i].state);
    // }

    // for (int i = 0; i < SENSORS_LENGTH; i++)
    // {
    //     printf("Sensor %d: %d\n", all_enviroment_data->system_data->sensors[i].gpio, all_enviroment_data->system_data->sensors[i].state);
    // }

    // printf("T %f U %f P %f", all_enviroment_data->system_data->bme280_data.temperature,
    //        all_enviroment_data->system_data->bme280_data.humidity, all_enviroment_data->system_data->bme280_data.temperature);
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
int initialize_tcp_server(struct all_system_data *all_enviroment_data)
{
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

        process_tcp_client(all_enviroment_data);

        close(client_socket_s);
    }

    close(server_socket_s);

    return 0;
}

/*!
 * @brief This functions is used to close all tcp_server connections.
 */
void handle_server_close()
{
    close(server_socket_s);
    close(client_socket_s);
}
