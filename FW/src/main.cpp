#include <cstdint>
#include "sam.h"

#include "gpio.hpp"
#include "System.hpp"
#include "IR_transmitter.hpp"
#include "DS3231.hpp"
#include "Date_time.hpp"



using namespace nd::peripherals::gpio_pins; // abych mohl psát jen PAxx a nemusel vypisovt celou

nd::peripherals::Output_pin led1{pa10};

// IR sender
rg::IR_transmitter ir_sender{};

// Prevent use of malloc, new
extern "C" void *__wrap_malloc(std::size_t)
{
    return nullptr;
}
/*
void EIC_Handler()
{

   if(EIC->INTFLAG.bit.EXTINT3)
    {
        led1.on();
        ir_sender.send_code(rg::IR_transmitter::POWER);
        led1.off();
        EIC->INTFLAG.reg |= EIC_INTFLAG_EXTINT3;
    }
    else if(EIC->INTFLAG.bit.EXTINT5)
    {
        led1.on();
        ir_sender.send_code(rg::IR_transmitter::MID_BUTTON);
        led1.off();
        EIC->INTFLAG.reg |= EIC_INTFLAG_EXTINT5;
    }
    else if(EIC->INTFLAG.bit.EXTINT15)
    {
        led1.on();
        ir_sender.send_code(rg::IR_transmitter::EVENING);
        led1.off();
        EIC->INTFLAG.reg |= EIC_INTFLAG_EXTINT15;
    }
}*/
void main()
{
    SystemInit();

    nd::system::setup_oscilators(); // nastavení hodin (připojení a GCLK a připojení ke zdrojům hodin)
    nd::system::setup_peripherals_clocks(); // připojení některých periferií k hodinám (GCLK)

    // Buttons
    /*nd::peripherals::Alternate_pin on_off{pa03};
    //on_off.set_pull_up();
    //on_off.set_alternate_function(nd::peripherals::Gpio_alternate::A);
    //nd::peripherals::Alternate_pin night{pa15};
    //night.set_pull_up();
    //night.set_alternate_function(nd::peripherals::Gpio_alternate::A);
    //nd::peripherals::Alternate_pin day{pb05};
    //day.set_pull_up();
    //day.set_alternate_function(nd::peripherals::Gpio_alternate::A);
    */

    //nd::system::setup_interrupts();

    DS3231 rtc;
    rtc.init();

    //rtc.set_time();

    ir_sender.init();
    led1.off();

    //nd::system::enter_standby_mode();

    bool ledka = false;

#pragma clang diagnostic push
    #pragma ide diagnostic ignored "EndlessLoop"
    while (true)
    {

        //ir_sender.send_code(rg::IR_transmitter::POWER);

//        if((second % 5) == 0)
//        if(minute == 40)

    Date_time time;

    time = rtc.read_time();

    if(time.getMinute() == 26)
        {
            led1.on();

//            if(ledka)
//            {
//                led1.on();
//                ledka = false;
//            }
//            else
//            {
//                led1.off();
//                ledka = true;
//            }

        }
        else
        {
            led1.off();
        }

        nd::peripherals::systick::delay_ms(100,1);

        //nd::system::enter_standby_mode();
        /*
        if (!on_off.is_set())
        {
            while(!on_off.is_set());
            led1.on();
            ir_sender.send_code(rg::IR_transmitter::POWER);
            led1.off();
        }
        if (!day.is_set())
        {
            while(!day.is_set());
            led1.on();
            ir_sender.send_code(rg::IR_transmitter::MID_BUTTON);
            led1.off();
        }

        if (!night.is_set())
        {
            while(!night.is_set());
            led1.on();
            ir_sender.send_code(rg::IR_transmitter::EVENING);
            led1.off();
        }
        */


    }
    #pragma clang diagnostic pop
}
