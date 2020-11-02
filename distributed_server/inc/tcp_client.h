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
 * @return void.
 *
 */
void build_server_struct();

/*!
 * @brief This function is used to setup server-client connection.
 *
 * @param[in] client_socket             :   socket to connect with server.
 * @param[in, out] server_address       :   struct with server address information.
 * 
 * @return void.
 *
 */
void connect_to_server();

/*!
 * @brief This function is used to setup socket configurations.
 * 
 * @return void.
 *
 */
void initialize_socket();

/*!
 * @brief This functions is used to send system data across socket connection.
 *
 * @param[in] transmitted_data      :   pointer to struct of system_data (all system data)
 * 
 * @return void.
 *
 */
void send_data(struct system_data *transmitted_data);

/*!
 * @brief This function is used to close socket connection.
 *
 * @return void.
 *
 */
void close_socket();

#endif /* TCP_CLIENT_H_ */
