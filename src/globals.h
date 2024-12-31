#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <timeData.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <RTClib.h>

extern Preferences preferences;

// WIFi
extern bool APMode;
extern bool NightMode;
extern String ssid;
extern String password;
extern DNSServer dnsServer;
extern AsyncWebServer server;
extern String KnownSSIDs[10];

// Time
extern const char* ntpServer;
extern const long  gmtOffset_sec;
extern const int   daylightOffset_sec;
extern String timezone;
extern hw_timer_t *My_timer;
extern TimeData myTimeData;

// RTC
extern bool RTCAvailable;
extern RTC_DS3231 rtc;

// LED
#define DATA_PIN    5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 114
#define num_leds_per_letter 1
extern CRGB leds[NUM_LEDS];
extern bool updatedLEDs[NUM_LEDS];
extern bool activeLEDs[NUM_LEDS];
extern uint8_t hue;

extern uint8_t brightness;
extern CRGB baseColor;
extern String design; // Solid, Rainbow, Palette

extern uint8_t brightnessDay;
extern CRGB baseColorDay;
extern String designDay; // Solid, Rainbow, Palette

extern bool nightModeActive;
extern uint8_t nightModeBeginHour;
extern uint8_t nightModeBeginMinute;
extern uint8_t nightModeEndHour;
extern uint8_t nightModeEndMinute;
extern uint8_t brightnessNight;
extern CRGB baseColorNight;
extern String designNight; // Solid, Rainbow, Palette