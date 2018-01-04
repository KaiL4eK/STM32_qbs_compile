#include <ch.h>
#include <hal.h>

#include <chprintf.h>

//#define SHELL_ENABLE

static THD_WORKING_AREA(waBlinker, 128);
static THD_FUNCTION(Blinker, arg)
{
    (void)arg;
    chRegSetThreadName("Red Blinker");

    while (true)
    {
        palTogglePad(GPIOC, GPIOC_LED);
        chThdSleepSeconds(1);
    }
}


static THD_WORKING_AREA(waSerialSender, 128);
static THD_FUNCTION(SerialSender, arg)
{
    (void)arg;
    chRegSetThreadName("SerialSender");

    while (true)
    {
        chprintf((BaseSequentialStream *)&SD2, "test from %s\n", chThdGetSelfX()->name);
        chThdSleepSeconds(2);
    }
}

/******************************/
/*            Shell           */
/******************************/
#ifdef SHELL_ENABLE
#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

#include <shell.h>

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[])
{
    size_t n, size;

    (void)argv;
    if (argc > 0) {
        shellUsage(chp, "mem");
        return;
    }

    chprintf(chp, "Response\r\n");

//    n = chHeapStatus(NULL, &size, NULL);
//    chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
//    chprintf(chp, "heap fragments   : %u\r\n", n);
//    chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *)&SD2, commands
};
#endif // SHELL_ENABLE

/******************************/
/*            Main            */
/******************************/

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

    /******************************/
    /*          Serial            */
    /******************************/
    static const SerialConfig sdcfg = {
      .speed = 921600,
      .cr1 = 0,
      .cr2 = USART_CR2_LINEN,
      .cr3 = 0
    };

    sdStart(&SD2, &sdcfg);
    // Required for switching to alternative function
    // Don`t set 3rd pin
    palSetPadMode( GPIOA, 2, PAL_MODE_STM32_ALTERNATE_PUSHPULL );

    /******************************/
    /*            PWM             */
    /******************************/
    static PWMConfig pwmcfg = {
       10000,
       1000,
       NULL,
       {
          {PWM_OUTPUT_DISABLED, NULL},
          {PWM_OUTPUT_DISABLED, NULL},
          {PWM_OUTPUT_ACTIVE_HIGH, NULL},
          {PWM_OUTPUT_ACTIVE_HIGH, NULL}
       }, 0, 0
    };

    pwmStart(&PWMD3, &pwmcfg);
    pwmEnableChannel(&PWMD3, 2, 500);
    palSetPadMode( GPIOB, 0, PAL_MODE_STM32_ALTERNATE_PUSHPULL );   // PWM3/3
    pwmEnableChannel(&PWMD3, 3, 500);
    palSetPadMode( GPIOB, 1, PAL_MODE_STM32_ALTERNATE_PUSHPULL );   // PWM3/4

    /******************************/
    /*          Threads           */
    /******************************/

    chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, Blinker, NULL);
    chThdCreateStatic(waSerialSender, sizeof(waSerialSender), NORMALPRIO, SerialSender, NULL);

#ifdef SHELL_ENABLE
    shellInit();
#endif // SHELL_ENABLE

    while (true)
    {
#ifdef SHELL_ENABLE
        thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1, shellThread, (void*)&shell_cfg1);
        chThdWait(shelltp);
#endif // SHELL_ENABLE
        chThdSleepSeconds(1);
    }

    return 0;
}
