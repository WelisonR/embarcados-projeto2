#include "system_defines.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "alarm.h"
#include <signal.h>
#include <pthread.h>

/* Global variables */
struct all_system_data all_system_data;
pthread_t set_environment_thread;

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

    all_system_data.alarm_state.alarm_status = OFF;
    all_system_data.alarm_state.is_alarm_enabled = ON;

    pthread_create(&set_environment_thread, NULL, &post_it, NULL);

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

/*!
 * @brief Function used to handle system interruptions and close all connections.
 */
void handle_all_interruptions(int signal)
{
    handle_server_close();
    exit(0);
}
