#include "system_windows.h"

/* Global variables */
ITEM **list_items;
MENU *selection_menu;
WINDOW *main_menu_window;
WINDOW *float_input_window;
WINDOW *system_status_window;

int user_input;
int choices_size;
char *selected_item_name;
char selected_item_value;

struct all_system_data *environment_data;

char *MENU_CHOICES[] = {
    "1. Invert lamp state.",
    "2. Invert alarm state.",
    "3. Activate/Deactivate air conditioning.",
    " ",
    "Press CTRL + C to stop the program!",
    (char *)NULL,
};

/*!
 * @brief Function used to setup ncurses (display) initial configurations.
 */
void setup_ncurses_initial_configs()
{
    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
}

/*!
 * @brief Function used to display text into a especific window at line, column.
 */
void display_text(WINDOW *win, int line, int column, char *text)
{
    if (win == NULL)
        win = stdscr;
    wmove(win, line, column);
    wclrtoeol(win);
    mvwprintw(win, line, column, text);
    wrefresh(win);
    box(win, 0, 0);
    wrefresh(win);
}

/*!
 * @brief Function used to display centered text into a especific window at line, column.
 */
void display_text_center(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}

/*!
 * @brief Function used to setup the title in header of a window.
 */
void setup_window_title(WINDOW *win, char *title)
{
    /* Print a border around the main window and print a title */
    box(win, 0, 0);
    display_text_center(win, 1, 0, DISPLAY_WIDTH, title, COLOR_PAIR(1));
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, DISPLAY_WIDTH - 2);
    mvwaddch(win, 2, DISPLAY_WIDTH - 1, ACS_RTEE);
    refresh();
}

/*!
 * @brief Function used to read float data with a specific message.
 */
float read_float(WINDOW *win, char *message, float minimum_value, float maximum_value)
{
    float value = -1;
    char error_message[50];

    echo();
    display_text(float_input_window, 3, 1, message);
    wscanw(float_input_window, "%f", &value);
    noecho();
    wrefresh(float_input_window);

    if (value < minimum_value || value > maximum_value)
    {
        sprintf(error_message, " >> Valid interval: %.2f - %.2f", minimum_value, maximum_value);
        display_text(float_input_window, 3, 1, error_message);
        return -1;
    }

    return value;
}

float read_int(WINDOW *win, char *message, int minimum_value, int maximum_value)
{
    int value = -1;
    char error_message[50];

    echo();
    display_text(float_input_window, 3, 1, message);
    wscanw(float_input_window, "%d", &value);
    noecho();
    wrefresh(float_input_window);

    if (value < minimum_value || value > maximum_value)
    {
        sprintf(error_message, " >> Valid interval: %d - %d", minimum_value, maximum_value);
        display_text(float_input_window, 3, 1, error_message);
        return -1;
    }

    return value;
}

/*!
 * @brief Function used to read float data with a specific message.
 *
 * @param[out] display               :   Display input (float data) window with border.
 *
 */
void setup_input_menu()
{
    float_input_window = newwin(INPUT_FIELD_HEIGHT, DISPLAY_WIDTH, MENU_HEIGHT + HEADER_Y, HEADER_X);
    setup_window_title(float_input_window, "System Inputs/Outputs");
    keypad(float_input_window, TRUE);
    box(float_input_window, 0, 0);
    wrefresh(float_input_window);
}

/*!
 * @brief Function used to setup the system status area.
 *
 * @param[out] display    :   Display window with system variables information.
 *
 */
void setup_system_status()
{
    system_status_window = newwin(SYSTEM_STATUS_HEIGHT, COLS, 0, 0);
    setup_window_title(system_status_window, "System Data Values");
    box(system_status_window, 0, 0);
    wrefresh(system_status_window);
}

/*!
 * @brief Function used to read float data with a specific message.
 */
