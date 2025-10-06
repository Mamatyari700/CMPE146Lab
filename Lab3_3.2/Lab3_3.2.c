#include "ti_msp_dl_config.h"
#include <stdio.h>

    #define QUEUE_SIZE 10

    const uint32_t sysClock = 32000000;
    bool pressed = false;
    uint32_t timeStart = 0;
    uint32_t timeStop = 0;
    uint32_t duration = 0; 
    
    struct record {
        uint32_t timestamp;
        uint8_t state;
    };
    struct record queue[QUEUE_SIZE];
    volatile int read_index = 0;
    volatile int write_index = 0;

    void GROUP1_IRQHandler(void){
        // Get the pending interrupt
        uint32_t interrupt_status = DL_GPIO_getEnabledInterruptStatus(GPIOB, DL_GPIO_PIN_21);
        if ((interrupt_status & DL_GPIO_PIN_21)){ // From S2
            queue[write_index].timestamp = DL_Timer_getTimerCount(TIMG12);
            queue[write_index].state = (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_21)!= 0);
            write_index = (write_index + 1) % QUEUE_SIZE;
            DL_GPIO_clearInterruptStatus(GPIOB, DL_GPIO_PIN_21);
            //  printf("write index%d\n", write_index);
        }
       
    }

int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_enablePower(GPIOB);
    
    // Blue LED segment in LED2
    DL_GPIO_initDigitalInput(IOMUX_PINCM50);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM50);
    DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_22);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);
    DL_GPIO_initPeripheralInputFunctionFeatures(IOMUX_PINCM49,
    IOMUX_PINCM49_PF_GPIOB_DIO21, DL_GPIO_INVERSION_DISABLE,
    DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_setUpperPinsPolarity(GPIOB, DL_GPIO_PIN_21_EDGE_RISE_FALL);
    DL_GPIO_clearInterruptStatus(GPIOB, DL_GPIO_PIN_21);
    DL_GPIO_enableInterrupt(GPIOB, DL_GPIO_PIN_21);
    NVIC_EnableIRQ(GPIOB_INT_IRQn);

   

    // Set up to use the 32-bit counter TIMG12
    DL_Timer_enablePower(TIMG12);
    DL_Timer_ClockConfig config;
    config.clockSel = DL_TIMER_CLOCK_BUSCLK;
    config.divideRatio = DL_TIMER_CLOCK_DIVIDE_1;
    config.prescale = 0;
    DL_Timer_setClockConfig(TIMG12, &config);
    DL_Timer_TimerConfig timerConfig;
    timerConfig.timerMode = DL_TIMER_TIMER_MODE_PERIODIC_UP;
    timerConfig.period = -1;
    timerConfig.startTimer = DL_TIMER_START;
    timerConfig.genIntermInt = DL_TIMER_INTERM_INT_DISABLED;
    timerConfig.counterVal = 0;
    DL_Timer_initTimerMode(TIMG12, &timerConfig);
 
    while (1) {
        delay_cycles(sysClock / 100); // 10ms delay
        if (read_index != write_index) {
            struct record current = queue[read_index];
            
            if (!current.state && !pressed) { 
                timeStart = current.timestamp;
                DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_22);
                // printf("timer count Start: %d\n", timeStart);
                pressed = true;
            }else if (current.state && pressed) {
                timeStop = current.timestamp;
                DL_GPIO_togglePins(GPIOB, DL_GPIO_PIN_22);
                uint32_t duration = ((uint64_t)(timeStop - timeStart) * 1000) / sysClock;
                printf("Duration: %d mS (read_index=%d)\n", duration, read_index);
                pressed = false;
            }
            read_index = (read_index + 1) % QUEUE_SIZE;
            // printf("read_index: %d\n", read_index);
        }
    }
}