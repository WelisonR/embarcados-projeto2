#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

/*!
 * @brief This function is used to check if everything was sent.
 */
int check_return_message(int transmitted_bytes, int required_bytes);

/*!
 * @brief This function is used to create a socket connection.
 *
 * @return int with socket identifier.
 *
 */
int create_client_socket();

/*!
 * @brief This function is used to build the server address structure.
 *
 * @return void.
 *
 */
void build_server_struct_c();

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
void initialize_client_socket();

/*!
 * @brief This functions is used to send system data across socket connection.
 *
 * @param[in] transmitted_data      :   pointer to struct of system_data (all system data)
 * 
 * @return void.
 *
 */
void send_int_data(int option);

/*!
 * @brief This functions is used to send temperature information to distributed server.
 *
 * @param[in] option                    :   4 or 5 - air conditioning 1 or air conditioning 2
 * @param[in] reference_temperature     :   float number with reference temperature.
 * @param[in] hysteresis                :   float number with hysteresis value.
 * 
 * @return void.
 *
 */
void send_temperature_data(int option, float reference_temperature, float hysteresis);

/*!
 * @brief This function is used to close socket connection.
 *
 * @return void.
 *
 */
void handle_client_close();

#endif /* TCP_CLIENT_H_ */
