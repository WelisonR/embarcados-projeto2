#include "tcp_server.h"
#include "tcp_client.h"
#include <signal.h>

struct system_data all_system_data;

/* Main functions */
void handle_all_interruptions(int signal);

/*!
 * @brief This function starts execution of the program.
 */
int main(int argc, char *argv[]) {
    signal(SIGHUP, handle_all_interruptions);
    signal(SIGINT, handle_all_interruptions);
    signal(SIGQUIT, handle_all_interruptions);
    signal(SIGILL, handle_all_interruptions);
    signal(SIGABRT, handle_all_interruptions);
    signal(SIGBUS, handle_all_interruptions);
    signal(SIGSEGV, handle_all_interruptions);
    signal(SIGPIPE, handle_all_interruptions);

    sleep(2);

    initialize_tcp_server(&all_system_data);

    return 0;
}

/*!
 * @brief Function used to handle system interruptions and close all connections.
 */
void handle_all_interruptions(int signal)
{
    // handle_server_close();
    exit(0);
}
