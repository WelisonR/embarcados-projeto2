#ifndef SYSTEM_DEFINES_H_
#define SYSTEM_DEFINES_H_

#define DEVICES_LENGTH 6
#define SENSORS_LENGTH 8

#define ON  1
#define OFF 0

// TODO: top level structures
struct bme280_data
{
    double pressure;
    double temperature;
    double humidity;
};

typedef struct
{
    int gpio;
    int state;
} gpio_state;

struct system_data
{
    gpio_state devices[DEVICES_LENGTH];
    gpio_state sensors[SENSORS_LENGTH];
    struct bme280_data bme280_data;
};

#endif /* SYSTEM_DEFINES_H_ */
