#include <Arduino.h>
#include <words.h>
#include <timeData.h>
#include <time.h>
#include <globals.h>
#include <FastLED.h>

TimeData::TimeData(){
    hour = 0;
    minute = 0;
    second = 0;
}

void TimeData::incTime(){
    second++;
    if(second == 60){
        second=0;
        minute++;
        showMinute = true;
    }
    if(minute % 5 == 0 && second == 0){
        showTime = true;
    }
    if(minute == 30 && second == 0){
        timesync = true;
    }
    if(minute == 60 && second == 0){
        minute=0;
        timesync = true;
        hour++;
    }
    if(hour==24 && second == 0){
        hour=0;
    }
}

void TimeData::init(){
    // design = "Palette";
    // baseColor = CRGB(30,140,190);
    // displayTime();
    // printTime();
    // printActiveLEDs();
    // animate();
    Serial.print(baseColor.r);
    Serial.print(" ");
    Serial.print(baseColor.g);
    Serial.print(" ");
    Serial.println(baseColor.b);
}

void TimeData::loop(){
    if(timesync){
        syncTime();
        timesync = false;
        showMinute = true;
        showTime = true;
    }
    if(showMinute){
        displayMinute();
        printActiveLEDs();
        showMinute=false;
    }
    if(showTime){
        displayTime();
        printTime();
        printActiveLEDs();
        showTime = false;
    }
    animate();
}

void TimeData::animate(){
    int numUpdates = 0;
    if(design == "Solid"){
        for(int j=0; j<20;j++){
            for(int i=0; i<NUM_LEDS; i++){
                if(updatedLEDs[i]){
                    numUpdates++;
                    if(activeLEDs[i]){
                        leds[i] = blend(CRGB::Black,baseColor,map(j,0,19,0,255));
                    }else{
                        leds[i] = blend(baseColor,CRGB::Black,map(j,0,19,0,255));
                    }
                }
            }
            FastLED.show();
            if(numUpdates == 0){
                break;
            }
            delay(50);
        }
        for(int i=0; i<NUM_LEDS && numUpdates != 0; i++){
            if(updatedLEDs[i]){
                updatedLEDs[i] = false;
            }
        }
    }
    if(design == "Palette"){
        bool activeSection = false;
        CRGB colors[NUM_LEDS];
        CRGB color;
        int delta = 100;
        // generate new colors
        for(int i=0; i<NUM_LEDS; i++){
            if(updatedLEDs[i]){
                numUpdates++;
                if(activeLEDs[i] && !activeSection){
                    activeSection = true;
                    int newR = baseColor.r+random(-delta,delta);
                    if(newR<0){newR=-newR;}
                    if(newR>255){newR=255-(newR-255);}
                    int newG = baseColor.g+random(-delta,delta);
                    if(newG<0){newG=-newG;}
                    if(newG>255){newG=255-(newG-255);}
                    int newB = baseColor.b+random(-delta,delta);
                    if(newB<0){newB=-newB;}
                    if(newB>255){newB=255-(newB-255);}
                    color = CRGB(newR,newG,newB);
                    Serial.print(baseColor.r);
                    Serial.print(" ");
                    Serial.print(baseColor.g);
                    Serial.print(" ");
                    Serial.println(baseColor.b);
                    Serial.print(color.r);
                    Serial.print(" ");
                    Serial.print(color.g);
                    Serial.print(" ");
                    Serial.println(color.b);
                }
                colors[i] = color;
            }else{
                activeSection = false;
            }
        }
        // update leds with new color
        for(int j=0; j<20;j++){
            for(int i=0; i<NUM_LEDS; i++){
                if(updatedLEDs[i]){
                    numUpdates++;
                    if(activeLEDs[i]){
                        leds[i] = blend(CRGB::Black,colors[i],map(j,0,19,0,255));
                    }else{
                        leds[i] = blend(colors[i],CRGB::Black,map(j,0,19,0,255));
                    }
                }
            }
            FastLED.show();
            if(numUpdates == 0){
                break;
            }
            delay(50);
        }
        for(int i=0; i<NUM_LEDS && numUpdates != 0; i++){
            if(updatedLEDs[i]){
                updatedLEDs[i] = false;
            }
        }
    }
    if(design == "Random"){
        bool activeSection = false;
        CRGB colors[NUM_LEDS];
        CRGB color;
        int delta = 100;
        // generate new colors
        for(int i=0; i<NUM_LEDS; i++){
            if(updatedLEDs[i]){
                numUpdates++;
                if(activeLEDs[i] && !activeSection){
                    activeSection = true;
                    do{
                        //color = CRGB(random(0,255),random(0,255),random(0,255));
                        color = CHSV(random(0,255),255,255);
                    }while(color.r+color.g+color.b>128);
                }
                colors[i] = color;
            }else{
                activeSection = false;
            }
        }
        // update leds with new color
        for(int j=0; j<20;j++){
            for(int i=0; i<NUM_LEDS; i++){
                if(updatedLEDs[i]){
                    numUpdates++;
                    if(activeLEDs[i]){
                        leds[i] = blend(CRGB::Black,colors[i],map(j,0,19,0,255));
                    }else{
                        leds[i] = blend(colors[i],CRGB::Black,map(j,0,19,0,255));
                    }
                }
            }
            FastLED.show();
            if(numUpdates == 0){
                break;
            }
            delay(50);
        }
        for(int i=0; i<NUM_LEDS && numUpdates != 0; i++){
            if(updatedLEDs[i]){
                updatedLEDs[i] = false;
            }
        }
    }
    if(design == "Rainbow"){
        uint8_t numLEDsToFill = 0;
        for(int i=0; i<NUM_LEDS-8; i++){ // ignore minute leds
            if(activeLEDs[i]){
                numLEDsToFill++;
            }
        }
        //fill_rainbow(leds,numLEDsToFill,hue,255/numLEDsToFill);
        uint8_t deltaHue = 255/numLEDsToFill;
        int ledHue = hue;
        for(int i=0; i<NUM_LEDS; i++){
            if(activeLEDs[i]){
                leds[i] = CHSV(ledHue,255,255);
                ledHue += deltaHue;
                if(ledHue > 255){
                    ledHue -=255;
                }
            }else{
                leds[i] = CRGB::Black;
            }
        }
        for(int i = NUM_LEDS-8;i<NUM_LEDS;i++){
            if(activeLEDs[i]){
                leds[i] = CHSV(hue,255,255);
            }
        }
        FastLED.show();
    }
}

