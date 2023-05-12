#pragma once

#include "gpio_definitions.hpp"

namespace nd::peripherals
{
    enum class Gpio_alternate : std::uint8_t
    {
            A = 0,
            B = 1,
            C = 2,
            D = 3,
            E = 4,
            F = 5,
            G = 6,
            H = 7,
            I = 8,
            J = 9,
            K = 10,
            L = 11,
            M = 12,
            N = 13,
    };

    class IO_pin
    {
        public:
            explicit IO_pin(const GPIO_pin& gpio_pin)
                : port{gpio_pin.port_group},
                  bit_pos{gpio_pin.pin_num},
                  bit_mask{1u << bit_pos}
            {}

            IO_pin(const GPIO_pin& gpio_pin, const bool inverted_p)
                    : port{gpio_pin.port_group},
                      bit_pos{gpio_pin.pin_num},
                      bit_mask{1u << bit_pos},
                      inverted{inverted_p}
            {}

            void set_as_input() const
            {
                port.DIRCLR.reg = bit_mask;
            }

            void set_as_output() const
            {
                port.DIRSET.reg = bit_mask;
            }

            void enable_input() const
            {
                port.PINCFG[bit_pos].bit.INEN = 1;
            }

            void disable_input() const
            {
                port.PINCFG[bit_pos].bit.INEN = 0;
            }

            void set_pull_up()
            {
                port.PINCFG[bit_pos].bit.PULLEN = 1;
                on();
            }

            void set_pull_down()
            {
                port.PINCFG[bit_pos].bit.PULLEN = 1;
                off();
            }

            void disable_pull()
            {
                port.PINCFG[bit_pos].bit.PULLEN = 0;
            }

            void set_driver_strength(const bool state) const
            {
                port.PINCFG[bit_pos].bit.DRVSTR = state;
            }

            void off() const
            {
                port.OUTCLR.reg = bit_mask;
            }

            void on() const
            {
                port.OUTSET.reg = bit_mask;
            }

            void toggle() const
            {
                port.OUTTGL.reg = bit_mask;
            }

            [[nodiscard]] bool is_set() const
            {
                const bool state = port.IN.reg & bit_mask;
                return inverted == !state;
            }

            void set_alternate_function(const Gpio_alternate alternate_function) const
            {
                // Select alternate function
                if (bit_pos % 2 == 0)
                {
                    // Even pins (0, 2, 4, ...)
                    port.PMUX[bit_pos/2].bit.PMUXE = static_cast<std::uint8_t>(alternate_function) & 0x0fu;
                }
                else
                {
                    // Odd pins (1, 3, 5, ...)
                    port.PMUX[(bit_pos-1)/2].bit.PMUXO = static_cast<std::uint8_t>(alternate_function) & 0x0fu;
                }

                // Enable multiplexing
                port.PINCFG[bit_pos].bit.PMUXEN = 1;
            }

            void disable_alternate_function() const
            {
                port.PINCFG[bit_pos].bit.PMUXEN = 0;
            }

        private:
            PortGroup& port;
            const std::uint8_t bit_pos;
            const std::uint32_t bit_mask;
            const bool inverted{false};
    };

    class Output_pin : private IO_pin
    {
        public:
            explicit Output_pin(const GPIO_pin &gpio_pin)
                : IO_pin(gpio_pin)
            {
                set_as_output();
            }

            using IO_pin::set_driver_strength;
            using IO_pin::on;
            using IO_pin::off;
            using IO_pin::toggle;
    };

    class Input_pin : private IO_pin
    {
        public:
            explicit Input_pin(const GPIO_pin &gpio_pin)
                    : IO_pin(gpio_pin)
            {
                set_as_input();
                enable_input();
            }

            Input_pin(const GPIO_pin &gpio_pin, const bool inverted_p)
                    : IO_pin(gpio_pin, inverted_p)
            {
                set_as_input();
                enable_input();
            }

            using IO_pin::set_pull_up;
            using IO_pin::set_pull_down;
            using IO_pin::disable_pull;
            using IO_pin::is_set;
    };

    class Alternate_pin : private IO_pin
    {
        public:
            explicit Alternate_pin(const GPIO_pin &gpio_pin)
                    : IO_pin(gpio_pin)
            {
                disable_input();
                disable_pull();
            }

            Alternate_pin(const GPIO_pin &gpio_pin, const Gpio_alternate alternate_function)
                    : IO_pin(gpio_pin)
            {
                disable_input();
                disable_pull();
                set_alternate_function(alternate_function);
            }

            using IO_pin::set_alternate_function;
            using IO_pin::disable_alternate_function;
            using IO_pin::set_pull_up;
            using IO_pin::set_pull_down;
            using IO_pin::disable_pull;
            using IO_pin::set_driver_strength;
    };
}
