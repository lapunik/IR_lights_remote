#include "System.hpp"
#include "sam.h"
#include "../peripherals/gpio.hpp"
#include "../peripherals/systick.hpp"

namespace nd::system
{
    void setup_oscilators()
    {
        //
        // OSC32K
        //

        const std::uint32_t osc32k_calibration = (*(std::uint32_t*)FUSES_OSC32K_CAL_ADDR & FUSES_OSC32K_CAL_Msk) >>  FUSES_OSC32K_CAL_Pos;
        SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_CALIB(osc32k_calibration) |
                SYSCTRL_OSC32K_STARTUP(5) |
                SYSCTRL_OSC32K_EN32K |
                SYSCTRL_OSC32K_ENABLE;
        while (!SYSCTRL->PCLKSR.bit.OSC32KRDY);


        // OSC32K as GCLK1 input
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) | GCLK_GENDIV_ID(1);
        GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC32K | GCLK_GENCTRL_ID(1);
        while(GCLK->STATUS.bit.SYNCBUSY);

        //Enable reference clock for DFLL
        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(1) | GCLK_CLKCTRL_ID_DFLL48;
        while(GCLK->STATUS.bit.SYNCBUSY);

        //Disable on-demand mode (errata)
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        SYSCTRL->DFLLCTRL.reg = 0;

        //Get and set coarse and fine values from NVM
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        SYSCTRL->DFLLVAL.reg = SYSCTRL_DFLLVAL_COARSE((*(uint32_t *) FUSES_DFLL48M_COARSE_CAL_ADDR >> FUSES_DFLL48M_COARSE_CAL_Pos)) |
                               SYSCTRL_DFLLVAL_FINE(  (*(uint32_t *) FUSES_DFLL48M_FINE_CAL_ADDR   >> FUSES_DFLL48M_FINE_CAL_Pos));

        //Set coarse step, fine step, multiplier
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP(5)  |
                               SYSCTRL_DFLLMUL_FSTEP(2) |
                               SYSCTRL_DFLLMUL_MUL(48000000/32768);

        //Start closed-loop
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        SYSCTRL->DFLLCTRL.reg = SYSCTRL_DFLLCTRL_QLDIS | SYSCTRL_DFLLCTRL_CCDIS | SYSCTRL_DFLLCTRL_MODE;

        //Enable DFLL 48MHz as clock source
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
        SYSCTRL->DFLLCTRL.bit.ENABLE = 1;
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY || !SYSCTRL->PCLKSR.bit.DFLLLCKC || !SYSCTRL->PCLKSR.bit.DFLLLCKF);
        while (!SYSCTRL->PCLKSR.bit.DFLLRDY);

        //Set flash wait state to 1, which we need to do at 48MHz (NVM characteristics p. 628, table 33-32)
        NVMCTRL->CTRLB.bit.RWS = 1;

        //Set DFLL as GCLK0 source
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) | GCLK_GENDIV_ID(0);
        GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_IDC | GCLK_GENCTRL_ID(0);
        while(GCLK->STATUS.bit.SYNCBUSY);

        SystemCoreClock = 48'000'000;

        //Set DFLL as GCLK2 source / 24 -> 2MHz
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(24) | GCLK_GENDIV_ID(2);
        GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_IDC | GCLK_GENCTRL_ID(2);
        while(GCLK->STATUS.bit.SYNCBUSY);

        // OSCULP32K as GCLK3 input
        GCLK->GENDIV.reg = GCLK_GENDIV_DIV(10) | GCLK_GENDIV_ID(3);
        GCLK->GENCTRL.reg = GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(3);
        while(GCLK->STATUS.bit.SYNCBUSY);
    }

    void setup_peripherals_clocks()
    {
        // Synchronous clocks
        PM->APBCMASK.bit.TCC1_ = 1;
        PM->APBAMASK.bit.EIC_ = 1;
        PM->APBCMASK.bit.SERCOM3_ = 1;
        //PM->APBCMASK.bit.EVSYS_ = 1;

        // Deactivate all unused peripherals
        PM->APBCMASK.reg &= ~(1 << PM_APBCMASK_EVSYS_Pos); // Disable EVSYS clock
        PM->APBAMASK.reg &= ~(1 << PM_APBAMASK_RTC); // Disable RTC clock

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_ID_TCC0_TCC1;
        while(GCLK->STATUS.bit.SYNCBUSY);

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_ID_EIC;
        while(GCLK->STATUS.bit.SYNCBUSY);

        GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_ID_SERCOM3_CORE;
        while(GCLK->STATUS.bit.SYNCBUSY);

        /*
        //EVSYS GCLK asi není potřeba pro jen generování trigru z timeru, ale každopádně kdyby náhodou
        //GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0) | GCLK_CLKCTRL_ID_EVSYS_1;
        //while (GCLK->STATUS.bit.SYNCBUSY);
         */

    }


    void setup_interrupts()
    {
        EIC->CONFIG[0].reg = EIC_CONFIG_SENSE3_FALL | EIC_CONFIG_FILTEN3 |
                             EIC_CONFIG_SENSE5_FALL | EIC_CONFIG_FILTEN5;
        EIC->CONFIG[1].reg = EIC_CONFIG_SENSE7_FALL | EIC_CONFIG_FILTEN7; // 15
        EIC->INTENSET.reg = EIC_INTENSET_EXTINT3 |
                            EIC_INTENSET_EXTINT5 |
                            EIC_INTENSET_EXTINT15;
        //EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN3 |
        //                  EIC_WAKEUP_WAKEUPEN5 |
        //                  EIC_WAKEUP_WAKEUPEN15;
        EIC->CTRL.reg = EIC_CTRL_ENABLE;
        while(EIC->STATUS.bit.SYNCBUSY);

        NVIC_EnableIRQ(EIC_IRQn);

        //PM->SLEEP.reg = 0x2;
        // Set the processor to standby mode
        //SCB->SCR |= (1 << SCB_SCR_SLEEPONEXIT_Pos);
        //SCB->SCR |= (1 << SCB_SCR_SLEEPDEEP_Pos);
    }

    void enter_standby_mode()
    {
        //__DSB();
        //__WFI(); // Wait for interrupt
    }
}
