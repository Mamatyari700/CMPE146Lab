    #include "ti_msp_dl_config.h"
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdint.h>

    #define SYSCLK_HZ                      32000000UL   // Timer clock frequency
    #define CALIB_SAMPLES                  10            // Number of calibration samples (no-touch)
    #define MEAS_CYCLES_PER_READING        20           // Cycles per frequency measurement
    #define CONTROL_MEAS_CYCLES            10           // Cycles per control loop (faster response)

        typedef struct {
        const char *name;     
        uint32_t    pincm;   
        uint32_t    pinNum; 
    } PinDef;

    static const PinDef kPins[] = {
        { "PB9",  IOMUX_PINCM26, DL_GPIO_PIN_9  },
        { "PB5",  IOMUX_PINCM18, DL_GPIO_PIN_5  },
        { "PB11", IOMUX_PINCM28, DL_GPIO_PIN_11 },
        { "PB21", IOMUX_PINCM49, DL_GPIO_PIN_21 },
    };

    uint32_t get_avg_of_10(uint32_t pincm, uint32_t pinNum);
    uint32_t measure_oscillator_frequency(uint32_t pincm, uint32_t pinNum);
    void oscillator_charge(uint32_t pincm);
    void oscillator_discharge(uint32_t pincm);
    bool read_pin_state(uint32_t pinNum);

    int main(void)
    {
        SYSCFG_DL_init();        

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


        for (unsigned i = 0; i < (sizeof(kPins)/sizeof(kPins[0])); ++i) {
            uint32_t f = get_avg_of_10(kPins[i].pincm, kPins[i].pinNum);
            printf("%s: %lu Hz\n", kPins[i].name, (unsigned long)f);
        }

        while (1) {
        }
    }
    uint32_t get_avg_of_10(uint32_t pincm, uint32_t pinNum){
        uint64_t sum = 0;
        for (int i = 0; i < CALIB_SAMPLES; i++) {
            sum += (uint32_t)measure_oscillator_frequency(pincm, pinNum);
        }

        uint32_t avg = sum / (uint64_t)CALIB_SAMPLES;

        return avg;
    }

    uint32_t measure_oscillator_frequency(uint32_t pincm, uint32_t pinNum)
    {
        uint32_t start_ticks, end_ticks, elapsed_ticks;
        const uint32_t cycles = (uint32_t)MEAS_CYCLES_PER_READING;

        // Ensure LOW
        oscillator_discharge(pincm);
        while (read_pin_state(pinNum)) { /* wait LOW */ }

        // Phase align: HIGH -> LOW
        oscillator_charge(pincm);    while (!read_pin_state(pinNum)) { }
        oscillator_discharge(pincm); while ( read_pin_state(pinNum))  { }

        start_ticks = DL_Timer_getTimerCount(TIMG12);
        for (uint32_t i = 0; i < cycles; ++i) {
            oscillator_charge(pincm);    while (!read_pin_state(pinNum)) { }
            oscillator_discharge(pincm); while ( read_pin_state(pinNum))  { }
        }
        end_ticks = DL_Timer_getTimerCount(TIMG12);

        if (end_ticks >= start_ticks) elapsed_ticks = end_ticks - start_ticks;
        else                          elapsed_ticks = (0xFFFFFFFFu - start_ticks + 1u) + end_ticks;

        if (!elapsed_ticks) return 0u;

        uint64_t num = (uint64_t)cycles * (uint64_t)SYSCLK_HZ;
        return (uint32_t)(num / (uint64_t)elapsed_ticks);
    }

    void oscillator_charge(uint32_t pincm) {
        DL_GPIO_initDigitalInputFeatures(
            pincm,
            DL_GPIO_INVERSION_DISABLE,
            DL_GPIO_RESISTOR_PULL_UP,
            DL_GPIO_HYSTERESIS_DISABLE,
            DL_GPIO_WAKEUP_DISABLE
        );
    }

    void oscillator_discharge(uint32_t pincm) {
        DL_GPIO_initDigitalInputFeatures(
            pincm,
            DL_GPIO_INVERSION_DISABLE,
            DL_GPIO_RESISTOR_PULL_DOWN,
            DL_GPIO_HYSTERESIS_DISABLE,
            DL_GPIO_WAKEUP_DISABLE
        );
    }

    bool read_pin_state(uint32_t pinNum) {
        return (DL_GPIO_readPins(GPIOB, pinNum) & pinNum) != 0;
    }
