#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

/* Header includes */
#include <arpa/inet.h>
#include "system_api.h"

/*!
 * @brief This functions is used to receive all system data from client (distributed server)
 *
 * @param[in, out] all_enviroment_data      :   pointer to struct of system_data (all system data)
 *
 * @return void.
 */
void process_tcp_client(struct system_data *all_environment_data);

/*!
 * @brief Create a TCP/IP socket connection.
 *
 * @return int with socket identifier.
 */
int create_server_socket();

/*!
 * @brief This functions is used to build the server address structure.
 *
 * @param[in, out] server_address      :   struct of sockaddr_in to be builded.
 * 
 * @return void.
 */
void build_server_struct(struct sockaddr_in *server_address);

/*!
 * @brief This function is used to reuse the connection port address of socket server.
 * 
 * @return void.
 */
void build_server_setsocket();

/*!
 * @brief This functions is used to bind server with address.
 *
 * @param[in, out] server_address      :   struct of sockaddr_in with server address.
 * 
 * @return void.
 */
void bind_server(struct sockaddr_in *server_address);

/*!
 * @brief This functions is used to setup server listen with queue size.
 * 
 * @return void.
 */
void listen_server();

/*!
 * @brief This functions is used to initialize the tcp server activities.
 *
 * @param[in] all_enviroment_data      :   pointer to struct of system_data (all system data)
 * 
 * @return void.
 */
void* initialize_tcp_server(void* args);

/*!
 * @brief This functions is used to close all tcp_server connections.
 * 
 * @return void.
 */
void handle_server_close();

#endif /* TCP_SERVER_H_ */