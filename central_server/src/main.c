#include "system_defines.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "alarm.h"
#include <signal.h>
#include <pthread.h>

/* Global variables */
struct system_data all_system_data;
pthread_t set_environment_thread;
pthread_t update_alarm_thread;
int alarm_status = OFF;    // ON, OFF
int is_alarm_enabled = ON; // ON, OFF

/* Main functions */
void handle_all_interruptions(int signal);
void* update_alarm();
void *post_it();

/*!
 * @brief This function starts execution of the program.
 */
int main(int argc, char *argv[])
{
    signal(SIGHUP, handle_all_interruptions);
    signal(SIGINT, handle_all_interruptions);
    signal(SIGQUIT, handle_all_interruptions);
    signal(SIGILL, handle_all_interruptions);
    signal(SIGABRT, handle_all_interruptions);
    signal(SIGBUS, handle_all_interruptions);
    signal(SIGSEGV, handle_all_interruptions);
    signal(SIGPIPE, handle_all_interruptions);

    pthread_create(&set_environment_thread, NULL, &post_it, NULL);
    pthread_create(&update_alarm_thread, NULL, &update_alarm, NULL);

    initialize_tcp_server(&all_system_data);

    return 0;
}

void *post_it()
{
    int option = 0;
    while (1)
    {
        scanf("%d", &option);
        send_data(option);
    }
}

void* update_alarm()
{
    while (1)
    {
        update_alarm_status(all_system_data.sensors, &alarm_status, &is_alarm_enabled);
        sleep(1);
    }
}

/*!
 * @brief Function used to handle system interruptions and close all connections.
 */
void handle_all_interruptions(int signal)
{
    handle_server_close();
    exit(0);
}
