#pragma once

#include <cstdint>
#include <span>
#include "sam.h"
#include "gpio.hpp"



class I2C
{
public:

    static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }

    void init()
    {
        if (getIsInitialized())
        {
            return;
        }

        nd::peripherals::Alternate_pin sda{nd::peripherals::gpio_pins::pa16};
        sda.set_driver_strength(true);
        sda.set_pull_up(); // ???
        sda.set_alternate_function(nd::peripherals::Gpio_alternate::D); // SERCOM_3 PAD_0

        nd::peripherals::Alternate_pin scl{nd::peripherals::gpio_pins::pa17};
        scl.set_driver_strength(true);
        scl.set_pull_up(); // ???
        scl.set_alternate_function(nd::peripherals::Gpio_alternate::D); //SERCOM_3 PAD_1

        // Main settings
        sercom.CTRLA.reg =
                SERCOM_I2CM_CTRLA_MODE(0x05) |        //master serial communication interface
                SERCOM_I2CM_CTRLA_SDAHOLD(0x02);    // ?Neg. edge of SCL -> 300-600 ns (1 ->50-100ns, 3-> 400-800ns)
        /*SERCOM_I2CM_CTRLA_LOWTOUTEN |     // ?Enable SCL low time-out? (for slow slaves)
        SERCOM_I2CM_CTRLA_INACTOUT(1);    // ?if the bus is inactive for 5-6SCL -> bus goes to idle state
        */

        // Enable smart mode
        sercom.CTRLB.bit.SMEN = 0x01;

        // 400kHz baudrate
        sercom.BAUD.reg = SERCOM_I2CM_BAUD_BAUD(55);

        // Enable SERCOM
        sercom.CTRLA.bit.ENABLE = 1;
        while (sercom.SYNCBUSY.bit.ENABLE);

        // Set idle state
        sercom.STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(0x01);
        while (sercom.SYNCBUSY.bit.SYSOP);

        setIsInitialized(true);
    }

    void write(const uint8_t address, const std::span<std::uint8_t> data, const bool stop_bit=true)
    {
        if (!isInitialized_) {
            init();
        }
        sercom.CTRLB.bit.ACKACT = 0;
        sercom.CTRLB.bit.CMD = 0x01;     // execute acknowledge act + repeated start bit
        while (sercom.SYNCBUSY.bit.SYSOP);

        // Send address
        sercom.ADDR.reg = SERCOM_I2CM_ADDR_ADDR((address << 1u) | 0u);            // 0th bit read/write flag (0 = write, 1 = read)
        while (sercom.SYNCBUSY.bit.SYSOP);
        while (!sercom.INTFLAG.bit.MB);
        //TODO: Maybe add 10us delay

        // Send data
        for (const auto byte : data)
        {
            sercom.DATA.reg = byte;
            while (sercom.SYNCBUSY.bit.SYSOP);
            while (!sercom.INTFLAG.bit.MB);
            //TODO: Maybe add 10us delay
        }

        if (stop_bit)
        {
            sercom.CTRLB.bit.CMD = 0x03;     // execute acknowledge act + stop bit
            while (sercom.SYNCBUSY.bit.SYSOP);
        }
        else
        {
            sercom.CTRLB.bit.CMD = 0x01;     // ACK + Start condition
            while (sercom.SYNCBUSY.bit.SYSOP);
        }
    }

    void read(const uint8_t address, const std::span<std::uint8_t> data_tx, const std::span<std::uint8_t> data_rx)
    {
        write(address, data_tx, false);     //third argument assures to do start condition at the end of write

        // Send address
        sercom.ADDR.reg = SERCOM_I2CM_ADDR_ADDR((static_cast<unsigned int>(address << 1u)) | 1u);            // bit 0 read/write flag (0 = write, 1 = read)
        while (sercom.SYNCBUSY.bit.SYSOP);

        // Receive data
        for (std::uint32_t i=0; i<data_rx.size(); ++i)
        {
            while (!sercom.INTFLAG.bit.SB);
            data_rx[i] = sercom.DATA.reg & 0xFF;

            if (i == data_rx.size()-2)
            {
                sercom.CTRLB.bit.ACKACT = 1;    //NO ACK
            }
        }

        sercom.CTRLB.bit.CMD = 0x03;    // stop bit
        while (sercom.SYNCBUSY.bit.SYSOP);
    }



    bool getIsInitialized() const
    {
        return isInitialized_;
    }

    void setIsInitialized(bool state)
    {
        isInitialized_ = state;
    }

private:
    SercomI2cm& sercom{SERCOM3->I2CM};
    bool isInitialized_ = false;
};

