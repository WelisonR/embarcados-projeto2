#include "tcp_server.h"

int client_socket;
int server_socket;

/*!
 * @brief This functions is used to receive all system data from client (distributed server)
 */
void process_tcp_client(struct system_data *all_enviroment_data)
{
    int received_length = recv(client_socket, (void *)all_enviroment_data, sizeof(struct system_data), 0);
    if (received_length != sizeof(struct system_data))
    {
        printf("Houve um problema ao receber os dados.");
    }

    for (int i = 0; i < DEVICES_LENGTH; i++)
    {
        printf("Device %d: %d\n", all_enviroment_data->devices[i].gpio, all_enviroment_data->devices[i].state);
    }

    for (int i = 0; i < SENSORS_LENGTH; i++)
    {
        printf("Sensor %d: %d\n", all_enviroment_data->sensors[i].gpio, all_enviroment_data->sensors[i].state);
    }

    printf("T %f U %f P %f", all_enviroment_data->bme280_data.temperature,
           all_enviroment_data->bme280_data.humidity, all_enviroment_data->bme280_data.temperature);
}

/*!
 * @brief Create a TCP/IP socket connection.
 */
int create_socket()
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
    server_address->sin_port = htons(SERVER_PORT);
}

/*!
 * @brief This functions is used to bind server with address.
 */
void bind_server(struct sockaddr_in *server_address)
{
    int status = bind(server_socket, (struct sockaddr *)server_address, sizeof(struct sockaddr_in));
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
    int status = listen(server_socket, queue_size);
    if (status < 0)
    {
        printf("Falha no Listen\n");
    }
}

/*!
 * @brief This functions is used to initialize the tcp server activities.
 */
int initialize_tcp_server(struct system_data *all_enviroment_data)
{
    struct sockaddr_in server_address, client_address;
    unsigned int client_length;

    server_socket = create_socket();
    build_server_struct(&server_address);
    bind_server(&server_address);
    listen_server();

    while (1)
    {
        client_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
        if (client_socket < 0)
        {
            printf("Falha no Accept\n");
        }

        process_tcp_client(all_enviroment_data);

        close(client_socket);
    }

    close(server_socket);

    return 0;
}

/*!
 * @brief This functions is used to close all tcp_server connections.
 */
void handle_server_close() {
    close(client_socket);
    close(server_socket);
}
