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

//struct for the tasks
typedef struct {
    uint32_t iomux;
    GPIO_Regs *port; 
    uint32_t pin;
    uint32_t delayUs;
    char *name;
} LedTaskParams;

static LedTaskParams redParams = {
    .iomux = IOMUX_PINCM1,
    .port = GPIOA,
    .pin = DL_GPIO_PIN_0,
    .delayUs = 250000,
    .name = "Red LED"
};

static LedTaskParams greenParams = {
    .iomux = IOMUX_PINCM58,
    .port = GPIOB,
    .pin = DL_GPIO_PIN_27,
    .delayUs = 1000000,
    .name = "Green LED"
};

void *ledTask(void *arg0)
{
    LedTaskParams *params = (LedTaskParams *)arg0;

printf("Task %s start: iomux=%lu, pin=%lu, delay=%lu us\n",
       params->name,
       (unsigned long)params->iomux,
       (unsigned long)params->pin,
       (unsigned long)params->delayUs);

    // GPIO setup
    DL_GPIO_enablePower(params->port);
    DL_GPIO_initDigitalOutput(params->iomux);
    DL_GPIO_enableOutput(params->port, params->pin);
    if(params->port == GPIOA && params->pin == DL_GPIO_PIN_0){
        DL_GPIO_setPins(params->port, params->pin); //LED1 is active low
    }
    else{
        DL_GPIO_clearPins(params->port, params->pin); 
    }
   
    while(1) {
        DL_GPIO_togglePins(params->port, params->pin);
        if(params->delayUs < 1000000){
            usleep(params->delayUs);
        }
        else{
            uint32_t delayS = params->delayUs / 1000000;
            sleep(delayS);
        }
        
    }

    return NULL;
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

    retc = pthread_create(&LED1Thread, &attrs, ledTask, &redParams);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {
        }
    }

    retc = pthread_create(&LED2Thread, &attrs, ledTask, &greenParams);
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
