#ifndef SYSTEM_MONITOR_H_
#define SYSTEM_MONITOR_H_

/*!
 * @brief This API used to save the current date (DD-MM-YYYY) into a string.
 *
 * @param[in, out] formatted_date    :   string with formatted hour.
 *
 * @return void.
 */
void set_current_formatted_date(char *formatted_date);

/*!
 * @brief This API used to save the current hour (HH:MM:SS) into a string.
 *
 * @param[in, out] fomatted_hour    :   string with formatted hour.
 *
 * @return void.
 */
void set_current_formatted_hour(char *formatted_hour);

/*!
 * @brief Function used to store in csv file the system logs.
 *
 * @param[in] message    :   Message to store into csv file.
 * @param[out] csv file  :   Store temperature data into a csv file.
 *
 * @return void.
 */
void store_system_logs(char *message);

#endif /* SYSTEM_MONITOR_H_ */