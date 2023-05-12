#pragma once

#include <cstdint>


class Date_time
{
public:

    Date_time(uint16_t y, uint8_t mo, uint8_t d, uint8_t h, uint8_t mi, uint8_t s) : year{y}, month{mo}, day{d},hour{h},minute{mi},second{s}
    {

    }

    Date_time(){};

    uint16_t getYear() const { return year; }
    uint8_t getMonth() const {return month;}
    uint8_t getDay() const {return day;}
    uint8_t getHour() const {return hour;}
    uint8_t getMinute() const {return minute;}
    uint8_t getSecond() const {return second;}

    void setYear(uint16_t year) {this->year = year;}
    void setMonth(uint8_t month) {this->month = month;}
    void setDay(uint8_t day) {this->day = day;}
    void setHour(uint8_t hour) {this->hour = hour;}
    void setMinute(uint8_t minute) {this->minute = minute;}
    void setSecond(uint8_t second) {this->second = second;}

private:
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};