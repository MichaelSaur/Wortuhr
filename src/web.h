#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <globals.h>
#include "SPIFFS.h"

#ifndef web_h
#define web_h

void setupServer();
String getIndexHTML();
String templateProcessor(const String& var);

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(SPIFFS,"/wortuhr.html","text/html",false,templateProcessor);
  }
};

#endif