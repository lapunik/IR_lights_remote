#pragma once

#include <cstdint>
#include <span>
#include "sam.h"
#include "gpio.hpp"
#include "I2C.hpp"
#include "Date_time.hpp"
//#include <cstring>
//#include <cstdlib>


class DS3231
{

public:

    void init()
    {
        iic.init();
    }

    Date_time read_time()
    {
        std::array<std::uint8_t,1> adresa_pro_cteni_casu{0b0000'0000};
        std::array<std::uint8_t,7> buffer;
        iic.read(0x68,adresa_pro_cteni_casu,buffer);
        return Date_time(I2C::bcd2bin(buffer[6]) + 2000U, I2C::bcd2bin(buffer[5] & 0x7F),I2C::bcd2bin(buffer[4]),I2C::bcd2bin(buffer[2]),I2C::bcd2bin(buffer[1]), I2C::bcd2bin(buffer[0] & 0x7F));

    }

    /*
    // NEFUNGUJE
    void set_time()
    {
        const char* date = __DATE__;
        char month[4], day[3], year[5];

        std::strncpy(month, date, 3);
        month[3] = '\0';
        std::strncpy(day, date + 4, 2);
        day[2] = '\0';
        std::strncpy(year, date + 7, 4);
        year[4] = '\0';

        const char* time = __TIME__;
        char hod[3], min[3], sec[3];

        std::strncpy(hod, time, 2);
        hod[2] = '\0';
        std::strncpy(min, time + 3, 2);
        min[2] = '\0';
        std::strncpy(sec, time + 6, 2);
        sec[2] = '\0';

        std::int16_t year_int = std::atoi(year);
        std::int8_t day_int = std::atoi(day);

        std::int8_t month_int;
        std::int8_t sec_int = std::atoi(sec);
        std::int8_t min_int = std::atoi(min);
        std::int8_t hod_int = std::atoi(hod);


        if (std::strcmp(month, "Jan") == 0) {
            month_int = 1;
        }
        else if (std::strcmp(month, "Feb") == 0) {
            month_int = 2;
        }
        else if (std::strcmp(month, "Mar") == 0) {
            month_int = 3;
        }
        else if (std::strcmp(month, "Apr") == 0) {
            month_int = 4;
        }
        else if (std::strcmp(month, "May") == 0) {
            month_int = 5;
        }
        else if (std::strcmp(month, "Jun") == 0) {
            month_int = 6;
        }
        else if (std::strcmp(month, "Jul") == 0) {
            month_int = 7;
        }
        else if (std::strcmp(month, "Aug") == 0) {
            month_int = 8;
        }
        else if (std::strcmp(month, "Sep") == 0) {
            month_int = 9;
        }
        else if (std::strcmp(month, "Oct") == 0) {
            month_int = 10;
        }
        else if (std::strcmp(month, "Nov") == 0) {
            month_int = 11;
        }
        else if (std::strcmp(month, "Dec") == 0) {
            month_int = 12;
        }
        int weekday_int = 1;

        uint8_t data[7];
        // nastavení sekund
        data[0] = ((sec_int / 10) << 4) | (sec_int % 10);
        // nastavení minut
        data[1] = ((min_int / 10) << 4) | (min_int % 10);
        // nastavení hodin
        data[2] = ((hod_int / 10) << 4) | (hod_int % 10);
        // nastavení dne v týdnu (1-7)
        data[3] = weekday_int;
        // nastavení dne v měsíci
        data[4] = ((day_int / 10) << 4) | (day_int % 10);
        // nastavení měsíce
        data[5] = ((month_int / 10) << 4) | (month_int % 10);
        // nastavení roku (poslední 2 číslice)
        data[6] = ((year_int / 10) << 4) | (year_int % 10);

        // odeslání dat na ds3231
        iic.write(0x68, data, 7);

    }
     */

private:
    I2C iic;
};

