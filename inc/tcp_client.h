#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

/* Header includes */
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "system_api.h"

/* System definitions */
#define SERVER_IP "192.168.0.53"
#define SERVER_PORT 10023

/*!
 * @brief This function is used to create a socket connection with server and port above.
 *
 * @return int with socket identifier.
 *
 */
int create_socket();

/*!
 * @brief This function is used to build the server address structure.
 *
 * @param[in, out] server_address    :   struct with server address information.
 * 
 * @return void.
 *
 */
void build_server_struct(struct sockaddr_in *server_address);

/*!
 * @brief This function is used to save the current hour (HH:MM:SS) into a string.
 *
 * @param[in] client_socket             :   socket to connect with server.
 * @param[in, out] server_address       :   struct with server address information.
 * 
 * @return void.
 *
 */
void connect_to_server(int client_socket, struct sockaddr_in *server_address);

/*!
 * @brief This function is used to setup socket configurations.
 * 
 * @return int with socket identifier.
 *
 */
int initialize_socket();

/*!
 * @brief This functions is used to send system data across socket connection.
 *
 * @param[in] client_socket         :   identifier to client socket
 * @param[in] transmitted_data      :   pointer to struct of system_data (all system data)
 * 
 * @return void.
 *
 */
void send_data(int client_socket, struct system_data *transmitted_data);

/*!
 * @brief This function is used to close socket connection.
 *
 * @param[in] client_socket         :   identifier to client socket
 * 
 * @return void.
 *
 */
void close_socket(int client_socket);

#endif /* TCP_CLIENT_H_ */
