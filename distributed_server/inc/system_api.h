#ifndef SYSTEM_MONITOR_H_
#define SYSTEM_MONITOR_H_

/* Header includes */
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "bme280_api.h"
#include "gpio_api.h"

/* System definitions */
#define DEVICES_LENGTH 6
#define SENSORS_LENGTH 8

/* System structures */
struct system_data
{
    gpio_state devices[DEVICES_LENGTH];
    gpio_state sensors[SENSORS_LENGTH];
    struct bme280_data bme280_data;
};


/*!
 * @brief This function starts execution of all system actuators.
 * 
 * @return void.
 */
void initialize_system();

/*!
 * @brief Function used to handle alarm that occurs every 100ms (one step)
 * and open threads execution.
 * 
 * @return void.
 * 
 */
void handle_alarm();

/*!
 * @brief Function used to handle system interruptions and close all connections.
 * 
 * @param[in] signal        :       system signal sent by event.
 * 
 */
void handle_system_interruption(int signal);

/*!
 * @brief Function used to set valid system temperatura, humidity and pressure
 */
void *set_environment_data();

/*!
 * @brief Function used to update actuators of continuous tracking
 * as presence sensor and touch sensor.
 */
void *update_actuators();

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

#endif /* SYSTEM_MONITOR_H_ */