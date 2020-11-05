#ifndef _ALARM_H_
#define _ALARM_H_

#include "system_defines.h"
#include "system_monitor.h"

void update_alarm_status(gpio_state *sensors, int *alarm_status, int *is_alarm_enabled);

#endif // _ALARM_H_