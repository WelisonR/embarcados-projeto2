#include "alarm.h"
#include "system_monitor.h"

void update_alarm_status(gpio_state *sensors, int *alarm_status, int *is_alarm_enabled)
{
    for (int i = 0; i < SENSORS_LENGTH; i++)
    {
        if (sensors[i].state == ON && *is_alarm_enabled == ON)
        {
            if (*alarm_status == OFF)
            {
                store_system_logs("Alarme ON");
                *alarm_status = ON;
                // play .mp3
            }
            return;
        }
    }

    if (*alarm_status == ON) {
        store_system_logs("Alarme OFF");
        // stop .mp3
    }
    *alarm_status = OFF;
}
