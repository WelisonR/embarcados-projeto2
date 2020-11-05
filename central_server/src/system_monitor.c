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

/*!
 * @brief This API used to store in csv file the datetime with temperatures.
 */
void store_system_logs(char *message) {
    char filepath[] = "data/logs.csv";
    FILE *fp = fopen(filepath,"a+");
    fseek (fp, 0, SEEK_END);

    /* If file is not defined, return */
    if(fp == NULL) {
        printf(">> Não foi possível salvar a média dos dados medidos.\n");
        return;
    }

    /* Add header if file is empty */
    if(ftell(fp) == 0) {
        fprintf(fp, "Data (DD-MM-YYYY), Hora (HH:MM:SS), Message\n");
    }

    /* Recover data as DD-MM-YYYY */
    const int date_size = 11;
    char formatted_date[date_size];
    set_current_formatted_date(formatted_date);

    /* Recover current hour as HH:MM:SS */
    const int hour_size = 9;
    char formatted_hour[hour_size];
    set_current_formatted_hour(formatted_hour);

    fprintf(fp, "%s, %s, %s\n", formatted_date, formatted_hour, message);

    // printf(">> Temperaturas salvas em %s.\n", filepath);
    fclose(fp);
}
