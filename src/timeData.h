#include <Arduino.h>
#include <FastLED.h>

#ifndef timeData_h
#define timeData_h

CHSV rgb2hsv(CRGB rgb);

class TimeData{
    public:
    TimeData();
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    void incTime();
    void loop();
    String getTimeString();
    void syncTime();
    void printTime();
    void displayTime();
    void updateColor();
    void init();
    void displayNumber(int num);
    void animate();
    void checkNightMode();
    
    private:
    bool timesync = true;
    bool showTime = false;
    bool showMinute = false;
    void displayMinute();
    void clear();
    void printActiveLEDs();
    void setLED(int index, bool state);
    void displayDigit(uint8_t digit, uint8_t position);
    void setNightMode(bool state);
};

#endif