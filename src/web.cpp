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
        design = newDesign;
        baseColor = newColor;
        brightness = newBrightness;
        FastLED.setBrightness(brightness);
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

String templateProcessor(const String& var){
    if(var == "ssidValue"){return ssid;}
    if(var == "passwordValue"){return password;}
    if(var == "colorRValue"){return String(baseColor.r);}
    if(var == "colorGValue"){return String(baseColor.g);}
    if(var == "colorBValue"){return String(baseColor.b);}
    if(var == "designValue"){return design;}
    if(var == "brightnessValue"){return String(brightness);}
    Serial.println(var + " not found");
    return "";
}