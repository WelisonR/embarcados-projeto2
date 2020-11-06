#include "system_defines.h"
#include "system_monitor.h"
#include "tcp_server.h"
#include "tcp_client.h"
#include "alarm.h"
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

    all_system_data.alarm_state.alarm_status = OFF;
    all_system_data.alarm_state.is_alarm_enabled = ON;

    init_system_apresentation(&all_system_data);

    pthread_create(&manage_user_inputs, NULL, &setup_menu_windows, NULL);
    sleep(1); /* Wait thread setup of ncurses input region */
    pthread_create(&display_system_status, NULL, &setup_system_status_interface, NULL);

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
