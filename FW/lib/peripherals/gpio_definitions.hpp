#pragma once

#include <cstdint>

#include "sam.h"



namespace nd::peripherals
{
    struct GPIO_pin
    {
        PortGroup& port_group;
        const std::uint8_t pin_num;
    };

    namespace gpio_pins
    {
        constexpr GPIO_pin pa00{PORT->Group[0], 0};
        constexpr GPIO_pin pa01{PORT->Group[0], 1};
        constexpr GPIO_pin pa02{PORT->Group[0], 2};
        constexpr GPIO_pin pa03{PORT->Group[0], 3};
        constexpr GPIO_pin pa04{PORT->Group[0], 4};
        constexpr GPIO_pin pa05{PORT->Group[0], 5};
        constexpr GPIO_pin pa06{PORT->Group[0], 6};
        constexpr GPIO_pin pa07{PORT->Group[0], 7};
        constexpr GPIO_pin pa08{PORT->Group[0], 8};
        constexpr GPIO_pin pa09{PORT->Group[0], 9};
        constexpr GPIO_pin pa10{PORT->Group[0], 10};
        constexpr GPIO_pin pa11{PORT->Group[0], 11};
        constexpr GPIO_pin pa12{PORT->Group[0], 12};
        constexpr GPIO_pin pa13{PORT->Group[0], 13};
        constexpr GPIO_pin pa14{PORT->Group[0], 14};
        constexpr GPIO_pin pa15{PORT->Group[0], 15};
        constexpr GPIO_pin pa16{PORT->Group[0], 16};
        constexpr GPIO_pin pa17{PORT->Group[0], 17};
        constexpr GPIO_pin pa18{PORT->Group[0], 18};
        constexpr GPIO_pin pa19{PORT->Group[0], 19};
        constexpr GPIO_pin pa20{PORT->Group[0], 20};
        constexpr GPIO_pin pa21{PORT->Group[0], 21};
        constexpr GPIO_pin pa22{PORT->Group[0], 22};
        constexpr GPIO_pin pa23{PORT->Group[0], 23};
        constexpr GPIO_pin pa24{PORT->Group[0], 24};
        constexpr GPIO_pin pa25{PORT->Group[0], 25};
        constexpr GPIO_pin pa27{PORT->Group[0], 27};
        constexpr GPIO_pin pa30{PORT->Group[0], 30};
        constexpr GPIO_pin pa31{PORT->Group[0], 31};

        constexpr GPIO_pin pb00{PORT->Group[1], 0};
        constexpr GPIO_pin pb01{PORT->Group[1], 1};
        constexpr GPIO_pin pb02{PORT->Group[1], 2};
        constexpr GPIO_pin pb03{PORT->Group[1], 3};
        constexpr GPIO_pin pb04{PORT->Group[1], 4};
        constexpr GPIO_pin pb05{PORT->Group[1], 5};
        constexpr GPIO_pin pb06{PORT->Group[1], 6};
        constexpr GPIO_pin pb07{PORT->Group[1], 7};
        constexpr GPIO_pin pb08{PORT->Group[1], 8};
        constexpr GPIO_pin pb09{PORT->Group[1], 9};
        constexpr GPIO_pin pb10{PORT->Group[1], 10};
        constexpr GPIO_pin pb11{PORT->Group[1], 11};
        constexpr GPIO_pin pb12{PORT->Group[1], 12};
        constexpr GPIO_pin pb13{PORT->Group[1], 13};
        constexpr GPIO_pin pb14{PORT->Group[1], 14};
        constexpr GPIO_pin pb15{PORT->Group[1], 15};
        constexpr GPIO_pin pb16{PORT->Group[1], 16};
        constexpr GPIO_pin pb17{PORT->Group[1], 17};
        constexpr GPIO_pin pb22{PORT->Group[1], 22};
        constexpr GPIO_pin pb23{PORT->Group[1], 23};
        constexpr GPIO_pin pb30{PORT->Group[1], 30};
        constexpr GPIO_pin pb31{PORT->Group[1], 31};
    }
}
