#ifndef SYSTEM_WINDOWS_H_
#define SYSTEM_WINDOWS_H_

/* System header files */
#include <ncurses.h>

/* Own header files */
#include "system_defines.h"

/*!
 * @brief Get "ON" or "OFF" string based on status (0 or 1).
 * 
 * @return "ON" or "OFF".
 */
char *get_on_off_string(int status);

/*!
 * @brief Get "YES" or "NO" string based on status (0 or 1).
 * 
 * @return "YES" or "NO".
 */
char *get_yes_no_string(int status);

/*!
 * @brief Get "OPEN" or "CLOSED" string based on status (0 or 1).
 * 
 * @return "OPEN" or "CLOSED".
 */
char *get_open_closed_string(int status);

/*!
 * @brief Function used to setup ncurses (display) initial configurations.
 * 
 * @return void.
 */
void setup_ncurses_initial_configs();

/*!
 * @brief Function used to display text into a especific window at line, column.
 *
 * @param[in] window                  :   Window instance of ncurses.
 * @param[in] line                    :   Start line to display text.
 * @param[in] column                  :   Start column to display text.
 * @param[in] text                    :   Text to be displayed.
 * @param[out] display                :   Display text into window.
 *
 * @return void.
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
 * @return void.
 */
void display_text_center(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

/*!
 * @brief Function used to setup the title in header of a window.
 *
 * @param[in] window                    :   Window instance of ncurses.
 * @param[in] title                     :   Header menu title.
 * @param[out] display                  :   Display text into header window.
 *
 * @return void.
 */
void setup_window_title(WINDOW *win, char *title);

/*!
 * @brief Function used to read float data in a range with a specific message.
 * 
 *
 * @param[in] window                      :   Window instance of ncurses.
 * @param[in] message                     :   Message to be displayed to read float.
 * @param[in] minimum_value               :   Minimum valid value.
 * @param[in] maximum_value               :   Maximum valid value.
 *
 * @return float value read from user.
 */
float read_float(WINDOW *win, char *message, float minimum_value, float maximum_value);

/*!
 * @brief Function used to read int data in a range with a specific message.
 *
 * @param[in] window                      :   Window instance of ncurses.
 * @param[in] message                     :   Message to be displayed to read float.
 * @param[in] minimum_value               :   Minimum valid value.
 * @param[in] maximum_value               :   Maximum valid value.
 *
 * @return int value read from user.
 */
float read_int(WINDOW *win, char *message, int minimum_value, int maximum_value);


/*!
 * @brief Function used to read float data with a specific message.
 *
 * @param[out] display               :   Display input window with border.
 *
 * @return void.
 */
void setup_input_menu();

/*!
 * @brief Function used to setup the system status area.
 *
 * @param[out] display    :   Display window with system variables information.
 *
 * @return void.
 */
void setup_system_status();

/*!
 * @brief Function used to read float data with a specific message.
 *
 * @param[out] display               :   Display input (menu selection) window with border.
 *
 * @return void.
 */
void setup_iterative_menu();

/*!
 * @brief Closure function to setup_input_menu and setup_iterative_menu. (thread)
 * 
 * @return void*.
 */
void* setup_menu_windows();

/*!
 * @brief Function used to display the system variables values.
 *
 * @param[out] display    :   Display system variables values into screen.
 *
 * @return void.
 */
void update_system_status_window();

/*!
 * @brief Closure function to setup_system_status and update_system_status_window. (thread)
 * 
 * @return void.
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

#endif /* SYSTEM_WINDOWS_H_ */
