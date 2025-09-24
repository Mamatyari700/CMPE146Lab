#include "ti_msp_dl_config.h"
#include <stdio.h>

int main(void)
{
    SYSCFG_DL_init();
    
    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    
    // Blue LED segment in LED2
    DL_GPIO_initDigitalInput(IOMUX_PINCM50);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM50);
    DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_22);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_22);
    // Keep other LED segments (green & red) in LED2 off
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_26);
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_27);
    // LED 1
    DL_GPIO_initDigitalInput(IOMUX_PINCM1);
    DL_GPIO_initDigitalOutput(IOMUX_PINCM1);
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_0);
    DL_GPIO_clearPins(GPIOA, DL_GPIO_PIN_0);

    uint32_t iOMux = 0x40428000;
    uint32_t PINCM1Ofset = 0x4u;
    volatile uint32_t *PINCM1 = (uint32_t *)(iOMux + PINCM1Ofset);
    *PINCM1 |= (1 << 26);  //bit 26 is the INV bit Inversing bit
    // DL_GPIO_INVERSION_ENABLE(GPIOA, DL_GPIO_PIN_0);

    
    // Alias address for LED2 Blue
    uint32_t GPIOB = 0x400A2000;      // GPIOB base address
    uint32_t DOUT23_20Ofset = 0x1214 + 2;  //adress of DOUT23_20(0x1214) + 2 byte(pin 22(bit 22));
    volatile uint8_t *const Pin22 = (uint8_t *)(GPIOB + DOUT23_20Ofset);
    
    // Alias address for LED1
    uint32_t GPIOA = 0x400A0000;
    uint32_t DOUT3_0Ofset = 0x1200 + 0;  //adress of DOUT3_0(0x1200) + 0 byte(pin 0(bit 0));
    volatile uint8_t *const Pin0 = (uint8_t *)(GPIOA + DOUT3_0Ofset);
    
    // bool LEDState = 0;
    
    while (1) {
        // Output alias addresses
        printf("Pin22(LED2, Blue) Adress: 0x%08X\n", (unsigned int)Pin22);
        printf("Pin0(LED1, Red) Adress: 0x%08X\n", (unsigned int)Pin0);
        // Turn both LEDs ON
        *Pin22 = 1;
        *Pin0 = 1;
        // LEDState = 1;
        // printf("LEDs state: %d (Both LEDs ON)\n", LEDState);
        delay_cycles(10000000);
        
        // Turn both LEDs OFF
        *Pin22 = 0;
        *Pin0 = 0;
        // LEDState = 0;
        // printf("LEDs state: %d (Both LEDs OFF)\n", LEDState);
        delay_cycles(10000000);
    }
}
