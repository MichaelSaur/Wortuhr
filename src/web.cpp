#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <DNSServer.h>
#include <globals.h>
#include <timeData.h>
#include "SPIFFS.h"

String getIndexHTML();

void setupServer(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", getIndexHTML().c_str()); 
        Serial.println("Client Connected");
    });
        
    server.on("/config", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        String inputParam;
        String newSsid;
        String newPassword;
    
        if (request->hasParam("ssid")) {
            inputMessage = request->getParam("ssid")->value();
            inputParam = "ssid";
            newSsid = inputMessage;
            Serial.println(inputMessage);
        }

        if (request->hasParam("password")) {
            inputMessage = request->getParam("password")->value();
            inputParam = "password";
            newPassword = inputMessage;
            Serial.println(inputMessage);
        }

        preferences.begin("wortuhr",false);
        preferences.putString("ssid",newSsid);
        preferences.putString("password",newPassword);
        preferences.end();
        ssid = newSsid;
        password = newPassword;

        request->redirect("/");
        	
        ESP.restart();
    });

    server.on("/color", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        String newDesign;
        CRGB newColor;
        uint8_t newBrightness;
    
        if (request->hasParam("design")) {
            inputMessage = request->getParam("design")->value();
            newDesign = inputMessage;
            Serial.println(inputMessage);
        }

        if (request->hasParam("colorR")) {
            inputMessage = request->getParam("colorR")->value();
            Serial.print("R: ");
            Serial.println(inputMessage);
            newColor.r = atoi(inputMessage.c_str());
        }
        if (request->hasParam("colorG")) {
            inputMessage = request->getParam("colorG")->value();
            Serial.print("G: ");
            Serial.println(inputMessage);
            newColor.g = atoi(inputMessage.c_str());
        }
        if (request->hasParam("colorB")) {
            inputMessage = request->getParam("colorB")->value();
            Serial.print("B: ");
            Serial.println(inputMessage);
            newColor.b = atoi(inputMessage.c_str());
        }
         if (request->hasParam("brightness")) {
            inputMessage = request->getParam("brightness")->value();
            Serial.print("brightness: ");
            Serial.println(inputMessage);
            newBrightness = atoi(inputMessage.c_str());
        }
        
        preferences.begin("wortuhr",false);
        preferences.putString("design",newDesign);
        preferences.putInt("baseColorR",newColor.r);
        preferences.putInt("baseColorG",newColor.g);
        preferences.putInt("baseColorB",newColor.b);
        preferences.putInt("brightness",newBrightness);
        preferences.end();
        design = newDesign;
        baseColor = newColor;
        brightness = newBrightness;
        FastLED.setBrightness(brightness);
        myTimeData.updateColor();
        request->redirect("/");
    });
    // Route for root / web page
    server.on("/js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/jquery.min.js", "application/javascript");
    });
    server.on("/js/iro.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/iro.js", "application/javascript");
    });
}

String getConfigHTML(){
    String html = String(config_html);
    html.replace("ssidValue",ssid);
    html.replace("passwordValue",password);
    //html.replace("pcolorValue",color);
    return html;
}

String getIndexHTML(){
    String html = String(index_html);
    html.replace("ssidValue",ssid);
    html.replace("passwordValue",password);
    html.replace("colorRValue",String(baseColor.r));
    html.replace("colorGValue",String(baseColor.g));
    html.replace("colorBValue",String(baseColor.b));
    html.replace("designValue",design);
    html.replace("brightnessValue",String(brightness));
    return html;
}