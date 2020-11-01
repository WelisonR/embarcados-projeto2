#include "gpio_api.h"

/*!
 * @brief Function used to setup the initial state of the actuators.
 */
int setup_devices()
{
    if (!bcm2835_init())
    {
        fprintf(stderr, "Cannot setup the actuators devices.");
        raise(SIGABRT);
    }

    const struct sched_param priority = {1};
    /* Set calling process with high priority in first-in, first-out policy */
    sched_setscheduler(0, SCHED_FIFO, &priority);
    /* mlockall() lock part or all of the calling process's virtual address space into RAM */
    mlockall(MCL_CURRENT | MCL_FUTURE);

    /* Initialize input/output pins */
    setup_pins();

    return BCM2835_SUCESS;
}

/*!
 * @brief Function used to setup input/output GPIOs pins.
 */
void setup_pins()
{
    /* Setup lamp as output */
    bcm2835_gpio_fsel(LAMP_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_2, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_3, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(LAMP_4, BCM2835_GPIO_FSEL_OUTP);

    /* Setup air-conditioning as output */
    bcm2835_gpio_fsel(AIR_CONDITIONING_1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AIR_CONDITIONING_2, BCM2835_GPIO_FSEL_OUTP);

    /* Setup presence sensor as input */
    bcm2835_gpio_fsel(PRESENCE_SENSOR_1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(PRESENCE_SENSOR_2, BCM2835_GPIO_FSEL_INPT);

    /* Setup touch sensor as input */
    bcm2835_gpio_fsel(TOUCH_SENSOR_1, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOUCH_SENSOR_2, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOUCH_SENSOR_3, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOUCH_SENSOR_4, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOUCH_SENSOR_5, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(TOUCH_SENSOR_6, BCM2835_GPIO_FSEL_INPT);
}

/*!
 * @brief Function used to update the gpio states with bcm2835_gpio_lev function.
 */
void update_gpio_state(gpio_state *sensors, int sensors_length)
{
    for (int i = 0; i < sensors_length; i++)
    {
        sensors[i].state = bcm2835_gpio_lev(sensors[i].gpio);
        printf("GPIO: %d, state: %d\n", sensors[i].gpio, sensors[i].state);
    }
    printf("\n\n");
}

/*!
 * @brief Function used to handle program interruption, disable devices and exit.
 */
void handle_actuators_interruption()
{
    bcm2835_close();
}