void TimeData::displayMinute(){
    int index = 0;
    bool state = true;
    switch (minute % 5)
    {
    case 0:
        index = 0;
        state = false;
        break;
    case 1:
        index = 6;
        break;
    case 2:
        index = 4;
        break;
    case 3:
        index = 2;
        break;
    case 4:
        index = 0;
        break;
    default:
        break;
    }
    for(int i = NUM_LEDS-8;i<NUM_LEDS-index;i++){
        if(!activeLEDs[i]){
            activeLEDs[i] = state;
            updatedLEDs[i] = true;
        }
    }
}

void TimeData::clear(){
    for(int i=0;i<NUM_LEDS;i++){
        if(activeLEDs[i]){
            activeLEDs[i] = false;
            updatedLEDs[i] = true;
        }
    }
}

void TimeData::updateColor(){
    for(int i=0;i<NUM_LEDS;i++){
        if(activeLEDs[i]){
            updatedLEDs[i]=true;
        }
    }
    FastLED.show();
}

void TimeData::setLED(int index, bool state){
    if(activeLEDs[index] != state){
        activeLEDs[index] = state;
        updatedLEDs[index] = true;
    }
}

void TimeData::displayTime(){
    clear();
    displayMinute();
    // ES
    for(int i=0; i<sizeof(es)/sizeof(es[0]); i++){
        setLED(es[i]*2,true);
        setLED(es[i]*2+1,true);
    }
    // ist
    for(int i=0; i<sizeof(ist)/sizeof(ist[0]); i++){
        setLED(ist[i]*2,true);
        setLED(ist[i]*2+1,true);
    }
    bool timeShift = false;
    if(0 <= minute < 5){
        // nothing else then the hour
    }
    if(5 <= minute && minute < 10){
        // fünf nach
        for(int i=0; i<sizeof(fuenf)/sizeof(fuenf[0]); i++){
            setLED(fuenf[i]*2,true);
            setLED(fuenf[i]*2+1,true);
        }
        for(int i=0; i<sizeof(nach)/sizeof(nach[0]); i++){
            setLED(nach[i]*2,true);
            setLED(nach[i]*2+1,true);
        }
    }
    if(10 <= minute && minute < 15){
        // Zehn nach
        for(int i=0; i<sizeof(zehn)/sizeof(zehn[0]); i++){
            setLED(zehn[i]*2,true);
            setLED(zehn[i]*2+1,true);
        }
        for(int i=0; i<sizeof(nach)/sizeof(nach[0]); i++){
            setLED(nach[i]*2,true);
            setLED(nach[i]*2+1,true);
        }
    }
    if(15 <= minute && minute < 20){
        // Viertel
        for(int i=0; i<sizeof(viertel)/sizeof(viertel[0]); i++){
            setLED(viertel[i]*2,true);
            setLED(viertel[i]*2+1,true);
        }
        timeShift = true;
    }
    if(20 <= minute && minute < 25){
        // Zehn vor halber
        for(int i=0; i<sizeof(zehn)/sizeof(zehn[0]); i++){
            setLED(zehn[i]*2,true);
            setLED(zehn[i]*2+1,true);
        }
        for(int i=0; i<sizeof(vor)/sizeof(vor[0]); i++){
            setLED(vor[i]*2,true);
            setLED(vor[i]*2+1,true);
        }
        for(int i=0; i<sizeof(halb)/sizeof(halb[0]); i++){
            setLED(halb[i]*2,true);
            setLED(halb[i]*2+1,true);
        }
        timeShift = true;
    }
    if(25 <= minute && minute < 30){
        // Fünf vor halb
        for(int i=0; i<sizeof(fuenf)/sizeof(fuenf[0]); i++){
            setLED(fuenf[i]*2,true);
            setLED(fuenf[i]*2+1,true);
        }
        for(int i=0; i<sizeof(vor)/sizeof(vor[0]); i++){
            setLED(vor[i]*2,true);
            setLED(vor[i]*2+1,true);
        }
        for(int i=0; i<sizeof(halb)/sizeof(halb[0]); i++){
            setLED(halb[i]*2,true);
            setLED(halb[i]*2+1,true);
        }
        timeShift = true;
    }
    if(30 <= minute && minute < 35){
        // halb
        for(int i=0; i<sizeof(halb)/sizeof(halb[0]); i++){
            setLED(halb[i]*2,true);
            setLED(halb[i]*2+1,true);
        }
        timeShift = true;
    }
    if(35 <= minute && minute < 40){
        // fünf nach halb
        for(int i=0; i<sizeof(fuenf)/sizeof(fuenf[0]); i++){
            setLED(fuenf[i]*2,true);
            setLED(fuenf[i]*2+1,true);
        }
        for(int i=0; i<sizeof(nach)/sizeof(nach[0]); i++){
            setLED(nach[i]*2,true);
            setLED(nach[i]*2+1,true);
        }
        for(int i=0; i<sizeof(halb)/sizeof(halb[0]); i++){
            setLED(halb[i]*2,true);
            setLED(halb[i]*2+1,true);
        }
        timeShift = true;
    }
    if(40 <= minute && minute < 45){
        float r = random(0,5);
        if(r <=2){
            // fünf vor dreiviertel
            for(int i=0; i<sizeof(fuenf)/sizeof(fuenf[0]); i++){
                setLED(fuenf[i]*2,true);
                setLED(fuenf[i]*2+1,true);
            }
            for(int i=0; i<sizeof(vor)/sizeof(vor[0]); i++){
                setLED(vor[i]*2,true);
                setLED(vor[i]*2+1,true);
            }
            for(int i=0; i<sizeof(dreiviertel)/sizeof(dreiviertel[0]); i++){
                setLED(dreiviertel[i]*2,true);
                setLED(dreiviertel[i]*2+1,true);
            }
            timeShift = true;
        }else if(2<r<3){
            // Zehn nach halber
            for(int i=0; i<sizeof(zehn)/sizeof(zehn[0]); i++){
                setLED(zehn[i]*2,true);
                setLED(zehn[i]*2+1,true);
            }
            for(int i=0; i<sizeof(nach)/sizeof(nach[0]); i++){
                setLED(nach[i]*2,true);
                setLED(nach[i]*2+1,true);
            }
            for(int i=0; i<sizeof(halb)/sizeof(halb[0]); i++){
                setLED(halb[i]*2,true);
                setLED(halb[i]*2+1,true);
            }
            timeShift = true;
        }else{
            // zwanzig vor
            for(int i=0; i<sizeof(zwanzig)/sizeof(zwanzig[0]); i++){
                setLED(zwanzig[i]*2,true);
                setLED(zwanzig[i]*2+1,true);
            }
            for(int i=0; i<sizeof(vor)/sizeof(vor[0]); i++){
                setLED(vor[i]*2,true);
                setLED(vor[i]*2+1,true);
            }
            timeShift = true;
        }
    }
    if(45 <= minute && minute < 50){
        // Dreiviertel
        for(int i=0; i<sizeof(dreiviertel)/sizeof(dreiviertel[0]); i++){
            setLED(dreiviertel[i]*2,true);
            setLED(dreiviertel[i]*2+1,true);
        }
        timeShift = true;
    }
        if(50 <= minute && minute < 55){
        // zehn vor
        for(int i=0; i<sizeof(zehn)/sizeof(zehn[0]); i++){
            setLED(zehn[i]*2,true);
            setLED(zehn[i]*2+1,true);
        }
        for(int i=0; i<sizeof(vor)/sizeof(vor[0]); i++){
            setLED(vor[i]*2,true);
            setLED(vor[i]*2+1,true);
        }
        timeShift = true;
    }
    if(55 <= minute && minute < 60){
        // fünf vor
        
        for(int i=0; i<sizeof(fuenf)/sizeof(fuenf[0]); i++){
            setLED(fuenf[i]*2,true);
            setLED(fuenf[i]*2+1,true);
        }
        for(int i=0; i<sizeof(vor)/sizeof(vor[0]); i++){
            setLED(vor[i]*2,true);
            setLED(vor[i]*2+1,true);
        }
        timeShift = true;
    }

    // Hour
    int HourCorrected = hour;
    if(timeShift){
        HourCorrected++;
    }
    HourCorrected = HourCorrected % 12;
    for(int i=0; i<sizeof(zeitNames[HourCorrected])/sizeof(zeitNames[HourCorrected][0]); i++){
        if(zeitNames[HourCorrected][i] != -1){
            setLED(zeitNames[HourCorrected][i]*2,true);
            setLED(zeitNames[HourCorrected][i]*2+1,true);
        }
    }
}

