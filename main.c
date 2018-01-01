#include <ch.h>
#include <hal.h>

static virtual_timer_t vt1, vt2;

static void restart(void *p) {

  (void)p;

  chSysLockFromISR();
  uartStartSendI(&UARTD1, 14, "Hello World!\r\n");
  chSysUnlockFromISR();
}

//#define GPIOC_LED 13

static void ledoff(void *p) {

  (void)p;

  palSetPad(GPIOC, GPIOC_LED);
}

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;

  palClearPad(GPIOC, GPIOC_LED);
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;

  palSetPad(GPIOC, GPIOC_LED);
  chSysLockFromISR();
  chVTResetI(&vt1);
  chVTDoSetI(&vt1, MS2ST(5000), restart, NULL);
  chSysUnlockFromISR();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  (void)c;

  /* Flashing the LED each time a character is received.*/
  palClearPad(GPIOC, GPIOC_LED);
  chSysLockFromISR();
  chVTResetI(&vt2);
  chVTDoSetI(&vt2, MS2ST(200), ledoff, NULL);
  chSysUnlockFromISR();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  115200,
  0,
  USART_CR2_LINEN,
  0
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */

 palSetPadMode(GPIOA, 9, PAL_MODE_STM32_ALTERNATE_PUSHPULL);      // USART1-TX
 palSetPadMode(GPIOA,10, PAL_MODE_INPUT);                         // USART1-RX


  uartStart(&UARTD1, &uart_cfg_1);

  /*
   * Starts the transmission, it will be handled entirely in background.
   */
  uartStartSend(&UARTD1, 13, "Starting...\r\n");

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
