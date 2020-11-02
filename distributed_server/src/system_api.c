#include "system_api.h"
#include "tcp_client.h"

#define ALARM_MAXIMUM_CYCLE 10
#define ALARM_TIME_SIZE 100000

/* Global variables */
int alarm_step = 0;
int tcp_client = -1;
struct system_data all_system_data = {
    {{LAMP_1, LOW},
     {LAMP_2, LOW},
     {LAMP_3, LOW},
     {LAMP_4, LOW},
     {AIR_CONDITIONING_1, LOW},
     {AIR_CONDITIONING_2, LOW}},
    {{PRESENCE_SENSOR_1, LOW},
     {PRESENCE_SENSOR_2, LOW},
     {TOUCH_SENSOR_1, LOW},
     {TOUCH_SENSOR_2, LOW},
     {TOUCH_SENSOR_3, LOW},
     {TOUCH_SENSOR_4, LOW},
     {TOUCH_SENSOR_5, LOW},
     {TOUCH_SENSOR_6, LOW}},
    {0.0, 0.0, 0.0}};

/* Program threads */
pthread_t set_environment_thread;
pthread_t update_actuators_thread;
pthread_t send_system_data_thread;

/* pthreads mutex controlled by alarm */
pthread_mutex_t set_environment_data_mutex;
pthread_mutex_t update_actuators_mutex;
pthread_mutex_t send_system_data_mutex;

/*!
 * @brief This function starts execution of all system actuators.
 */
void initialize_system()
{
    signal(SIGALRM, handle_alarm);

    /* Setup actuators devices */
    setup_devices();
    set_gpio_devices_low(all_system_data.devices, DEVICES_LENGTH);

    /* Setup bme280 - External temperature */
    setup_bme280();

    /* Setup client socket */
    tcp_client = initialize_socket();

    /* Initialize mutex to threads */
    pthread_mutex_init(&set_environment_data_mutex, NULL);
    pthread_mutex_init(&update_actuators_mutex, NULL);
    pthread_mutex_init(&send_system_data_mutex, NULL);

    /* Lock thread executions */
    pthread_mutex_lock(&set_environment_data_mutex);
    pthread_mutex_lock(&update_actuators_mutex);
    pthread_mutex_lock(&send_system_data_mutex);

    /* Create system threads */
    pthread_create(&set_environment_thread, NULL, &set_environment_data, NULL);
    pthread_create(&update_actuators_thread, NULL, &update_actuators, NULL);
    pthread_create(&send_system_data_thread, NULL, &send_system_data, NULL);

    usleep(10000); /* Wait thread setup of ncurses input region */

    /* Initialize alarm callbacks */
    ualarm(ALARM_TIME_SIZE, ALARM_TIME_SIZE);

    /* Join and finalize threads */
    pthread_join(set_environment_thread, NULL);
    pthread_join(update_actuators_thread, NULL);
    pthread_join(send_system_data_thread, NULL);
}

/*!
 * @brief Function used to handle alarm that occurs every 100ms (one step)
 * and open threads execution.
 */
void handle_alarm()
{
    /* Unlock set temperature and control actuators every 100ms */
    pthread_mutex_unlock(&set_environment_data_mutex);

    /* Unlock LCD display and data storage every 1s */
    if (alarm_step == 0)
    {
        pthread_mutex_unlock(&update_actuators_mutex);
        pthread_mutex_unlock(&send_system_data_mutex);
    }

    /* Control thread execution based on 1s cycles */
    alarm_step = (alarm_step + 1) % ALARM_MAXIMUM_CYCLE;
}

/*!
 * @brief Function used to handle system interruptions and close all connections.
 */
void handle_system_interruption(int signal)
{
    /* As first argument is zero, cancel alarms on Linux (and others) systems */
    ualarm(0, 0);

    /* Close socket connection */
    close_socket(tcp_client);

    /* Cancel all threads activies */
    pthread_cancel(set_environment_thread);
    pthread_cancel(update_actuators_thread);
    pthread_cancel(send_system_data_thread);

    /* Destroy thread mutex */
    pthread_mutex_destroy(&set_environment_data_mutex);
    pthread_mutex_destroy(&update_actuators_mutex);
    pthread_mutex_destroy(&send_system_data_mutex);

    /* Close important system resources */
    handle_actuators_interruption(all_system_data.devices, DEVICES_LENGTH);
    close_bme280();
    exit(0);
}

/*!
 * @brief Function used to send system data across tcp/ip connection.
 */
void *send_system_data()
{
    while (1)
    {
        pthread_mutex_lock(&send_system_data_mutex);
        send_data(tcp_client, &all_system_data);
    }
}

/*!
 * @brief Function used to set valid system temperatura, humidity and pressure
 */
void *set_environment_data()
{
    struct bme280_data sensor_data_temp;

    while (1)
    {
        pthread_mutex_lock(&set_environment_data_mutex);

        int8_t response = set_bme280_data(&sensor_data_temp);
        if (response == BME280_OK)
        {
            all_system_data.bme280_data.temperature = sensor_data_temp.temperature;
            all_system_data.bme280_data.humidity = sensor_data_temp.humidity;
            all_system_data.bme280_data.pressure = sensor_data_temp.pressure;
        }
    }
}

/*!
 * @brief Function used to update actuators of continuous tracking
 * as presence sensor and touch sensor.
 */
void *update_actuators()
{
    while (1)
    {
        pthread_mutex_lock(&update_actuators_mutex);

        update_gpio_state(all_system_data.sensors, SENSORS_LENGTH);
        // TODO: informar ao servidor central se algum sensor está ativado
    }
}

/*!
 * @brief This API used to save the current date (DD-MM-YYYY) into a string.
 */
void set_current_formatted_date(char *formatted_date)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(formatted_date, "%02d-%02d-%04d",
            timeinfo->tm_mday, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
}

/*!
 * @brief This API used to save the current hour (HH:MM:SS) into a string.
 */
void set_current_formatted_hour(char *formatted_hour)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(formatted_hour, "%02d:%02d:%02d",
            timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}
