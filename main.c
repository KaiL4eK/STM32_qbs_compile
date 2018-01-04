#include <ch.h>
#include <hal.h>

#include <chprintf.h>

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

static const SerialConfig sdcfg = {
  38400,
  0,
  USART_CR2_LINEN,
  0
};

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

    palSetPadMode( GPIOA, 2, PAL_MODE_STM32_ALTERNATE_PUSHPULL );
    palSetPadMode( GPIOA, 3, PAL_MODE_STM32_ALTERNATE_PUSHPULL );

    chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, Blinker, NULL);
    sdStart(&SD2, &sdcfg);

    /*
    * Normal main() thread activity, in this demo it does nothing.
    */
    while (true) {
        chprintf(&SD2, "test....\r\n");
        chThdSleepMilliseconds(500);
    }

    return 0;
}
