#ifndef BCM2835_API_H_
#define BCM2835_API_H_

/* header includes */
#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <signal.h>

/* Definitions to bcm2835 */
#define BCM2835_SUCESS      1
#define BCM2835_FAIL        -1
#define VENTILATOR          RPI_V2_GPIO_P1_18
#define RESISTANCE          RPI_V2_GPIO_P1_16

/*!
 * @brief Function used to enable a specific device.
 *
 * @param[in] device       :       device to be enabled.
 *
 * device:
 * - VENTILATOR
 * - RESISTANCE
 *
 */
void enable_device(int device);

/*!
 * @brief Function used to disable a specific device.
 *
 * @param[in] device       :       device to be disabled.
 *
 * device:
 * - VENTILATOR
 * - RESISTANCE
 *
 */
void disable_device(int device);

/*!
 * @brief Function used to setup the initial state of the actuators.
 *
 * @param[out] bme2835_status       :       success or failure startup
 *
 * bme2835_status:
 * - BCM2835_FAIL: unexpected error when initializing.
 * - BCM2835_SUCESS: devices successfully initialized
 */
int setup_devices();

/*!
 * @brief Function used to handle program interruption, disable devices and exit.
 *
 * @param[in] signal           :   Received system signal.
 *
 */
void handle_interruption(int signal);

#endif /* BCM2835_API_H_ */
