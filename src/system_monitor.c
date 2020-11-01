#include "system_monitor.h"

/*!
 * @brief This API used to save the current date (DD-MM-YYYY) into a string.
 */
void set_current_formatted_date(char *formatted_date) {
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(formatted_date, "%02d-%02d-%04d",
        timeinfo->tm_mday, timeinfo->tm_mon+1, 1900+timeinfo->tm_year);
}

/*!
 * @brief This API used to save the current hour (HH:MM:SS) into a string.
 */
void set_current_formatted_hour(char *formatted_hour) {
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(formatted_hour, "%02d:%02d:%02d",
        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

