#pragma once

#include <cstdint>

#include "sam.h"
#include "systick.hpp"
#include "gpio.hpp"
#include "Timer.hpp"

#define NEC_ADDRESS_BITS        16 // 16 bit address or 8 bit address and 8 bit inverted address
#define NEC_COMMAND_BITS        16 // Command and inverted command

#define NEC_BITS                (NEC_ADDRESS_BITS + NEC_COMMAND_BITS)
#define NEC_UNIT                560             // 21.28 periods of 38 kHz, 11.2 ticks TICKS_LOW = 8.358 TICKS_HIGH = 15.0

#define NEC_HEADER_MARK         (16 * NEC_UNIT) // 9000 | 180
#define NEC_HEADER_SPACE        (8 * NEC_UNIT)  // 4500 | 90

#define NEC_BIT_MARK            NEC_UNIT
#define NEC_ONE_SPACE           (3 * NEC_UNIT)  // 1690 | 33.8  TICKS_LOW = 25.07 TICKS_HIGH = 45.0
#define NEC_ZERO_SPACE          NEC_UNIT

#define NEC_REPEAT_HEADER_SPACE (4 * NEC_UNIT)  // 2250

#define NEC_AVERAGE_DURATION    62000 // NEC_HEADER_MARK + NEC_HEADER_SPACE + 32 * 2,5 * NEC_UNIT + NEC_UNIT // 2.5 because we assume more zeros than ones
#define NEC_MINIMAL_DURATION    49900 // NEC_HEADER_MARK + NEC_HEADER_SPACE + 32 * 2 * NEC_UNIT + NEC_UNIT // 2.5 because we assume more zeros than ones
#define NEC_REPEAT_DURATION     (NEC_HEADER_MARK  + NEC_REPEAT_HEADER_SPACE + NEC_BIT_MARK) // 12 ms
#define NEC_REPEAT_PERIOD       110000 // Commands are repeated every 110 ms (measured from start to start) for as long as the key on the remote control is held down.
#define NEC_REPEAT_DISTANCE         (NEC_REPEAT_PERIOD - NEC_AVERAGE_DURATION) // 48 ms
#define NEC_MAXIMUM_REPEAT_DISTANCE (NEC_REPEAT_PERIOD - NEC_MINIMAL_DURATION + 5) // 65 ms

namespace rg
{
    class IR_transmitter {
    public:

        enum CODES
                {
                    POWER = 0xBE41FF00,
                    EVENING = 0xBF40FF00,
                    SHINE_UP = 0xA35CFF00,
                    SHINE_DOWN = 0xA25DFF00,
                    COLOR_UP = 0xBB44FF00,
                    COLOR_DOWN = 0xB748FF00,
                    MID_BUTTON = 0xF906FF00,
                    THREE_STATES = 0xAF50FF00,
                    SLEEP = 0xB24DFF00,
                    HALF_HOURS = 0xAE51FF00,
        };

        IR_transmitter()
        {
        }

        void init()
        {
            IR_pin.init();
        }


        void send_code(uint32_t message)
        {
            IR_pin.enable();
            nd::peripherals::systick::delay_us(NEC_HEADER_MARK);
            IR_pin.disable();
            nd::peripherals::systick::delay_us(NEC_HEADER_SPACE);

            necSendMessage(message);

            // Konec signálu
            IR_pin.enable();
            nd::peripherals::systick::delay_us(NEC_REPEAT_HEADER_SPACE);
            IR_pin.disable();
        }



        rg::Timer IR_pin{*TCC1, nd::peripherals::gpio_pins::pa07};

        //nd::peripherals::Output_pin IR_pin{nd::peripherals::gpio_pins::pa07};

    private:

        // Funkce pro vyslání jednoho bitu v protokolu NEC
        void necSendBit(bool bit)
        {
            if (bit) {
                // Odeslání bitu 1
                IR_pin.enable();
                nd::peripherals::systick::delay_us(NEC_BIT_MARK);
                IR_pin.disable();
                nd::peripherals::systick::delay_us(NEC_ONE_SPACE);
            } else {
                // Odeslání bitu 0
                IR_pin.enable();
                nd::peripherals::systick::delay_us(NEC_BIT_MARK);
                IR_pin.disable();
                nd::peripherals::systick::delay_us(NEC_ZERO_SPACE);
            }
        }

        void necSendMessage(uint32_t bytes)
        {
            for (int i = 0; i < 32; i++) {
                bool bit = (bytes >> i) & 0x01;
                necSendBit(bit);
            }
        }
    };
}
