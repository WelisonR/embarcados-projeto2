#ifndef SYSTEM_MONITOR_H_
#define SYSTEM_MONITOR_H_

/* header includes */
#include <stdio.h>
#include <time.h>

/*!
 * @brief This API used to save the current date (DD-MM-YYYY) into a string.
 *
 * @param[in, out] formatted_date    :   string with formatted hour.
 *
 */
void set_current_formatted_date(char *formatted_date);

/*!
 * @brief This API used to save the current hour (HH:MM:SS) into a string.
 *
 * @param[in, out] fomatted_hour    :   string with formatted hour.
 *
 */
void set_current_formatted_hour(char *formatted_hour);

/*!
 * @brief Function used to store in csv file the measured temperatures.
 *
 * @param[in] system_data    :   Structure instance of system_data.
 * @param[out] csv file             :   Store temperature data into a csv file.
 *
 */
void store_system_logs(char *message);

#endif /* SYSTEM_MONITOR_H_ */