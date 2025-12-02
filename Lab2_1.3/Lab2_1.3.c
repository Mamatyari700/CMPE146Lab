#include "ti_msp_dl_config.h"
#include <stdio.h>

int main(void)
{
    SYSCFG_DL_init();

    DL_GPIO_enablePower(GPIOB);
    DL_GPIO_initDigitalInput(IOMUX_PINCM50);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM50);
    DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_22);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_26);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_27);
    bool LED2State = 0;

    uint32_t GPIO1 = 0x400A2000u;      // GPIOB base address
    uint32_t DOUT23_20Ofset = 0x1214u + 2u;  //adress of DOUT23_20(0x1214) + 2 byte(pin 22(bit 22));
    volatile uint8_t *const Pin22 = (uint8_t *)(GPIO1 + DOUT23_20Ofset);
    
    while (1) {
    printf("Adress: 0x%08X\n", (unsigned int)Pin22);
    delay_cycles(10000000);
    *Pin22 = 1;
    LED2State = 1;

    printf("LED2 state: %d\n", LED2State);


    delay_cycles(10000000);
    *Pin22 = 0;
    LED2State = 0;

    printf("LED2 state: %d\n", LED2State);

    }
}
