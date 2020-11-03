#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* System definitions */
#define SERVER_PORT 10023
#define DEVICES_LENGTH 6
#define SENSORS_LENGTH 8

// TODO: top level structures
struct bme280_data
{
    double pressure;
    double temperature;
    double humidity;
};

typedef struct
{
    int gpio;
    int state;
} gpio_state;

struct system_data
{
    gpio_state devices[DEVICES_LENGTH];
    gpio_state sensors[SENSORS_LENGTH];
    struct bme280_data bme280_data;
};

struct system_data all_system_data;

int client_socket;
int server_socket;
struct sockaddr_in server_address;
struct sockaddr_in client_address;

void process_tcp_client()
{
    int received_length = recv(client_socket, (void *)&all_system_data, sizeof(struct system_data), 0);
    if (received_length != sizeof(struct system_data))
    {
        printf("Houve um problema ao receber os dados.");
    }

    for (int i = 0; i < DEVICES_LENGTH; i++)
    {
        printf("Device %d: %d\n", all_system_data.devices[i].gpio, all_system_data.devices[i].state);
    }

    for (int i = 0; i < SENSORS_LENGTH; i++)
    {
        printf("Sensor %d: %d\n", all_system_data.sensors[i].gpio, all_system_data.sensors[i].state);
    }

    printf("T %f U %f P %f", all_system_data.bme280_data.temperature,
           all_system_data.bme280_data.humidity, all_system_data.bme280_data.temperature);
}

void create_socket()
{
    client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0)
    {
        printf("Erro no socket()\n");
    }
}

void build_server_struct()
{
    memset(&server_address, 0, sizeof(struct sockaddr_in)); // Zerando a estrutura de dados
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);
}

void bind_server()
{
    int status = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (status < 0)
    {
        printf("Falha no Bind\n");
    }
}

void listen_server()
{
    int queue_size = 10;
    int status = listen(server_socket, queue_size);
    if (status < 0)
    {
        printf("Falha no Listen\n");
    }
}

void accept_client()
{
    unsigned int client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);
    if (client_socket < 0) {
        printf("Falha no Accept\n");
    }
}

void initialize_socket() {
    create_socket();
    build_server_struct();
    bind_server();
    listen_server();
}

void close_client()
{
    close(client_socket);
}

void close_server()
{
    close(server_socket);
}

int main()
{
    initialize_socket();

    while (1)
    {
        accept_client();
        process_tcp_client();
        close_client();
    }

    close_server();

    return 0;
}
