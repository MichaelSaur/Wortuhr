#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <DNSServer.h>
#include <web.h>
#include "time.h"
#include "words.h"
#include "timeData.h"
#include <globals.h>
#include <Preferences.h>
#include <RTClib.h>
#include "SPIFFS.h"

// preferences
Preferences preferences;
// WiFi
bool APMode = false;
bool NightMode = false;
bool OTA = false;
String ssid;
String password;
DNSServer dnsServer;
AsyncWebServer server(80);
TaskHandle_t Task1;
bool scanComplete = false;
String KnownSSIDs[10];

// Time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
String timezone = "CET-1CEST,M3.5.0,M10.5.0/3";
hw_timer_t *My_timer = NULL;
TimeData myTimeData = TimeData();

// RTC
bool RTCAvailable = true;
RTC_DS3231 rtc;

// LED
#define DATA_PIN    5
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
bool updatedLEDs[NUM_LEDS];
bool activeLEDs[NUM_LEDS];
uint8_t hue = 0;

uint8_t brightness = 32;
CRGB baseColor = CRGB::Blue;
String design = "Solid"; // Solid, Rainbow, Palette, Random

uint8_t brightnessDay = 32;
CRGB baseColorDay;
String designDay; // Solid, Rainbow, Palette, Random

bool nightModeActive;
uint8_t nightModeBeginHour;
uint8_t nightModeBeginMinute;
uint8_t nightModeEndHour;
uint8_t nightModeEndMinute;

uint8_t brightnessNight = 5;
CRGB baseColorNight;
String designNight; // Solid, Rainbow, Palette, Random

// Functions
void getPreferences();
void connectWiFi(String ssid, String password);
void scanNetworks();
void enableAP();
void checkWiFi();
void animationLoading();
void setTimezone(String timezone);
void initTime(String timezone);
void printLocalTime();
void IRAM_ATTR onTimer(){
  myTimeData.incTime();
}
void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  OTA = true;
  // <Add your own code here>
}
void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  OTA = false;
  // <Add your own code here>
}

