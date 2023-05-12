#pragma once

#include "sam.h"



namespace nd::peripherals::watchdog
{
    inline void enable()
    {
        // 2s timeout
        WDT->CONFIG.reg = WDT_CONFIG_PER_8K;

        WDT->CTRL.reg = WDT_CTRL_ENABLE;
        while (WDT->STATUS.bit.SYNCBUSY);
    }

    inline void disable()
    {
        WDT->CTRL.reg = 0;
        while (WDT->STATUS.bit.SYNCBUSY);
    }

    inline void feed()
    {
        WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
        while (WDT->STATUS.bit.SYNCBUSY);
    }
}
