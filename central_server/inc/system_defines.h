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

struct air_temperature
{
  int air_1_enabled;
  int air_2_enabled;
  float reference_temperature;
  float hysteresis;
};

struct system_data
{
    gpio_state devices[DEVICES_LENGTH];
    gpio_state sensors[SENSORS_LENGTH];
    struct bme280_data bme280_data;
    struct air_temperature air_temperature;
};

struct alarm_state
{
    int alarm_status;
    int is_alarm_enabled;
};

struct all_system_data
{
    struct system_data system_data;
    struct alarm_state alarm_state;
};

#endif /* SYSTEM_DEFINES_H_ */
