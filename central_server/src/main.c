#include "system_defines.h"
#include "system_monitor.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "alarm.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "system_windows.h"

/* Global variables */
struct all_system_data all_system_data;

pthread_t set_environment_thread;
pthread_t manage_user_inputs;
pthread_t display_system_status;

/* Main functions */
void handle_all_interruptions(int signal);
void *update_alarm();
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

    /* Set alarm initial state as deactivated and avaliable */
    all_system_data.alarm_state.alarm_status = OFF;
    all_system_data.alarm_state.is_alarm_enabled = ON;

    /* Initialize ncurses apresentation */
    init_system_apresentation(&all_system_data);

    /* Create thread to display data and get system inputs */
    pthread_create(&manage_user_inputs, NULL, &setup_menu_windows, NULL);
    usleep(100000); /* Wait thread setup of ncurses input region */
    pthread_create(&display_system_status, NULL, &setup_system_status_interface, NULL);

    /* Initialize tcp communication to retrieve environment informations */
    initialize_tcp_server(&all_system_data);

    return 0;
}

/*!
 * @brief Function used to handle system interruptions and close all connections.
 */
void handle_all_interruptions(int signal)
{
    handle_server_close();
    pthread_cancel(display_system_status);
    pthread_cancel(manage_user_inputs);
    clear_ncurses_alocation();
    exit(0);
}
