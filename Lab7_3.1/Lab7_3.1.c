/* Standard includes */
#include <stdio.h>

/* POSIX header files */
#include <pthread.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <unistd.h>

/* TI includes for driver configuration */
#include "ti_msp_dl_config.h"

/* Stack size in bytes */
#define THREADSTACKSIZE 1024

/* Set up the hardware ready to run this demo */
static void prvSetupHardware(void);

void *redLEDThread(void *arg0){
    DL_GPIO_enablePower(GPIOA);

    // Initialize LED1
    DL_GPIO_initDigitalOutput(IOMUX_PINCM1);
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_0);
    DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0); //LED1 is active low

    while(1){
        DL_GPIO_togglePins(GPIOA, DL_GPIO_PIN_0);
        usleep(250000); //wait for 250ms(2Hz=1/2 => 1 cycle is 0.5s => togle in 250ms(on for 250ms, off for 250ms))
    }
}

void *greenLEDThread(void *arg0){
    DL_GPIO_enablePower(GPIOB);

    //  Initialize LED2 to green
    DL_GPIO_initDigitalOutput(IOMUX_PINCM58);
    DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_27);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_27);

    while(1){
        DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_27);
        sleep(1); //wait for 1s(0.5Hz=1/0.5 => 1 cycle is 2s => togle in 1s(on for 1s, off for 1s))
    }


}

int main(void)
{
    pthread_t LED1Thread, LED2Thread;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;

    /* Initialize the system locks */
#ifdef __ICCARM__
    __iar_Initlocks();
#endif

    /* Prepare the hardware to run this demo. */
    prvSetupHardware();

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc                    = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {
        }
    }

    retc = pthread_create(&LED1Thread, &attrs, redLEDThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {
        }
    }

    retc = pthread_create(&LED2Thread, &attrs, greenLEDThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {
        }
    }

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}

static void prvSetupHardware(void)
{
    SYSCFG_DL_init();
}

#if (configCHECK_FOR_STACK_OVERFLOW)

#if defined(__IAR_SYSTEMS_ICC__)
__weak void vApplicationStackOverflowHook(
    TaskHandle_t pxTask, char *pcTaskName)
#elif (defined(__TI_COMPILER_VERSION__))
#pragma WEAK(vApplicationStackOverflowHook)
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
#elif (defined(__GNUC__) || defined(__ti_version__))
void __attribute__((weak))
vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
#endif
{
    /* default to spin upon stack overflow */
    while (1) {
    }
}
#endif