void setup_iterative_menu()
{
    /* Create items */
    choices_size = ARRAY_SIZE(MENU_CHOICES);
    list_items = (ITEM **)calloc(choices_size, sizeof(ITEM *));
    for (int i = 0; i < choices_size; ++i)
    {
        /* empty description, first char is the command type */
        list_items[i] = new_item(MENU_CHOICES[i], "");
    }
    /* Empty row and exit row are not selectable */
    item_opts_off(list_items[3], O_SELECTABLE);
    item_opts_off(list_items[4], O_SELECTABLE);

    /* Crate menu */
    selection_menu = new_menu((ITEM **)list_items);

    /* Create the window to be associated with the menu */
    main_menu_window = newwin(MENU_HEIGHT, DISPLAY_WIDTH, HEADER_Y, HEADER_X);
    keypad(main_menu_window, TRUE);

    WINDOW *iterative_menu = subwin(main_menu_window, MENU_HEIGHT - 4, DISPLAY_WIDTH - 1, HEADER_Y + 4, HEADER_X + 1);

    /* Set main window and sub window */
    set_menu_win(selection_menu, main_menu_window);
    set_menu_sub(selection_menu, iterative_menu);
    refresh();

    /* Set menu mark to the string " * " */
    set_menu_mark(selection_menu, " * ");

    setup_window_title(main_menu_window, "Definition of System Variables");

    /* Post the menu */
    post_menu(selection_menu);
    wrefresh(main_menu_window);

    int option;
    float reference_temperature, hysteresis;
    char message[100];
    /* Execute until CTRL + C */
    while (1)
    {
        user_input = wgetch(main_menu_window);

        reference_temperature = -1.0f;
        hysteresis = -1.0f;

        switch (user_input)
        {
        case KEY_DOWN:
            menu_driver(selection_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(selection_menu, REQ_UP_ITEM);
            break;
        case ENTER_KEY:
            selected_item_name = (char *)item_name(current_item(selection_menu));
            selected_item_value = *selected_item_name;
            if (selected_item_value == '1')
            {
                option = read_int(float_input_window, "Type the lamp number >> ", MIN_LAMP_NUMBER, MAX_LAMP_NUMBER);
                wrefresh(float_input_window);
                if (option == -1)
                {
                    continue;
                }

                option--;

                if (environment_data->system_data.devices[option].state == ON)
                {
                    sprintf(message, "Lamp %d OFF", option+1);
                }
                else
                {
                    sprintf(message, "Lamp %d ON", option+1);
                }
            }
            else if (selected_item_value == '2')
            {
                option = 7-1;
                if (environment_data->alarm_state.is_alarm_enabled == ON)
                {
                    environment_data->alarm_state.is_alarm_enabled = OFF;
                    sprintf(message, "Alarm disabled");
                }
                else
                {
                    sprintf(message, "Alarm enabled");
                    environment_data->alarm_state.is_alarm_enabled = ON;
                }
            }
            else if (selected_item_value == '3')
            {
                option = 5-1; // 5 or 6, both are air conditioning
                if (environment_data->system_data.devices[option].state == ON)
                {
                    sprintf(message, "Air conditioning 1 e 2 OFF");
                }
                else
                {
                    reference_temperature = read_float(float_input_window, "Type the reference temperature >> ",
                                                       MIN_REFERENCE_TEMPERATUER, MAX_REFERENCE_TEMPERATURE);
                    hysteresis = read_float(float_input_window, "Type the hysteresis value >> ",
                                            MIN_SYSTEM_HYSTERESIS, MAX_SYSTEM_HYSTERESIS);
                    sprintf(message, "Air conditioning 1 e 2 ON with TR %.2f °C and hysteresis %.2f °C",
                            reference_temperature, hysteresis);
                }

                display_text(float_input_window, 3, 1, message);
                store_system_logs(message);
                send_temperature_data(option, reference_temperature, hysteresis);

                continue;
            }


            if (option >= 0 && option <= 6)
            {
                store_system_logs(message);
                display_text(float_input_window, 3, 1, message);
            }

            if (option >= 0 && option <= 3)
            {
                send_int_data(option);
            }

            move(13, 1);
            pos_menu_cursor(selection_menu);
            break;
        }
        wrefresh(main_menu_window);
        refresh();
    }
}

char *get_on_off_string(int status)
{
    if (status == 0)
    {
        return "OFF";
    }
    else
    {
        return "ON";
    }
}

char *get_yes_no_string(int status)
{
    if (status == 0)
    {
        return "NO";
    }
    else
    {
        return "YES";
    }
}

char *get_open_closed_string(int status)
{
    if (status == 0)
    {
        return "CLOSED";
    }
    else
    {
        return "OPEN";
    }
}

/*!
 * @brief Function used to display the system variables values.
 *
 * @param[out] display    :   Display system variables values into screen.
 *
 */
void update_system_status_window()
{
    char temperature[50];
    char humidity[50];
    char reference_temperature[50];
    char hysteresis[50];
    char lamps[50];
    char air[50];
    char alarm_avaliable[50];
    char alarm_status[50];
    char presence[50];
    char bedroom[50];
    char kitchen[50];
    char living_room[50];

    while (1)
    {
        sprintf(temperature, "Temperature: %.2f ºC.",
                environment_data->system_data.bme280_data.temperature);
        sprintf(humidity, "Humidity: %.2f percent.",
                environment_data->system_data.bme280_data.humidity);
        sprintf(reference_temperature, "Reference temperature: %.2f ºC.",
                environment_data->system_data.air_temperature.reference_temperature);
        sprintf(hysteresis, "Hysteresis: %.2f ºC.",
                environment_data->system_data.air_temperature.hysteresis);

        sprintf(lamps, "Lamps: L1 %s, L2 %s, L3 %s, L4 %s.",
                get_on_off_string(environment_data->system_data.devices[0].state),
                get_on_off_string(environment_data->system_data.devices[1].state),
                get_on_off_string(environment_data->system_data.devices[2].state),
                get_on_off_string(environment_data->system_data.devices[3].state));
        sprintf(air, "Ar conditioning (%s): A1 %s, A2 %s.",
                get_on_off_string(environment_data->system_data.air_temperature.air_1_enabled ||
                                  environment_data->system_data.air_temperature.air_2_enabled),
                get_on_off_string(environment_data->system_data.devices[4].state),
                get_on_off_string(environment_data->system_data.devices[5].state));

        sprintf(alarm_avaliable, "Alarm avaliable: %s.",
                get_yes_no_string(environment_data->alarm_state.is_alarm_enabled));
        sprintf(alarm_status, "Alarm status: %s",
                get_on_off_string(environment_data->alarm_state.alarm_status));
        sprintf(presence, "Presence: P1 %s, P2 %s.",
                get_on_off_string(environment_data->system_data.sensors[0].state),
                get_on_off_string(environment_data->system_data.sensors[1].state));
        sprintf(bedroom, "Bedroom: window 1 %s, window 2 %s.",
                get_open_closed_string(environment_data->system_data.sensors[6].state),
                get_open_closed_string(environment_data->system_data.sensors[7].state));
        sprintf(kitchen, "Kitchen: door %s, window %s.",
                get_open_closed_string(environment_data->system_data.sensors[2].state),
                get_open_closed_string(environment_data->system_data.sensors[3].state));
        sprintf(living_room, "Living room: door %s, window %s.",
                get_open_closed_string(environment_data->system_data.sensors[4].state),
                get_open_closed_string(environment_data->system_data.sensors[5].state));

        display_text(system_status_window, 3, 2, temperature);
        display_text(system_status_window, 4, 2, reference_temperature);
        display_text(system_status_window, 6, 2, lamps);
        display_text(system_status_window, 8, 2, alarm_avaliable);
        display_text(system_status_window, 9, 2, presence);
        display_text(system_status_window, 10, 2, kitchen);

        display_text(system_status_window, 3, 40, humidity);
        display_text(system_status_window, 4, 40, hysteresis);
        display_text(system_status_window, 6, 40, air);
        display_text(system_status_window, 8, 40, alarm_status);
        display_text(system_status_window, 9, 40, bedroom);
        display_text(system_status_window, 10, 40, living_room);

        wrefresh(system_status_window);
        usleep(500000);
    }
}

/*!
 * @brief Closure function to setup_input_menu and setup_iterative_menu. (thread)
 */
void *setup_menu_windows()
{
    while (1)
    {
        setup_input_menu();
        setup_iterative_menu();
    }
}

/*!
 * @brief Closure function to setup_system_status and update_system_status_window. (thread)
 */
void *setup_system_status_interface()
{
    while (1)
    {
        setup_system_status();
        update_system_status_window();
    }
}

/*!
 * @brief Clean all resources allocated by ncurses.
 */
void clear_ncurses_alocation()
{
    /* Unpost and free all the memory taken up */
    unpost_menu(selection_menu);
    for (int i = 0; i < choices_size; ++i)
    {
        free_item(list_items[i]);
    }
    free_menu(selection_menu);

    delwin(main_menu_window);
    delwin(float_input_window);

    endwin();
}

/*!
 * @brief Setup initial configurations to ncurses and set pointer to system_display_value.
 */
void init_system_apresentation(struct all_system_data *env_data)
{
    environment_data = env_data;
    setup_ncurses_initial_configs();
}