void setup() {
  // Serial
  Serial.begin(115200);
  Wire.begin();
  if(!rtc.begin()){
    Serial.println("Konnte RTC nicht finden!");
    RTCAvailable = false;
  }
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // LEDS
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  
  animationLoading();
  FastLED.clear();
  FastLED.show();

  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000000, true);
  timerAlarmEnable(My_timer);

  // get WiFi credentials and colors, design,... from flash
  getPreferences();

  FastLED.setBrightness(brightness);

  myTimeData.init();
  setupServer();

  if (ssid == "" || password == ""){
    Serial.println("No values saved for ssid or password");
    // --> AP mode
    enableAP();
    for(int i=0;i<20;i++){
      fadeToBlackBy(leds,NUM_LEDS-8,80);
      delay(50);
    }
  }
  if(ssid != "" && password != ""){
    Serial.println("ssid: " + ssid + ", password: " + password);
    // --> WiFi Mode
    connectWiFi(ssid,password);
  }

  //preferences.putString("password", password);
    // Überprüfe, ob die RTC die aktuelle Zeit hat, andernfalls setze sie auf die Systemzeit
  if (rtc.lostPower() && RTCAvailable) {
    DateTime now = rtc.now();
    Serial.println(now.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.println("RTC hat die Zeit verloren! Setze die Zeit auf die Systemzeit.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  initTime(timezone);

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  //   request->send(200, "text/plain", "Hi! I am ESP32.");
  // });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  ElegantOTA.onStart(onOTAStart);
  //ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
  server.begin();
  Serial.println("HTTP server started");
  FastLED.clear();
  FastLED.show();
}

void loop() {
  if(!OTA){ 
    myTimeData.loop();
    if(scanComplete){
      ElegantOTA.loop();
      if (APMode){
        dnsServer.processNextRequest();
      }
    }else{
      scanNetworks();
    }
    delay(100);
    EVERY_N_MILLISECONDS(500) {
      hue++;
      if(hue == 255){
        hue = 0;
      }
    }
  }
}

void getPreferences(){
  preferences.begin("wortuhr",false);
  ssid = preferences.getString("ssid", ""); 
  password = preferences.getString("password", "");
  // day variables
  baseColorDay.r = preferences.getInt("baseColorR", 255);
  baseColorDay.g = preferences.getInt("baseColorG", 255);
  baseColorDay.b = preferences.getInt("baseColorB", 255);
  designDay = preferences.getString("design", "Solid");
  if (designDay ==""){
    designDay = "Solid";
  }
  brightnessDay = preferences.getInt("brightness", 32);
  // night variables
  nightModeActive = preferences.getBool("nightModeActive", false);
  nightModeBeginHour = preferences.getInt("nightModeBeginH", 23);
  nightModeBeginMinute = preferences.getInt("nightModeBeginM", 0);
  nightModeEndHour = preferences.getInt("nightModeEndH", 8);
  nightModeEndMinute = preferences.getInt("nightModeEndM", 0);
  baseColorNight.r = preferences.getInt("baseColorNightR", 255);
  baseColorNight.g = preferences.getInt("baseColorNightG", 255);
  baseColorNight.b = preferences.getInt("baseColorNightB", 255);
  designNight = preferences.getString("designNight", "Solid");
  if (designNight ==""){
    designNight = "Solid";
  }
  brightnessNight = preferences.getInt("brightnessNight", 5);
  preferences.end();
}

void connectWiFi(String ssid, String password){
  APMode = false;
  int trys = 0;
  WiFi.setHostname("Wortuhr");
  WiFi.begin(ssid.c_str(),password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    animationLoading();
    trys++;
    Serial.println("Connecting to WiFi..");
    if(WiFi.status() == WL_CONNECT_FAILED || trys > 10){
      Serial.println("failed");
      enableAP();
      break;
    }
  }
  for(int i=0;i<20;i++){
    fadeToBlackBy(leds,NUM_LEDS-8,80);
    delay(50);
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.localIP()[0]);
  myTimeData.displayNumber(WiFi.localIP()[0]);
  delay(2000);
  Serial.println(WiFi.localIP()[1]);
  myTimeData.displayNumber(WiFi.localIP()[1]);
  delay(2000);
  Serial.println(WiFi.localIP()[2]);
  myTimeData.displayNumber(WiFi.localIP()[2]);
  delay(2000);
  Serial.println(WiFi.localIP()[3]);
  myTimeData.displayNumber(WiFi.localIP()[3]);
  delay(2000);
}

void enableAP(){
  APMode = true;
  WiFi.setHostname("Wortuhr");
  WiFi.mode(WIFI_AP); 
  WiFi.softAP("Wortuhr", "FünfVorZwölf");
  Serial.println("Creating access point..");
  // while (WiFi.status() != WL_IDLE_STATUS) {
  //   animationLoading();
  //   Serial.println("Creating access point..");
  // }
  Serial.println(WiFi.softAPIP());
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
}

void checkWiFi(){
  if(WiFi.status() == WL_CONNECTED){
    return;
  }else{
    connectWiFi(ssid,password);
    return;
  }
}

void animationLoading(){
  for(int i=0;i<11;i++){
    fadeToBlackBy(leds,NUM_LEDS-4*num_leds_per_letter,80);
    for(int j=0;j<NUM_LEDS-4*num_leds_per_letter;j++){
      if(j % 22 == i || j % 22 == 21-i){
        leds[j] = CRGB::Blue;
      }
    }
    FastLED.show();
    delay(50);
  }
  // for(int i= 0; i<20;i++){
  //   leds[52] = CRGB(beatsin16(20,10,255),beatsin16(20,10,128),0);
  //   leds[53] = CRGB(beatsin16(20,10,255),beatsin16(20,10,128),0);
  //   leds[54] = CRGB(beatsin16(20,10,255),beatsin16(20,10,128),0);
  //   leds[55] = CRGB(beatsin16(20,10,255),beatsin16(20,10,128),0);
  //   FastLED.show();
  //   delay(50);
  // }
}

void animationSuccess(){
  for(int i= 0; i<40;i++){
    leds[52] = CRGB(0,beatsin16(20,10,128),0);
    leds[53] = CRGB(0,beatsin16(20,10,128),0);
    leds[54] = CRGB(0,beatsin16(20,10,128),0);
    leds[55] = CRGB(0,beatsin16(20,10,128),0);
    FastLED.show();
    delay(50);
  }
  while(beatsin16(20,10,128)>20){
    leds[52] = CRGB(0,beatsin16(20,10,128),0);
    leds[53] = CRGB(0,beatsin16(20,10,128),0);
    leds[54] = CRGB(0,beatsin16(20,10,128),0);
    leds[55] = CRGB(0,beatsin16(20,10,128),0);
    FastLED.show();
    delay(50);
  }
  leds[52] = CRGB::Black;
  leds[53] = CRGB::Black;
  leds[54] = CRGB::Black;
  leds[55] = CRGB::Black;
  FastLED.show();
}

void initTime(String timezone){
  if (!APMode){
    struct tm timeinfo;

    Serial.println("Setting up time");
    configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
    if(!getLocalTime(&timeinfo)){
      Serial.println("  Failed to obtain time");
      return;
    }
    Serial.println("  Got the time from NTP");
    // Now we can set the real timezone
    setTimezone(timezone);
  }
}

void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time 1");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}

void scanNetworks(){
  int n = WiFi.scanNetworks();
    if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    if (n>10){
      n=10;
    }
    for (int i = 0; i < n; i++) {
      // Print SSID and RSSI for each network found
      KnownSSIDs[i] = WiFi.SSID(i);
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(KnownSSIDs[i]);
      delay(10);
    }
  }
  scanComplete = true;
}