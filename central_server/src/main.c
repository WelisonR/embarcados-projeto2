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

void process_tcp_client(int client_socket)
{
    struct system_data all_system_data;
    int received_length = recv(client_socket, (void *)&all_system_data, sizeof(struct system_data), 0);
    if(received_length != sizeof(struct system_data)) {
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

int create_socket()
{
    int client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0)
    {
        printf("Erro no socket()\n");
    }

    return client_socket;
}

void build_server_struct(struct sockaddr_in *server_address)
{
    memset(server_address, 0, sizeof(struct sockaddr_in)); // Zerando a estrutura de dados
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);
    server_address->sin_port = htons(SERVER_PORT);
}

int main()
{
    int client_socket;
    struct sockaddr_in server_address, client_address;
    unsigned int client_length;

    int server_socket = create_socket();
    build_server_struct(&server_address);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        printf("Falha no Bind\n");

    if (listen(server_socket, 10) < 0)
        printf("Falha no Listen\n");

    while (1)
    {
        client_length = sizeof(client_address);
        if ((client_socket = accept(server_socket,
                                    (struct sockaddr *)&client_address,
                                    &client_length)) < 0)
            printf("Falha no Accept\n");

        process_tcp_client(client_socket);

        close(client_socket);
    }

    close(server_socket);

    return 0;
}