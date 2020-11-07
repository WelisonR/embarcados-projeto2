#ifndef SYSTEM_API_H_
#define SYSTEM_API_H_

/* Header includes */
#include "bme280_defs.h"
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
 */
void handle_alarm();

/*!
 * @brief Function used to handle system interruptions and close all connections.
 * 
 * @param[in] signal        :       system signal sent by event.
 * 
 * @return void.
 */
void handle_system_interruption(int signal);

/*!
 * @brief Function used to send all system data across tcp/ip connection (therad).
 * 
 * @return void*.
 */
void *send_system_data();

/*!
 * @brief Function used to set valid system temperature, humidity and pressure from bme280 (thread).
 * 
 * @return void*.
 */
void *set_environment_data();

/*!
 * @brief Function used to update actuators to continuous track
 * presence sensors and touch sensors (thread).
 * 
 * @return void*.
 */
void *update_actuators();

/*!
 * @brief Function used to control air conditionings according to reference temperature,
 * hysteresis and user choices. (thread)
 * 
 * @return void*.
 */
void *update_air_conditioning();

#endif /* SYSTEM_API_H_ */