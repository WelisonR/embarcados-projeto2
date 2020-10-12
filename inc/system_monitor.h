#ifndef SYSTEM_MONITOR_H_
#define SYSTEM_MONITOR_H_

/* header includes */
#include <stdio.h>
#include <time.h>

/* Structure that stores the system temperature */
struct system_data
{
    // Internal temperature (°C);
    float internal;
    // External temperature (°C);
    float external;
    // Reference temperature (°C) - defined from terminal or potentiometer.
    float reference;
};

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
void store_temperature_data(struct system_data *temperatures);

/*!
 * @brief Function used to store in csv file the measured temperatures.
 *
 * @param[in] file_descriptor                 :   file descriptor to use with lcd.
 * @param[in] temperature                     :   all system temperature.
 * @param[out] temperature in lcd             :   display temperature in 16x2 lcd display.
 *
 * temperature:
 * - internal.
 * - external.
 * - reference.
 *
 */
void display_temperatures_lcd(int file_descriptor, struct system_data *temperature);

#endif /* SYSTEM_MONITOR_H_ */