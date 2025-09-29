#include "ti_msp_dl_config.h"
#include <stdio.h>

int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_enablePower(GPIOB);
    
    // Blue LED segment in LED2
    DL_GPIO_initDigitalInput(IOMUX_PINCM50);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM50);

    DL_GPIO_initPeripheralInputFunctionFeatures(IOMUX_PINCM49,
    IOMUX_PINCM49_PF_GPIOB_DIO21, DL_GPIO_INVERSION_DISABLE,
    DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

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
    
    const uint32_t sysClock = 32000000;
    bool pressed = false;
    uint32_t timeStart = 0;
    uint32_t timeStop = 0;
    uint32_t duration = 0;  
 
 
    while (1) {
        uint32_t status_pb = DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_21);
        
        if(!status_pb && !pressed){
            timeStart = DL_Timer_getTimerCount(TIMG12);
            pressed = true;
            /*printf("timer count Start: %d\n", timeStart);*/
        }
        else if (status_pb && pressed){
             timeStop = DL_Timer_getTimerCount(TIMG12);
            /*printf("timer count Stop: %d\n", timeStop);*/
            duration = (((uint64_t)(timeStop - timeStart)) * 1000) / sysClock;//convert the time to mS
            
            // print the duration 
            printf("Duration: %dmS\n", duration);
            pressed = false;
        }
      
    }
}
