#ifndef _ALARM_H_
#define _ALARM_H_

#include "system_defines.h"

/*!
 * @brief Function used to update alarm status (ON or OFF).
 *
 * @param[in] sensors                :   Struct with sensors that activate alarm.
 * @param[in] alarm_status           :   Alarm status (ON or OFF).
 * @param[out] is_alarm_enabled      :   Alarm avaliable (ON or OFF).
 *
 * @return void.
 */
void update_alarm_status(gpio_state *sensors, int *alarm_status, int *is_alarm_enabled);

#endif // _ALARM_H_