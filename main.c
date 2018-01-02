#include <ch.h>
#include <hal.h>

static THD_WORKING_AREA(waBlinker, 128);

static void Blinker(void *arg)
{
    (void)arg;
    chRegSetThreadName("Green Blinker");

    while (true)
    {
        palSetPad(GPIOC, GPIOC_LED);
        chThdSleepSeconds(1);
        palClearPad(GPIOC, GPIOC_LED);
        chThdSleepSeconds(1);
    }
}

/*
 * Application entry point.
 */
int main(void)
{
    /*
    * System initializations.
    * - HAL initialization, this also initializes the configured device drivers
    *   and performs the board-specific initializations.
    * - Kernel initialization, the main() function becomes a thread and the
    *   RTOS is active.
    */
    halInit();
    chSysInit();

    chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, Blinker, NULL);

    /*
    * Normal main() thread activity, in this demo it does nothing.
    */
    while (true) {

    chThdSleepMilliseconds(500);
    }
    return 0;
}
