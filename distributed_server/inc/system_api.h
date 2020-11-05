#ifndef SYSTEM_API_H_
#define SYSTEM_API_H_

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
    struct air_temperature air_temperature;
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
 * @brief Function used to send system data across tcp/ip connection.
 */
void *send_system_data();

/*!
 * @brief Function used to set valid system temperatura, humidity and pressure
 */
void *set_environment_data();

/*!
 * @brief Function used to update actuators of continuous tracking
 * as presence sensor and touch sensor.
 */
void *update_actuators();

void *update_air_conditioning();

#endif /* SYSTEM_API_H_ */