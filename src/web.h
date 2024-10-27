#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <DNSServer.h>
#include <globals.h>
#include "SPIFFS.h"

#ifndef web_h
#define web_h

extern const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
      <title>Wortuhr Einstellungen</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <script src="js/jquery.min.js"></script>
      <script src="js/iro.js"></script>
      <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.5.0/css/font-awesome.min.css">
      <style>
        *{
          margin: 0;
          padding: 0;
        }
        .vc{
          display:flex;
          flex-direction: column;
          justify-content: center;
        }
        .hc{
          display:flex;
          flex-direction: row;
          justify-content: center;
        }
        .hl{
          display:flex;
          flex-direction: row;
          justify-content: flex-start;
        }
        body{
          background-color: var(--light-primary);
          text-align: center;
        }
        .header{
          background-color: var(--primary);
          height: 5em;
          color: white;
        }
        .heading{
          width: 300px;
          min-width: 300px;
          text-align: left;
        }
        .heading p{
          color: var(--secondary);
          font-size: large;
        }
        form{
          display: grid;
          grid-template-columns: 100px 200px;
          text-align: left;
          grid-gap: 1em;
          border: 2px dashed var(--secondary);
          border-radius: 1em;
          padding: 1em;
          width: calc(300px + 2em);
        }
        .separator{
          border-bottom: 1px solid var(--secondary);
          grid-column: 1/3;
        }
        .submit{
          grid-column: 1/3;
          text-align: center;
        }
        .slidercontainer{
          grid-column: 1/3;
          height: 2em;
        }
        .slider {
          appearance: none;
          -webkit-appearance: none;
          width: calc(100% - 2em);
          height: .5em;
          background: #aaaaaa;
          outline: none;
          background-color: var(--dark-primary);
          margin-top: 1em;
        }
        .slider::-webkit-slider-thumb {
          -webkit-appearance: none;
          appearance: none;
          width: 1.25em;
          height: 1.25em;
          border-radius: 100%;
          content: "";
          display: block;
          margin-top: -.125em;
          transition: background-color .25s, filter .25s, margin-left .25s;
          position: relative;
          z-index: 1;
          background-color: var(--secondary);
          filter: drop-shadow(0 1px 1px var(--dark-primary));
        }
        .slider::-moz-range-thumb {
          width: 1.25em;
          height: 1.25em;
          border-radius: 100%;
          content: "";
          display: block;
          margin-top: -.125em;
          transition: background-color .25s, filter .25s, margin-left .25s;
          position: relative;
          z-index: 1;
          background-color: var(--primary);
          filter: drop-shadow(0 1px 1px var(--dark-primary));
        }
        .slidervalue{
          width: 2em;
          margin-left: .5em;
        }
        .toggle-password{
          cursor: pointer;
        }
        :root {
          --primary: #316e75;
          --light-primary: #5eb1bf;
          --dark-primary: #042a2b;
          --secondary: #042a2b;
        }
      </style>
  </head>
  <body>
      <div class="header vc"><h1>Wortuhr Einstellungen</h1></div>
      <br><br>
      <div class="hc"><div class="heading"><p >Farb Einstellungen</p></div></div>
      <div class="hc">
        <form action="/color">
          <input id="designField" name="designField" type="text" value="designValue" hidden></input>
          <p>Design:</p><div class="hc"><select id="design" name="design">
              <option value="Solid">Einfarbig</option>
              <option value="Random">Zufall</option>
              <option value="Rainbow">Regenbogen</option>
              <option value="Palette">Palette</option>
          </select></div>
          <p>Farbe:</p><div class="hc" style="grid-column: 1/3;"><div id="picker"></div></div>
          <input id="colorR" name="colorR" type="text" hidden></input>
          <input id="colorG" name="colorG" type="text" hidden></input>
          <input id="colorB" name="colorB" type="text" hidden></input>
          <nobr>Helligkeit:</nobr>
          <div class="hl slidercontainer">
            <input type="range" min="0" max="255" step="5" value="brightnessValue" class="slider" name="brightness" id="brightness"><div class="vc slidervalue"><nobr class="brightness">brightnessValue</nobr></div>
          </div>
          <div class="separator"></div>
          <div class="submit"><input type="submit" value="speichern"></div>
        </form>
      </div>
      <br>
      <div class="hc"><div class="heading"><p >WiFi Einstellungen</p></div></div>
      <div class="hc">
        <form action="/config">
          <p>SSID:</p><div class="hl"><input type="text" name="ssid" value="ssidValue"></div>
          <p>Password:</p><div class="hl"><input type="password" name="password" value="passwordValue" id="password"><div class="vc"><span toggle="#password" class="fa fa-fw fa-eye toggle-password"></span></div></div>
          <div class="separator"></div>
          <div class="submit" ><input type="submit" value="speichern"></div>
        </form>
      </div>
      <br>
      <div class="hc">
        <a href="/update">Software update</a>
      </div>
      <script type="text/javascript">
        $(document).ready(function(){
          $('switch').on('click', function( event ){
            switchClick(event.target);
          })
          $('.slider').on('input', function( event ){
            changeValue(event.target);
          })
        });
        function switchClick(elem){
          if(elem.attributes.checked.value == "true"){
            elem.attributes.checked.value= "false";
          }else{
            elem.attributes.checked.value= "true";
          }
        }
        function changeValue(elem){
          $(".slidervalue ." + elem.id).html(elem.value);
          console.log($(".slidervalue ." + elem.id));
        }
        $(".toggle-password").click(function() {
          $(this).toggleClass("fa-eye fa-eye-slash");
          var input = $($(this).attr("toggle"));
          if (input.attr("type") == "password") {
            input.attr("type", "text");
          } else {
            input.attr("type", "password");
          }
        });
        var colorPicker = new iro.ColorPicker('#picker', {
          width: 200,
          color: "rgb(colorRValue,colorGValue,colorBValue)"
        });
        var design = $("#designField").val();
        console.log(design);
        $("#design").val(design);
        $("#colorR").val(colorPicker.color.rgb.r);
        $("#colorG").val(colorPicker.color.rgb.g);
        $("#colorB").val(colorPicker.color.rgb.b);

        colorPicker.on('color:change', function(color) {
          console.log(color.rgb);
          $("#colorR").val(color.rgb.r);
          $("#colorG").val(color.rgb.g);
          $("#colorB").val(color.rgb.b);
        });
      </script>
    </body>
</html>)rawliteral";

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