#pragma once

#include "sam.h"



namespace nd::peripherals::systick
{
    inline void delay_us(const unsigned int delay)
    {
        SysTick->VAL = 0;
        SysTick->LOAD = (SystemCoreClock/(10000000/delay/10)-1) & 0x00FFFFFF;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) ;;

        SysTick->CTRL = 0;
    }

    inline void delay_ms(const unsigned int delay)
    {
        //Maximum is 93.2ms at 180MHz
        //if (delay > 10000) delay = 10000;

        SysTick->VAL = 0;
        SysTick->LOAD = (SystemCoreClock/(10000/delay/10)-1) & 0x00FFFFFF;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) ;;

        SysTick->CTRL = 0;
    }

    inline void delay_ms(const unsigned int delay, const unsigned int repeat)
    {
        for (unsigned int i=0; i<repeat; i++)
        {
            delay_ms(delay);
        }
    }


    inline void delay_cycles(const unsigned int cycles)
    {
        SysTick->VAL = 0;
        SysTick->LOAD = cycles & 0x00FFFFFF;
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) ;;

        SysTick->CTRL = 0;
    }
}