void TimeData::printTime(){
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.print(" ");
    Serial.println(getTimeString());
}

void TimeData::syncTime(){
struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time 1");
        return;
    }
    int secondsoff = (timeinfo.tm_hour-hour)*3600;
    secondsoff += (timeinfo.tm_min-minute)*60;
    secondsoff += timeinfo.tm_sec-second;
    hour = timeinfo.tm_hour;
    minute = timeinfo.tm_min;
    second = timeinfo.tm_sec;
    Serial.print("synchonized time, time was wrong by ");
    Serial.print(secondsoff);
    Serial.println("s");
}

String TimeData::getTimeString(){
    String timeStrings[6] = {esString,istString,"","","",""};
    uint8_t timeStringIndex = 2;
    bool timeShift = false;
    if(0 <= minute < 5){

    }
    if(5 <= minute && minute < 10){
        // fünf nach
        timeStrings[timeStringIndex++] = fuenfString;
        timeStrings[timeStringIndex++] = nachString;
    }
    if(10 <= minute && minute < 15){
        // Zehn nach
        timeStrings[timeStringIndex++] = zehnString;
        timeStrings[timeStringIndex++] = nachString;
    }
    if(15 <= minute && minute < 20){
        if(random(0,5) <=3){
            // Viertel nach
            timeStrings[timeStringIndex++] = viertelString;
            timeStrings[timeStringIndex++] = nachString;
        }else{
            // Viertel
            timeStrings[timeStringIndex++] = viertelString;
            timeShift = true;
        }
    }
    if(20 <= minute && minute < 25){
        if(random(0,5) <=3){
            // zwanzig nach
            timeStrings[timeStringIndex++] = zwanzigString;
            timeStrings[timeStringIndex++] = nachString;
        }else{
            // Zehn vor halber
            timeStrings[timeStringIndex++] = zehnString;
            timeStrings[timeStringIndex++] = vorString;
            timeStrings[timeStringIndex++] = halbString;
            timeShift = true;
        }
    }
    if(25 <= minute && minute < 30){
        // Fünf vor halb
        timeStrings[timeStringIndex++] = fuenfString;
        timeStrings[timeStringIndex++] = vorString;
        timeStrings[timeStringIndex++] = halbString;
        timeShift = true;
    }
    if(30 <= minute && minute < 35){
        // halb
        timeStrings[timeStringIndex++] = halbString;
        timeShift = true;
    }
    if(35 <= minute && minute < 40){
        // fünf nach halb
        timeStrings[timeStringIndex++] = fuenfString;
        timeStrings[timeStringIndex++] = nachString;
        timeStrings[timeStringIndex++] = halbString;
        timeShift = true;
    }
    if(40 <= minute && minute < 45){
        float r = random(0,5);
        if(r <=2){
            // fünf vor dreiviertel
            timeStrings[timeStringIndex++] = fuenfString;
            timeStrings[timeStringIndex++] = vorString;
            timeStrings[timeStringIndex++] = dreiviertelString;
            timeShift = true;
        }else if(2<r<3){
            // Zehn nach halber
            timeStrings[timeStringIndex++] = zehnString;
            timeStrings[timeStringIndex++] = nachString;
            timeStrings[timeStringIndex++] = halbString;
            timeShift = true;
        }else{
            // zwanzig vor
            timeStrings[timeStringIndex++] = zwanzigString;
            timeStrings[timeStringIndex++] = vorString;
            timeShift = true;
        }
    }
    if(45 <= minute && minute < 50){
        if(random(0,5) <=3){
            // viertel vor
            timeStrings[timeStringIndex++] = viertelString;
            timeStrings[timeStringIndex++] = vorString;
            timeShift = true;
        }else{
            // Dreiviertel
            timeStrings[timeStringIndex++] = dreiviertelString;
            timeShift = true;
        }
    }
        if(50 <= minute && minute < 55){
        // zehn vor
        timeStrings[timeStringIndex++] = zehnString;
        timeStrings[timeStringIndex++] = vorString;
        timeShift = true;
    }
    if(55 <= minute && minute < 60){
        // fünf vor
        timeStrings[timeStringIndex++] = fuenfString;
        timeStrings[timeStringIndex++] = vorString;
        timeShift = true;
    }

    // Hour
    int HourCorrected = hour;
    if(timeShift){
        HourCorrected++;
    }
    HourCorrected = HourCorrected % 12;
    timeStrings[timeStringIndex] = zeitNameStrings[HourCorrected];

    String ret = "";
    for(int i=0;i<6;i++){
        if(timeStrings[i] != ""){
            ret += " ";
            ret += timeStrings[i];
        }
    }
    return ret;
};

