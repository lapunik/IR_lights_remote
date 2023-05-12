#pragma once

#include <cstdint>

#include "sam.h"

#include "gpio.hpp"


namespace rg
{
    class Timer
    {
    public:
        Timer(Tcc& timer_p, nd::peripherals::GPIO_pin pin) : timer{timer_p}
        {
            nd::peripherals::Alternate_pin pwm_pin{pin};
            pwm_pin.set_alternate_function(nd::peripherals::Gpio_alternate::E);
            pwm_pin.set_driver_strength(true);
        }

        void init() const
        {

            // Disable before setting
            timer.CTRLA.bit.ENABLE = 0;
            while (timer.SYNCBUSY.bit.ENABLE);

            //timer.DRVCTRL.reg |= TCC_DRVCTRL_INVEN1;
            timer.DRVCTRL.reg &= ~TCC_DRVCTRL_INVEN1;

            // Normal PWM
            // TC_CTRLA_WAVEGEN_NPWM (wrong shift)
            timer.WAVE.reg = TC_CTRLA_WAVEGEN_NPWM_Val;
            while (timer.SYNCBUSY.bit.WAVE);

            // PWM (zdroj pro TCC1 (GCLK) je nastaven na 48MHz)
            timer.PER.reg = 48000000/(38*1000) - 1; // 38kHz chci na výstupu
            while (timer.SYNCBUSY.bit.PER);

            timer.CCB[1].reg = timer.PER.reg/2; // 50% dutycyce
            //timer.CCB[1].reg = timer.PER.reg; // 100% dutycyce
            while (timer.SYNCBUSY.bit.CCB1);

            timer.CTRLA.bit.ENABLE = 1;
            while (timer.SYNCBUSY.bit.ENABLE);


        }

        void enable() const
        {
            //timer.CCB[0].reg = timer.CCB[1].reg = timer.PER.reg/2; // 50% dutycyce
            timer.CTRLA.bit.ENABLE = 1;
            while (timer.SYNCBUSY.bit.ENABLE);
        }

        void disable() const
        {
            //timer.CCB[1].reg = timer.PER.reg; // 100% dutycyce
            timer.CTRLA.bit.ENABLE = 0;
            while (timer.SYNCBUSY.bit.ENABLE);
        }


        void set_duty(std::uint8_t duty) const
        {
            timer.CCB[0].reg = timer.PER.reg*duty; // 50% dutycyce
            while (timer.SYNCBUSY.bit.CCB0);
        }

    private:
        Tcc& timer;
    };
}
