#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <timeData.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <DNSServer.h>
#include <Preferences.h>

extern Preferences preferences;

// WIFi
extern String ssid;
extern String password;
extern DNSServer dnsServer;
extern AsyncWebServer server;
extern const char index_html[] PROGMEM;
extern const char config_html[] PROGMEM;

// Time
extern const char* ntpServer;
extern const long  gmtOffset_sec;
extern const int   daylightOffset_sec;
extern String timezone;
extern hw_timer_t *My_timer;
extern TimeData myTimeData;

// LED
#define DATA_PIN    5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 228
extern CRGB leds[NUM_LEDS];
extern bool updatedLEDs[NUM_LEDS];
extern bool activeLEDs[NUM_LEDS];
extern uint8_t brightness;
extern CRGB baseColor;
extern String design; // Solid, Rainbow, Palette
extern uint8_t hue;