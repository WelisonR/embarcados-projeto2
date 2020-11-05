#include "system_defines.h"
#include "system_monitor.h"
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

    pthread_create(&set_environment_thread, NULL, &post_it, NULL);

    initialize_tcp_server(&all_system_data);

    return 0;
}

// OPT = 0 -> Alterna lâmpada 1
// OPT = 1 -> Alterna lâmpada 2
// OPT = 2 -> Alterna lâmpada 3
// OPT = 3 -> Alterna lâmpada 4
// OPT = 4 -> Ar-condicionado 1 -> VALOR DA TEMPERATURA
// OPT = 5 -> Ar-condicionado 2 -> VALOR DA TEMPERATURA
// OPT = 6 -> Alterna o alarme
void *post_it()
{
    int option = 0;
    float reference_temperature = 0;
    char message[50];
    while (1)
    {
        scanf("%d", &option);

        if (option >= 0 && option <= 3)
        {
            if (all_system_data.system_data.devices[option].state == ON)
            {
                sprintf(message, "Lâmpada %d OFF", option+1);
            }
            else
            {
                sprintf(message, "Lâmpada %d ON", option+1);
            }
        }
        else if (option >= 4 && option <= 5)
        {
            if (all_system_data.system_data.devices[option].state == ON)
            {
                sprintf(message, "Ar-condicionados 1 e 2 OFF");
            }
            else
            {
                scanf("%f", &reference_temperature);
                sprintf(message, "Ar-condicionados 1 e 2 ON com TR %.2f °C", reference_temperature);
            }
        }

        if(option >= 0 && option <= 5) {
            send_data(option);
        }
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
