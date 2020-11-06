#ifndef SYSTEM_WINDOWS_H_
#define SYSTEM_WINDOWS_H_

#include <menu.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "system_defines.h"
#include "system_monitor.h"
#include "tcp_client.h"

/* Header definitions */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4
#define ENTER_KEY 10

#define DISPLAY_WIDTH COLS
#define HEADER_X 0
#define HEADER_Y 13
#define SYSTEM_STATUS_HEIGHT 13
#define MENU_HEIGHT 10
#define INPUT_FIELD_HEIGHT 5

#define MAX_REFERENCE_TEMPERATURE 100.0f
#define MIN_REFERENCE_TEMPERATUER 10.0f
#define MIN_SYSTEM_HYSTERESIS 0.0f
#define MAX_SYSTEM_HYSTERESIS 25.0f
#define MIN_LAMP_NUMBER 1
#define MAX_LAMP_NUMBER 4

/*!
 * @brief Function used to setup ncurses (display) initial configurations.
 */
void setup_ncurses_initial_configs();

/*!
 * @brief Function used to display text into a especific window at line, column.
 *
 * @param[in] window                  :   Window instance of ncurses.
 * @param[in] line                    :   Start line to display text.
 * @param[in] column                  :   Start column to display text.
 * @param[in] text                    :   Text to be displayed.
 * @param[out] display                :    Display text into window.
 *
 */
void display_text(WINDOW *win, int line, int column, char *text);

/*!
 * @brief Function used to display centered text into a especific window at line, column.
 *
 * @param[in] window                    :   Window instance of ncurses.
 * @param[in] starty                    :   Y coordinate of pair (X, Y)
 * @param[in] startx                    :   X coordinate of pair (X, Y)
 * @param[in] width                     :   Width to be centralized
 * @param[in] string                    :   Text to be displayed.
 * @param[in] color                     :   Color of background and foreground.
 * @param[out] display                  :   Display text into window.
 *
 */
void display_text_center(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

/*!
 * @brief Function used to setup the title in header of a window.
 *
 * @param[in] window                    :   Window instance of ncurses.
 * @param[in] title                     :   Header menu title.
 * @param[out] display                  :   Display text into header window.
 *
 */
void setup_window_title(WINDOW *win, char *title);

/*!
 * @brief Function used to read float data with a specific message.
 *
 * @param[in] window                      :   Window instance of ncurses.
 * @param[in] message                     :   Message to be displayed to read float.
 * @param[in] minimum_value               :   Minimum valid value.
 * @param[in] maximum_value               :   Maximum valid value.
 *
 * @retval Read float value.
 *
 */
float read_float(WINDOW *win, char *message, float minimum_value, float maximum_value);

/*!
 * @brief Function used to set user input as reference temperature (IS_KEYBOARD_REFERENCE)
 *
 * @param[in] temp                      :   Value to be defined as reference temperature.
 *
 */
void set_keyboard_reference_temperature(float value);

/*!
 * @brief Function used to set potentiometer as reference temperature. (IS_POTENTIOMETER_REFERENCE)
 */
void set_potentiometer_reference_temperature();

/*!
 * @brief Function used to set a float value to system hysteresis.
 *
 * @param[in] temp                      :   Value to be defined as hysteresis.
 *
 */
void set_hysteresis(float value);

/*!
 * @brief Function used to read float data with a specific message.
 *
 * @param[out] display               :   Display input (float data) window with border.
 *
 */
void setup_input_menu();

/*!
 * @brief Function used to setup the system status area.
 *
 * @param[out] display    :   Display window with system variables information.
 *
 */
void setup_system_status();

/*!
 * @brief Function used to read float data with a specific message.
 *
 * @param[out] display               :   Display input (menu selection) window with border.
 *
 */
void setup_iterative_menu();

/*!
 * @brief Closure function to setup_input_menu and setup_iterative_menu. (thread)
 */
void* setup_menu_windows();

/*!
 * @brief Function used to display the system variables values.
 *
 * @param[out] display    :   Display system variables values into screen.
 *
 */
void update_system_status_window();

/*!
 * @brief Closure function to setup_system_status and update_system_status_window. (thread)
 */
void* setup_system_status_interface();

/*!
 * @brief Clean all resources allocated by ncurses.
 */
void clear_ncurses_alocation();

/*!
 * @brief Setup initial configurations to ncurses and set pointer to enviroment_data.
 */
void init_system_apresentation(struct all_system_data *env_data);

float read_int(WINDOW *win, char *message, int minimum_value, int maximum_value);

char *get_on_off_string(int status);

char *get_yes_no_string(int status);

char *get_open_closed_string(int status);

#endif /* SYSTEM_WINDOWS_H_ */
