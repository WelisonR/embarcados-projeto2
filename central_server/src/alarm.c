/* Own header files */
#include "alarm.h"
#include "system_monitor.h"

/*!
 * @brief Function used to update alarm status (ON or OFF).
 */
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
                // TODO: play system audio at this point
            }
            return;
        }
    }

    if (*alarm_status == ON)
    {
        store_system_logs("Alarme OFF");
        // TODO: stop system audio at this point
    }

    *alarm_status = OFF;
}
