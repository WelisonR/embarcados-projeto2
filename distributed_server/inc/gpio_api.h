#ifndef BCM2835_API_H_
#define BCM2835_API_H_

/* header includes */
#include <bcm2835.h>
#include "bme280_defs.h"

/* Structure to store gpio pin and current state */
typedef struct
{
  int gpio;
  int state;
} gpio_state;

struct air_temperature
{
  int air_1_enabled;
  int air_2_enabled;
  float reference_temperature;
  float hysteresis;
};

/* Definition of lamp GPIOs */
#define LAMP_1 RPI_V2_GPIO_P1_11 // GPIO 17
#define LAMP_2 RPI_V2_GPIO_P1_12 // GPIO 18
#define LAMP_3 RPI_V2_GPIO_P1_13 // GPIO 27
#define LAMP_4 RPI_V2_GPIO_P1_15 // GPIO 22

/* Definition of air-conditioning GPIOs */ 
#define AIR_CONDITIONING_1 RPI_V2_GPIO_P1_16 // GPIO 23
#define AIR_CONDITIONING_2 RPI_V2_GPIO_P1_18 // GPIO 24

/* Lamp and air conditioning array positions */
#define AIR_CONDITIONING_1_POS 4
#define AIR_CONDITIONING_2_POS 5

/* Definition of presence sensor GPIOs */
#define PRESENCE_SENSOR_1 RPI_V2_GPIO_P1_22 // GPIO 25
#define PRESENCE_SENSOR_2 RPI_V2_GPIO_P1_37 // GPIO 26

/* Definition of touch sensors GPIOs */
#define TOUCH_SENSOR_1 RPI_V2_GPIO_P1_29 // GPIO 05 (Kitchen door)
#define TOUCH_SENSOR_2 RPI_V2_GPIO_P1_31 // GPIO 06 (Kitchen window)
#define TOUCH_SENSOR_3 RPI_V2_GPIO_P1_32 // GPIO 12 (Living room's door)
#define TOUCH_SENSOR_4 RPI_V2_GPIO_P1_36 // GPIO 16 (Living room's window)
#define TOUCH_SENSOR_5 RPI_V2_GPIO_P1_38 // GPIO 20 (Bedroom window 01)
#define TOUCH_SENSOR_6 RPI_V2_GPIO_P1_40 // GPIO 21 (Bedroom window 02)

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
 * @brief Function used to setup input/output GPIOs pins.
 * 
 * @return void.
 * 
 */
void setup_pins();

/*!
 * @brief Function used to update the gpio states with bcm2835_gpio_lev function.
 * 
 * @param[in, out] sensors          :       all system sensors state defined in gpio_api.h
 * @param[in] sensors_length        :       number of system sensors
 * 
 * @return void.
 */
void update_sensors_state(gpio_state *sensors, int sensors_length);

/*!
 * @brief Function used to invert a device state based on an option (check gpio_api.h).
 * 
 * @param[in, out] devices          :       devices array with 6 items (lamp and air)
 * @param[in] sensors_length        :       option from lamp and air devices (0 to 5)
 * 
 * @return void.
 */
void invert_device_state(gpio_state *devices, struct air_temperature *air, int option);

/*!
 * @brief Function used to setup all devices (lamp and air) to LOW.
 * 
 * @param[in, out] devices          :       devices array with 6 items (lamp and air)
 * @param[in] devices_length        :       length of devices array
 * 
 * @return void.
 */
void set_gpio_devices_low(gpio_state *devices, int devices_length);

/*!
 * @brief Function used to handle program interruption, disable devices and exit.
 * 
 * @param[in, out] devices          :       devices array with 6 items (lamp and air)
 * @param[in] devices_length        :       length of devices array
 * 
 * @return void.
 */
void handle_actuators_interruption(gpio_state *devices, int devices_length);

/*!
 * @brief Function used to control air conditionings according to reference temperature,
 * hysteresis and user choices.
 * 
 * @param[in, out] devices          :       pointer to struct with gpio devices data.
 * @param[in, out] bme280_data          :       pointer to struct with bme280 sensor data.
 * @param[in] air          :       pointer to struct with air configurations.
 * 
 * @return void.
 */
void control_temperature(gpio_state *devices, struct bme280_data *bme280_data, struct air_temperature *air);

#endif /* BCM2835_API_H_ */
