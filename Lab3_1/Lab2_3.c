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
    DL_GPIO_initPeripheralInputFunctionFeatures(IOMUX_PINCM49,
    IOMUX_PINCM49_PF_GPIOB_DIO21, DL_GPIO_INVERSION_DISABLE,
    DL_GPIO_RESISTOR_PULL_UP, DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);


    
    // Alias address for LED2 Blue
    uint32_t GPIOB = 0x400A2000;      // GPIOB base address
    uint32_t DOUT23_20Ofset = 0x1214 + 2;  //adress of DOUT23_20(0x1214) + 2 byte(pin 22(bit 22));
    volatile uint8_t *const Pin22 = (uint8_t *)(GPIOB + DOUT23_20Ofset);
    
    // Alias address for S2
    uint32_t DIN23_20Ofset = 0x1314 + 1;  //adress of DIN23_20(0x1314) + 1 byte(pin 21(bit 21));
    volatile uint8_t *const Pin21 = (uint8_t *)(GPIOB + DIN23_20Ofset);
    
    // bool LEDState = 0;
    uint8_t previousState = 0;
    uint8_t LEDState = 0;
    *Pin22 = LEDState;
    while (1) {
        // Output alias addresses
        printf("Pin22 Adress: 0x%08X\n", (unsigned int)Pin22);
        printf("Pin21 Adress: 0x%08X\n", (unsigned int)Pin21);
        // Toggle LED with S2 switch
        uint8_t buttonState = *Pin21;
        if(previousState == 1 && buttonState == 0){
            LEDState ^= 1;
            *Pin22 = LEDState;
        }   
        previousState = buttonState;
    }
}