void TimeData::printActiveLEDs(){
    for(int i=0; i<NUM_LEDS; i++){
        if(activeLEDs[i]){
            Serial.print(" 1");
        }else{
            Serial.print(" 0");
        }
        if(i%22 == 21){
            Serial.println("");
        }
    }
    Serial.println("");
}

void TimeData::displayNumber(int num){ // 356
    uint8_t digit1 = num/100; // 3,56 = 3
    uint8_t digit2 = (num-100*digit1)/10; // (356-3*100)/10 = 56/10=5,6=5
    uint8_t digit3 = num-100*digit1-10*digit2; // 356-3*100-5*10 = 356-300-50 = 6
    clear();
    displayDigit(digit1,0);
    displayDigit(digit2,4);
    displayDigit(digit3,8);
    animate();
}

void TimeData::displayDigit(uint8_t digit, uint8_t position){
    switch (digit)
    {
    case 0:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((43-position)*2,true);
        setLED((43-position)*2+1,true);
        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((65-position)*2,true);
        setLED((65-position)*2+1,true);
        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 1:
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 2:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((65-position)*2,true);
        setLED((65-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 3:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 4:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((43-position)*2,true);
        setLED((43-position)*2+1,true);
        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 5:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((43-position)*2,true);
        setLED((43-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 6:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((43-position)*2,true);
        setLED((43-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((65-position)*2,true);
        setLED((65-position)*2+1,true);
        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 7:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 8:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((43-position)*2,true);
        setLED((43-position)*2+1,true);
        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((65-position)*2,true);
        setLED((65-position)*2+1,true);
        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    case 9:
        setLED((22+position)*2,true);
        setLED((22+position)*2+1,true);
        setLED((23+position)*2,true);
        setLED((23+position)*2+1,true);
        setLED((24+position)*2,true);
        setLED((24+position)*2+1,true);

        setLED((43-position)*2,true);
        setLED((43-position)*2+1,true);
        setLED((41-position)*2,true);
        setLED((41-position)*2+1,true);

        setLED((44+position)*2,true);
        setLED((44+position)*2+1,true);
        setLED((45+position)*2,true);
        setLED((45+position)*2+1,true);
        setLED((46+position)*2,true);
        setLED((46+position)*2+1,true);

        setLED((63-position)*2,true);
        setLED((63-position)*2+1,true);

        setLED((66+position)*2,true);
        setLED((66+position)*2+1,true);
        setLED((67+position)*2,true);
        setLED((67+position)*2+1,true);
        setLED((68+position)*2,true);
        setLED((68+position)*2+1,true);
        break;
    default:
        break;
    }
}