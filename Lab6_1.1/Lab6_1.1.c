    #include "ti_msp_dl_config.h"
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdint.h>

     int main(void){
        SYSCFG_DL_init(); 

        static const DL_UART_Main_ClockConfig gUART_0ClockConfig = {
        .clockSel = DL_UART_MAIN_CLOCK_BUSCLK,
        .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
        };
        static const DL_UART_Main_Config gUART_0Config = {
        .mode = DL_UART_MAIN_MODE_NORMAL,
        .direction = DL_UART_MAIN_DIRECTION_TX_RX,
        .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
        .parity = DL_UART_MAIN_PARITY_NONE,
        .wordLength = DL_UART_MAIN_WORD_LENGTH_8_BITS,
        .stopBits = DL_UART_MAIN_STOP_BITS_ONE
        };
        DL_UART_Main_reset(UART0);
        DL_UART_Main_enablePower(UART0);
        delay_cycles(POWER_STARTUP_DELAY);
        DL_UART_Main_setClockConfig(UART0, (DL_UART_Main_ClockConfig *)
        &gUART_0ClockConfig);
        DL_UART_Main_init(UART0, (DL_UART_Main_Config *) &gUART_0Config);
        DL_UART_Main_setOversampling(UART0, DL_UART_OVERSAMPLING_RATE_16X);
        DL_UART_Main_setBaudRateDivisor(UART0, 17, 23);
        DL_UART_Main_enable(UART0);
        DL_GPIO_initPeripheralOutputFunction(IOMUX_PINCM21, IOMUX_PINCM21_PF_UART0_TX);
        DL_GPIO_initPeripheralInputFunction(IOMUX_PINCM22, IOMUX_PINCM22_PF_UART0_RX);

        while(1){
            if (!DL_UART_Main_isRXFIFOEmpty(UART0)) {
                uint8_t rx = DL_UART_Main_receiveData(UART0);
                while (DL_UART_Main_isTXFIFOFull(UART0)){
                }
                DL_UART_Main_transmitData(UART0, rx);
            }
        }
     }

  