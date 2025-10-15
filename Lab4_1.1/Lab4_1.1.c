#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <stdlib.h>


#define CRC32_SEED 0xFFFFFFFF
#define DMA_CH0_CHAN_ID (0)
#define DATA_SIZE 10240


uint32_t compute_simple_checksum(uint8_t* data, uint32_t length);
static uint32_t calculateCRC32(uint8_t* data, uint32_t length);
static uint8_t myData[10240];

volatile bool dma_done;

void DMA_IRQHandler(void)
{ switch (DL_DMA_getPendingInterrupt(DMA)) {
case DL_DMA_EVENT_IIDX_DMACH0:
dma_done = true;
break;
default:
break;
}
}

uint32_t crc_hardware(uint8_t *data, uint32_t length)
{
    DL_CRC_setSeed32(CRC, CRC32_SEED);
    for (uint32_t i = 0; i < length; i++) {
        DL_CRC_feedData8(CRC, data[i]);
    }
    return DL_CRC_getResult32(CRC);
}

uint32_t crc_dma(uint8_t *data, uint32_t length)
{
    dma_done = false;
    DL_CRC_setSeed32(CRC, CRC32_SEED);

    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)data);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, DL_CRC_getCRCINAddr(CRC));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, length);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
    DL_DMA_startTransfer(DMA, DMA_CH0_CHAN_ID);

    while (!dma_done)
        ;

    return DL_CRC_getResult32(CRC);
}


int main(void)
{
   SYSCFG_DL_init();


   // Utilizing CRC Hardware accelerator + CRC Setup
   DL_CRC_reset(CRC);
   DL_CRC_enablePower(CRC);
   delay_cycles(POWER_STARTUP_DELAY);
   DL_CRC_init(
       CRC, DL_CRC_32_POLYNOMIAL,
       DL_CRC_BIT_REVERSED,
       DL_CRC_INPUT_ENDIANESS_LITTLE_ENDIAN,
       DL_CRC_OUTPUT_BYTESWAP_DISABLED);
   DL_CRC_setSeed32(CRC, CRC32_SEED);


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

   static const DL_DMA_Config gDMA_CH0Config = {
        .transferMode = DL_DMA_SINGLE_BLOCK_TRANSFER_MODE,
        .extendedMode = DL_DMA_NORMAL_MODE,
        .destIncrement = DL_DMA_ADDR_UNCHANGED,
        .srcIncrement = DL_DMA_ADDR_INCREMENT,
        .destWidth = DL_DMA_WIDTH_BYTE,
        .srcWidth = DL_DMA_WIDTH_BYTE,
        .trigger = DMA_SOFTWARE_TRIG,
        .triggerType = DL_DMA_TRIGGER_TYPE_EXTERNAL,
    };
    DL_DMA_clearInterruptStatus(DMA, DL_DMA_INTERRUPT_CHANNEL0);
    DL_DMA_enableInterrupt(DMA, DL_DMA_INTERRUPT_CHANNEL0);
    DL_DMA_initChannel(DMA, DMA_CH0_CHAN_ID , (DL_DMA_Config *) &gDMA_CH0Config);
    NVIC_EnableIRQ(DMA_INT_IRQn);


   srand(12345); // Set seed for random number generator
   for (int i=0; i<10240; i++) {
       myData[i] = rand();
   }

   DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&myData[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, DL_CRC_getCRCINAddr(CRC));
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, DATA_SIZE);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
    DL_CRC_setSeed32(CRC, CRC32_SEED);
    dma_done = false;
    DL_DMA_startTransfer(DMA, DMA_CH0_CHAN_ID);


   printf("First four bytes - VERIFICATION:\n");
   for (int i=0; i<4; i++) {
       printf("0x");
       printf("%02X\n", myData[i]);
   }


   const uint32_t systemClk = 32000000;        // MCU Clk freq.
   uint32_t SimpTime_duration = 0;             // Simple_checksum func duration
   uint32_t HardTime_duration = 0;             // Hardware checksum duration
   uint32_t SoftTime_duration = 0;             // Software checksum duration
   uint32_t start_time = 0;                    // Start timer mark
   uint32_t stop_time = 0;                     // Stop timer mark

    // Checksum generation using CRC accelerator
    uint32_t start_hw = DL_Timer_getTimerCount(TIMG12);
    uint32_t crc_hw_result = crc_hardware(myData, DATA_SIZE);
    uint32_t end_hw = DL_Timer_getTimerCount(TIMG12);
    uint32_t hw_time = ((uint64_t)(end_hw - start_hw) * 1000000) / 32000000;
    // Checksum generation using CRC accelerator and DMA
    uint32_t start_dma = DL_Timer_getTimerCount(TIMG12);
    uint32_t crc_dma_result = crc_dma(myData, DATA_SIZE);
    uint32_t end_dma = DL_Timer_getTimerCount(TIMG12);
    uint32_t dma_time = ((uint64_t)(end_dma - start_dma) * 1000000) / 32000000;


   //Speedup ratio calculation
   float speedup = (float)hw_time / dma_time;


    // Checksums for the 3 methods (compute_simple_checksum, software, hardware)
    printf("Hardware CRC: 0x%08lX (%lu us)\n", crc_hw_result, hw_time);
    printf("DMA CRC:      0x%08lX (%lu us)\n", crc_dma_result, dma_time);
    printf("Speedup (HW/DMA): %.2fx\n", speedup);

       if (crc_hw_result == crc_dma_result) {
        printf("CRC match confirmed.\n");
    } else {
        printf("CRC mismatch!\n");
    }

   return 0;
}


uint32_t compute_simple_checksum(uint8_t* data, uint32_t length) {
   uint32_t sum = 0x00000000;


   for (uint32_t i=0; i<length; i++) {
       uint32_t shiftVal = (data[i] % 4);
       uint32_t dataPostShift = data[i] << (shiftVal * 8);
       sum = sum + dataPostShift;
   }


   sum = ~sum;
   return sum;
}


static uint32_t calculateCRC32(uint8_t* data, uint32_t length) {
   const uint32_t crc32_poly = 0xEDB88320;
   uint32_t ii, jj, byte, crc, mask;
   crc = CRC32_SEED;
  
   for(ii=0;ii<length;ii++) {
       byte = data[ii];
       crc = crc ^ byte;
      
       for (jj = 0; jj < 8; jj++) {
           mask = -(crc & 1);
           crc = (crc >> 1) ^ (crc32_poly & mask);
       }
   }


return crc;
}
