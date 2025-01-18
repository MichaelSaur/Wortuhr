#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#include <globals.h>
#include <timeData.h>
#include "SPIFFS.h"

String getIndexHTML();
String templateProcessor(const String& var);

void setupServer(){
    server.serveStatic("/",SPIFFS,"/");
    server.onNotFound([](AsyncWebServerRequest *request){
        if (request->url() == "/"){
            request->send(SPIFFS,"/wortuhr.html","text/html",false,templateProcessor);
        }else{
            request->send(404);
        } 
    });

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    //     //request->send(SPIFFS,);
    //     String url = request->url();
    //     if (url == "/"){
    //         url = "/index.html";
    //     }
    //     Serial.println(url);
    //     request->send(SPIFFS,url,String(),false,templateProcessor);
    //     //request->send_P(200, "text/html", getIndexHTML().c_str()); 
    //     //Serial.println("Client Connected");
    // });
        
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
        designDay = newDesign;
        baseColorDay = newColor;
        brightnessDay = newBrightness;
        if(!NightMode){
            design = newDesign;
            baseColor = newColor;
            brightness = newBrightness;
            FastLED.setBrightness(brightness);
        }
        myTimeData.updateColor();
        request->redirect("/");
    });

    // night
    server.on("/colorNight", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        String newDesign;
        CRGB newColor;
        uint8_t newBrightness;
        bool active = false;
        uint8_t newBeginHour;
        uint8_t newBeginMinute;
        uint8_t newEndHour;
        uint8_t newEndMinute;

        if (request->hasParam("nightModeActiveInt")) {
            inputMessage = request->getParam("nightModeActiveInt")->value();
            Serial.print("active: ");
            Serial.println(inputMessage);
            int activeInt = atoi(inputMessage.c_str());
            if(activeInt == 1){
                active = true;
            }else{
                active = false;
            }
        }
        if (request->hasParam("nightModeBeginHour")) {
            inputMessage = request->getParam("nightModeBeginHour")->value();
            Serial.print("BeginH: ");
            Serial.println(inputMessage);
            newBeginHour = atoi(inputMessage.c_str());
        }
        if (request->hasParam("nightModeBeginMinute")) {
            inputMessage = request->getParam("nightModeBeginMinute")->value();
            Serial.print("BeginM: ");
            Serial.println(inputMessage);
            newBeginMinute = atoi(inputMessage.c_str());
        }
        if (request->hasParam("nightModeEndHour")) {
            inputMessage = request->getParam("nightModeEndHour")->value();
            Serial.print("EndH: ");
            Serial.println(inputMessage);
            newEndHour = atoi(inputMessage.c_str());
        }
        if (request->hasParam("nightModeEndMinute")) {
            inputMessage = request->getParam("nightModeEndMinute")->value();
            Serial.print("EndM: ");
            Serial.println(inputMessage);
            newEndMinute = atoi(inputMessage.c_str());
        }

        if (request->hasParam("designNight")) {
            inputMessage = request->getParam("designNight")->value();
            newDesign = inputMessage;
            Serial.println(inputMessage);
        }

        if (request->hasParam("colorNightR")) {
            inputMessage = request->getParam("colorNightR")->value();
            Serial.print("RNight: ");
            Serial.println(inputMessage);
            newColor.r = atoi(inputMessage.c_str());
        }
        if (request->hasParam("colorNightG")) {
            inputMessage = request->getParam("colorNightG")->value();
            Serial.print("GNight: ");
            Serial.println(inputMessage);
            newColor.g = atoi(inputMessage.c_str());
        }
        if (request->hasParam("colorNightB")) {
            inputMessage = request->getParam("colorNightB")->value();
            Serial.print("BNight: ");
            Serial.println(inputMessage);
            newColor.b = atoi(inputMessage.c_str());
        }
        if (request->hasParam("brightnessNight")) {
            inputMessage = request->getParam("brightnessNight")->value();
            Serial.print("brightnessNight: ");
            Serial.println(inputMessage);
            newBrightness = atoi(inputMessage.c_str());
        }
        
        preferences.begin("wortuhr",false);
        preferences.putInt("nightModeBeginH",newBeginHour);
        nightModeBeginHour = newBeginHour;
        preferences.putInt("nightModeBeginM",newBeginMinute);
        nightModeBeginMinute = newBeginMinute;
        preferences.putInt("nightModeEndH",newEndHour);
        nightModeEndHour = newEndHour;
        preferences.putInt("nightModeEndM",newEndMinute);
        nightModeEndMinute = newEndMinute;

        preferences.putBool("nightModeActive",active);
        preferences.putString("designNight",newDesign);
        preferences.putInt("baseColorNightR",newColor.r);
        preferences.putInt("baseColorNightG",newColor.g);
        preferences.putInt("baseColorNightB",newColor.b);
        preferences.putInt("brightnessNight",newBrightness);
        preferences.end();
        nightModeActive = active;
        designNight = newDesign;
        baseColorNight = newColor;
        brightnessNight = newBrightness;
        if(NightMode){
            design = newDesign;
            baseColor = newColor;
            brightness = newBrightness;
            FastLED.setBrightness(brightness);
        }
        myTimeData.checkNightMode();
        myTimeData.updateColor();
        request->redirect("/");
    });
    
    server.on("/time",[] (AsyncWebServerRequest *request){
        int timeUnix;
        if (request->hasParam("timeUnix")) {
            timeUnix = atoi(request->getParam("timeUnix")->value().c_str());
            Serial.println(timeUnix);
            DateTime dt = timeUnix;
            if (RTCAvailable){
                rtc.adjust(dt);
            }
            myTimeData.syncTime(); // force clock to syschronise from rtc if in AP mode
        }
        request->redirect("/");
    });
}

String templateProcessor(const String& var){
    if(var == "ssidValue"){return ssid;}
    if (var == "knownSSIDs"){
        String ret = "<option></option>";
        for(int i=0;i<10;i++){
            if (KnownSSIDs[i] != ""){
                ret += "<option>" + KnownSSIDs[i] + "</option>";
            }
        }
        return ret;
    }
    if(var == "passwordValue"){return password;}
    // day
    if(var == "colorRValue"){return String(baseColorDay.r);}
    if(var == "colorGValue"){return String(baseColorDay.g);}
    if(var == "colorBValue"){return String(baseColorDay.b);}
    if(var == "designValue"){return designDay;}
    if(var == "brightnessValue"){return String(brightnessDay);}
    // night
    if(var == "nightModeActive"){
        if(nightModeActive){
            return "true";
        }else{
            return "false";
        }
    }
    if(var=="nightModeActiveInt"){
        if(nightModeActive){
            return "1";
        }else{
            return "0";
        }
    }
    if(var == "nightModeBeginHourValue"){return String(nightModeBeginHour);}
    if(var == "nightModeBeginMinuteValue"){return String(nightModeBeginMinute);}
    if(var == "nightModeEndHourValue"){return String(nightModeEndHour);}
    if(var == "nightModeEndMinuteValue"){return String(nightModeEndMinute);}
    if(var == "colorNightRValue"){return String(baseColorNight.r);}
    if(var == "colorNightGValue"){return String(baseColorNight.g);}
    if(var == "colorNightBValue"){return String(baseColorNight.b);}
    if(var == "designNightValue"){return designNight;}
    if(var == "brightnessNightValue"){return String(brightnessNight);}
    Serial.println(var + " not found");
    return "";
}