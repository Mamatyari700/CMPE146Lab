#include "ti_msp_dl_config.h"
#include <stdio.h>
#include <stdbool.h>

#define SYSCLK_HZ                      32000000UL   // Timer clock frequency
#define CALIB_SAMPLES                  10            // Number of calibration samples (no-touch)
#define MEAS_CYCLES_PER_READING        20           // Cycles per frequency measurement
#define CONTROL_MEAS_CYCLES            10           // Cycles per control loop (faster response)
#define HYST_TOUCH_RATIO               0.75f        // Touch threshold ratio (75% of base)
#define HYST_RELEASE_RATIO             0.85f        // Release threshold ratio (85% of base)

uint32_t measure_oscillator_frequency(void);
void oscillator_charge(void);
void oscillator_discharge(void);
bool read_pin_state(void);
void delay_ms(uint32_t ms);

int main(void)
{
    SYSCFG_DL_init();           

    // Initialize LED1
    DL_GPIO_initDigitalOutput(IOMUX_PINCM1);
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_0);
    DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);//LED1 is active low

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

    //Calibration(get the average of non touched frequency)
    float sum = 0.f;
    for (int i = 0; i < CALIB_SAMPLES; i++) {
        sum += (float)measure_oscillator_frequency();
    }

    float f_base    = sum / (float)CALIB_SAMPLES;
    float f_touch   = f_base * HYST_TOUCH_RATIO;
    float f_release = f_base * HYST_RELEASE_RATIO;

    printf("Calibration done:\nBase = %.1f Hz, Touch = %.1f Hz, Release = %.1f Hz\n",
           f_base, f_touch, f_release);

    //State machine for LED toggle
    bool pressed = false;
    bool led_on  = false;

    while (1) {
        float f = (float)measure_oscillator_frequency(); // Measure current frequency

        // Detect touch (frequency drop below threshold)
        if (!pressed && f < f_touch) {
            pressed = true;
            led_on = !led_on;
            if (led_on)
                DL_GPIO_clearPins(GPIOA, DL_GPIO_PIN_0);
            else
                DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);
        }
        // Detect release (frequency rises back)
        else if (pressed && f > f_release) {
            pressed = false;
        }

        delay_ms(10);
    }
}

uint32_t measure_oscillator_frequency(void)
{
    uint32_t start_ticks, end_ticks, elapsed_ticks;
    uint32_t cycles = (uint32_t)MEAS_CYCLES_PER_READING;

    oscillator_discharge();
    while (read_pin_state()) { /* wait for LOW */ }

    oscillator_charge();    while (!read_pin_state()) { /* wait for HIGH */ }
    oscillator_discharge(); while ( read_pin_state())  { /* wait for LOW  */ }

    start_ticks = DL_Timer_getTimerCount(TIMG12);

    for (uint32_t i = 0; i < cycles; i++) {
        oscillator_charge();    while (!read_pin_state()) { }
        oscillator_discharge(); while ( read_pin_state())  { }
    }

    end_ticks = DL_Timer_getTimerCount(TIMG12);

    // Handle overflow of 32-bit counter
    if (end_ticks >= start_ticks) {
        elapsed_ticks = end_ticks - start_ticks;
    } else {
        elapsed_ticks = (0xFFFFFFFFu - start_ticks + 1u) + end_ticks;
    }

    if (elapsed_ticks == 0) return 0u;

    // Frequency = cycles / (elapsed_ticks / SYSCLK_HZ)
    uint64_t numerator = (uint64_t)cycles * (uint64_t)SYSCLK_HZ;
    uint32_t freq_hz   = (uint32_t)(numerator / (uint64_t)elapsed_ticks);
    return freq_hz;
}

void oscillator_charge(void)
{
    DL_GPIO_initDigitalInputFeatures(
        IOMUX_PINCM26,                 // PB9
        DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_PULL_UP,      // charge (pull-up)
        DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE
    );
}

void oscillator_discharge(void)
{
    DL_GPIO_initDigitalInputFeatures(
        IOMUX_PINCM26,                 // PB9
        DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_PULL_DOWN,    // discharge (pull-down)
        DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE
    );
}

bool read_pin_state(void)
{
    return (DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_9) & DL_GPIO_PIN_9) != 0;
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        for (volatile uint32_t j = 0; j < (SYSCLK_HZ/4000UL); j++) {
            __asm(" NOP");
        }
    }
}
